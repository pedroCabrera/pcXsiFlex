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

CStatus RegisterPC_FLEX_GET_DIFFPARTICLES(PluginRegistrar& in_reg)
{
	ICENodeDef nodeDef;
	nodeDef = Application().GetFactory().CreateICENodeDef(L"PC_FLEX_GET_DIFFPARTICLES", L"PC_FLEX_GET_DIFFUSE_PARTICLES");

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
	st = nodeDef.AddOutputPort(ID_OUT_velocities, siICENodeDataVector3, siICENodeStructureArray, siICENodeContextSingleton, L"velocities", L"velocities", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();



	PluginItem nodeItem = in_reg.RegisterICENode(nodeDef);
	nodeItem.PutCategories(L"PC_Tools/Flex");

	return CStatus::OK;
}

SICALLBACK PC_FLEX_GET_DIFFPARTICLES_BeginEvaluate(ICENodeContext& in_ctxt)
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
			if (prevSys != FlexSys) {
				in_ctxt.PutUserData((CValue::siPtrType)prevSys);
			}
		}
	}

	return CStatus::OK;
}
SICALLBACK PC_FLEX_GET_DIFFPARTICLES_Evaluate(ICENodeContext& in_ctxt)
{
	// The current output port being evaluated...
	ULONG out_portID = in_ctxt.GetEvaluatedOutputPortID();
	CValue userData = in_ctxt.GetUserData();
	FlexSystem* FlexSys = NULL;
	int numDiffuse = 0;
	if (!userData.IsEmpty()) {
		FlexSys = (FlexSystem*)(CValue::siPtrType)in_ctxt.GetUserData();
		numDiffuse = NvFlexGetDiffuseParticles(FlexSys->g_flex, NULL, NULL, NULL);
	}
	if (numDiffuse != 0)
	{
		switch (out_portID)
		{
		case ID_OUT_positions:
		{
			// Get the output port array ...			
			CDataArray2DVector4f outData(in_ctxt);
			if (!userData.IsEmpty()) {
				CDataArray2DVector4f::Accessor outPositions = outData.Resize(0, FlexSys->g_buffers->diffusePositions.size());
				for (int x = 0; x < FlexSys->g_buffers->diffusePositions.size(); ++x)
				{
					//outPositions[x].Set(FlexSys->g_buffers->diffusePositions[x].x, FlexSys->g_buffers->diffusePositions[x].y, FlexSys->g_buffers->diffusePositions[x].z, FlexSys->g_buffers->diffusePositions[x].w);
					outPositions[x] = MATH::CVector4f(FlexSys->g_buffers->diffusePositions[x].x, FlexSys->g_buffers->diffusePositions[x].y, FlexSys->g_buffers->diffusePositions[x].z, 1);
					// .Set(FlexSys->g_buffers->diffusePositions[FlexSys->g_buffers->diffuseIndices[x]].x, FlexSys->g_buffers->diffusePositions[FlexSys->g_buffers->diffuseIndices[x]].y, FlexSys->g_buffers->diffusePositions[FlexSys->g_buffers->diffuseIndices[x]].z, FlexSys->g_buffers->diffusePositions[FlexSys->g_buffers->diffuseIndices[x]].w);
				}
			}
		}
		break;
		case ID_OUT_velocities:
		{
			// Get the output port array ...			
			CDataArray2DVector3f outData(in_ctxt);
			if (!userData.IsEmpty()) {
				CDataArray2DVector3f::Accessor outvelocities = outData.Resize(0, numDiffuse);
				for (int x = 0; x < numDiffuse; ++x)
				{
					outvelocities[x].Set(FlexSys->g_buffers->diffuseVelocities[FlexSys->g_buffers->diffuseIndices[x]].x, FlexSys->g_buffers->diffuseVelocities[FlexSys->g_buffers->diffuseIndices[x]].y, FlexSys->g_buffers->diffuseVelocities[FlexSys->g_buffers->diffuseIndices[x]].z);
				}
			}
		}
		break;
		// Other output ports...
		};
	}

	return CStatus::OK;
}

SICALLBACK PC_FLEX_GET_DIFFPARTICLES_Term(ICENodeContext& in_ctxt)
{
	//Application().LogMessage(CString("PC_FLEX_GET_PARTICLES_Term"));
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