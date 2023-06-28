#include "..\Helpers.h"


// Defines port, group and map identifiers used for registering the ICENode
enum IDs
{
	ID_IN_solver = 0,
	ID_IN_numIterations = 1,
	ID_IN_gravity = 2,
	ID_IN_radius = 3,
	ID_IN_solidRestDistance = 4,
	ID_IN_fluidRestDistance = 5,
	ID_IN_dynamicFriction = 6,
	ID_IN_staticFriction = 7,
	ID_IN_particleFriction = 8,
	ID_IN_restitution = 9,
	ID_IN_adhesion = 10,
	ID_IN_sleepThreshold = 11,
	ID_IN_maxSpeed = 12,
	ID_IN_maxAcceleration = 13,
	ID_IN_shockPropagation = 14,
	ID_IN_dissipation = 15,
	ID_IN_damping = 16,
	ID_IN_wind = 17,
	ID_IN_drag = 18,
	ID_IN_lift = 19,
	ID_IN_fluid = 20,
	ID_IN_cohesion = 21,
	ID_IN_surfaceTension = 22,
	ID_IN_viscosity = 23,
	ID_IN_vorticityConfinement = 24,
	ID_IN_anisotropyScale = 25,
	ID_IN_anisotropyMin = 26,
	ID_IN_anisotropyMax = 27,
	ID_IN_smoothing = 28,
	ID_IN_solidPressure = 29,
	ID_IN_freeSurfaceDrag = 30,
	ID_IN_buoyancy = 31,
	ID_IN_diffuseThreshold = 32,
	ID_IN_diffuseBuoyancy = 33,
	ID_IN_diffuseDrag = 34,
	ID_IN_diffuseBallistic = 35,
	ID_IN_diffuseSortAxis = 36,
	ID_IN_diffuseLifetime = 37,
	ID_IN_plasticThreshold = 38,
	ID_IN_plasticCreep = 39,
	ID_IN_collisionDistance = 40,
	ID_IN_particleCollisionMargin = 41,
	ID_IN_shapeCollisionMargin = 42,
	ID_IN_planes = 43,
	ID_IN_numPlanes = 44,
	ID_IN_relaxationMode = 45,
	ID_IN_relaxationFactor = 46,
	ID_IN_substeps = 47,
	ID_IN_fps = 48,
	ID_G_100 = 100,
	ID_OUT_Solver = 200,
	ID_TYPE_CNS = 400,
	ID_STRUCT_CNS,
	ID_CTXT_CNS,
	ID_UNDEF = ULONG_MAX
};

using namespace XSI;

