#include "FlexSystem.h"
#include <xsi_application.h>
using namespace XSI;
inline void ErrorCallback(NvFlexErrorSeverity, const char* msg, const char* file, int line)
{
	Application().LogMessage(CString(("Flex: %s - %s:%d\n", msg, file, line)));
	printf("Flex: %s - %s:%d\n", msg, file, line);
	//g_Error = true;
	//assert(0); asserts are bad for TeamCity
}


SimBuffers::SimBuffers(NvFlexLibrary* l) :
	positions(l), restPositions(l), velocities(l), phases(l), densities(l),
	anisotropy1(l), anisotropy2(l), anisotropy3(l), normals(l), smoothPositions(l),
	diffusePositions(l), diffuseVelocities(l), diffuseIndices(l), activeIndices(l),
	shapeGeometry(l), shapePositions(l), shapeRotations(l), shapePrevPositions(l),
	shapePrevRotations(l), shapeFlags(l), rigidOffsets(l), rigidIndices(l), rigidMeshSize(l),
	rigidCoefficients(l), rigidRotations(l), rigidTranslations(l),
	rigidLocalPositions(l), rigidLocalNormals(l), inflatableTriOffsets(l),
	inflatableTriCounts(l), inflatableVolumes(l), inflatableCoefficients(l),
	inflatablePressures(l), springIndices(l), springLengths(l),
	springStiffness(l), triangles(l), triangleNormals(l), uvs(l){


}

SimBuffers::~SimBuffers() {
	//positions.empty();
	positions.destroy();
	restPositions.destroy();
	velocities.destroy();
	phases.destroy();
	densities.destroy();
	anisotropy1.destroy();
	anisotropy2.destroy();
	anisotropy3.destroy();
	normals.destroy();
	diffusePositions.destroy();
	diffuseVelocities.destroy();
	diffuseIndices.destroy();
	smoothPositions.destroy();
	activeIndices.destroy();

	// convexes
	shapeGeometry.destroy();
	shapePositions.destroy();
	shapeRotations.destroy();
	shapePrevPositions.destroy();
	shapePrevRotations.destroy();
	shapeFlags.destroy();

	// rigids
	rigidOffsets.destroy();
	rigidIndices.destroy();
	rigidMeshSize.destroy();
	rigidCoefficients.destroy();
	rigidRotations.destroy();
	rigidTranslations.destroy();
	rigidLocalPositions.destroy();
	rigidLocalNormals.destroy();

	// springs
	springIndices.destroy();
	springLengths.destroy();
	springStiffness.destroy();

	// inflatables
	inflatableTriOffsets.destroy();
	inflatableTriCounts.destroy();
	inflatableVolumes.destroy();
	inflatableCoefficients.destroy();
	inflatablePressures.destroy();

	// triangles
	triangles.destroy();
	triangleNormals.destroy();
	uvs.destroy();


}

void SimBuffers::MapBuffers() {

	positions.map();
	restPositions.map();
	velocities.map();
	phases.map();
	densities.map();
	anisotropy1.map();
	anisotropy2.map();
	anisotropy3.map();
	normals.map();
	diffusePositions.map();
	diffuseVelocities.map();
	diffuseIndices.map();
	smoothPositions.map();
	activeIndices.map();

	// convexes
	shapeGeometry.map();
	shapePositions.map();
	shapeRotations.map();
	shapePrevPositions.map();
	shapePrevRotations.map();
	shapeFlags.map();

	rigidOffsets.map();
	rigidIndices.map();
	rigidMeshSize.map();
	rigidCoefficients.map();
	rigidRotations.map();
	rigidTranslations.map();
	rigidLocalPositions.map();
	rigidLocalNormals.map();

	springIndices.map();
	springLengths.map();
	springStiffness.map();

	// inflatables
	inflatableTriOffsets.map();
	inflatableTriCounts.map();
	inflatableVolumes.map();
	inflatableCoefficients.map();
	inflatablePressures.map();

	triangles.map();
	triangleNormals.map();
	uvs.map();

	maped = true;


}

