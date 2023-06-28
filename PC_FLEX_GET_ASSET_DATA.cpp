#include "..\Helpers.h"

// Defines port, group and map identifiers used for registering the ICENode
enum IDs
{
	ID_IN_solver = 0,
	ID_In_name = 1,
	ID_G_100 = 100,
	ID_OUT_positions = 200,
	ID_OUT_velocities = 201,
	ID_OUT_densities = 202,
	ID_OUT_smoothPos = 203,
	ID_OUT_anysotropic = 204,
	ID_OUT_rigids = 205,
	ID_TYPE_CNS = 400,
	ID_STRUCT_CNS,
	ID_CTXT_CNS,
	ID_UNDEF = ULONG_MAX
};

using namespace XSI;

CStatus RegisterPC_FLEX_GET_ASSET_DATA(PluginRegistrar& in_reg)
{
	ICENodeDef nodeDef;
	nodeDef = Application().GetFactory().CreateICENodeDef(L"PC_FLEX_GET_ASSET_DATA", L"PC_FLEX_GET_ASSET_DATA");

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
	// Add output ports.
	st = nodeDef.AddOutputPort(ID_OUT_positions, siICENodeDataVector4, siICENodeStructureArray, siICENodeContextSingleton, L"positions", L"positions", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddOutputPort(ID_OUT_smoothPos, siICENodeDataVector4, siICENodeStructureArray, siICENodeContextSingleton, L"smoothPositions", L"smoothPositions", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddOutputPort(ID_OUT_velocities, siICENodeDataVector3, siICENodeStructureArray, siICENodeContextSingleton, L"velocities", L"velocities", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddOutputPort(ID_OUT_densities, siICENodeDataFloat, siICENodeStructureArray, siICENodeContextSingleton, L"densities", L"densities", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddOutputPort(ID_OUT_anysotropic, siICENodeDataMatrix33, siICENodeStructureArray, siICENodeContextSingleton, L"anysotropic", L"anysotropic", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	st = nodeDef.AddOutputPort(ID_OUT_rigids, siICENodeDataMatrix44, siICENodeStructureArray, siICENodeContextSingleton, L"rigids", L"rigids", ID_UNDEF, ID_UNDEF, ID_CTXT_CNS);
	st.AssertSucceeded();
	
	PluginItem nodeItem = in_reg.RegisterICENode(nodeDef);
	nodeItem.PutCategories(L"PC_Tools/Flex");

	return CStatus::OK;
}

SICALLBACK PC_FLEX_GET_ASSET_DATA_BeginEvaluate(ICENodeContext& in_ctxt)
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
SICALLBACK PC_FLEX_GET_ASSET_DATA_Evaluate(ICENodeContext& in_ctxt)
{
	// The current output port being evaluated...
	ULONG out_portID = in_ctxt.GetEvaluatedOutputPortID();
	CValue userData = in_ctxt.GetUserData();

	if (!userData.IsEmpty()) {
		CDataArrayString nameData(in_ctxt, ID_In_name);

		FlexSystem* FlexSys = (FlexSystem*)(CValue::siPtrType)in_ctxt.GetUserData();
		CString name = nameData[0];

		bool found = false;
		int foundInd = -1;
		for (size_t i = 0; i < FlexSys->g_assets.size(); i++)
		{
			if (FlexSys->g_assets[i].name == name) {
				found = true;
				foundInd = i;
				break;
			}
		}
		if (!FlexSys->g_buffers->maped)
			FlexSys->g_buffers->MapBuffers();
		switch (out_portID)
		{
		case ID_OUT_positions:
		{
			if (found)
			{
				// Get the output port array ...	
				CDataArray2DVector4f outData(in_ctxt);
				CDataArray2DVector4f::Accessor outPositions = outData.Resize(0, FlexSys->g_assets[foundInd].nbpoints);
				int counter = 0;
				for (int i = 0; i < FlexSys->g_assets[foundInd].nbpoints; ++i)
				{
					float x, y, z, w;
					x = FlexSys->g_buffers->positions[i + FlexSys->g_assets[foundInd].startpoint].x;
					y = FlexSys->g_buffers->positions[i + FlexSys->g_assets[foundInd].startpoint].y;
					z = FlexSys->g_buffers->positions[i + FlexSys->g_assets[foundInd].startpoint].z;
					w = FlexSys->g_buffers->positions[i + FlexSys->g_assets[foundInd].startpoint].w;
					outPositions[i].Set(x, y, z, w);
				}
			}

		}
		break;
		case ID_OUT_velocities:
		{
			if (found)
			{
				// Get the output port array ...	
				CDataArray2DVector3f outData(in_ctxt);
				CDataArray2DVector3f::Accessor outVelocities = outData.Resize(0, FlexSys->g_assets[foundInd].nbpoints);
				for (int i = 0; i < FlexSys->g_assets[foundInd].nbpoints; ++i)
				{
					float x, y, z, w;
					x = FlexSys->g_buffers->velocities[i + FlexSys->g_assets[foundInd].startpoint].x;
					y = FlexSys->g_buffers->velocities[i + FlexSys->g_assets[foundInd].startpoint].y;
					z = FlexSys->g_buffers->velocities[i + FlexSys->g_assets[foundInd].startpoint].z;
					outVelocities[i].Set(x, y, z);
				}
			}

		}
		break;
		case ID_OUT_densities:
		{
			if (found)
			{
				// Get the output port array ...	
				CDataArray2DFloat outData(in_ctxt);
				CDataArray2DFloat::Accessor outDensities = outData.Resize(0, FlexSys->g_assets[foundInd].nbpoints);
				for (int i = 0; i <FlexSys->g_assets[foundInd].nbpoints; ++i)
				{
					outDensities[i] = FlexSys->g_buffers->densities[i + FlexSys->g_assets[foundInd].startpoint];
				}
			}

		}
		break;
		case ID_OUT_smoothPos:
		{
			if (found && FlexSys->g_params.smoothing > 0)
			{
				// Get the output port array ...	
				CDataArray2DVector4f outData(in_ctxt);
				CDataArray2DVector4f::Accessor outSmoothPos = outData.Resize(0, FlexSys->g_assets[foundInd].nbpoints);
				for (int i = 0; i < FlexSys->g_assets[foundInd].nbpoints; ++i)
				{
					float x, y, z, w;
					x = FlexSys->g_buffers->smoothPositions[i + FlexSys->g_assets[foundInd].startpoint].x; 
					y = FlexSys->g_buffers->smoothPositions[i + FlexSys->g_assets[foundInd].startpoint].y;
					z = FlexSys->g_buffers->smoothPositions[i + FlexSys->g_assets[foundInd].startpoint].z;
					w = FlexSys->g_buffers->smoothPositions[i + FlexSys->g_assets[foundInd].startpoint].w;
					outSmoothPos[i].Set(x, y, z,w);
				}
			}

		}
		break;
		case ID_OUT_anysotropic:
		{
			if (found && FlexSys->g_params.anisotropyScale > 0)
			{
				// Get the output port array ...	
				CDataArray2DMatrix3f outData(in_ctxt);
				CDataArray2DMatrix3f::Accessor outSmoothPos = outData.Resize(0, FlexSys->g_assets[foundInd].nbpoints);
				for (int i = 0; i < FlexSys->g_assets[foundInd].nbpoints; ++i)
				{
					
					Vec4 x, y, z;
					x = FlexSys->g_buffers->anisotropy1[i + FlexSys->g_assets[foundInd].startpoint];
					y = FlexSys->g_buffers->anisotropy2[i + FlexSys->g_assets[foundInd].startpoint];
					z = FlexSys->g_buffers->anisotropy3[i + FlexSys->g_assets[foundInd].startpoint];
					float f = 1.0f / FlexSys->g_params.radius;
					outSmoothPos[i].Set(x.x*(f*x.w), x.y*(f*x.w), x.z*(f*x.w),
										y.x*(f*y.w), y.y*(f*y.w), y.z*(f*y.w),
										z.x*(f*z.w), z.y*(f*z.w), z.z*(f*z.w));
				}
			}

		}
		break;
		case ID_OUT_rigids:
		{
			if (found && FlexSys->g_assets[foundInd].numShapes > 0)
			{
				// Get the output port array ...	
				CDataArray2DMatrix4f outData(in_ctxt);
				CDataArray2DMatrix4f::Accessor outRigids = outData.Resize(0, FlexSys->g_assets[foundInd].numShapes);
				for (int i = 0; i < FlexSys->g_assets[foundInd].numShapes; ++i)
				{
					MATH::CTransformation transf;
					transf.SetIdentity();
					transf = transf.SetRotationFromQuaternion(MATH::CQuaternion(FlexSys->g_assets[foundInd].shapeRots[i].w, FlexSys->g_assets[foundInd].shapeRots[i].x, FlexSys->g_assets[foundInd].shapeRots[i].y, FlexSys->g_assets[foundInd].shapeRots[i].z));
					transf = transf.SetTranslationFromValues(FlexSys->g_assets[foundInd].shapeCenters[i].x, FlexSys->g_assets[foundInd].shapeCenters[i].y, FlexSys->g_assets[foundInd].shapeCenters[i].z);
					double io_f00, io_f01, io_f02, io_f03, io_f10, io_f11, io_f12, io_f13, io_f20, io_f21, io_f22, io_f23, io_f30, io_f31, io_f32, io_f33;
					MATH::CMatrix4f mat;
					MATH::CMatrix4 mat2;
					mat2 = transf.GetMatrix4(); 
					mat2.Get(io_f00, io_f01, io_f02, io_f03, io_f10, io_f11, io_f12, io_f13, io_f20, io_f21, io_f22, io_f23, io_f30, io_f31, io_f32, io_f33);
					mat.Set(io_f00, io_f01, io_f02, io_f03, io_f10, io_f11, io_f12, io_f13, io_f20, io_f21, io_f22, io_f23, io_f30, io_f31, io_f32, io_f33);
					outRigids[i] = mat; 
				}
			}

		}
		break;
		
		// Other output ports...
		};
	}

	return CStatus::OK;
}

SICALLBACK PC_FLEX_GET_ASSET_DATA_Term(ICENodeContext& in_ctxt)
{
	//Application().LogMessage(CString("PC_FLEX_GET_ASSET_DATA_Term"));
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