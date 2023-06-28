#include "..\Helpers.h"


// Defines port, group and map identifiers used for registering the ICENode
enum IDs
{
	ID_IN_maxParticles = 47,
	ID_IN_maxDiffuseParticles = 48,
	ID_IN_maxNeighborsPerParticle = 49,
	ID_IN_reset = 50,
	ID_IN_flush = 51,

	ID_G_100 = 100,
	ID_OUT_Solver = 200,
	ID_TYPE_CNS = 400,
	ID_STRUCT_CNS,
	ID_CTXT_CNS,
	ID_UNDEF = ULONG_MAX
};

using namespace XSI;
using namespace std;

CStatus RegisterPC_FLEX_SOLVER(PluginRegistrar& in_reg)
{
	ICENodeDef nodeDef;
	nodeDef = Application().GetFactory().CreateICENodeDef(L"PC_FLEX_SOLVER", L"PC_FLEX_SOLVER");

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

	st = nodeDef.AddInputPort(ID_IN_reset, ID_G_100, siICENodeDataBool, siICENodeStructureSingle, siICENodeContextSingleton, L"reset", L"reset", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_flush, ID_G_100, siICENodeDataBool, siICENodeStructureSingle, siICENodeContextSingleton, L"flush", L"flush", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_maxParticles, ID_G_100, siICENodeDataLong, siICENodeStructureSingle, siICENodeContextSingleton, L"maxParticles", L"maxParticles", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_maxDiffuseParticles, ID_G_100, siICENodeDataLong, siICENodeStructureSingle, siICENodeContextSingleton, L"maxDiffuseParticles", L"maxDiffuseParticles", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_maxNeighborsPerParticle, ID_G_100, siICENodeDataLong, siICENodeStructureSingle, siICENodeContextSingleton, L"maxNeighborsPerParticle", L"maxNeighborsPerParticle", 96, CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	// Add output ports.

	st = nodeDef.AddOutputPort(ID_OUT_Solver,SolverCustomType,siICENodeStructureSingle,siICENodeContextSingleton,L"Solver",L"Solver",ID_UNDEF,ID_UNDEF,ID_CTXT_CNS);
	//st = nodeDef.AddOutputPort(ID_OUT_Solver, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"Solver", L"Solver", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();

	PluginItem nodeItem = in_reg.RegisterICENode(nodeDef);
	nodeItem.PutCategories(L"Custom ICENode");

	return CStatus::OK;
}

SICALLBACK PC_FLEX_SOLVER_BeginEvaluate(ICENodeContext& in_ctxt)
{
	CValue userData = in_ctxt.GetUserData();

	CDataArrayLong maxParticlesData(in_ctxt, ID_IN_maxParticles);
	CDataArrayLong maxDiffuseParticlesData(in_ctxt, ID_IN_maxDiffuseParticles);
	CDataArrayLong maxNeighborsPerParticleData(in_ctxt, ID_IN_maxNeighborsPerParticle);

	CDataArrayBool resetData(in_ctxt, ID_IN_reset);
	CDataArrayBool flushData(in_ctxt, ID_IN_flush);

	FlexSystem* FlexSys = NULL;
	NvFlexLibrary* g_FlexLib = NULL;

	CRefArray plugins = Application().GetPlugins();//.Plugins("My plugin");
	CRef reference = plugins.GetItem("PC_FLEX_Plugin");
	Plugin plugin(reference);

	g_FlexLib = (NvFlexLibrary*)(CValue::siPtrType)plugin.GetUserData();

	if (userData.IsEmpty())
	{
		if (!flushData[0])
		{
			FlexSys = new FlexSystem();
			FlexSys->initScene(g_FlexLib, maxParticlesData[0], maxDiffuseParticlesData[0], maxNeighborsPerParticleData[0]);
			in_ctxt.PutUserData((CValue::siPtrType)FlexSys);
		}
	}
	else
	{
		FlexSys = (FlexSystem*)(CValue::siPtrType)in_ctxt.GetUserData();

	}
	if (flushData[0])
	{
		delete FlexSys;
		in_ctxt.PutUserData(CValue());
		return CStatus::OK;
	}

	if (resetData[0])
	{
		FlexSys->initScene(g_FlexLib, maxParticlesData[0], maxDiffuseParticlesData[0], maxNeighborsPerParticleData[0]);
	}


	return CStatus::OK;
}
SICALLBACK PC_FLEX_SOLVER_Evaluate(ICENodeContext& in_ctxt)
{
	ULONG out_portID = in_ctxt.GetEvaluatedOutputPortID();
	in_ctxt.SetAsTimeVarying();
	switch (out_portID)
	{
		case ID_OUT_Solver:{	
			CValue userData = in_ctxt.GetUserData();
			if (!userData.IsEmpty())
			{
				CDataArrayCustomType outData(in_ctxt);
				FlexSystem* FlexSys = (FlexSystem*)(CValue::siPtrType)in_ctxt.GetUserData();		
				outData.SetData(0, (CDataArrayCustomType::TData*)FlexSys, sizeof(*FlexSys));
			}
		}break;
	};

	return CStatus::OK;
}
#include <time.h>
SICALLBACK PC_FLEX_SOLVER_Term(ICENodeContext& in_ctxt)
{
	Application().LogMessage(CString("PC_FLEX_SOLVER_TERM"));
	// Release memory allocated in BeginEvaluate
	CValue userData = in_ctxt.GetUserData();
	if (userData.IsEmpty())
	{
		Application().LogMessage(CString("PC_FLEX_SOLVER_TERM_empty?"));
		//return CStatus::OK;
	}

	FlexSystem* FlexSys = (FlexSystem*)(CValue::siPtrType)in_ctxt.GetUserData();
	//Application().LogMessage(CString("TEST"+FlexSys->activeParticles));
	delete FlexSys;
	// Clear user data"] ); 
	in_ctxt.PutUserData(CValue());
	return CStatus::OK;
}