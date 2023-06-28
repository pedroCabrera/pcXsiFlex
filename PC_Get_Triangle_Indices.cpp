#include "..\Helpers.h"

// Defines port, group and map identifiers used for registering the ICENode
enum IDs
{
	ID_IN_geometries = 1,
	ID_G_100 = 100,
	ID_OUT_triangleInds = 203,
	ID_TYPE_CNS = 400,
	ID_STRUCT_CNS,
	ID_CTXT_CNS,
	ID_UNDEF = ULONG_MAX

};

using namespace XSI;

CStatus RegisterPC_Get_Triangle_Indices(PluginRegistrar& in_reg)
{
	ICENodeDef nodeDef;
	nodeDef = Application().GetFactory().CreateICENodeDef(L"PC_Get_Triangle_Indices", L"PC_Get_Triangle_Indices");

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

	// Add output ports.
	st = nodeDef.AddOutputPort(ID_OUT_triangleInds, siICENodeDataLong, siICENodeStructureArray, siICENodeContextSingleton, L"triangleIndex", L"triangleIndex", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();

	PluginItem nodeItem = in_reg.RegisterICENode(nodeDef);
	nodeItem.PutCategories(L"PC_Tools/Flex");

	return CStatus::OK;
}
struct Data
{
public:
	int nbtriangles = 0;
	int validgeom = 0;
	std::vector<int> triangleIndices;
};

SICALLBACK PC_Get_Triangle_Indices_BeginEvaluate(ICENodeContext& in_ctxt)
{

	CICEGeometry geom(in_ctxt, ID_IN_geometries);
	CICEPortState geoport(in_ctxt, ID_IN_geometries);

	CValue userData = in_ctxt.GetUserData();
	Data * pData = new Data;

	if (geom.GetPointPositionCount() == 0 )//i.e. there is some valid geo otherwise we get crashes
	{
		if (!in_ctxt.GetUserData().IsEmpty())//if it's not empty, empty it
		{
			Data * pData = (Data *)(CValue::siPtrType)in_ctxt.GetUserData();
			delete pData;
			in_ctxt.PutUserData((CValue::siPtrType) NULL);
		}
		return CStatus::OK;//exit out of BeginEvaluate
	}
	bool dirty = false;
	dirty = (in_ctxt.GetUserData().IsEmpty() || geoport.IsDirty(CICEGeometry::siTopologyDirtyState));
	if (dirty)
	{
		//Application().LogMessage(CString("PC_Get Trinagle Index Dirty"));
		if (geom.GetPointPositionCount() != 0)
		{
			CLongArray indices;
			geom.GetTrianglePointIndices(indices);
			ULONG nOffset = 0;

			pData->triangleIndices.resize(0);
			for (size_t i = 0; i < indices.GetCount(); i++)
			{
				pData->triangleIndices.push_back(indices[i]);
			}

			pData->nbtriangles = geom.GetTriangleCount();

			pData->validgeom = 1;
			geoport.ClearState();

			//Application().LogMessage(CString(geom.GetSubGeometryCount()));
			in_ctxt.PutUserData((CValue::siPtrType) pData);
		}

	}
	else
	{
		Data * pData = (Data *)(CValue::siPtrType)in_ctxt.GetUserData();
	}
	return CStatus::OK;
}

SICALLBACK PC_Get_Triangle_Indices_Evaluate(ICENodeContext& in_ctxt)
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
		case ID_OUT_triangleInds:
		{
			CDataArray2DLong outTrinaglesData(in_ctxt);
			CDataArray2DLong::Accessor outTrinagles = outTrinaglesData.Resize(0, pData->triangleIndices.size());
			for (int x = 0; x < pData->triangleIndices.size(); ++x)
			{
				outTrinagles[x] = pData->triangleIndices[x];
			}
		}
		break;
		// Other output ports...
		};
	}

	return CStatus::OK;
}

SICALLBACK PC_Get_Triangle_Indices_Term(ICENodeContext& in_ctxt)
{
	//Application().LogMessage(CString("PC_FLEX_CREATE_CLOTH_SPRINGS_Term"));
	Context ctxt(in_ctxt);
	CValue userData = ctxt.GetUserData();
	if (userData.IsEmpty())
	{
		return CStatus::OK;
	}
	Data *pData = (Data*)(CValue::siPtrType)ctxt.GetUserData();

	pData->triangleIndices.clear();
	delete pData;

	// Clear user data
	ctxt.PutUserData(CValue());

	return CStatus::OK;

}