void SimBuffers::UnmapBuffers() {

	// particles
	positions.unmap();
	restPositions.unmap();
	velocities.unmap();
	phases.unmap();
	densities.unmap();
	anisotropy1.unmap();
	anisotropy2.unmap();
	anisotropy3.unmap();
	normals.unmap();
	diffusePositions.unmap();
	diffuseVelocities.unmap();
	diffuseIndices.unmap();
	smoothPositions.unmap();
	activeIndices.unmap();

	// convexes
	shapeGeometry.unmap();
	shapePositions.unmap();
	shapeRotations.unmap();
	shapePrevPositions.unmap();
	shapePrevRotations.unmap();
	shapeFlags.unmap();

	// rigids
	rigidOffsets.unmap();
	rigidIndices.unmap();
	rigidMeshSize.unmap();
	rigidCoefficients.unmap();
	rigidRotations.unmap();
	rigidTranslations.unmap();
	rigidLocalPositions.unmap();
	rigidLocalNormals.unmap();

	// springs
	springIndices.unmap();
	springLengths.unmap();
	springStiffness.unmap();

	// inflatables
	inflatableTriOffsets.unmap();
	inflatableTriCounts.unmap();
	inflatableVolumes.unmap();
	inflatableCoefficients.unmap();
	inflatablePressures.unmap();

	// triangles
	triangles.unmap();
	triangleNormals.unmap();
	uvs.unmap();

	maped = false;

}

void SimBuffers::InitBuffers() {

	positions.resize(0);
	restPositions.resize(0);
	velocities.resize(0);
	phases.resize(0);
	densities.resize(0);
	anisotropy1.resize(0);
	anisotropy2.resize(0);
	anisotropy3.resize(0);
	normals.resize(0);
	diffusePositions.resize(0);
	diffuseVelocities.resize(0);
	diffuseIndices.resize(0);
	smoothPositions.resize(0);
	activeIndices.resize(0);

	// convexes
	shapeGeometry.resize(0);
	shapePositions.resize(0);
	shapeRotations.resize(0);
	shapePrevPositions.resize(0);
	shapePrevRotations.resize(0);
	shapeFlags.resize(0);

	// rigids
	rigidOffsets.resize(0);
	rigidIndices.resize(0);
	rigidMeshSize.resize(0);
	rigidCoefficients.resize(0);
	rigidRotations.resize(0);
	rigidTranslations.resize(0);
	rigidLocalPositions.resize(0);
	rigidLocalNormals.resize(0);

	// springs
	springIndices.resize(0);
	springLengths.resize(0);
	springStiffness.resize(0);

	// inflatables
	inflatableTriOffsets.resize(0);
	inflatableTriCounts.resize(0);
	inflatableVolumes.resize(0);
	inflatableCoefficients.resize(0);
	inflatablePressures.resize(0);

	// triangles
	triangles.resize(0);
	triangleNormals.resize(0);
	uvs.resize(0);



}

pcFlexAsset::pcFlexAsset() {
	name = "";
	flexAsset = NULL;
	nbpoints = 0;
	startpoint = 0;
	nbtriangles = 0;
	numSprings = 0,
	startrigid = 0;
	inflatstartTri = 0;
	inflatRest = 0.0;
	inflatcoef = 0.0;
	inflatpressure = 0.0;
	nbRestPoints = 0;
	nbVelocities = 0;
	nbPhases = 0;
	numShapes = 0;

	fpoints.resize(0);
	fvelocities.resize(0);
	phases.resize(0);
	springIndices.resize(0);
	springRestLengths.resize(0);
	springStiffness.resize(0);
	triangleIndices.resize(0);
	frestpoints.resize(0);

	numShapeIndices = 0;
	shapeIndices.resize(0);
	shapeOffsets.resize(0);
	shapeCoefficients.resize(0);
	shapeCenters.resize(0);
	shapeRots.resize(0);
	rigidLocalPos.resize(0);
	
}
pcFlexAsset::~pcFlexAsset() {
	name = "";
	if (flexAsset)
	{
		NvFlexExtDestroyAsset(flexAsset);
		flexAsset = NULL;
	}
	nbpoints = 0;
	startpoint = 0;
	nbtriangles = 0;
	numSprings = 0,
	fpoints.resize(0);
	fvelocities.resize(0);
	phases.resize(0);
	springIndices.resize(0);
	springRestLengths.resize(0);
	springStiffness.resize(0);
	triangleIndices.resize(0);
	frestpoints.resize(0);
	shapeIndices.resize(0);
	shapeOffsets.resize(0);
	shapeCoefficients.resize(0);
	shapeCenters.resize(0);
	shapeRots.resize(0);
	rigidLocalPos.resize(0);
}

