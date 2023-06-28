#include "..\Helpers.h"

// Defines port, group and map identifiers used for registering the ICENode
enum IDs
{
	ID_IN_geometries = 1,
	ID_IN_stretchStiffness=2,
	ID_IN_bendStiffness = 3,
	ID_IN_tetherStiffness =4,
	ID_IN_tetherGive = 5,
	ID_IN_pressure = 6,
	ID_G_100 = 100,
	ID_OUT_spInds = 200,
	ID_OUT_spRests = 201,
	ID_OUT_spStiff = 202,
	ID_OUT_triangleInds = 203,
	ID_OUT_inflatableData = 204,
	ID_TYPE_CNS = 400,
	ID_STRUCT_CNS,
	ID_CTXT_CNS,
	ID_UNDEF = ULONG_MAX

};

using namespace XSI;

CStatus RegisterPC_FLEX_CREATE_CLOTH_SPRINGS(PluginRegistrar& in_reg)
{
	ICENodeDef nodeDef;
	nodeDef = Application().GetFactory().CreateICENodeDef(L"PC_FLEX_CREATE_CLOTH_SPRINGS", L"PC_FLEX_CREATE_CLOTH_SPRINGS");

	CStatus st;
	//set single thread
	st = nodeDef.PutThreadingModel(XSI::siICENodeSingleThreading);
	//Set color and Custom Type
	st = nodeDef.PutColor(154, 188, 102);
	st.AssertSucceeded();

	// Add custom types definition.
	st = nodeDef.DefineCustomType(L"FLEX_Solver", L"FLEX_Solver", L"FLEX_Solver", 13, 80, 132);
	st.AssertSucceeded();
	CStringArray SolverCustomType(1);
	SolverCustomType[0] = L"FLEX_Solver";


	// Add input ports and groups.
	st = nodeDef.AddPortGroup(ID_G_100);
	st.AssertSucceeded();

	st = nodeDef.AddInputPort(ID_IN_geometries, ID_G_100, siICENodeDataGeometry, siICENodeStructureSingle, siICENodeContextSingleton, L"Static geometries", L"Static_geometries", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_stretchStiffness, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"stretchStiffness", L"stretchStiffness", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_bendStiffness, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"bendStiffness", L"bendStiffness", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_tetherStiffness, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"tetherStiffness", L"tetherStiffness", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_tetherGive, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"tetherGive", L"tetherGive", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_pressure, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"pressure", L"pressure", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();

	// Add output ports.
	st = nodeDef.AddOutputPort(ID_OUT_triangleInds, siICENodeDataLong, siICENodeStructureArray, siICENodeContextSingleton, L"triangleIndex", L"triangleIndex", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddOutputPort(ID_OUT_spInds, siICENodeDataLong, siICENodeStructureArray, siICENodeContextSingleton, L"springIndex", L"springIndex", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddOutputPort(ID_OUT_spRests, siICENodeDataFloat, siICENodeStructureArray, siICENodeContextSingleton, L"springRests", L"springRests", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddOutputPort(ID_OUT_spStiff, siICENodeDataFloat, siICENodeStructureArray, siICENodeContextSingleton, L"springStiffs", L"springStiffs", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddOutputPort(ID_OUT_inflatableData, siICENodeDataFloat, siICENodeStructureArray, siICENodeContextSingleton, L"inflatableData", L"inflatableData", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();

	PluginItem nodeItem = in_reg.RegisterICENode(nodeDef);
	nodeItem.PutCategories(L"Custom ICENode");

	return CStatus::OK;
}
struct Data
{
	public:
	int nbtriangles = 0;
	int numSprings = 0;
	int validgeom = 0;
	bool inflatable = false;
	float inflatablevolume = 0;
	float inflatablePressure = 0;
	float inflatableStiffness = 0;
	std::vector<float> fpoints;
	std::vector<int> springIndices;
	std::vector<float> springRestLengths;
	std::vector<float> springStiffness;
	std::vector<int> triangleIndices;
};

SICALLBACK PC_FLEX_CREATE_CLOTH_SPRINGS_BeginEvaluate(ICENodeContext& in_ctxt)
{

	NvFlexLibrary* g_FlexLib = NULL;

	CRefArray plugins = Application().GetPlugins();//.Plugins("My plugin");
	CRef reference = plugins.GetItem("PC_FLEX_Plugin");
	Plugin plugin(reference);

	g_FlexLib = (NvFlexLibrary*)(CValue::siPtrType)plugin.GetUserData();

	CICEGeometry geom(in_ctxt, ID_IN_geometries);
	CICEPortState geoport(in_ctxt, ID_IN_geometries);
	CICEPortState stretchport(in_ctxt, ID_IN_stretchStiffness);
	CICEPortState bendport(in_ctxt, ID_IN_bendStiffness);
	CICEPortState tetherport(in_ctxt, ID_IN_tetherStiffness);
	CICEPortState tetherGport(in_ctxt, ID_IN_tetherGive);
	CICEPortState pressureort(in_ctxt, ID_IN_pressure);
	CValue userData = in_ctxt.GetUserData();
	Data * pData = new Data;
	if (!in_ctxt.GetUserData().IsEmpty()) {
		pData->validgeom = 0;
	}
	if (geom.GetPointPositionCount() == 0)//i.e. there is some valid geo otherwise we get crashes
	{
		if (!in_ctxt.GetUserData().IsEmpty())//if it's not empty, empty it
		{
			Data * pData = (Data *)(CValue::siPtrType)in_ctxt.GetUserData();
			delete pData;
		}
		

		//pData->validgeom = 0;
		//in_ctxt.PutUserData((CValue::siPtrType) pData);
		//geoport.ClearState();
		return CStatus::OK;//exit out of BeginEvaluate
	}

	if (in_ctxt.GetUserData().IsEmpty() || geoport.IsDirty() || stretchport.IsDirty() || bendport.IsDirty() || tetherport.IsDirty() || tetherGport.IsDirty() || pressureort.IsDirty())
	{
		Application().LogMessage(CString("PC_FLEX_CREATE_CLOTH_SPRINGS_Is Dirty"));
		CDataArrayFloat stretchStiffnessData(in_ctxt, ID_IN_stretchStiffness);
		CDataArrayFloat bendStiffnessData(in_ctxt, ID_IN_bendStiffness);
		CDataArrayFloat tetherStiffnessData(in_ctxt, ID_IN_tetherStiffness);
		CDataArrayFloat tetherGiveData(in_ctxt, ID_IN_tetherGive);
		CDataArrayFloat pressureData(in_ctxt, ID_IN_pressure);

		if (geom.GetPointPositionCount() != 0)
		{
			CLongArray indices;
			geom.GetTrianglePointIndices(indices);
			ULONG nOffset = 0;

			pData->fpoints.resize(0);
			pData->triangleIndices.resize(0);
			pData->springIndices.resize(0);
			pData->springRestLengths.resize(0);
			pData->springStiffness.resize(0);

			CDoubleArray points;
			geom.GetPointPositions(points);

			for (ULONG i = 0; i < geom.GetPointPositionCount(); i++)
			{
				float x, y, z;
				x = points[nOffset];
				pData->fpoints.push_back(x); nOffset++;
				y = points[nOffset];
				pData->fpoints.push_back(y); nOffset++;
				z = points[nOffset];
				pData->fpoints.push_back(z); nOffset++;
				pData->fpoints.push_back(1.0f);
			}
			
			//int *uniqueVerts, *originalToUniqueMap;
			//NvFlexExtCreateWeldedMeshIndices(fpoints, geom.GetPointPositionCount() ,uniqueVerts,originalToUniqueMap, threshold);
			for (size_t i = 0; i < indices.GetCount(); i++)
			{
				pData->triangleIndices.push_back(indices[i]);
				Application().LogMessage(CString(indices[i]));
			}
			NvFlexExtAsset* flexAsset = NvFlexExtCreateClothFromMesh(pData->fpoints.data(), geom.GetPointPositionCount(), pData->triangleIndices.data(), geom.GetTriangleCount(), stretchStiffnessData[0], bendStiffnessData[0], tetherStiffnessData[0], tetherGiveData[0], pressureData[0]);
			
			pData->numSprings = flexAsset->numSprings;
			pData->nbtriangles = flexAsset->numTriangles;
			pData->springIndices.assign(&flexAsset->springIndices[0], &flexAsset->springIndices[(flexAsset->numSprings*2) ]);
			pData->springRestLengths.assign(&flexAsset->springRestLengths[0], &flexAsset->springRestLengths[flexAsset->numSprings ]);
			pData->springStiffness.assign(&flexAsset->springCoefficients[0], &flexAsset->springCoefficients[flexAsset->numSprings ]);
			pData->inflatable = flexAsset->inflatable;
			pData->inflatablevolume = flexAsset->inflatableVolume;
			pData->inflatablePressure = flexAsset->inflatablePressure;
			pData->inflatableStiffness = flexAsset->inflatableStiffness;

			NvFlexExtDestroyAsset(flexAsset);

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

SICALLBACK PC_FLEX_CREATE_CLOTH_SPRINGS_Evaluate(ICENodeContext& in_ctxt)
{
	// The current output port being evaluated...
	ULONG out_portID = in_ctxt.GetEvaluatedOutputPortID();
	Data * pData = new Data;
	pData = (Data *)(CValue::siPtrType)in_ctxt.GetUserData();
	if (pData->validgeom)
	{
		switch (out_portID)
		{
		case ID_OUT_triangleInds:
		{
			CDataArray2DLong outTrinaglesData(in_ctxt);
			CDataArray2DLong::Accessor outTrinagles = outTrinaglesData.Resize(0, pData->triangleIndices.size());
			for (int x = 0; x <   pData->triangleIndices.size(); ++x)
			{
				outTrinagles[x] = pData->triangleIndices[x];
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
			for (int x = 0; x <  pData->springStiffness.size(); ++x)
			{
				outSprings[x] = pData->springStiffness[x];
			}
		}
		break;
		case ID_OUT_inflatableData: 
		{
			CDataArray2DFloat outInflatData(in_ctxt);
			CDataArray2DFloat::Accessor outInflat = outInflatData.Resize(0, 4);
			outInflat[0] = float(pData->inflatable);
			outInflat[1] = pData->inflatablevolume;
			outInflat[2] = pData->inflatablePressure;
			outInflat[3] = pData->inflatableStiffness;

		}break;
		// Other output ports...
		};
	}

	return CStatus::OK;
}

SICALLBACK PC_FLEX_CREATE_CLOTH_SPRINGS_Term(ICENodeContext& in_ctxt)
{
	Application().LogMessage(CString("PC_FLEX_CREATE_CLOTH_SPRINGS_Term"));
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