#include "..\Helpers.h"

XSI::CStatus RegisterPC_FLEX_SOLVER( XSI::PluginRegistrar& in_reg );
XSI::CStatus RegisterPC_FLEX_SOLVER_UPDATE(XSI::PluginRegistrar& in_reg);
XSI::CStatus RegisterPC_FLEX_SET_SOLVER_PARAMS(XSI::PluginRegistrar& in_reg);
XSI::CStatus RegisterPC_FLEX_ADD_COLLISION_SHAPES(XSI::PluginRegistrar& in_reg);
XSI::CStatus RegisterPC_FLEX_ADD_PARTICLES(XSI::PluginRegistrar& in_reg);
XSI::CStatus RegisterPC_FLEX_GET_PARTICLES(XSI::PluginRegistrar& in_reg);
XSI::CStatus RegisterPC_FLEX_ALL_IN(XSI::PluginRegistrar& in_reg);
XSI::CStatus RegisterPC_FLEX_CREATE_CLOTH_SPRINGS(XSI::PluginRegistrar& in_reg);
XSI::CStatus RegisterPC_FLEX_CREATE_ASSET(XSI::PluginRegistrar& in_reg);
XSI::CStatus RegisterPC_FLEX_GET_ASSET_DATA(XSI::PluginRegistrar& in_reg);
XSI::CStatus RegisterPC_FLEX_GET_DIFFPARTICLES(XSI::PluginRegistrar& in_reg);
XSI::CStatus RegisterPC_FLEX_CREATE_SOFTBODY(XSI::PluginRegistrar& in_reg);




using namespace XSI; 

SICALLBACK XSILoadPlugin( PluginRegistrar& in_reg )
{
	in_reg.PutAuthor(L"Pedrito");
	in_reg.PutName(L"PC_FLEX_Plugin");
	in_reg.PutVersion(1,0);


	int g_device = -1;
	g_device = NvFlexDeviceGetSuggestedOrdinal();

	// Create an optimized CUDA context for Flex and set it on the 
	// calling thread. This is an optional call, it is fine to use 
	// a regular CUDA context, although creating one through this API
	// is recommended for best performance.
	bool success = NvFlexDeviceCreateCudaContext(g_device);

	if (!success)
	{

		Application().LogMessage(CString("Error creating CUDA context"));
	}
	else {
		Application().LogMessage(CString("PC_Flex Succesfully created CUDA context"));
	}
	

	NvFlexInitDesc desc;
	desc.deviceIndex = g_device;
	desc.enableExtensions = true;
	desc.renderDevice = 0;
	desc.renderContext = 0;
	desc.computeType = eNvFlexCUDA;

	NvFlexLibrary* g_flexLib = NvFlexInit(NV_FLEX_VERSION, ErrorCallback, &desc);


	in_reg.PutUserData((CValue::siPtrType)g_flexLib);

	RegisterPC_FLEX_SOLVER( in_reg );
	RegisterPC_FLEX_SOLVER_UPDATE(in_reg);
	RegisterPC_FLEX_SET_SOLVER_PARAMS(in_reg);
	RegisterPC_FLEX_ADD_COLLISION_SHAPES(in_reg);
	RegisterPC_FLEX_ADD_PARTICLES(in_reg);
	RegisterPC_FLEX_GET_PARTICLES(in_reg);
	RegisterPC_FLEX_GET_DIFFPARTICLES(in_reg);
	RegisterPC_FLEX_ALL_IN(in_reg);
	RegisterPC_FLEX_CREATE_CLOTH_SPRINGS(in_reg);
	RegisterPC_FLEX_CREATE_ASSET(in_reg);
	RegisterPC_FLEX_GET_ASSET_DATA(in_reg);
	RegisterPC_FLEX_CREATE_SOFTBODY(in_reg);

	//RegistrationInsertionPoint - do not remove this line

	return CStatus::OK;
}

SICALLBACK XSIUnloadPlugin( const PluginRegistrar& in_reg )
{
	CString strPluginName;
	strPluginName = in_reg.GetName();
	Application().LogMessage(strPluginName + L" has been unloaded.",siVerboseMsg);

	NvFlexLibrary* g_flexLib = (NvFlexLibrary*)(CValue::siPtrType)in_reg.GetUserData();
	NvFlexShutdown(g_flexLib);
	//NvFlexDeviceDestroyCudaContext();

	return CStatus::OK;
}