inline MATH::CTransformation Matrix4ToTransf(MATH::CMatrix4f mat) {

	MATH::CTransformation trans;
	float io_f00, io_f01, io_f02, io_f03, io_f10, io_f11, io_f12, io_f13, io_f20, io_f21, io_f22, io_f23, io_f30, io_f31, io_f32, io_f33;
	mat.Get(io_f00, io_f01, io_f02, io_f03, io_f10, io_f11, io_f12, io_f13, io_f20, io_f21, io_f22, io_f23, io_f30, io_f31, io_f32, io_f33);

	trans.SetMatrix4(XSI::MATH::CMatrix4(io_f00, io_f01, io_f02, io_f03, io_f10, io_f11, io_f12, io_f13, io_f20, io_f21, io_f22, io_f23, io_f30, io_f31, io_f32, io_f33));

	return trans;
}

inline void DecomposeTranf(MATH::CTransformation transf, Vec3& scale, Vec3& pos, Quat& quat) {

	scale.Set(transf.GetScaling().GetX(), transf.GetScaling().GetY(), transf.GetScaling().GetZ());
	pos.Set(transf.GetPosX(), transf.GetPosY(), transf.GetPosZ());
	quat.Set(transf.GetRotationQuaternion().GetX(), transf.GetRotationQuaternion().GetY(), transf.GetRotationQuaternion().GetZ(), transf.GetRotationQuaternion().GetW());
}

FlexSystem::FlexSystem()
{	

	g_profile = false;
	
	nEmitter = 0;
	nVolumeBoxes = 0;
	nAssets = 0;

	maxParticles = 9;
	g_maxDiffuseParticles=0;
	numDiffuse = 0;

	g_flex = NULL;


	time1 = 0;
	time2 = 0;
	time3 = 0;
	time4 = 0;

	memset(&g_timers, 0, sizeof(g_timers));

	cursor = 0;
	ID = 0;

}

FlexSystem::~FlexSystem(){


	if (g_flex)
	{
		if(g_buffers)
			delete g_buffers;

		NvFlexDestroySolver(g_flex);
		//NvFlexShutdown(g_flexLib);
	}

	

	//NvFlexDeviceDestroyCudaContext();

}

void FlexSystem::getSimTimers() {

	if (g_profile) {
		memset(&g_timers, 0, sizeof(g_timers));
		NvFlexGetTimers(g_flex, &g_timers);
		simLatency = g_timers.total;
	}
	else
		simLatency = NvFlexGetDeviceLatency(g_flex);

}

void FlexSystem::initSystem() {

	int g_device = -1;
	g_device = NvFlexDeviceGetSuggestedOrdinal();

	// Create an optimized CUDA context for Flex and set it on the 
	// calling thread. This is an optional call, it is fine to use 
	// a regular CUDA context, although creating one through this API
	// is recommended for best performance.
	bool success = NvFlexDeviceCreateCudaContext(g_device);

	if (!success)
	{
		
		Application().LogMessage(CString("Error creating CUDA context.\n"));
		printf("Error creating CUDA context.\n");
	}

	NvFlexInitDesc desc;
	desc.deviceIndex = g_device;
	desc.enableExtensions = true;
	desc.renderDevice = 0;
	desc.renderContext = 0;
	desc.computeType = eNvFlexCUDA;

	g_flexLib = NvFlexInit(NV_FLEX_VERSION, ErrorCallback, &desc);

}


