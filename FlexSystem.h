#pragma once

#include <stddef.h>

#include <NvFlex.h>
#include <vector>

#include <types.h>
#include <maths.h>


#include <platform.h>
#include <mesh.h>

#include <perlin.h>


#include <NvFlexExt.h>
#include <NvFlexDevice.h>

#include <xsi_application.h>
#include <xsi_context.h>
#include <xsi_plugin.h>
#include <xsi_pluginregistrar.h>
#include <xsi_status.h>

#include <xsi_icenodecontext.h>
#include <xsi_icenodedef.h>
#include <xsi_command.h>
#include <xsi_factory.h>
#include <xsi_longarray.h>
#include <xsi_doublearray.h>
#include <xsi_math.h>
#include <xsi_vector2f.h>
#include <xsi_vector3f.h>
#include <xsi_vector4f.h>
#include <xsi_matrix3f.h>
#include <xsi_matrix4f.h>
#include <xsi_rotationf.h>
#include <xsi_quaternionf.h>
#include <xsi_color4f.h>
#include <xsi_shape.h>
#include <xsi_icegeometry.h>
#include <xsi_iceportstate.h>
#include <xsi_indexset.h>
#include <xsi_dataarray.h>
#include <xsi_dataarray2D.h>
#include <memory.h>

using namespace std;
using namespace XSI;
inline float sqr(float x) { return x*x; }

struct SimBuffers
{
	NvFlexVector<Vec4> positions;
	NvFlexVector<Vec4> restPositions;
	NvFlexVector<Vec3> velocities;
	NvFlexVector<int> phases;
	NvFlexVector<float> densities;
	NvFlexVector<Vec4> anisotropy1;
	NvFlexVector<Vec4> anisotropy2;
	NvFlexVector<Vec4> anisotropy3;
	NvFlexVector<Vec4> normals;
	NvFlexVector<Vec4> smoothPositions;
	NvFlexVector<Vec4> diffusePositions;
	NvFlexVector<Vec4> diffuseVelocities;
	NvFlexVector<int> diffuseIndices;
	NvFlexVector<int> activeIndices;

	// convexes
	NvFlexVector<NvFlexCollisionGeometry> shapeGeometry;
	NvFlexVector<Vec4> shapePositions;
	NvFlexVector<Quat> shapeRotations;
	NvFlexVector<Vec4> shapePrevPositions;
	NvFlexVector<Quat> shapePrevRotations;
	NvFlexVector<int> shapeFlags;

	// rigids
	NvFlexVector<int> rigidOffsets;
	NvFlexVector<int> rigidIndices;
	NvFlexVector<int> rigidMeshSize;
	NvFlexVector<float> rigidCoefficients;
	NvFlexVector<Vec4> rigidRotations;
	NvFlexVector<Vec3> rigidTranslations;
	NvFlexVector<Vec3> rigidLocalPositions;
	NvFlexVector<Vec4> rigidLocalNormals;

	// inflatables
	NvFlexVector<int> inflatableTriOffsets;
	NvFlexVector<int> inflatableTriCounts;
	NvFlexVector<float> inflatableVolumes;
	NvFlexVector<float> inflatableCoefficients;
	NvFlexVector<float> inflatablePressures;

	// springs
	NvFlexVector<int> springIndices;
	NvFlexVector<float> springLengths;
	NvFlexVector<float> springStiffness;

	NvFlexVector<int> triangles;
	NvFlexVector<Vec3> triangleNormals;
	NvFlexVector<Vec3> uvs;


	bool maped = false;

	SimBuffers(NvFlexLibrary* l);
	~SimBuffers();

	void MapBuffers();
	void UnmapBuffers();
	void InitBuffers();

};

class pcFlexAsset
{
	public:
	pcFlexAsset();
	~pcFlexAsset();
	XSI::CString name;
	NvFlexExtAsset* flexAsset = NULL;
	
	//int maxParticles;
	int startpoint;
	int startrigid;
	

	int nbpoints;
	int nbRestPoints;
	int nbVelocities;
	int nbPhases;
	std::vector<Vec4> fpoints;
	std::vector<Vec4> frestpoints;
	std::vector<Vec3> fvelocities;
	std::vector<int> phases;
	std::vector<float> densities;

