#include "..\Helpers.h"

// Defines port, group and map identifiers used for registering the ICENode
enum IDs
{
	ID_IN_solver = 0,
	ID_IN_types = 1,
	ID_IN_transforms = 2,
	ID_In_dynamics = 3,
	ID_IN_geometries = 4,
	ID_IN_Dyngeometries = 5,
	ID_IN_prevTransforms = 6,
	ID_G_100 = 100,
	ID_OUT_solver = 200,
	ID_TYPE_CNS = 400,
	ID_STRUCT_CNS,
	ID_CTXT_CNS,
	ID_UNDEF = ULONG_MAX
};

using namespace XSI;

CStatus RegisterPC_FLEX_ADD_COLLISION_SHAPES(PluginRegistrar& in_reg)
{
	ICENodeDef nodeDef;
	nodeDef = Application().GetFactory().CreateICENodeDef(L"PC_FLEX_ADD_COLLISION_SHAPES", L"PC_FLEX_ADD_COLLISION_SHAPES");

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

	st = nodeDef.AddInputPort(ID_IN_solver, ID_G_100, SolverCustomType, siICENodeStructureSingle, siICENodeContextSingleton, L"inSolver", L"inSolver", ID_UNDEF, ID_UNDEF, ID_UNDEF);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_types, ID_G_100, siICENodeDataLong, siICENodeStructureAny, siICENodeContextSingleton, L"Types", L"Types", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_transforms, ID_G_100, siICENodeDataMatrix44, siICENodeStructureAny, siICENodeContextSingleton, L"transforms", L"transforms", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_prevTransforms, ID_G_100, siICENodeDataMatrix44, siICENodeStructureAny, siICENodeContextSingleton, L"prevTransforms", L"prevTransforms", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_In_dynamics, ID_G_100, siICENodeDataBool, siICENodeStructureAny, siICENodeContextSingleton, L"dynamics", L"dynamics", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();

	st = nodeDef.AddInputPort(ID_IN_geometries, ID_G_100, siICENodeDataGeometry, siICENodeStructureSingle, siICENodeContextSingleton, L"Static geometries", L"Static_geometries", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_Dyngeometries, ID_G_100, siICENodeDataGeometry, siICENodeStructureSingle, siICENodeContextSingleton, L"Dynamic geometries", L"Dynamic_geometries", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	
	// Add output ports.

	st = nodeDef.AddOutputPort(ID_OUT_solver, SolverCustomType, siICENodeStructureSingle, siICENodeContextSingleton, L"Solver", L"Solver", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();

	PluginItem nodeItem = in_reg.RegisterICENode(nodeDef);
	nodeItem.PutCategories(L"Custom ICENode");

	return CStatus::OK;
}

SICALLBACK PC_FLEX_ADD_COLLISION_SHAPES_BeginEvaluate(ICENodeContext& in_ctxt)
{
	CValue userData = in_ctxt.GetUserData();
	CDataArrayCustomType inSolverData(in_ctxt, ID_IN_solver);
	FlexSystem* FlexSys = NULL;
	ULONG inSolverInputSize;
	inSolverData.GetData(0, (const CDataArrayCustomType::TData**)&FlexSys, inSolverInputSize);
	if (userData.IsEmpty())
	{
		if (inSolverInputSize == 0) {
			return CStatus::OK;
		}
		else {
			in_ctxt.PutUserData((CValue::siPtrType)FlexSys);
		}
	}
	else
	{
		if (inSolverInputSize == 0) {
			in_ctxt.PutUserData(CValue());
		}
		else {
			FlexSystem* prevSys = FlexSys;
			FlexSys = (FlexSystem*)(CValue::siPtrType)in_ctxt.GetUserData();
			if (prevSys == FlexSys) {
				Application().LogMessage(CString("PC_FLEX_ADD_COLLISION_SHAPES_usingCachedData"));
			}
			else {
				in_ctxt.PutUserData((CValue::siPtrType)prevSys);
			}
		}
	}

	return CStatus::OK;
}

template < class T >
inline vector<T> ensureArray(ICENodeContext& in_ctxt, ULONG ID) {

	XSI::siICENodeDataType inPortType;
	XSI::siICENodeStructureType inPortStruct;
	XSI::siICENodeContextType inPortContext;
	in_ctxt.GetPortInfo(ID, inPortType, inPortStruct, inPortContext);

	vector<T> arr;
	if (inPortStruct == XSI::siICENodeStructureArray )
	{
		CDataArray2D<T> inData(in_ctxt,ID);
		typename CDataArray2D<T>::Accessor accesor = inData[0];
		for (size_t i = 0; i < accesor.GetCount(); i++)
		{
			arr.push_back(accesor[i]);
		}
	}
	else if (inPortStruct == XSI::siICENodeStructureSingle){
		CDataArray<T> inData(in_ctxt, ID);
		arr.push_back(inData[0]);
	}
	return arr;
}

SICALLBACK PC_FLEX_ADD_COLLISION_SHAPES_Evaluate(ICENodeContext& in_ctxt)
{
	// The current output port being evaluated...
	ULONG out_portID = in_ctxt.GetEvaluatedOutputPortID();

	switch (out_portID)
	{
	case ID_OUT_solver:
	{
		// Get the output port array ...			
		CDataArrayCustomType outData(in_ctxt);
		// Get the input data buffers for each port
		CDataArrayCustomType inSolverData(in_ctxt, ID_IN_solver);

		CICEGeometry Staticgeom(in_ctxt, ID_IN_geometries);
		CICEGeometry Dyngeom(in_ctxt, ID_IN_Dyngeometries);

		// Add code to set output port...
		vector<LONG> types = ensureArray<LONG>(in_ctxt, ID_IN_types);
		vector<XSI::MATH::CMatrix4f> transforms = ensureArray<XSI::MATH::CMatrix4f>(in_ctxt, ID_IN_transforms);
		vector<XSI::MATH::CMatrix4f> prevTransforms = ensureArray<XSI::MATH::CMatrix4f>(in_ctxt, ID_IN_prevTransforms);
		vector<bool> dynamics = ensureArray<bool>(in_ctxt, ID_In_dynamics);
	
		
		//Get the input solver if exist
		CValue userData = in_ctxt.GetUserData();
		
		if (!userData.IsEmpty()) {
			FlexSystem* FlexSys = (FlexSystem*)(CValue::siPtrType)in_ctxt.GetUserData();

			if (!FlexSys->g_buffers->maped)
			{
				FlexSys->g_buffers->MapBuffers();
			}
			FlexSys->ClearShapes();
			
			for (int x = 0; x < transforms.size(); ++x)
			{
				int dInd, tInd;
				dInd = (x < dynamics.size())	?	x : dynamics.size() - 1;
				tInd = (x < types.size())		?	x : types.size() - 1 ;
				XSI::MATH::CMatrix4f prev;
				if (prevTransforms.size() != transforms.size())
				{
					prev = transforms[x];
				}
				else {
					prev = prevTransforms[x];
				}
				
				TransformToCollideShape(FlexSys, transforms[x], prev, dynamics[dInd], types[tInd]);
			}
			
			for (size_t i = 0; i <= Staticgeom.GetSubGeometryCount(); i++)
			{
				CICEGeometry geom;
				geom = (Staticgeom.GetSubGeometryCount()== 0) ? Staticgeom : Staticgeom.GetSubGeometry(i);
				GeoToCollideMesh(FlexSys,geom);
			}

			for (size_t i = 0; i <= Dyngeom.GetSubGeometryCount(); i++)
			{
				CICEGeometry geom, prevgeom;
				geom = (Dyngeom.GetSubGeometryCount() == 0) ? Dyngeom : Dyngeom.GetSubGeometry(i);
				GeoToCollideMesh(FlexSys, geom, true);
			}

			outData.SetData(0, (CDataArrayCustomType::TData*)FlexSys, sizeof(*FlexSys));
		}


	}
	break;

	// Other output ports...
	};

	return CStatus::OK;
}

SICALLBACK PC_FLEX_ADD_COLLISION_SHAPES_Term(ICENodeContext& in_ctxt)
{
	Application().LogMessage(CString("PC_FLEX_ADD_COLLISION_SHAPES_Term"));
	// Release memory allocated in BeginEvaluate
	CValue userData = in_ctxt.GetUserData();
	if (userData.IsEmpty())
	{
		//return CStatus::OK;
	}

	FlexSystem* FlexSys = (FlexSystem*)(CValue::siPtrType)in_ctxt.GetUserData();
	delete FlexSys;
	// Clear user data"] ); 
	in_ctxt.PutUserData(CValue());


	return CStatus::OK;
}