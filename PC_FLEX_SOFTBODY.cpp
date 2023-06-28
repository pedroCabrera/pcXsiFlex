#include "..\Helpers.h"

// Defines port, group and map identifiers used for registering the ICENode
enum IDs
{
	ID_IN_geometries = 1,
	ID_IN_particleSpacing = 2,
	ID_IN_volumeSampling = 3,
	ID_IN_surfaceSampling = 4,
	ID_IN_clusterSpacing = 5,
	ID_IN_clusterRadius = 6,
	ID_IN_clusterStiffness = 7,
	ID_IN_linkRadius = 8,
	ID_IN_linkStiffness = 9,
	ID_IN_globalStiffness = 10,
	ID_IN_skinningFalloff = 11,
	ID_IN_skinningMaxDistance = 12,
	ID_G_100 = 100,
	ID_OUT_spInds = 200,
	ID_OUT_spRests = 201,
	ID_OUT_spStiff = 202,
	ID_OUT_positions = 203,
	ID_OUT_ShapeIndices = 204,
	ID_OUT_ShapeOffsets = 205,
	ID_OUT_ShapeCenters = 206,
	ID_OUT_ShapeCoefs = 207,
	ID_OUT_SkinningIndices = 208,
	ID_OUT_SkinningWeights = 209,
	ID_OUT_localRest = 210,

	ID_TYPE_CNS = 400,
	ID_STRUCT_CNS,
	ID_CTXT_CNS,
	ID_UNDEF = ULONG_MAX

};

using namespace XSI;

