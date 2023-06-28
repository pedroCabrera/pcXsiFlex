#include "..\Helpers.h"

// Defines port, group and map identifiers used for registering the ICENode
enum IDs
{
	ID_IN_solver = 0,
	ID_IN_positions = 1,
	ID_IN_velocities = 2,
	ID_In_phases = 3,
	ID_In_name = 4,
	ID_In_springIndices = 5,
	ID_In_springLengths = 6,
	ID_IN_springStiffness = 7,
	ID_IN_triangleIndices = 8,
	ID_IN_restPositions = 9,
	ID_In_Tooglers = 10,
	ID_In_Inflat = 11,
	ID_In_shapeInds = 12,
	ID_In_shapeOff = 13,
	ID_In_shapeCent = 14,
	ID_In_shapeCoef = 15,
	ID_In_shapeOris = 16,
	ID_In_shaperestLocal = 17,

	ID_G_100 = 100,
	ID_OUT_solver = 200,
	ID_TYPE_CNS = 400,
	ID_STRUCT_CNS,
	ID_CTXT_CNS,
	ID_UNDEF = ULONG_MAX
};

using namespace XSI;

CStatus RegisterPC_FLEX_CREATE_ASSET(PluginRegistrar& in_reg)
{
	ICENodeDef nodeDef;
	nodeDef = Application().GetFactory().CreateICENodeDef(L"PC_FLEX_CREATE_ASSET", L"PC_FLEX_CREATE_ASSET");

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
	st = nodeDef.AddInputPort(ID_In_name, ID_G_100, siICENodeDataString, siICENodeStructureSingle, siICENodeContextSingleton, L"name", L"name", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_In_Tooglers, ID_G_100, siICENodeDataBool, siICENodeStructureArray, siICENodeContextSingleton, L"tooglers", L"tooglers", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_positions, ID_G_100, siICENodeDataVector4, siICENodeStructureArray, siICENodeContextSingleton, L"positions", L"positions", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_restPositions, ID_G_100, siICENodeDataVector4, siICENodeStructureArray, siICENodeContextSingleton, L"restPositions", L"restPositions", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_velocities, ID_G_100, siICENodeDataVector3, siICENodeStructureArray, siICENodeContextSingleton, L"velocities", L"velocities", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_In_phases, ID_G_100, siICENodeDataLong, siICENodeStructureArray, siICENodeContextSingleton, L"phases", L"phases", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_In_springIndices, ID_G_100, siICENodeDataLong, siICENodeStructureArray, siICENodeContextSingleton, L"springIndices", L"springIndices", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_In_springLengths, ID_G_100, siICENodeDataFloat, siICENodeStructureArray, siICENodeContextSingleton, L"springLengths", L"springLengths", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_springStiffness, ID_G_100, siICENodeDataFloat, siICENodeStructureArray, siICENodeContextSingleton, L"springStiffness", L"springStiffness", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_triangleIndices, ID_G_100, siICENodeDataLong, siICENodeStructureArray, siICENodeContextSingleton, L"triangleIndices", L"triangleIndices", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_In_Inflat, ID_G_100, siICENodeDataFloat, siICENodeStructureArray, siICENodeContextSingleton, L"inflatableData", L"inflatableData", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_In_shapeInds, ID_G_100, siICENodeDataLong, siICENodeStructureArray, siICENodeContextSingleton, L"shapeIndices", L"shapeIndices", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_In_shapeOff, ID_G_100, siICENodeDataLong, siICENodeStructureArray, siICENodeContextSingleton, L"shapeOffsets", L"shapeOffsets", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_In_shapeCent, ID_G_100, siICENodeDataVector3, siICENodeStructureArray, siICENodeContextSingleton, L"shapeCenters", L"shapeCenters", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_In_shapeOris, ID_G_100, siICENodeDataQuaternion, siICENodeStructureArray, siICENodeContextSingleton, L"shapeRots", L"shapeRots", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_In_shapeCoef, ID_G_100, siICENodeDataFloat, siICENodeStructureArray, siICENodeContextSingleton, L"shapeCoefs", L"shapeCoefs", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_In_shaperestLocal, ID_G_100, siICENodeDataVector3, siICENodeStructureArray, siICENodeContextSingleton, L"shaperestLocal", L"shaperestLocal", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	// Add output ports.

	st = nodeDef.AddOutputPort(ID_OUT_solver, SolverCustomType, siICENodeStructureSingle, siICENodeContextSingleton, L"Solver", L"Solver", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();

	PluginItem nodeItem = in_reg.RegisterICENode(nodeDef);
	nodeItem.PutCategories(L"Custom ICENode");

	return CStatus::OK;
}

SICALLBACK PC_FLEX_CREATE_ASSET_BeginEvaluate(ICENodeContext& in_ctxt)
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
				Application().LogMessage(CString("PC_FLEX_CREATE_ASSET_usingCachedData"));
			}
			else {
				in_ctxt.PutUserData((CValue::siPtrType)prevSys);
			}
		}
	}

	return CStatus::OK;
}
SICALLBACK PC_FLEX_CREATE_ASSET_Evaluate(ICENodeContext& in_ctxt)
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
		CDataArray2DVector4f restPositionsData(in_ctxt, ID_IN_restPositions);
		CDataArray2DVector3f velocitiesData(in_ctxt, ID_IN_velocities);
		CDataArray2DLong phasesData(in_ctxt, ID_In_phases);
		CDataArrayString nameData(in_ctxt, ID_In_name);
		CDataArray2DFloat inflatData(in_ctxt, ID_In_Inflat);

		CDataArray2DLong spIndData(in_ctxt, ID_In_springIndices);
		CDataArray2DFloat spdLenData(in_ctxt, ID_In_springLengths);
		CDataArray2DFloat spStifData(in_ctxt, ID_IN_springStiffness);
		CDataArray2DLong triangleData(in_ctxt, ID_IN_triangleIndices);

		CDataArray2DLong shapeIndsData(in_ctxt, ID_In_shapeInds);
		CDataArray2DLong shapeOffData(in_ctxt, ID_In_shapeOff);
		CDataArray2DVector3f shapeCentData(in_ctxt, ID_In_shapeCent);
		CDataArray2DFloat shapeCoefData(in_ctxt, ID_In_shapeCoef);
		CDataArray2DQuaternionf shapeOriData(in_ctxt, ID_In_shapeOris);
		CDataArray2DVector3f shapeRestData(in_ctxt, ID_In_shaperestLocal);
		

		CDataArray2DBool toogleData(in_ctxt, ID_In_Tooglers);
		
		// Add code to set output port...
		CDataArray2DVector4f::Accessor positionsIce = positionsData[0];
		CDataArray2DVector4f::Accessor restPositionsIce = restPositionsData[0];
		CDataArray2DVector3f::Accessor velocitiesIce = velocitiesData[0];
		CDataArray2DLong::Accessor phasesIce = phasesData[0];

		CDataArray2DLong::Accessor spIndIce = spIndData[0];
		CDataArray2DFloat::Accessor spdLenIce = spdLenData[0];
		CDataArray2DFloat::Accessor spStifIce = spStifData[0];
		CDataArray2DLong::Accessor trianglesIce = triangleData[0];

		CDataArray2DLong::Accessor shapeIndsIce = shapeIndsData[0];
		CDataArray2DLong::Accessor shapeOffIce = shapeOffData[0];
		CDataArray2DVector3f::Accessor shapeCentIce = shapeCentData[0];
		CDataArray2DFloat::Accessor shapeCoefIce = shapeCoefData[0];
		CDataArray2DQuaternionf::Accessor shapeOriIce = shapeOriData[0];
		CDataArray2DVector3f::Accessor shapeRestIce = shapeRestData[0];

		CDataArray2DBool::Accessor toogleIce = toogleData[0];
		CDataArray2DFloat::Accessor inflatIce = inflatData[0];

		//Get the input solver if exist
		CValue userData = in_ctxt.GetUserData();

		if (!userData.IsEmpty()) {
			FlexSystem* FlexSys = (FlexSystem*)(CValue::siPtrType)in_ctxt.GetUserData();
			CString name = nameData[0];

			bool found = false;
			int foundInd = -1;
			for (size_t i = 0; i < FlexSys->g_assets.size(); i++)
			{
				if (FlexSys->g_assets[i].name == name) {
					Application().LogMessage(CString("USING_ASSET__") + CString(name));
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

			if (toogleIce.GetCount()!=9)
			{
				return CStatus::OK;
			}
			//set positions
			if (toogleIce[0]) {
				FlexSys->g_assets[foundInd].fpoints.resize(0);
				FlexSys->g_assets[foundInd].nbpoints = positionsIce.GetCount();
				for (int x = 0; x < positionsIce.GetCount(); ++x)
				{
					FlexSys->g_assets[foundInd].fpoints.push_back(positionsIce[x].GetX());
					FlexSys->g_assets[foundInd].fpoints.push_back(positionsIce[x].GetY());
					FlexSys->g_assets[foundInd].fpoints.push_back(positionsIce[x].GetZ());
					FlexSys->g_assets[foundInd].fpoints.push_back(positionsIce[x].GetW());
					if (!toogleIce[1])
					{
						if (FlexSys->g_assets[foundInd].frestpoints.size()/4 != positionsIce.GetCount() && x>= FlexSys->g_assets[foundInd].frestpoints.size() / 4)
						{
							FlexSys->g_assets[foundInd].frestpoints.push_back(positionsIce[x].GetX()*1.0001f);
							FlexSys->g_assets[foundInd].frestpoints.push_back(positionsIce[x].GetY()*1.0001f);
							FlexSys->g_assets[foundInd].frestpoints.push_back(positionsIce[x].GetZ()*1.0001f);
							FlexSys->g_assets[foundInd].frestpoints.push_back(positionsIce[x].GetW());
						}
					}
				}
			}
			//set restpositions
			if (toogleIce[1]) {
				FlexSys->g_assets[foundInd].frestpoints.resize(0);
				for (int x = 0; x < restPositionsIce.GetCount(); ++x)
				{
					FlexSys->g_assets[foundInd].frestpoints.push_back(restPositionsIce[x].GetX()*1.0001f);
					FlexSys->g_assets[foundInd].frestpoints.push_back(restPositionsIce[x].GetY()*1.0001f);
					FlexSys->g_assets[foundInd].frestpoints.push_back(restPositionsIce[x].GetZ()*1.0001f);
					FlexSys->g_assets[foundInd].frestpoints.push_back(restPositionsIce[x].GetW());
				}
			}
			//set velocities
			if (toogleIce[2]) {
				FlexSys->g_assets[foundInd].fvelocities.resize(0);
				for (int x = 0; x < velocitiesIce.GetCount(); ++x)
				{
					FlexSys->g_assets[foundInd].fvelocities.push_back(Vec3(velocitiesIce[x].GetX(), velocitiesIce[x].GetY(), velocitiesIce[x].GetZ()));
				}
			}
			//set phases
			if (toogleIce[3]) {
				int phase;
				FlexSys->g_assets[foundInd].phases.resize(0);
				for (int x = 0; x < phasesIce.GetCount(); ++x)
				{
					if (phasesIce[x] == 0)		// create a fluid phase that self collides and generates density constraints
						phase = NvFlexMakePhase(0, eNvFlexPhaseSelfCollide  | eNvFlexPhaseFluid);
					else {
						if (phasesIce[x] < 0)	// create a rigid phase that collides against other groups, but does not self-collide
							phase = NvFlexMakePhase(-phasesIce[x],0);
						else					// create a cloth phase that collides against other groups and itself
							phase = NvFlexMakePhase(phasesIce[x], eNvFlexPhaseSelfCollide | eNvFlexPhaseSelfCollideFilter);
					}
					FlexSys->g_assets[foundInd].phases.push_back(phase);
				}
			}
			//set springIndex
			if (toogleIce[4]) {
				FlexSys->g_assets[foundInd].springIndices.resize(0);
				FlexSys->g_assets[foundInd].numSprings = spIndIce.GetCount()/2;
				for (int x = 0; x < spIndIce.GetCount(); ++x)
				{
					FlexSys->g_assets[foundInd].springIndices.push_back(spIndIce[x]);
				}
			}
			//set springLengths
			if (toogleIce[5]) {
				FlexSys->g_assets[foundInd].springRestLengths.resize(0);
				for (int x = 0; x < spdLenIce.GetCount(); ++x)
				{
					FlexSys->g_assets[foundInd].springRestLengths.push_back(spdLenIce[x]);
				}
			}
			//set springStiff
			if (toogleIce[6]) {
				FlexSys->g_assets[foundInd].springStiffness.resize(0);
				for (int x = 0; x < spStifIce.GetCount(); ++x)
				{
					FlexSys->g_assets[foundInd].springStiffness.push_back(spStifIce[x]);
				}
			}
			//set triangles
			if (toogleIce[7]) {
				FlexSys->g_assets[foundInd].triangleIndices.resize(0);
				FlexSys->g_assets[foundInd].nbtriangles = trianglesIce.GetCount()/3;
				for (int x = 0; x < trianglesIce.GetCount(); ++x)
				{
					FlexSys->g_assets[foundInd].triangleIndices.push_back(trianglesIce[x]);
				}
				if (inflatIce.GetCount() == 4)
				{
					if (inflatIce[0]==1 && inflatIce[1]>0)
					{
						FlexSys->g_assets[foundInd].inflatRest = inflatIce[1];
						FlexSys->g_assets[foundInd].inflatpressure = inflatIce[2];
						FlexSys->g_assets[foundInd].inflatcoef = inflatIce[3];
						FlexSys->g_assets[foundInd].inflatstartTri = 0;
					}
				}
			}
			//set Shapes
			if (toogleIce[8]) {
				if (shapeCentIce.GetCount() == shapeOffIce.GetCount() == shapeCoefIce.GetCount() == shapeOriIce.GetCount()  && shapeIndsIce.GetCount() != 0)//shapeRestIce.GetCount() )
				{
					FlexSys->g_assets[foundInd].shapeIndices.resize(0);
					FlexSys->g_assets[foundInd].shapeOffsets.resize(0);
					FlexSys->g_assets[foundInd].shapeCoefficients.resize(0);
					FlexSys->g_assets[foundInd].shapeCenters.resize(0);
					FlexSys->g_assets[foundInd].shapeRots.resize(0);
					FlexSys->g_assets[foundInd].numShapeIndices = shapeCentIce.GetCount();

					for (int x = 0; x < shapeIndsIce.GetCount(); ++x)
					{
						FlexSys->g_assets[foundInd].shapeIndices.push_back(shapeIndsIce[x]);
						FlexSys->g_assets[foundInd].rigidLocalPos.push_back(Vec3(shapeRestIce[x].GetX(), shapeRestIce[x].GetY(), shapeRestIce[x].GetZ()));
					}
					for (int x = 0; x < shapeOffIce.GetCount(); ++x)
					{
						FlexSys->g_assets[foundInd].shapeOffsets.push_back(shapeOffIce[x]);
						FlexSys->g_assets[foundInd].shapeCenters.push_back(shapeCentIce[x].GetX());
						FlexSys->g_assets[foundInd].shapeCenters.push_back(shapeCentIce[x].GetY());
						FlexSys->g_assets[foundInd].shapeCenters.push_back(shapeCentIce[x].GetZ());
						FlexSys->g_assets[foundInd].shapeCoefficients.push_back(shapeCoefIce[x]);

						FlexSys->g_assets[foundInd].shapeRots.push_back(shapeOriIce[x].GetX());
						FlexSys->g_assets[foundInd].shapeRots.push_back(shapeOriIce[x].GetY());
						FlexSys->g_assets[foundInd].shapeRots.push_back(shapeOriIce[x].GetZ());
						FlexSys->g_assets[foundInd].shapeRots.push_back(shapeOriIce[x].GetW());

					}


				}
			}

			outData.SetData(0, (CDataArrayCustomType::TData*)FlexSys, sizeof(*FlexSys));
		}


	}
	break;

	// Other output ports...
	};

	return CStatus::OK;
}

SICALLBACK PC_FLEX_CREATE_ASSET_Term(ICENodeContext& in_ctxt)
{
	Application().LogMessage(CString("PC_FLEX_CREATE_ASSET_Term"));
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