#include "..\Helpers.h"

// Defines port, group and map identifiers used for registering the ICENode
enum IDs
{
	ID_IN_solver = 0,
	ID_G_100 = 100,
	ID_OUT_positions = 200,
	ID_OUT_velocities = 201,
	ID_TYPE_CNS = 400,
	ID_STRUCT_CNS,
	ID_CTXT_CNS,
	ID_UNDEF = ULONG_MAX
};

using namespace XSI;

CStatus RegisterPC_FLEX_GET_PARTICLES(PluginRegistrar& in_reg)
{
	ICENodeDef nodeDef;
	nodeDef = Application().GetFactory().CreateICENodeDef(L"PC_FLEX_GET_PARTICLES", L"PC_FLEX_GET_PARTICLES");

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
	st = nodeDef.AddOutputPort(ID_OUT_positions, siICENodeDataVector4, siICENodeStructureArray, siICENodeContextSingleton, L"positions", L"positions", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddOutputPort(ID_OUT_velocities, siICENodeDataVector3, siICENodeStructureArray, siICENodeContextSingleton, L"velocities", L"velocities",ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();



	PluginItem nodeItem = in_reg.RegisterICENode(nodeDef);
	nodeItem.PutCategories(L"Custom ICENode");

	return CStatus::OK;
}

SICALLBACK PC_FLEX_GET_PARTICLES_BeginEvaluate(ICENodeContext& in_ctxt)
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
				Application().LogMessage(CString("PC_FLEX_GET_PARTICLES_usingCachedData"));
			}
			else {
				in_ctxt.PutUserData((CValue::siPtrType)prevSys);
			}
		}
	}

	return CStatus::OK;
}
SICALLBACK PC_FLEX_GET_PARTICLES_Evaluate(ICENodeContext& in_ctxt)
{
	// The current output port being evaluated...
	ULONG out_portID = in_ctxt.GetEvaluatedOutputPortID();

	switch (out_portID)
	{
	case ID_OUT_positions:
	{
		// Get the output port array ...			
		CDataArray2DVector4f outData(in_ctxt);
		// Get the input data buffers for each port
		CDataArrayCustomType inSolverData(in_ctxt, ID_IN_solver);
		CValue userData = in_ctxt.GetUserData();

		if (!userData.IsEmpty()) {
			FlexSystem* FlexSys = (FlexSystem*)(CValue::siPtrType)in_ctxt.GetUserData();
			FlexSys->g_buffers->MapBuffers();
			CDataArray2DVector4f::Accessor outPositions = outData.Resize(0, FlexSys->g_buffers->activeIndices.size());
			for (int x = 0; x < FlexSys->g_buffers->activeIndices.size(); ++x)
			{
				outPositions[x].Set(FlexSys->g_buffers->positions[FlexSys->g_buffers->activeIndices[x]].x, FlexSys->g_buffers->positions[FlexSys->g_buffers->activeIndices[x]].y, FlexSys->g_buffers->positions[FlexSys->g_buffers->activeIndices[x]].z, FlexSys->g_buffers->positions[FlexSys->g_buffers->activeIndices[x]].w);
			}
			FlexSys->g_buffers->UnmapBuffers();
		}
	}
	break;
	case ID_OUT_velocities:
	{
		// Get the output port array ...			
		CDataArray2DVector3f outData(in_ctxt);
		// Get the input data buffers for each port
		CDataArrayCustomType inSolverData(in_ctxt, ID_IN_solver);
		CValue userData = in_ctxt.GetUserData();

		if (!userData.IsEmpty()) {
			FlexSystem* FlexSys = (FlexSystem*)(CValue::siPtrType)in_ctxt.GetUserData();
			FlexSys->g_buffers->MapBuffers();
			CDataArray2DVector3f::Accessor outvelocities = outData.Resize(0, FlexSys->g_buffers->activeIndices.size());
			for (int x = 0; x < FlexSys->g_buffers->activeIndices.size(); ++x)
			{
				outvelocities[x].Set(FlexSys->g_buffers->velocities[FlexSys->g_buffers->activeIndices[x]].x, FlexSys->g_buffers->velocities[FlexSys->g_buffers->activeIndices[x]].y, FlexSys->g_buffers->velocities[FlexSys->g_buffers->activeIndices[x]].z);
			}
			FlexSys->g_buffers->UnmapBuffers();
		}
	}
	break;
	// Other output ports...
	};

	return CStatus::OK;
}

SICALLBACK PC_FLEX_GET_PARTICLES_Term(ICENodeContext& in_ctxt)
{
	Application().LogMessage(CString("PC_FLEX_GET_PARTICLES_Term"));
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