void FlexSystem::initParams() {

	// sim params
	g_params.gravity[0] = 0.0f;
	g_params.gravity[1] = -9.8f;
	g_params.gravity[2] = 0.0f;

	g_params.wind[0] = 0.0f;
	g_params.wind[1] = 0.0f;
	g_params.wind[2] = 0.0f;

	g_params.radius = 0.15f;
	g_params.viscosity = 0.0f;
	g_params.dynamicFriction = 0.0f;
	g_params.staticFriction = 0.0f;
	g_params.particleFriction = 0.0f; // scale friction between particles by default
	g_params.freeSurfaceDrag = 0.0f;
	g_params.drag = 0.0f;
	g_params.lift = 0.0f;
	g_params.numIterations = 3;
	g_params.fluidRestDistance = 0.0f;
	g_params.solidRestDistance = 0.0f;

	g_params.anisotropyScale = 1.0f;
	g_params.anisotropyMin = 0.1f;
	g_params.anisotropyMax = 2.0f;
	g_params.smoothing = 1.0f;

	g_params.dissipation = 0.0f;
	g_params.damping = 0.0f;
	g_params.particleCollisionMargin = 0.0f;
	g_params.shapeCollisionMargin = 0.0f;
	g_params.collisionDistance = 0.0f;
	g_params.plasticThreshold = 0.0f;
	g_params.plasticCreep = 0.0f;
	g_params.fluid = true;
	g_params.sleepThreshold = 0.0f;
	g_params.shockPropagation = 0.0f;
	g_params.restitution = 0.001f;

	g_params.maxSpeed = FLT_MAX;
	g_params.maxAcceleration = 100.0f;	// approximately 10x gravity

	g_params.smoothing = 1.0f;

	g_params.relaxationMode = eNvFlexRelaxationLocal;
	g_params.relaxationFactor = 1.0f;
	g_params.solidPressure = 1.0f;
	g_params.adhesion = 0.0f;
	g_params.cohesion = 0.1f;
	g_params.surfaceTension = 0.0f;
	g_params.vorticityConfinement = 80.0f;
	g_params.buoyancy = 1.0f;
	g_params.diffuseThreshold = 100.0f;
	g_params.diffuseBuoyancy = 1.0f;
	g_params.diffuseDrag = 0.8f;
	g_params.diffuseBallistic = 16;
	g_params.diffuseSortAxis[0] = 0.0f;
	g_params.diffuseSortAxis[1] = 0.0f;
	g_params.diffuseSortAxis[2] = 0.0f;
	g_params.diffuseLifetime = 2.0f;

	g_params.numPlanes = 0;

}
void FlexSystem::updateParams() {

	if (!g_params.fluid) {
		g_params.particleCollisionMargin = g_params.radius*0.5f;
		g_params.shapeCollisionMargin = g_params.radius*0.5f;
	}
	if (g_params.fluid)
		g_params.fluidRestDistance = g_params.radius*0.65f;

	// by default solid particles use the maximum radius
	if (g_params.fluid && g_params.solidRestDistance == 0.0f)
		g_params.solidRestDistance = g_params.fluidRestDistance;
	else
		g_params.solidRestDistance = g_params.radius;

	// collision distance with shapes half the radius
	if (g_params.collisionDistance == 0.0f)
	{
		g_params.collisionDistance = g_params.radius*0.5f;

		if (g_params.fluid)
			g_params.collisionDistance = g_params.fluidRestDistance*0.5f;
	}

	// default particle friction to 10% of shape friction
	if (g_params.particleFriction == 0.0f)
		g_params.particleFriction = g_params.dynamicFriction*0.1f;

	// add a margin for detecting contacts between particles and shapes
	if (g_params.shapeCollisionMargin == 0.0f)
		g_params.shapeCollisionMargin = g_params.collisionDistance*0.5f;

	g_params.anisotropyScale = 3.0f / g_params.radius;
}

