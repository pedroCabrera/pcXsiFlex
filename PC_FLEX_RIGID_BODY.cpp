#include "..\Helpers.h"

#// Defines port, group and map identifiers used for registering the ICENode
enum IDs
{
	ID_IN_geometries = 1,
	ID_IN_particleSpacing = 2,
	ID_IN_expand = 3,
	ID_G_100 = 100,
	ID_OUT_positions = 203,
	ID_OUT_ShapeIndices = 204,
	ID_OUT_ShapeOffsets = 205,
	ID_OUT_ShapeCenters = 206,
	ID_OUT_ShapeCoefs = 207,
	ID_OUT_localRest = 210,

	ID_TYPE_CNS = 400,
	ID_STRUCT_CNS,
	ID_CTXT_CNS,
	ID_UNDEF = ULONG_MAX

};

using namespace XSI;

CStatus RegisterPC_FLEX_CREATE_RIGIDBODY(PluginRegistrar& in_reg)
{
	ICENodeDef nodeDef;
	nodeDef = Application().GetFactory().CreateICENodeDef(L"PC_FLEX_CREATE_RIGIDBODY", L"PC_FLEX_CREATE_RIGIDBODY");

	CStatus st;
	//set single thread
	st = nodeDef.PutThreadingModel(XSI::siICENodeSingleThreading);
	//Set color and Custom Type
	st = nodeDef.PutColor(154, 188, 102);
	st.AssertSucceeded();

	// Add input ports and groups.
	st = nodeDef.AddPortGroup(ID_G_100);
	st.AssertSucceeded();

	st = nodeDef.AddInputPort(ID_IN_geometries, ID_G_100, siICENodeDataGeometry, siICENodeStructureSingle, siICENodeContextSingleton, L"geometries", L"geometries", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_particleSpacing, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"particleSpacing", L"particleSpacing", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_expand, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"expand", L"expand", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();


	// Add output ports.
	st = nodeDef.AddOutputPort(ID_OUT_positions, siICENodeDataVector4, siICENodeStructureArray, siICENodeContextSingleton, L"positions", L"positions", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddOutputPort(ID_OUT_ShapeIndices, siICENodeDataLong, siICENodeStructureArray, siICENodeContextSingleton, L"ShapeIndices", L"ShapeIndices", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddOutputPort(ID_OUT_ShapeOffsets, siICENodeDataLong, siICENodeStructureArray, siICENodeContextSingleton, L"ShapeOffsets", L"ShapeOffsets", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddOutputPort(ID_OUT_ShapeCenters, siICENodeDataVector3, siICENodeStructureArray, siICENodeContextSingleton, L"ShapeCenters", L"ShapeCenters", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddOutputPort(ID_OUT_ShapeCoefs, siICENodeDataFloat, siICENodeStructureArray, siICENodeContextSingleton, L"ShapeCoefs", L"ShapeCoefs", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddOutputPort(ID_OUT_localRest, siICENodeDataVector3, siICENodeStructureArray, siICENodeContextSingleton, L"ShapelocalRest", L"ShapelocalRest", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();


	PluginItem nodeItem = in_reg.RegisterICENode(nodeDef);
	nodeItem.PutCategories(L"PC_Tools/Flex");

	return CStatus::OK;
}
struct Data
{
public:
	int nbtriangles = 0;
	int numSprings = 0;
	int validgeom = 0;
	int numShapes = 0;
	std::vector<int> triangleIndices;
	std::vector<float> fpoints;
	std::vector<float> npoints;
	std::vector<int> ShapeIndices;
	std::vector<int> ShapeOffsets;
	std::vector<float> ShapeCenters;
	std::vector<float> ShapeCoefficients;
	std::vector<Vec3> restData;
};

SICALLBACK PC_FLEX_CREATE_RIGIDBODY_BeginEvaluate(ICENodeContext& in_ctxt)
{

	CICEGeometry geom(in_ctxt, ID_IN_geometries);
	CICEPortState geoport(in_ctxt, ID_IN_geometries);
	CICEPortState particleSpacingport(in_ctxt, ID_IN_particleSpacing);
	CICEPortState expandport(in_ctxt, ID_IN_expand);
	CValue userData = in_ctxt.GetUserData();
	Data * pData = new Data;

	if (geom.GetPointPositionCount() == 0)//i.e. there is some valid geo otherwise we get crashes
	{
		if (!in_ctxt.GetUserData().IsEmpty())//if it's not empty, empty it
		{
			Data * pData = (Data *)(CValue::siPtrType)in_ctxt.GetUserData();
			delete pData;
			in_ctxt.PutUserData(CValue());
		}

		return CStatus::OK;//exit out of BeginEvaluate
	}

	if (in_ctxt.GetUserData().IsEmpty() ||
		geoport.IsDirty(CICEPortState::siDataDirtyState) ||
		geom.IsDirty(CICEGeometry::siGeometryTypeDirtyState | CICEGeometry::siTransformationDirtyState | CICEGeometry::siPointPositionDirtyState | CICEGeometry::siTopologyDirtyState) ||
		particleSpacingport.IsDirty(CICEPortState::siDataDirtyState) ||
		expandport.IsDirty(CICEPortState::siDataDirtyState))
	{
		//Application().LogMessage(CString("PC_FLEX_CREATE_SOFTBODY_Is Dirty"));
		CDataArrayFloat particleSpacingData(in_ctxt, ID_IN_particleSpacing);
		CDataArrayFloat expandData(in_ctxt, ID_IN_expand);

		if (geom.GetPointPositionCount() != 0)
		{
			CLongArray indices;
			CDoubleArray points;

			geom.GetTrianglePointIndices(indices);
			geom.GetPointPositions(points);

			pData->fpoints.resize(0);
			pData->triangleIndices.resize(0);

			ULONG nOffset = 0;
			for (ULONG i = 0; i < geom.GetPointPositionCount(); i++)
			{
				pData->fpoints.push_back(points[nOffset]); nOffset++;
				pData->fpoints.push_back(points[nOffset]); nOffset++;
				pData->fpoints.push_back(points[nOffset]); nOffset++;
			}

			for (size_t i = 0; i < indices.GetCount(); i++)
				pData->triangleIndices.push_back(indices[i]);

			
			NvFlexExtAsset* flexAsset = NvFlexExtCreateRigidFromMesh(
				pData->fpoints.data(),
				geom.GetPointPositionCount(),
				pData->triangleIndices.data(),
				geom.GetTriangleCount()*3,
				particleSpacingData[0],
				expandData[0]);
			
			pData->npoints.resize(flexAsset->numParticles * 4);
			pData->npoints.assign(&flexAsset->particles[0], &flexAsset->particles[(flexAsset->numParticles * 4)]);

			pData->nbtriangles = flexAsset->numTriangles;

			pData->numShapes = flexAsset->numShapes;
			pData->ShapeIndices.assign(&flexAsset->shapeIndices[0], &flexAsset->shapeIndices[flexAsset->numShapeIndices]);
			pData->ShapeOffsets.assign(&flexAsset->shapeOffsets[0], &flexAsset->shapeOffsets[flexAsset->numShapes]);
			pData->ShapeCoefficients.assign(&flexAsset->shapeCoefficients[0], &flexAsset->shapeCoefficients[flexAsset->numShapes]);
			pData->ShapeCenters.assign(&flexAsset->shapeCenters[0], &flexAsset->shapeCenters[flexAsset->numShapes * 3]);

			NvFlexExtDestroyAsset(flexAsset);

			int count = 0;
			// Calculate RigidLocalPositions
			//Application().LogMessage(CString("Calculated RigidLocalPositions"));
			pData->restData.resize(0);
			std::vector<int>::iterator it;
			it = pData->ShapeOffsets.begin();
			pData->ShapeOffsets.insert(it, 0);
			CalculateRigidLocalPositions(pData->npoints, pData->ShapeOffsets, pData->ShapeIndices, pData->numShapes, pData->restData);
			it = pData->ShapeOffsets.begin();
			pData->ShapeOffsets.erase(it);

			//Clear Dirty States
			geoport.ClearState();
			geom.ClearState();
			particleSpacingport.ClearState();
			expandport.ClearState();

			pData->validgeom = 1;
			in_ctxt.PutUserData((CValue::siPtrType) pData);
		}

	}
	else
	{
		Data * pData = (Data *)(CValue::siPtrType)in_ctxt.GetUserData();
	}
	return CStatus::OK;
}

SICALLBACK PC_FLEX_CREATE_RIGIDBODY_Evaluate(ICENodeContext& in_ctxt)
{
	// The current output port being evaluated...
	ULONG out_portID = in_ctxt.GetEvaluatedOutputPortID();
	Data * pData = new Data;
	pData = (Data *)(CValue::siPtrType)in_ctxt.GetUserData();
	CValue userData = in_ctxt.GetUserData();
	if (!userData.IsEmpty() && pData->validgeom)
	{
		switch (out_portID)
		{
		case ID_OUT_positions:
		{
			CDataArray2DVector4f outPositionsData(in_ctxt);
			CDataArray2DVector4f::Accessor outPositions = outPositionsData.Resize(0, pData->npoints.size() / 4);
			for (int i = 0; i < pData->npoints.size() / 4; ++i)
			{
				outPositions[i].Set(pData->npoints[i * 4], pData->npoints[(i * 4) + 1], pData->npoints[(i * 4) + 2], pData->npoints[(i * 4) + 3]);
			}
		}
		break;
		case ID_OUT_ShapeIndices:
		{
			CDataArray2DLong outShapeIndicesData(in_ctxt);
			CDataArray2DLong::Accessor outShapeIndices = outShapeIndicesData.Resize(0, pData->ShapeIndices.size());
			for (int x = 0; x < pData->ShapeIndices.size(); ++x)
			{
				outShapeIndices[x] = pData->ShapeIndices[x];
			}
		}
		break;
		case ID_OUT_ShapeOffsets:
		{
			CDataArray2DLong outShapeOffsetsData(in_ctxt);
			CDataArray2DLong::Accessor outShapeOffsets = outShapeOffsetsData.Resize(0, pData->ShapeOffsets.size());
			for (int x = 0; x < pData->ShapeOffsets.size(); ++x)
			{
				outShapeOffsets[x] = pData->ShapeOffsets[x];
			}
		}
		break;
		case ID_OUT_ShapeCenters:
		{
			CDataArray2DVector3f outShapeCentersData(in_ctxt);
			CDataArray2DVector3f::Accessor outShapeCenters = outShapeCentersData.Resize(0, pData->ShapeCenters.size() / 3);
			for (int i = 0; i < pData->ShapeCenters.size() / 3; ++i)
			{
				float x, y, z;
				x = pData->ShapeCenters[i * 3];
				y = pData->ShapeCenters[(i * 3) + 1];
				z = pData->ShapeCenters[(i * 3) + 2];
				outShapeCenters[i].Set(x, y, z);
			}
		}
		break;
		case ID_OUT_ShapeCoefs:
		{
			CDataArray2DFloat outShapeCoefsData(in_ctxt);
			CDataArray2DFloat::Accessor outShapeCoefs = outShapeCoefsData.Resize(0, pData->ShapeCoefficients.size());
			for (int i = 0; i < pData->ShapeCoefficients.size(); ++i)
			{
				outShapeCoefs[i] = pData->ShapeCoefficients[i];
			}
		}
		break;
		case ID_OUT_localRest:
		{
			CDataArray2DVector3f outlocalRestData(in_ctxt);
			CDataArray2DVector3f::Accessor outShapeRest = outlocalRestData.Resize(0, pData->restData.size());
			int count = 0;
			for (int r = 0; r < pData->restData.size(); ++r)
			{
				outShapeRest[r].Set(pData->restData[r].x, pData->restData[r].y, pData->restData[r].z);
			}
		}
		break;
		// Other output ports...
		};
	}

	return CStatus::OK;
}

SICALLBACK PC_FLEX_CREATE_RIGIDBODY_Term(ICENodeContext& in_ctxt)
{
	//Application().LogMessage(CString("PC_FLEX_CREATE_RIGIDBODY_Term"));
	Context ctxt(in_ctxt);
	CValue userData = ctxt.GetUserData();
	if (userData.IsEmpty())
	{
		return CStatus::OK;
	}
	Data *pData = (Data*)(CValue::siPtrType)ctxt.GetUserData();

	pData->fpoints.clear();
	pData->triangleIndices.clear();
	delete pData;

	// Clear user data
	ctxt.PutUserData(CValue());

	return CStatus::OK;

}