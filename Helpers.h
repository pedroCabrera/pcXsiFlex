#pragma once

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

#include <iostream>
#include <map>
//Include FLEX
#include <NvFlex.h>
#include <NvFlexExt.h>
#include "FlexSystem.h"

using namespace XSI;
inline void ErrorCallback(NvFlexErrorSeverity, const char* msg, const char* file, int line)
{
	printf("Flex: %s - %s:%d\n", msg, file, line);
	//g_Error = true;
	//assert(0); asserts are bad for TeamCity
}

inline MATH::CTransformation Matrix4ToTransf(MATH::CMatrix4f mat) {

	MATH::CTransformation trans;
	float io_f00, io_f01, io_f02, io_f03, io_f10, io_f11, io_f12, io_f13, io_f20, io_f21, io_f22, io_f23, io_f30, io_f31, io_f32, io_f33;
	mat.Get(io_f00, io_f01, io_f02, io_f03, io_f10, io_f11, io_f12, io_f13, io_f20, io_f21, io_f22, io_f23, io_f30, io_f31, io_f32, io_f33);

	trans.SetMatrix4(XSI::MATH::CMatrix4(io_f00, io_f01, io_f02, io_f03, io_f10, io_f11, io_f12, io_f13, io_f20, io_f21, io_f22, io_f23, io_f30, io_f31, io_f32, io_f33));

	return trans;
}

inline void DecomposeTranf(MATH::CTransformation transf,Vec3& scale,Vec3& pos,Quat& quat) {

	scale.Set(transf.GetScaling().GetX(), transf.GetScaling().GetY(), transf.GetScaling().GetZ());
	pos.Set(transf.GetPosX(), transf.GetPosY(), transf.GetPosZ());
	quat.Set(transf.GetRotationQuaternion().GetX(), transf.GetRotationQuaternion().GetY(), transf.GetRotationQuaternion().GetZ(), transf.GetRotationQuaternion().GetW());
}

inline void GeoToCollideMesh(FlexSystem *FlexSys, CICEGeometry geom, bool dynamic=false) {
	if (geom.GetPointPositionCount() != 0)
	{

		CDoubleArray points;
		CLongArray indices;
		ULONG nCount = geom.GetTriangleCount();
		geom.GetPointPositions(points);
		geom.GetTrianglePointIndices(indices);

		Vec3 pos, scale;
		Quat q;
		XSI::MATH::CMatrix4f mat;
		XSI::MATH::CTransformation iceTrans;
		XSI::MATH::CTransformation prevTrans;

		geom.GetTransformation(mat);
		
		iceTrans = Matrix4ToTransf(mat);
		DecomposeTranf(iceTrans, scale, pos, q);


		std::vector<Vec3> v3fArray;
		std::vector<int> indArray;
		ULONG nOffset = 0;
		for (ULONG i = 0; i < geom.GetPointPositionCount(); i++)
		{
			float x, y, z;
			x = points[nOffset];	nOffset++;
			y = points[nOffset];	nOffset++;
			z = points[nOffset];	nOffset++;
			Vec3 v(x, y, z);
			v3fArray.push_back(v);
		}
		for (size_t i = 0; i < indices.GetCount(); i++)
		{
			indArray.push_back(indices[i]);
		}
		FlexSys->AddTriangleMesh(scale, pos, q, v3fArray, indArray, v3fArray.size(), nCount, dynamic);
	}
}


inline void TransformToCollideShape(FlexSystem *FlexSys, XSI::MATH::CMatrix4f transform, XSI::MATH::CMatrix4f prevTransform, bool dynamic, int type) {

	Vec3 pos, scale,prevpos,prevScale;
	Quat q,prevq;
	XSI::MATH::CTransformation iceTrans;
	iceTrans = Matrix4ToTransf(transform);
	DecomposeTranf(iceTrans, scale, pos, q);
	iceTrans = Matrix4ToTransf(prevTransform);
	DecomposeTranf(iceTrans, prevScale, prevpos, prevq);

	FlexSys->AddShape(type, scale, pos, prevpos, q, prevq, dynamic);

}
struct CollisionAsset
{
public:
	NvFlexCollisionGeometry geo;
	NvFlexCollisionShapeType type;
	bool dynamic;
	Vec3 pos, prevPos;
	Quat rot, prevRot;

};