void FlexSystem::initScene(NvFlexLibrary* flexLib,int maxparticles,int maxdiffuse, int maxneis){


	//RandInit();

	cursor = 0;
	ID = 0;
	g_flexLib = flexLib;

	if (g_flex)
	{
		
		if (g_buffers)
			delete g_buffers;

		NvFlexDestroySolver(g_flex);
		g_flex = NULL;

	}

	// alloc buffers
	g_buffers = new SimBuffers(g_flexLib);

	// map during initialization
	g_buffers->MapBuffers();
	//getSimTimers();
	g_buffers->InitBuffers();

	g_assets.resize(0);
	nAssets = 0;
	g_numDiffuse = 0;
	initParams();

	g_numSubsteps = 2;
	g_dt = 1 / 30.0;
	g_sceneLower = FLT_MAX;
	g_sceneUpper = -FLT_MAX;
	g_numtriangles = 0;
	g_numInflatables = 0;
	ClearShapes();


	maxParticles = maxparticles;

	g_maxDiffuseParticles = maxdiffuse;
	g_maxNeighborsPerParticle = maxneis;

	g_buffers->diffusePositions.resize(g_maxDiffuseParticles);
	g_buffers->diffuseVelocities.resize(g_maxDiffuseParticles);
	g_buffers->diffuseIndices.resize(g_maxDiffuseParticles);

	// resize particle buffers to fit
	g_buffers->positions.resize(maxParticles);
	g_buffers->restPositions.resize(maxParticles);

	g_buffers->normals.resize(0);
	g_buffers->normals.resize(maxParticles);

	g_buffers->velocities.resize(maxParticles);
	g_buffers->phases.resize(maxParticles);

	g_buffers->densities.resize(maxParticles);
	g_buffers->anisotropy1.resize(maxParticles);
	g_buffers->anisotropy2.resize(maxParticles);
	g_buffers->anisotropy3.resize(maxParticles);
	// for fluid rendering these are the Laplacian smoothed positions
	g_buffers->smoothPositions.resize(maxParticles);

	g_flex = NvFlexCreateSolver(g_flexLib, maxParticles, g_maxDiffuseParticles, g_maxNeighborsPerParticle);

}

void FlexSystem::AddShape(int type,Vec3 halfEdge, Vec3 center,Vec3 prevCenter, Quat quat,Quat prevQuat, bool dynamic)
{
	// transform

	g_buffers->shapePositions.push_back(Vec4(center, 0.0f));
	g_buffers->shapeRotations.push_back(quat);

	g_buffers->shapePrevPositions.push_back(Vec4(prevCenter, 0.0f));
	g_buffers->shapePrevRotations.push_back(prevQuat);

	NvFlexCollisionGeometry geo;
	switch (type)
	{
	case 0: {
		geo.sphere.radius = halfEdge.x;
		g_buffers->shapeFlags.push_back(NvFlexMakeShapeFlags(eNvFlexShapeSphere, dynamic));
	}break;
	case 1: {
		geo.box.halfExtents[0] = halfEdge.x*0.5;
		geo.box.halfExtents[1] = halfEdge.y*0.5;
		geo.box.halfExtents[2] = halfEdge.z*0.5;
		g_buffers->shapeFlags.push_back(NvFlexMakeShapeFlags(eNvFlexShapeBox, dynamic));
	}break;
	case 2: {
		geo.capsule.radius = halfEdge.y*0.5;
		geo.capsule.halfHeight = halfEdge.x*0.5;
		g_buffers->shapeFlags.push_back(NvFlexMakeShapeFlags(eNvFlexShapeCapsule, dynamic));
	}break;
	default: {
		geo.sphere.radius = halfEdge.x;
		g_buffers->shapeFlags.push_back(NvFlexMakeShapeFlags(eNvFlexShapeSphere, dynamic));
	}
		break;
	}
	g_buffers->shapeGeometry.push_back(geo);
}
void FlexSystem::AddTriangleMesh(Vec3 scale, Vec3 center, Quat quat, std::vector<Vec3> vertices, std::vector<int> indices,int numvertices, int numtriangles, bool dynamic)
{
	// transform
	g_buffers->shapePositions.push_back(Vec4(center.x, center.y, center.z, 0.0f));
	g_buffers->shapeRotations.push_back(quat);

	g_buffers->shapePrevPositions.push_back(g_buffers->shapePositions.back());
	g_buffers->shapePrevRotations.push_back(g_buffers->shapeRotations.back());
	
	NvFlexCollisionGeometry geo;
	NvFlexBuffer* positionsBuf = NvFlexAllocBuffer(g_flexLib, numvertices, sizeof(Vec3), eNvFlexBufferHost);
	Vec3* positions = (Vec3*)NvFlexMap(positionsBuf, 0);
	NvFlexBuffer* indicesBuf = NvFlexAllocBuffer(g_flexLib, numtriangles*3, sizeof(int), eNvFlexBufferHost);
	int* indicesArr = (int*)NvFlexMap(indicesBuf, 0);
	Vec3 lower = Vec3(FLT_MAX);
	Vec3 upper = Vec3(-FLT_MAX);
	for (size_t i = 0; i < numvertices; i++)
	{
		positions[i] = vertices[i];
		lower = Min(positions[i], lower);
		upper = Max(positions[i], upper);
	}
	for (size_t i = 0; i < numtriangles*3; i++)
	{
		indicesArr[i] = indices[i];
	}
	NvFlexUnmap(positionsBuf);
	NvFlexUnmap(indicesBuf);

	geo.triMesh.mesh = NvFlexCreateTriangleMesh(g_flexLib);
	geo.triMesh.scale[0]=scale.x;
	geo.triMesh.scale[1] = scale.y;
	geo.triMesh.scale[2] = scale.z;
	NvFlexUpdateTriangleMesh(g_flexLib, geo.triMesh.mesh, positionsBuf, indicesBuf, numvertices, numtriangles, lower, upper);


	g_buffers->shapeGeometry.push_back(geo);
	g_buffers->shapeFlags.push_back(NvFlexMakeShapeFlags(eNvFlexShapeTriangleMesh, dynamic));
}