CStatus RegisterPC_FLEX_SET_SOLVER_PARAMS(PluginRegistrar& in_reg)
{
	ICENodeDef nodeDef;
	nodeDef = Application().GetFactory().CreateICENodeDef(L"PC_FLEX_SET_SOLVER_PARAMS", L"PC_FLEX_SET_SOLVER_PARAMS");

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
	st = nodeDef.AddInputPort(ID_IN_substeps, ID_G_100, siICENodeDataLong, siICENodeStructureSingle, siICENodeContextSingleton, L"substeps", L"substeps", 2, 0, 10, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_numIterations, ID_G_100, siICENodeDataLong, siICENodeStructureSingle, siICENodeContextSingleton, L"numIterations", L"numIterations",3,0,20, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_fps, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"FPS", L"FPS",30.0f,23.978f, 60.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_gravity, ID_G_100, siICENodeDataVector3, siICENodeStructureSingle, siICENodeContextSingleton, L"gravity", L"gravity", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_radius, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"radius", L"radius", 0.15f,0, 1.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_solidRestDistance, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"solidRestDistance", L"solidRestDistance",0,0,1.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_fluidRestDistance, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"fluidRestDistance", L"fluidRestDistance", 0, 0, 1.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_dynamicFriction, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"dynamicFriction", L"dynamicFriction", 0, 0, 1.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_staticFriction, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"staticFriction", L"staticFriction", 0, 0, 1.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_particleFriction, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"particleFriction", L"particleFriction", 0, 0, 1.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_restitution, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"restitution", L"restitution", 0.001f ,0,0.1f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_adhesion, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"adhesion", L"adhesion", 0, 0, 1.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_sleepThreshold, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"sleepThreshold", L"sleepThreshold", 0, 0, 1.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_maxSpeed, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"maxSpeed", L"maxSpeed", 1000.0f, 0, 1000.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_maxAcceleration, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"maxAcceleration", L"maxAcceleration", 100.0f, 0, 100.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_shockPropagation, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"shockPropagation", L"shockPropagation", 0, 0, 1.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_dissipation, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"dissipation", L"dissipation", 0, 0, 1.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_damping, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"damping", L"damping", 0, 0, 1.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_wind, ID_G_100, siICENodeDataVector3, siICENodeStructureSingle, siICENodeContextSingleton, L"wind", L"wind", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_drag, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"drag", L"drag", 0, 0, 1.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_lift, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"lift", L"lift", 0, 0, 1.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_fluid, ID_G_100, siICENodeDataBool, siICENodeStructureSingle, siICENodeContextSingleton, L"fluid", L"fluid", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_cohesion, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"cohesion", L"cohesion", 0.1f, 0, 1.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_surfaceTension, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"surfaceTension", L"surfaceTension", 0, 0, 1.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_viscosity, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"viscosity", L"viscosity", 0, 0, 1.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_vorticityConfinement, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"vorticityConfinement", L"vorticityConfinement", 80.0f, 0, 200.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_anisotropyScale, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"anisotropyScale", L"anisotropyScale", 1.0f, 0, 1.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_anisotropyMin, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"anisotropyMin", L"anisotropyMin", 0.1f, 0, 1.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_anisotropyMax, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"anisotropyMax", L"anisotropyMax", 2.0f, 0,10.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_smoothing, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"smoothing", L"smoothing", 1.0f, 0, 2.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_solidPressure, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"solidPressure", L"solidPressure", 1.0f, 0, 1.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_freeSurfaceDrag, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"freeSurfaceDrag", L"freeSurfaceDrag", 0, 0, 1.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_buoyancy, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"buoyancy", L"buoyancy", 1.0f,0, 1.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_diffuseThreshold, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"diffuseThreshold", L"diffuseThreshold", 100.0f, 0,100.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_diffuseBuoyancy, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"diffuseBuoyancy", L"diffuseBuoyancy", 1.0f,0, 1.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_diffuseDrag, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"diffuseDrag", L"diffuseDrag", 0.8f, 0, 1.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_diffuseBallistic, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"diffuseBallistic", L"diffuseBallistic", 16.0f, 0, 24.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_diffuseSortAxis, ID_G_100, siICENodeDataVector3, siICENodeStructureSingle, siICENodeContextSingleton, L"diffuseSortAxis", L"diffuseSortAxis", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_diffuseLifetime, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"diffuseLifetime", L"diffuseLifetime", 2.0f, 0, 5.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_plasticThreshold, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"plasticThreshold", L"plasticThreshold", 0, 0, 1.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_plasticCreep, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"plasticCreep", L"plasticCreep", 0, 0, 1.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_collisionDistance, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"collisionDistance", L"collisionDistance", 0, 0, 1.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_particleCollisionMargin, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"particleCollisionMargin", L"particleCollisionMargin", 0, 0, 1.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_shapeCollisionMargin, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"shapeCollisionMargin", L"shapeCollisionMargin", 0, 0, 1.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_planes, ID_G_100, siICENodeDataVector4, siICENodeStructureArray, siICENodeContextSingleton, L"planes", L"planes", CValue(), CValue(), CValue(), ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_numPlanes, ID_G_100, siICENodeDataLong, siICENodeStructureSingle, siICENodeContextSingleton, L"numPlanes", L"numPlanes", 0, 0, 4, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_relaxationMode, ID_G_100, siICENodeDataLong, siICENodeStructureSingle, siICENodeContextSingleton, L"relaxationMode", L"relaxationMode", 0, 0, 1, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddInputPort(ID_IN_relaxationFactor, ID_G_100, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"relaxationFactor", L"relaxationFactor", 1.0f,0, 1.0f, ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	// Add output ports.

	st = nodeDef.AddOutputPort(ID_OUT_Solver, SolverCustomType, siICENodeStructureSingle, siICENodeContextSingleton, L"Solver", L"Solver", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	//st = nodeDef.AddOutputPort(ID_OUT_Solver, siICENodeDataFloat, siICENodeStructureSingle, siICENodeContextSingleton, L"Solver", L"Solver", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();

	PluginItem nodeItem = in_reg.RegisterICENode(nodeDef);
	nodeItem.PutCategories(L"Custom ICENode");

	return CStatus::OK;
}

SICALLBACK PC_FLEX_SET_SOLVER_PARAMS_BeginEvaluate(ICENodeContext& in_ctxt)
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
				Application().LogMessage(CString("PC_FLEX_SET_SOLVER_PARAMS_usingCachedData"));
			}
			else {
				in_ctxt.PutUserData((CValue::siPtrType)prevSys);
			}
		}
	}

	return CStatus::OK;
}
SICALLBACK PC_FLEX_SET_SOLVER_PARAMS_Evaluate(ICENodeContext& in_ctxt)
{
	// The current output port being evaluated...
	ULONG out_portID = in_ctxt.GetEvaluatedOutputPortID();

	switch (out_portID)
	{
	case ID_OUT_Solver:
	{
		// Get the output port array ...			
		CDataArrayCustomType outData(in_ctxt);
		//CDataArrayFloat outData(in_ctxt);

		// Get the input data buffers for each port
		CDataArrayCustomType inSolverData(in_ctxt, ID_IN_solver);
		CDataArrayLong substepsData(in_ctxt, ID_IN_substeps);
		CDataArrayLong numIterationsData(in_ctxt, ID_IN_numIterations);
		CDataArrayFloat FPSData(in_ctxt, ID_IN_fps);
		CDataArrayVector3f gravityData(in_ctxt, ID_IN_gravity);
		CDataArrayFloat radiusData(in_ctxt, ID_IN_radius);
		CDataArrayFloat solidRestDistanceData(in_ctxt, ID_IN_solidRestDistance);
		CDataArrayFloat fluidRestDistanceData(in_ctxt, ID_IN_fluidRestDistance);
		CDataArrayFloat dynamicFrictionData(in_ctxt, ID_IN_dynamicFriction);
		CDataArrayFloat staticFrictionData(in_ctxt, ID_IN_staticFriction);
		CDataArrayFloat particleFrictionData(in_ctxt, ID_IN_particleFriction);
		CDataArrayFloat restitutionData(in_ctxt, ID_IN_restitution);
		CDataArrayFloat adhesionData(in_ctxt, ID_IN_adhesion);
		CDataArrayFloat sleepThresholdData(in_ctxt, ID_IN_sleepThreshold);
		CDataArrayFloat maxSpeedData(in_ctxt, ID_IN_maxSpeed);
		CDataArrayFloat maxAccelerationData(in_ctxt, ID_IN_maxAcceleration);
		CDataArrayFloat shockPropagationData(in_ctxt, ID_IN_shockPropagation);
		CDataArrayFloat dissipationData(in_ctxt, ID_IN_dissipation);
		CDataArrayFloat dampingData(in_ctxt, ID_IN_damping);
		CDataArrayVector3f windData(in_ctxt, ID_IN_wind);
		CDataArrayFloat dragData(in_ctxt, ID_IN_drag);
		CDataArrayFloat liftData(in_ctxt, ID_IN_lift);
		CDataArrayBool fluidData(in_ctxt, ID_IN_fluid);
		CDataArrayFloat cohesionData(in_ctxt, ID_IN_cohesion);
		CDataArrayFloat surfaceTensionData(in_ctxt, ID_IN_surfaceTension);
		CDataArrayFloat viscosityData(in_ctxt, ID_IN_viscosity);
		CDataArrayFloat vorticityConfinementData(in_ctxt, ID_IN_vorticityConfinement);
		CDataArrayFloat anisotropyScaleData(in_ctxt, ID_IN_anisotropyScale);
		CDataArrayFloat anisotropyMinData(in_ctxt, ID_IN_anisotropyMin);
		CDataArrayFloat anisotropyMaxData(in_ctxt, ID_IN_anisotropyMax);
		CDataArrayFloat smoothingData(in_ctxt, ID_IN_smoothing);
		CDataArrayFloat solidPressureData(in_ctxt, ID_IN_solidPressure);
		CDataArrayFloat freeSurfaceDragData(in_ctxt, ID_IN_freeSurfaceDrag);
		CDataArrayFloat buoyancyData(in_ctxt, ID_IN_buoyancy);
		CDataArrayFloat diffuseThresholdData(in_ctxt, ID_IN_diffuseThreshold);
		CDataArrayFloat diffuseBuoyancyData(in_ctxt, ID_IN_diffuseBuoyancy);
		CDataArrayFloat diffuseDragData(in_ctxt, ID_IN_diffuseDrag);
		CDataArrayFloat diffuseBallisticData(in_ctxt, ID_IN_diffuseBallistic);
		CDataArrayVector3f diffuseSortAxisData(in_ctxt, ID_IN_diffuseSortAxis);
		CDataArrayFloat diffuseLifetimeData(in_ctxt, ID_IN_diffuseLifetime);
		CDataArrayFloat plasticThresholdData(in_ctxt, ID_IN_plasticThreshold);
		CDataArrayFloat plasticCreepData(in_ctxt, ID_IN_plasticCreep);
		CDataArrayFloat collisionDistanceData(in_ctxt, ID_IN_collisionDistance);
		CDataArrayFloat particleCollisionMarginData(in_ctxt, ID_IN_particleCollisionMargin);
		CDataArrayFloat shapeCollisionMarginData(in_ctxt, ID_IN_shapeCollisionMargin);
		CDataArray2DVector4f planesData(in_ctxt, ID_IN_planes);
		CDataArrayLong numPlanesData(in_ctxt, ID_IN_numPlanes);
		CDataArrayLong relaxationModeData(in_ctxt, ID_IN_relaxationMode);
		CDataArrayFloat relaxationFactorData(in_ctxt, ID_IN_relaxationFactor);
		//CDataArrayFloat Data(in_ctxt, ID_IN_);

		ULONG it = 0;
		CValue userData = in_ctxt.GetUserData();

		if (!userData.IsEmpty()) {
			FlexSystem* FlexSys = (FlexSystem*)(CValue::siPtrType)in_ctxt.GetUserData();
			// sim params
			FlexSys->g_numSubsteps = substepsData[it];
			FlexSys->g_dt = 1.0f / FPSData[it];
			FlexSys->g_params.gravity[0] = gravityData[it].GetX();
			FlexSys->g_params.gravity[1] = gravityData[it].GetY();
			FlexSys->g_params.gravity[2] = gravityData[it].GetZ();

			FlexSys->g_params.wind[0] = windData[it].GetX();
			FlexSys->g_params.wind[1] = windData[it].GetY();
			FlexSys->g_params.wind[2] = windData[it].GetZ();

			FlexSys->g_params.radius = radiusData[it];
			FlexSys->g_params.viscosity = viscosityData[it];
			FlexSys->g_params.dynamicFriction = dynamicFrictionData[it];
			FlexSys->g_params.staticFriction = staticFrictionData[it];
			FlexSys->g_params.particleFriction = particleFrictionData[it]; // scale friction between particles by default
			FlexSys->g_params.freeSurfaceDrag = freeSurfaceDragData[it];
			FlexSys->g_params.drag = dragData[it];
			FlexSys->g_params.lift = liftData[it];
			FlexSys->g_params.numIterations = numIterationsData[it];
			FlexSys->g_params.fluidRestDistance = fluidRestDistanceData[it];
			FlexSys->g_params.solidRestDistance = solidRestDistanceData[it];

			FlexSys->g_params.anisotropyScale = anisotropyScaleData[it];
			FlexSys->g_params.anisotropyMin = anisotropyMinData[it];
			FlexSys->g_params.anisotropyMax = anisotropyMaxData[it];
			FlexSys->g_params.smoothing = smoothingData[it];

			FlexSys->g_params.dissipation = dissipationData[it];
			FlexSys->g_params.damping = dampingData[it];
			FlexSys->g_params.particleCollisionMargin = particleCollisionMarginData[it];
			FlexSys->g_params.shapeCollisionMargin = shapeCollisionMarginData[it];
			FlexSys->g_params.collisionDistance = collisionDistanceData[it];
			FlexSys->g_params.plasticThreshold = plasticThresholdData[it];
			FlexSys->g_params.plasticCreep = plasticCreepData[it];
			FlexSys->g_params.fluid = fluidData[it];
			FlexSys->g_params.sleepThreshold = sleepThresholdData[it];
			FlexSys->g_params.shockPropagation = shockPropagationData[it];
			FlexSys->g_params.restitution = restitutionData[it];

			FlexSys->g_params.maxSpeed = maxSpeedData[it]; //FLT_MAX;
			FlexSys->g_params.maxAcceleration = maxAccelerationData[it];	// approximately 10x gravity

			switch (relaxationModeData[it]) {
				case 0: {
					FlexSys->g_params.relaxationMode = eNvFlexRelaxationGlobal;
				}break;
				case 1: {
					FlexSys->g_params.relaxationMode = eNvFlexRelaxationLocal;
				}break;
			}

			FlexSys->g_params.relaxationFactor = relaxationFactorData[it];
			FlexSys->g_params.solidPressure = solidPressureData[it];
			FlexSys->g_params.adhesion = adhesionData[it];
			FlexSys->g_params.cohesion = cohesionData[it];
			FlexSys->g_params.surfaceTension = surfaceTensionData[it];
			FlexSys->g_params.vorticityConfinement = vorticityConfinementData[it];
			FlexSys->g_params.buoyancy = buoyancyData[it];
			FlexSys->g_params.diffuseThreshold = diffuseThresholdData[it];
			FlexSys->g_params.diffuseBuoyancy = diffuseBuoyancyData[it];
			FlexSys->g_params.diffuseDrag = diffuseDragData[it];
			FlexSys->g_params.diffuseBallistic = diffuseBallisticData[it];
			FlexSys->g_params.diffuseSortAxis[0] = diffuseSortAxisData[it].GetX();
			FlexSys->g_params.diffuseSortAxis[1] = diffuseSortAxisData[it].GetY();
			FlexSys->g_params.diffuseSortAxis[2] = diffuseSortAxisData[it].GetZ();
			FlexSys->g_params.diffuseLifetime = diffuseLifetimeData[it];

			FlexSys->g_params.numPlanes = numPlanesData[it];
			CDataArray2DVector4f::Accessor planes = planesData[it];
			for (int i = 0; i < numPlanesData[it]; i++) {
				(Vec4&)FlexSys->g_params.planes[i] = Vec4(planes[i].GetX(),
					planes[i].GetY(),
					planes[i].GetZ(),
					planes[i].GetW());
			}

			FlexSys->updateParams();

			outData.SetData(0, (CDataArrayCustomType::TData*)FlexSys, sizeof(*FlexSys));
		}

	}
	break;

	// Other output ports...
	};

	return CStatus::OK;
}
SICALLBACK PC_FLEX_SET_SOLVER_PARAMS_Term(ICENodeContext& in_ctxt)
{
	Application().LogMessage(CString("PC_FLEX_SET_SOLVER_PARAMS_TERM"));
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