CStatus RegisterPC_FLEX_CREATE_SOFTBODY(PluginRegistrar& in_reg)
{
	ICENodeDef nodeDef;
	nodeDef = Application().GetFactory().CreateICENodeDef(L"PC_FLEX_CREATE_SOFTBODY", L"PC_FLEX_CREATE_SOFTBODY");

	CStatus st;
	//set single thread
	st = nodeDef.PutThreadingModel(XSI::siICENodeSingleThreading);
	//Set color and Custom Type
	st = nodeDef.PutColor(154, 188, 102);
	st.AssertSucceeded();


	// Add input ports and groups.
	st = nodeDef.AddPortGroup(ID_G_100);
	st.AssertSucceeded();

	st = nodeDef.AddInputPort(ID_IN_geometries, ID_G_100, siICENodeDataGeometry, siICENodeStructureSingle, siICENodeContextSingleton, L"Static geometries", L"Static_geometries", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_particleSpacing, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"particleSpacing", L"particleSpacing", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_volumeSampling, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"volumeSampling", L"volumeSampling", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_surfaceSampling, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"surfaceSampling", L"surfaceSampling", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_clusterSpacing, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"clusterSpacing", L"clusterSpacing", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_clusterRadius, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"clusterRadius", L"clusterRadius", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_clusterStiffness, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"clusterStiffness", L"clusterStiffness", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_linkRadius, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"linkRadius", L"linkRadius", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_linkStiffness, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"linkStiffness", L"linkStiffness", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_globalStiffness, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"globalStiffness ", L"globalStiffness ", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_skinningFalloff, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"skinningFalloff ", L"skinningFalloff ", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_skinningMaxDistance, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"skinningMaxDistance ", L"skinningMaxDistance ", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();

	// Add output ports.
	st = nodeDef.AddOutputPort(ID_OUT_positions, siICENodeDataVector4, siICENodeStructureArray, siICENodeContextSingleton, L"positions", L"positions", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddOutputPort(ID_OUT_spInds, siICENodeDataLong, siICENodeStructureArray, siICENodeContextSingleton, L"springIndex", L"springIndex", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddOutputPort(ID_OUT_spRests, siICENodeDataFloat, siICENodeStructureArray, siICENodeContextSingleton, L"springRests", L"springRests", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddOutputPort(ID_OUT_spStiff, siICENodeDataFloat, siICENodeStructureArray, siICENodeContextSingleton, L"springStiffs", L"springStiffs", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
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
	st = nodeDef.AddOutputPort(ID_OUT_SkinningIndices, siICENodeDataLong, siICENodeStructureArray, siICENodeContextSingleton, L"SkinningIndices", L"SkinningIndices", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddOutputPort(ID_OUT_SkinningWeights, siICENodeDataFloat, siICENodeStructureArray, siICENodeContextSingleton, L"SkinningWeights", L"SkinningWeights", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
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
	std::vector<int> SkinningIndices;
	std::vector<float> SkinningWeights;
	std::vector<int> ShapeIndices;
	std::vector<int> ShapeOffsets;
	std::vector<float> ShapeCenters;
	std::vector<float> ShapeCoefficients;
	std::vector<int> springIndices;
	std::vector<float> springRestLengths;
	std::vector<float> springStiffness;
	std::vector<Vec3> restData;
};

SICALLBACK PC_FLEX_CREATE_SOFTBODY_BeginEvaluate(ICENodeContext& in_ctxt)
{


	CICEGeometry geom(in_ctxt, ID_IN_geometries);
	CICEPortState geoport(in_ctxt, ID_IN_geometries);
	CICEPortState particleSpacingport(in_ctxt, ID_IN_particleSpacing);
	CICEPortState volumeSamplingport(in_ctxt, ID_IN_volumeSampling);
	CICEPortState surfaceSamplingport(in_ctxt, ID_IN_surfaceSampling);
	CICEPortState clusterSpacingport(in_ctxt, ID_IN_clusterSpacing);
	CICEPortState clusterRadiusport(in_ctxt, ID_IN_clusterRadius);
	CICEPortState clusterStiffnessport(in_ctxt, ID_IN_clusterStiffness);
	CICEPortState linkRadiusport(in_ctxt, ID_IN_linkRadius);
	CICEPortState linkStiffnessport(in_ctxt, ID_IN_linkStiffness);
	CICEPortState globalStiffnessport(in_ctxt, ID_IN_globalStiffness);
	CICEPortState skinningFalloffport(in_ctxt, ID_IN_skinningFalloff);
	CICEPortState skinningMaxDistanceport(in_ctxt, ID_IN_skinningMaxDistance);
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
		particleSpacingport.IsDirty(CICEPortState::siDataDirtyState)	||
		volumeSamplingport.IsDirty(CICEPortState::siDataDirtyState)		||
		surfaceSamplingport.IsDirty(CICEPortState::siDataDirtyState)	||
		clusterSpacingport.IsDirty(CICEPortState::siDataDirtyState)		||
		clusterRadiusport.IsDirty(CICEPortState::siDataDirtyState)		|| 
		clusterStiffnessport.IsDirty(CICEPortState::siDataDirtyState)	|| 
		linkRadiusport.IsDirty(CICEPortState::siDataDirtyState)			|| 
		linkStiffnessport.IsDirty(CICEPortState::siDataDirtyState)		||
		globalStiffnessport.IsDirty(CICEPortState::siDataDirtyState)	|| 
		skinningFalloffport.IsDirty(CICEPortState::siDataDirtyState)	|| 
		skinningMaxDistanceport.IsDirty(CICEPortState::siDataDirtyState))
	{
		//Application().LogMessage(CString("PC_FLEX_CREATE_SOFTBODY_Is Dirty"));
		CDataArrayFloat particleSpacingData(in_ctxt, ID_IN_particleSpacing);
		CDataArrayFloat volumeSamplingData(in_ctxt, ID_IN_volumeSampling);
		CDataArrayFloat surfaceSamplingData(in_ctxt, ID_IN_surfaceSampling);
		CDataArrayFloat clusterSpacingData(in_ctxt, ID_IN_clusterSpacing);
		CDataArrayFloat clusterRadiusData(in_ctxt, ID_IN_clusterRadius);
		CDataArrayFloat clusterStiffnessData(in_ctxt, ID_IN_clusterStiffness);
		CDataArrayFloat linkRadiusData(in_ctxt, ID_IN_linkRadius);
		CDataArrayFloat linkStiffnessata(in_ctxt, ID_IN_linkStiffness);
		CDataArrayFloat globalStiffnessData(in_ctxt, ID_IN_globalStiffness);
		CDataArrayFloat skinningFalloffData(in_ctxt, ID_IN_skinningFalloff);
		CDataArrayFloat skinningMaxDistanceData(in_ctxt, ID_IN_skinningMaxDistance);

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
			
			NvFlexExtAsset* flexAsset = NvFlexExtCreateSoftFromMesh(pData->fpoints.data(),
				geom.GetPointPositionCount(), 
				pData->triangleIndices.data(), 
				geom.GetTriangleCount()*3, 
				particleSpacingData[0], volumeSamplingData[0], surfaceSamplingData[0], 
				clusterSpacingData[0], clusterRadiusData[0], clusterStiffnessData[0], 
				linkRadiusData[0], linkStiffnessata[0], globalStiffnessData[0]);
			
			if (skinningMaxDistanceData[0] > 0) {
				const int maxWeights = 4;
				pData->SkinningIndices.resize(geom.GetPointPositionCount()*maxWeights);
				pData->SkinningWeights.resize(geom.GetPointPositionCount()*maxWeights);
				NvFlexExtCreateSoftMeshSkinning(
					pData->fpoints.data(),
					geom.GetPointPositionCount(),
					flexAsset->shapeCenters,
					flexAsset->numShapes,
					skinningFalloffData[0],
					skinningMaxDistanceData[0],
					pData->SkinningWeights.data(),
					pData->SkinningIndices.data()
				);
			}

			pData->npoints.resize(flexAsset->numParticles * 4);
			pData->npoints.assign(&flexAsset->particles[0], &flexAsset->particles[(flexAsset->numParticles * 4)]);

			pData->numSprings = flexAsset->numSprings;
			pData->nbtriangles = flexAsset->numTriangles;
			pData->springIndices.assign(&flexAsset->springIndices[0], &flexAsset->springIndices[(flexAsset->numSprings * 2)]);
			pData->springRestLengths.assign(&flexAsset->springRestLengths[0], &flexAsset->springRestLengths[flexAsset->numSprings]);
			pData->springStiffness.assign(&flexAsset->springCoefficients[0], &flexAsset->springCoefficients[flexAsset->numSprings]);

			pData->numShapes = flexAsset->numShapes;
			pData->ShapeIndices.assign(&flexAsset->shapeIndices[0], &flexAsset->shapeIndices[flexAsset->numShapeIndices]);
			pData->ShapeOffsets.assign(&flexAsset->shapeOffsets[0], &flexAsset->shapeOffsets[flexAsset->numShapes]);
			pData->ShapeCoefficients.assign(&flexAsset->shapeCoefficients[0], &flexAsset->shapeCoefficients[flexAsset->numShapes]);
			pData->ShapeCenters.assign(&flexAsset->shapeCenters[0], &flexAsset->shapeCenters[flexAsset->numShapes*3]);

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
			volumeSamplingport.ClearState();
			surfaceSamplingport.ClearState();
			clusterSpacingport.ClearState();
			clusterRadiusport.ClearState();
			clusterStiffnessport.ClearState();
			linkRadiusport.ClearState();
			linkStiffnessport.ClearState();
			globalStiffnessport.ClearState();
			skinningFalloffport.ClearState();
			skinningMaxDistanceport.ClearState();

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

SICALLBACK PC_FLEX_CREATE_SOFTBODY_Evaluate(ICENodeContext& in_ctxt)
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
			CDataArray2DVector4f::Accessor outPositions = outPositionsData.Resize(0, pData->npoints.size()/4);
			for (int i = 0; i < pData->npoints.size() / 4; ++i)
			{
				outPositions[i].Set(pData->npoints[i * 4], pData->npoints[(i * 4) + 1], pData->npoints[(i * 4) + 2], pData->npoints[(i * 4) + 3]);
			}
		}
		break;
		case ID_OUT_spInds:
		{
			CDataArray2DLong outSpringsData(in_ctxt);
			CDataArray2DLong::Accessor outSprings = outSpringsData.Resize(0, pData->springIndices.size());
			for (int x = 0; x < pData->springIndices.size(); ++x)
			{
				outSprings[x] = pData->springIndices[x];
			}
		}
		break;
		case ID_OUT_spRests:
		{
			CDataArray2DFloat outSpringsData(in_ctxt);
			CDataArray2DFloat::Accessor outSprings = outSpringsData.Resize(0, pData->springRestLengths.size());
			for (int x = 0; x < pData->springRestLengths.size(); ++x)
			{
				outSprings[x] = pData->springRestLengths[x];
			}
		}
		break;
		case ID_OUT_spStiff:
		{
			CDataArray2DFloat outSpringsData(in_ctxt);
			CDataArray2DFloat::Accessor outSprings = outSpringsData.Resize(0, pData->springStiffness.size());
			for (int x = 0; x < pData->springStiffness.size(); ++x)
			{
				outSprings[x] = pData->springStiffness[x];
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
			CDataArray2DVector3f::Accessor outShapeCenters = outShapeCentersData.Resize(0, pData->ShapeCenters.size()/3);
			for (int i = 0; i < pData->ShapeCenters.size() / 3; ++i)
			{
				float x, y, z;
				x = pData->ShapeCenters[i*3];
				y = pData->ShapeCenters[(i * 3)+1];
				z = pData->ShapeCenters[(i * 3) +2];
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
		case ID_OUT_SkinningIndices:
		{
			CDataArray2DLong outSkinningIndicesData(in_ctxt);
			CDataArray2DLong::Accessor outShapeIndices = outSkinningIndicesData.Resize(0, pData->SkinningIndices.size());
			for (int i = 0; i < pData->SkinningIndices.size(); ++i)
			{
				outShapeIndices[i] = pData->SkinningIndices[i];
			}
		}
		break;
		case ID_OUT_SkinningWeights:
		{
			CDataArray2DFloat outSkinningIndicesData(in_ctxt);
			CDataArray2DFloat::Accessor outShapeWeights = outSkinningIndicesData.Resize(0, pData->SkinningWeights.size());
			for (int i = 0; i < pData->SkinningWeights.size(); ++i)
			{
				outShapeWeights[i] = pData->SkinningWeights[i];
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

SICALLBACK PC_FLEX_CREATE_SOFTBODY_Term(ICENodeContext& in_ctxt)
{
	//Application().LogMessage(CString("PC_FLEX_CREATE_SOFTBODY_Term"));
	Context ctxt(in_ctxt);
	CValue userData = ctxt.GetUserData();
	if (userData.IsEmpty())
	{
		return CStatus::OK;
	}
	Data *pData = (Data*)(CValue::siPtrType)ctxt.GetUserData();

	pData->fpoints.clear();
	pData->springIndices.clear();
	pData->springRestLengths.clear();
	pData->springStiffness.clear();
	pData->triangleIndices.clear();
	delete pData;

	// Clear user data
	ctxt.PutUserData(CValue());

	return CStatus::OK;

}