void FlexSystem::GetParticleBounds(Vec3& lower, Vec3& upper)
{
	lower = Vec3(FLT_MAX);
	upper = Vec3(-FLT_MAX);

	for (size_t i=0; i < g_buffers->positions.size(); ++i)
	{
		lower = Min(Vec3(g_buffers->positions[i]), lower);
		upper = Max(Vec3(g_buffers->positions[i]), upper);
	}
}
void FlexSystem::ClearShapes(bool del)
{
	if (del)
	{
		for (size_t i = 0; i < g_buffers->shapeGeometry.size(); i++)
		{
			if (g_buffers->shapeGeometry[i].triMesh.mesh )
			{
				NvFlexDestroyTriangleMesh(g_flexLib, g_buffers->shapeGeometry[i].triMesh.mesh);
			}
		}
	}

	g_buffers->shapeGeometry.resize(0);
	g_buffers->shapePositions.resize(0);
	g_buffers->shapeRotations.resize(0);
	g_buffers->shapePrevPositions.resize(0);
	g_buffers->shapePrevRotations.resize(0);
	g_buffers->shapeFlags.resize(0);
}
void FlexSystem::setShapes(){

	if(g_buffers->shapeFlags.size()){
	NvFlexSetShapes(
				g_flex,
				g_buffers->shapeGeometry.buffer,
				g_buffers->shapePositions.buffer,
				g_buffers->shapeRotations.buffer,
				g_buffers->shapePrevPositions.buffer,
				g_buffers->shapePrevRotations.buffer,
				g_buffers->shapeFlags.buffer,
				g_buffers->shapeFlags.size());
		}
		

}