	int numSprings;
	std::vector<int> springIndices;
	std::vector<float> springRestLengths;
	std::vector<float> springStiffness;
	
	int nbtriangles;
	std::vector<int> triangleIndices;

	int inflatstartTri;
	float inflatRest;
	float inflatcoef;
	float inflatpressure;

	int numShapeIndices;
	std::vector<int> shapeIndices;
	std::vector<int> shapeOffsets;
	std::vector<float> shapeCoefficients;
	//std::vector<float> shapeRestCenters;
	std::vector<Vec3> shapeCenters;
	std::vector<Vec4> shapeRots;
	std::vector<Vec3> rigidLocalPos;
	int numShapes;
};

class FlexSystem {

	public:

	FlexSystem();
	~FlexSystem();

	void initSystem();
	void initParams();
	void updateParams();

	void getSimTimers();

	void GetParticleBounds(Vec3& lower, Vec3& upper);
	
	void ClearShapes(bool del=false);
	void setShapes();
	void setAssets();
	void updateAssets();


	void AddTriangleMesh(Vec3 scale, Vec3 center, Quat quat, std::vector<Vec3> vertices, std::vector<int> indices, int numvertices, int numtriangles, bool dynamic);
	void AddShape(int type,Vec3 halfEdge = Vec3(2.0f), Vec3 center = Vec3(0.0f), Vec3 prevCenter = Vec3(0.0f), Quat quat = Quat(), Quat prevQuat = Quat(), bool dynamic = false);



	void update();

	void initScene(NvFlexLibrary* g_flexLib,int maxparticles, int maxdiffuse, int maxneis);
	
	
	NvFlexSolver* g_flex;
	NvFlexLibrary* g_flexLib;

	NvFlexParams g_params;
	NvFlexParams g_defaultParams;

	SimBuffers* g_buffers;

	
	vector<int> g_inactiveIndices;

	bool g_profile;

	int activeParticles;
	int maxParticles;
	int numDiffuse;
	int g_numtriangles;
	int g_numDiffuse;
	int g_numInflatables;

	vector<pcFlexAsset> g_assets;

	int g_numSubsteps;
	float g_dt;	// the time delta used for simulation

	Vec3 g_sceneLower;
	Vec3 g_sceneUpper;


	int g_maxDiffuseParticles;
	unsigned char g_maxNeighborsPerParticle;


	int nEmitter;
	int nVolumeBoxes;
	int nAssets;

	double time1;
	double time2;
	double time3;
	double time4;

	NvFlexTimers g_timers;

	float simLatency;

	int cursor;
	int ID;




};
// calculates local space positions given a set of particles and rigid indices
inline void CalculateRigidLocalPositions(std::vector<float> restPositions, std::vector<int> offsets, std::vector<int> indices, int numRigids,std::vector<Vec3> &localPositions)
{

	// To improve the accuracy of the result, first transform the restPositions to relative coordinates (by finding the mean and subtracting that from all points)
	// Note: If this is not done, one might see ghost forces if the mean of the restPositions is far from the origin.

	// Calculate mean
	Vec3 shapeOffset(0.0f);
	for (int i = 0; i < restPositions.size()/4; i++)
	{
		shapeOffset += Vec3(restPositions[(i*4)], restPositions[(i * 4)+1], restPositions[(i * 4)+2]);
	}

	shapeOffset /= float(restPositions.size() / 4);
	int count = 0;

	for (int r = 0; r < numRigids; ++r)
	{
		
		const int startIndex = offsets[r];
		const int endIndex = offsets[r + 1];

		const int n = endIndex - startIndex;
		assert(n);

		Vec3 com;

		for (int i = startIndex; i < endIndex; ++i)
		{
			const int r = indices[i];

			// By substracting meshOffset the calculation is done in relative coordinates
			com += Vec3(restPositions[(r * 4)], restPositions[(r * 4) + 1], restPositions[(r * 4) + 2]) - shapeOffset;
		}

		com /= float(n);
		for (int i = startIndex; i < endIndex; ++i) 
		{
			const int r = indices[i];
			// By substracting meshOffset the calculation is done in relative coordinates
			localPositions.push_back((Vec3(restPositions[(r * 4)], restPositions[(r * 4) + 1], restPositions[(r * 4) + 2]) - shapeOffset) - com);
		}
	}
}