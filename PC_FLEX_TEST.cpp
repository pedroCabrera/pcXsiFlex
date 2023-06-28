#include "..\Helpers.h"

// Defines port, group and map identifiers used for registering the ICENode
enum IDs
{
	ID_IN_solver = 0,
	ID_G_100 = 100,
	ID_OUT_Test = 200,
	ID_TYPE_CNS = 400,
	ID_STRUCT_CNS,
	ID_CTXT_CNS,
	ID_UNDEF = ULONG_MAX
};

using namespace XSI;

CStatus RegisterPC_FLEX_TEST(PluginRegistrar& in_reg)
{
	ICENodeDef nodeDef;
	nodeDef = Application().GetFactory().CreateICENodeDef(L"PC_FLEX_TEST", L"PC_FLEX_TEST");

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
	// Add output ports.

	st = nodeDef.AddOutputPort(ID_OUT_Test, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"Test", L"Test", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();

	PluginItem nodeItem = in_reg.RegisterICENode(nodeDef);
	nodeItem.PutCategories(L"Custom ICENode");

	return CStatus::OK;
}

SICALLBACK PC_FLEX_TEST_BeginEvaluate(ICENodeContext& in_ctxt)
{
	//Application().LogMessage(CString("BEGIN_EVALUATE"));
	CValue userData = in_ctxt.GetUserData();
	CDataArrayCustomType inSolverData(in_ctxt, ID_IN_solver);

	FlexSystem* FlexSys = NULL;

	ULONG inSolverInputSize;
	inSolverData.GetData(0, (const CDataArrayCustomType::TData**)&FlexSys, inSolverInputSize);
	if (userData.IsEmpty())
	{
		if (inSolverInputSize == 0) {
			Application().LogMessage(CString("PC_FLEX_TEST_Not SOlver Detected"));
			return CStatus::OK;
		}
		else {
			Application().LogMessage(CString("PC_FLEX_TEST_inputSolverDetected"));
			in_ctxt.PutUserData((CValue::siPtrType)FlexSys);
		}
	}
	else
	{
		if (inSolverInputSize == 0) {
			Application().LogMessage(CString("PC_FLEX_TEST_Not SOlver Detected"));
			in_ctxt.PutUserData(CValue());
		}
		else {
			FlexSystem* prevSys = FlexSys;
			
			FlexSys = (FlexSystem*)(CValue::siPtrType)in_ctxt.GetUserData();
			Application().LogMessage("PC_FLEX_TEST"+CString(prevSys == FlexSys));
			if (prevSys == FlexSys) {
				Application().LogMessage(CString("PC_FLEX_TEST_usingCachedData"));
			}
			else {
				Application().LogMessage(CString("PC_FLEX_TEST_usingInput"));
				in_ctxt.PutUserData((CValue::siPtrType)prevSys);
			}

		}
	}

	return CStatus::OK;
}
SICALLBACK PC_FLEX_TEST_Evaluate(ICENodeContext& in_ctxt)
{
	// The current output port being evaluated...
	ULONG out_portID = in_ctxt.GetEvaluatedOutputPortID();

	switch (out_portID)
	{
	case ID_OUT_Test:
	{
		// Get the output port array ...			
		CDataArrayFloat outData(in_ctxt);
		// Get the input data buffers for each port

			// Add code to set output port...
		CValue userData = in_ctxt.GetUserData();
		if (!userData.IsEmpty()) {
			FlexSystem* FlexSys = (FlexSystem*)(CValue::siPtrType)in_ctxt.GetUserData();
			//Application().LogMessage(CString(FlexSys->g_buffers->positions[0].x));
			//Application().LogMessage(CString(FlexSys->g_buffers->positions[0].y));
			//Application().LogMessage(CString(FlexSys->g_buffers->positions[0].z));
			Application().LogMessage(CString(FlexSys->g_params.radius));
			outData[0] = FlexSys->maxParticles;

		}


	}
	break;

	// Other output ports...
	};

	return CStatus::OK;
}SICALLBACK PC_FLEX_TEST_Term(ICENodeContext& in_ctxt)
{
	Application().LogMessage(CString("PC_FLEX_SET_SOLVER_PARAMS_TERM"));
	// Release memory allocated in BeginEvaluate
	CValue userData = in_ctxt.GetUserData();
	if (userData.IsEmpty())
	{
		return CStatus::OK;
	}

	FlexSystem* FlexSys = (FlexSystem*)(CValue::siPtrType)in_ctxt.GetUserData();
	delete FlexSys;
	// Clear user data"] ); 
	in_ctxt.PutUserData(CValue());


	return CStatus::OK;
}