void FlexSystem::setAssets() {

	if (!g_buffers->maped) {
		g_buffers->MapBuffers();
		//getSimTimers();
	}

	
	if (!nAssets == 0) {
		g_buffers->activeIndices.resize(0);
		g_buffers->triangles.resize(0);
		g_buffers->normals.resize(0);
		g_buffers->springIndices.resize(0);
		g_buffers->springLengths.resize(0);
		g_buffers->springStiffness.resize(0);

		g_buffers->rigidIndices.resize(0);
		g_buffers->rigidOffsets.resize(0);
		g_buffers->rigidTranslations.resize(0);
		g_buffers->rigidRotations.resize(0);
		g_buffers->rigidCoefficients.resize(0);
		g_buffers->rigidLocalPositions.resize(0);

		g_buffers->inflatableTriOffsets.resize(0);
		g_buffers->inflatableTriCounts.resize(0);
		g_buffers->inflatablePressures.resize(0);
		g_buffers->inflatableVolumes.resize(0);
		g_buffers->inflatableCoefficients.resize(0);
		g_numtriangles = 0;
		g_numInflatables = 0;

		//cursor = 0;
		int currInd = 0;
		int numRigids = 0;
		
		for (size_t i = 0; i < nAssets; i++)
		{
			int counter = 0;
			for (size_t e = 0; e < g_assets[i].nbpoints; e++)
			{
				g_buffers->activeIndices.push_back(e+ g_assets[i].startpoint);
			}
			if (g_assets[i].nbtriangles)
			{
				if (g_assets[i].inflatRest > 0)
				{
					g_assets[i].inflatstartTri = g_numtriangles;
					g_buffers->inflatableTriOffsets.push_back(g_numtriangles);
					g_buffers->inflatableTriCounts.push_back(g_assets[i].nbtriangles);
					g_buffers->inflatablePressures.push_back(g_assets[i].inflatpressure);
					g_buffers->inflatableVolumes.push_back(g_assets[i].inflatRest);
					g_buffers->inflatableCoefficients.push_back(g_assets[i].inflatcoef);
					g_numInflatables += 1;
				}
				g_numtriangles += g_assets[i].nbtriangles;
				g_buffers->normals.resize(g_buffers->normals.size() + (g_assets[i].nbtriangles * 3));
				for (size_t e = 0; e < g_assets[i].nbtriangles * 3; e++)
				{
					g_buffers->triangles.push_back(g_assets[i].triangleIndices[e] + currInd);
				}
			}
			if (g_assets[i].numSprings && g_assets[i].springIndices.size() == g_assets[i].numSprings * 2 && g_assets[i].springRestLengths.size() == g_assets[i].numSprings && g_assets[i].springStiffness.size() == g_assets[i].numSprings)
			{
				for (size_t e = 0; e < g_assets[i].numSprings; e++)
				{
					g_buffers->springIndices.push_back(g_assets[i].springIndices[e * 2] + currInd);
					g_buffers->springIndices.push_back(g_assets[i].springIndices[(e * 2) + 1] + currInd);

					g_buffers->springLengths.push_back(g_assets[i].springRestLengths[e]);
					g_buffers->springStiffness.push_back(g_assets[i].springStiffness[e]);
				}
			}
			//Application().LogMessage(CString("START RIGIDS"));
			//Application().LogMessage(CString(g_assets[i].numShapeIndices));
			//Application().LogMessage(CString(g_assets[i].shapeOffsets.size()));
			//Application().LogMessage(CString(g_assets[i].shapeCenters.size()));
			//Application().LogMessage(CString(g_assets[i].shapeCoefficients.size()));
			//Application().LogMessage(CString(g_assets[i].shapeIndices.size() != 0));
			
			//if (g_assets[i].numShapes>0 && g_assets[i].numShapes == g_assets[i].shapeOffsets.size() == g_assets[i].shapeCenters.size() == g_assets[i].shapeCoefficients.size() == g_assets[i].shapeRots.size() && g_assets[i].shapeIndices.size() != 0)
			//{
				if(g_buffers->rigidOffsets.empty())
					g_buffers->rigidOffsets.push_back(0);
				g_assets[i].startrigid = numRigids;
				int shapeOff = g_buffers->rigidOffsets.back();
				for (int e = 0; e < g_assets[i].numShapeIndices; ++e)
				{
					g_buffers->rigidIndices.push_back(g_assets[i].shapeIndices[e] + currInd);
				}
				for (int e = 0; e < g_assets[i].rigidLocalPos.size(); ++e)
				{
					g_buffers->rigidLocalPositions.push_back(g_assets[i].rigidLocalPos[e]);
				}
				for (int e = 0; e < g_assets[i].numShapes; ++e)
				{
					g_buffers->rigidOffsets.push_back(g_assets[i].shapeOffsets[e] + shapeOff);
					g_buffers->rigidTranslations.push_back(g_assets[i].shapeCenters[e]);
					g_buffers->rigidRotations.push_back(g_assets[i].shapeRots[e]);
					g_buffers->rigidCoefficients.push_back(g_assets[i].shapeCoefficients[e]);
					numRigids++;
				}
			//}
			currInd += g_assets[i].nbpoints;
		}
	}
}

void FlexSystem::updateAssets() {

	if (!g_buffers->maped)
		g_buffers->MapBuffers();
	for (size_t i = 0; i < nAssets; i++)
	{
		if (g_assets[i].numShapes > 0) {
			for (size_t e = 0; e < g_assets[i].numShapes; e++)
			{
				g_assets[i].shapeCenters[e] = g_buffers->rigidTranslations[e + g_assets[i].startrigid];
				g_assets[i].shapeRots[e] = g_buffers->rigidRotations[e + g_assets[i].startrigid];
			}
		}
	}


}