inline CollisionAsset TransformToCollideShape2(XSI::MATH::CMatrix4f transform, XSI::MATH::CMatrix4f prevTransform, bool dynamic, int type) {

	CollisionAsset collider;

	Vec3 pos, scale, prevpos, prevScale;
	Quat q, prevq;
	XSI::MATH::CTransformation iceTrans;
	iceTrans = Matrix4ToTransf(transform);
	DecomposeTranf(iceTrans, scale, pos, q);
	iceTrans = Matrix4ToTransf(prevTransform);
	DecomposeTranf(iceTrans, prevScale, prevpos, prevq);

	collider.pos = pos;
	collider.prevPos = prevpos;
	collider.rot = q;
	collider.prevRot = prevq;

	switch (type)
	{
		case 0: {
			collider.geo.sphere.radius = scale.x;
			collider.type = eNvFlexShapeSphere;
			collider.dynamic = dynamic;
		}break;
		case 1: {
			collider.geo.box.halfExtents[0] = scale.x*0.5;
			collider.geo.box.halfExtents[1] = scale.y*0.5;
			collider.geo.box.halfExtents[2] = scale.z*0.5;
			collider.type = eNvFlexShapeBox;
			collider.dynamic = dynamic;
		}break;
		case 2: {
			collider.geo.capsule.radius = scale.y*0.5;
			collider.geo.capsule.halfHeight = scale.x*0.5;
			collider.type = eNvFlexShapeCapsule;
			collider.dynamic = dynamic;
		}break;
		default: {
			collider.geo.sphere.radius = scale.x;
			collider.type = eNvFlexShapeSphere;
			collider.dynamic = dynamic;
		}break;
	}
	return collider;
};
inline CollisionAsset GeoToCollideMesh2(NvFlexLibrary* g_FlexLib,CICEGeometry geom, bool dynamic = false,Vec3 prevPos = Vec3(FLT_MAX),Quat prevQuat = Quat(FLT_MAX, FLT_MAX)) {
	if (geom.GetPointPositionCount() != 0)
	{
		CollisionAsset collider;
		CDoubleArray points;
		CLongArray indices;
		ULONG nCount = geom.GetTriangleCount();
		geom.GetPointPositions(points);
		geom.GetTrianglePointIndices(indices);

		Vec3 pos, scale;
		Quat q;
		XSI::MATH::CMatrix4f mat;
		XSI::MATH::CTransformation iceTrans;
		XSI::MATH::CTransformation prevTrans;

		geom.GetTransformation(mat);

		iceTrans = Matrix4ToTransf(mat);
		DecomposeTranf(iceTrans, scale, pos, q);

		ULONG nOffset = 0;

		NvFlexBuffer* positionsBuf = NvFlexAllocBuffer(g_FlexLib, geom.GetPointPositionCount(), sizeof(Vec3), eNvFlexBufferHost);
		Vec3* positions = (Vec3*)NvFlexMap(positionsBuf, eNvFlexMapWait);
		NvFlexBuffer* indicesBuf = NvFlexAllocBuffer(g_FlexLib, indices.GetCount(), sizeof(int), eNvFlexBufferHost);
		int* indicesArr = (int*)NvFlexMap(indicesBuf, eNvFlexMapWait);
		Vec3 lower = Vec3(FLT_MAX);
		Vec3 upper = Vec3(-FLT_MAX);

		for (ULONG i = 0; i < geom.GetPointPositionCount(); i++)
		{
			float x, y, z;
			x = points[nOffset];	nOffset++;
			y = points[nOffset];	nOffset++;
			z = points[nOffset];	nOffset++;
			Vec3 v(x, y, z);
			positions[i] = v;
			lower = Min(positions[i], lower);
			upper = Max(positions[i], upper);
		}
		for (size_t i = 0; i < indices.GetCount(); i++)
		{
			indicesArr[i] = indices[i];
		}
		NvFlexUnmap(positionsBuf);
		NvFlexUnmap(indicesBuf);

		// transform
		collider.pos = pos;
		collider.rot = q;
		if (prevPos != Vec3(FLT_MAX))
			collider.prevPos = prevPos;
		else
			collider.prevPos = pos;
		if (prevQuat != Quat(FLT_MAX, FLT_MAX))
			collider.prevRot = prevQuat;
		else
			collider.prevRot = q;

		NvFlexCollisionGeometry geo;
		geo.triMesh.mesh = NvFlexCreateTriangleMesh(g_FlexLib);
		geo.triMesh.scale[0] = scale.x;
		geo.triMesh.scale[1] = scale.y;
		geo.triMesh.scale[2] = scale.z;
		NvFlexUpdateTriangleMesh(g_FlexLib, geo.triMesh.mesh, positionsBuf, indicesBuf, geom.GetPointPositionCount(), geom.GetTriangleCount(), lower, upper);

		collider.geo = geo;
		//NvFlexDestroyTriangleMesh(g_FlexLib,geo.triMesh.mesh);
		collider.type = eNvFlexShapeTriangleMesh;
		collider.dynamic = dynamic;
		return collider;
	}
	
};

template < class T >
inline vector<T> ensureArray(ICENodeContext& in_ctxt, ULONG ID) {

	XSI::siICENodeDataType inPortType;
	XSI::siICENodeStructureType inPortStruct;
	XSI::siICENodeContextType inPortContext;
	in_ctxt.GetPortInfo(ID, inPortType, inPortStruct, inPortContext);

	vector<T> arr;
	if (inPortStruct == XSI::siICENodeStructureArray)
	{
		CDataArray2D<T> inData(in_ctxt, ID);
		typename CDataArray2D<T>::Accessor accesor = inData[0];
		for (size_t i = 0; i < accesor.GetCount(); i++)
		{
			arr.push_back(accesor[i]);
		}
	}
	else if (inPortStruct == XSI::siICENodeStructureSingle) {
		CDataArray<T> inData(in_ctxt, ID);
		arr.push_back(inData[0]);
	}
	return arr;
}