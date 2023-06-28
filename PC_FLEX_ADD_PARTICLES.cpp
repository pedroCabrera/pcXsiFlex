#include "..\Helpers.h"

// Defines port, group and map identifiers used for registering the ICENode
enum IDs
{
	ID_IN_solver = 0,
	ID_IN_positions = 1,
	ID_IN_velocities = 2,
	ID_In_phases = 3,
	ID_In_name = 4,
	ID_G_100 = 100,
	ID_OUT_solver = 200,
	ID_TYPE_CNS = 400,
	ID_STRUCT_CNS,
	ID_CTXT_CNS,
	ID_UNDEF = ULONG_MAX
};

using namespace XSI;

CStatus RegisterPC_FLEX_ADD_PARTICLES(PluginRegistrar& in_reg)
{
	ICENodeDef nodeDef;
	nodeDef = Application().GetFactory().CreateICENodeDef(L"PC_FLEX_ADD_PARTICLES", L"PC_FLEX_ADD_PARTICLES");

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
	st = nodeDef.AddInputPort(ID_IN_positions, ID_G_100, siICENodeDataVector4, siICENodeStructureArray, siICENodeContextSingleton, L"positions", L"positions", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_velocities, ID_G_100, siICENodeDataVector3, siICENodeStructureArray, siICENodeContextSingleton, L"velocities", L"velocities", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_In_phases, ID_G_100, siICENodeDataLong, siICENodeStructureArray, siICENodeContextSingleton, L"phases", L"phases", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_In_name, ID_G_100, siICENodeDataString, siICENodeStructureSingle, siICENodeContextSingleton, L"name", L"name", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	// Add output ports.

	st = nodeDef.AddOutputPort(ID_OUT_solver, SolverCustomType, siICENodeStructureSingle, siICENodeContextSingleton, L"Solver", L"Solver", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();

	PluginItem nodeItem = in_reg.RegisterICENode(nodeDef);
	nodeItem.PutCategories(L"Custom ICENode");

	return CStatus::OK;
}

SICALLBACK PC_FLEX_ADD_PARTICLES_BeginEvaluate(ICENodeContext& in_ctxt)
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
				Application().LogMessage(CString("PC_FLEX_ADD_PARTICLES_usingCachedData"));
			}
			else {
				in_ctxt.PutUserData((CValue::siPtrType)prevSys);
			}
		}
	}

	return CStatus::OK;
}
SICALLBACK PC_FLEX_ADD_PARTICLES_Evaluate(ICENodeContext& in_ctxt)
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
		CDataArray2DVector4f positionsData(in_ctxt, ID_IN_positions);
		CDataArray2DVector3f velocitiesData(in_ctxt, ID_IN_velocities);
		CDataArray2DLong phasesData(in_ctxt, ID_In_phases);
		CDataArrayString nameData(in_ctxt, ID_In_name);

		// Add code to set output port...
		CDataArray2DVector4f::Accessor positionsIce = positionsData[0];
		CDataArray2DVector3f::Accessor velocitiesIce = velocitiesData[0];
		CDataArray2DLong::Accessor phasesIce = phasesData[0];
		
		//Get the input solver if exist
		CValue userData = in_ctxt.GetUserData();

		if (!userData.IsEmpty()) {
			FlexSystem* FlexSys = (FlexSystem*)(CValue::siPtrType)in_ctxt.GetUserData();
			CString name = nameData[0];
			
			bool found = false;
			int foundInd = -1;
			Application().LogMessage(CString("ADDPARTICLES"));
			for (size_t i = 0; i < FlexSys->g_assets.size(); i++)
			{
				if (FlexSys->g_assets[i].name == name) {
					Application().LogMessage(CString("USING_ASSET__")+ CString(name));
					found = true;
					foundInd = i;
					break;
				}
			}
			if (!found)
			{
				Application().LogMessage(CString("CREATED__ASSET__") + CString(name));
				pcFlexAsset particleAsset;
				particleAsset.name = name;
				foundInd = FlexSys->nAssets;
				FlexSys->nAssets += 1;
				FlexSys->g_assets.push_back(particleAsset);
			}

			int phase;

			FlexSys->g_assets[foundInd].fpoints.resize(0);
			FlexSys->g_assets[foundInd].fvelocities.resize(0);
			FlexSys->g_assets[foundInd].phases.resize(0);
			FlexSys->g_assets[foundInd].nbpoints = positionsIce.GetCount();
			for (int x = 0; x < positionsIce.GetCount(); ++x)
			{

				int vInd, pInd;
				vInd = (x <= velocitiesIce.GetCount()) ? x : velocitiesIce.GetCount() - 1;
				pInd = (x <= phasesIce.GetCount()) ? x : phasesIce.GetCount() - 1;
				Vec3 pos, vel;
				//pos.Set(positionsIce[x].GetX(), positionsIce[x].GetY(), positionsIce[x].GetZ());
				vel.Set(velocitiesIce[vInd].GetX(), velocitiesIce[vInd].GetY(), velocitiesIce[vInd].GetZ());
				if (phasesIce[pInd] == 0)
				{
					phase = NvFlexMakePhase(0, eNvFlexPhaseSelfCollide | eNvFlexPhaseFluid);
				}
				else {
					phase = NvFlexMakePhase(1, eNvFlexPhaseSelfCollide);
				}
				FlexSys->g_assets[foundInd].fpoints.push_back(positionsIce[x].GetX());//[x]=positionsIce[x].GetX();
				FlexSys->g_assets[foundInd].fpoints.push_back(positionsIce[x].GetY());
				FlexSys->g_assets[foundInd].fpoints.push_back(positionsIce[x].GetZ());
				FlexSys->g_assets[foundInd].fpoints.push_back(positionsIce[x].GetW());
				FlexSys->g_assets[foundInd].fvelocities.push_back(vel);
				FlexSys->g_assets[foundInd].phases.push_back(phase);
			}
			
		
			outData.SetData(0, (CDataArrayCustomType::TData*)FlexSys, sizeof(*FlexSys));
		}


	}
	break;

	// Other output ports...
	};

	return CStatus::OK;
}

SICALLBACK PC_FLEX_ADD_PARTICLES_Term(ICENodeContext& in_ctxt)
{
	Application().LogMessage(CString("PC_FLEX_ADD_PARTICLES_Term"));
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