void FlexSystem::update(){


		activeParticles = NvFlexGetActiveCount(g_flex);

		if (g_buffers->maped)
			g_buffers->UnmapBuffers();
		
		//Set Active Indices
		if (g_buffers->activeIndices.size())
			NvFlexSetActive(g_flex, g_buffers->activeIndices.buffer, g_buffers->activeIndices.size());

		if (g_buffers->positions.size()) {
			//Set Rests
			NvFlexSetRestParticles(g_flex, g_buffers->restPositions.buffer, g_buffers->restPositions.size());
			//Set Particles
			NvFlexSetParticles(g_flex, g_buffers->positions.buffer, g_buffers->positions.size());
			//Set Velocities
			NvFlexSetVelocities(g_flex, g_buffers->velocities.buffer, g_buffers->velocities.size());
			//Set Phases
			NvFlexSetPhases(g_flex, g_buffers->phases.buffer, g_buffers->phases.size());
			//Set Springs
			NvFlexSetSprings(g_flex, g_buffers->springIndices.buffer, g_buffers->springLengths.buffer, g_buffers->springStiffness.buffer, g_buffers->springLengths.size());
			//Set trinagles
			if (g_numtriangles !=0)
				NvFlexSetDynamicTriangles(g_flex, g_buffers->triangles.buffer, g_buffers->normals.buffer, g_numtriangles);
			if(g_numInflatables != 0)
				NvFlexSetInflatables(g_flex, g_buffers->inflatableTriOffsets.buffer, g_buffers->inflatableTriCounts.buffer, g_buffers->inflatableVolumes.buffer, g_buffers->inflatablePressures.buffer, g_buffers->inflatableCoefficients.buffer, g_numInflatables);
			//Set rigids g_buffers->rigidLocalNormals.buffer
			if (g_buffers->rigidOffsets.size())
				NvFlexSetRigids(g_flex, g_buffers->rigidOffsets.buffer, g_buffers->rigidIndices.buffer, g_buffers->rigidLocalPositions.buffer,NULL , g_buffers->rigidCoefficients.buffer, g_buffers->rigidRotations.buffer, g_buffers->rigidTranslations.buffer, g_buffers->rigidOffsets.size() - 1, g_buffers->rigidIndices.size());
		}

		setShapes();

		NvFlexSetParams(g_flex, &g_params);
		NvFlexUpdateSolver(g_flex,g_dt, g_numSubsteps, g_profile);

		float lat = NvFlexGetDeviceLatency(g_flex);

		if(g_buffers->positions.size()){

			//Get Particles
			NvFlexGetParticles(g_flex, g_buffers->positions.buffer, g_buffers->positions.size());
			//Get Velocities
			NvFlexGetVelocities(g_flex, g_buffers->velocities.buffer, g_buffers->velocities.size());
			if (!nAssets == 0) {
				//Get Densities
				NvFlexGetDensities(g_flex, g_buffers->densities.buffer, g_buffers->densities.size());
				//Get Smooth Positions
				if (g_params.smoothing > 0)
					NvFlexGetSmoothParticles(g_flex, g_buffers->smoothPositions.buffer, g_buffers->smoothPositions.size());
				if (g_params.anisotropyScale > 0)
					NvFlexGetAnisotropy(g_flex, g_buffers->anisotropy1.buffer, g_buffers->anisotropy2.buffer, g_buffers->anisotropy3.buffer);
				//Get Diffuse Particles
				g_numDiffuse = NvFlexGetDiffuseParticles(g_flex, NULL, NULL, NULL);
				if (g_numDiffuse)
					NvFlexGetDiffuseParticles(g_flex, g_buffers->diffusePositions.buffer, g_buffers->diffuseVelocities.buffer, g_buffers->diffuseIndices.buffer);
				//Get rigids
				if (g_buffers->rigidOffsets.size())
				{
					NvFlexGetRigidTransforms(g_flex, g_buffers->rigidRotations.buffer, g_buffers->rigidTranslations.buffer);
				}
			}
		}

		activeParticles = NvFlexGetActiveCount(g_flex);

}