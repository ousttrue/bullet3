/*
Bullet Collision Detection and Physics Library http://bulletphysics.org
This file is Copyright (c) 2014 Google Inc. 

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

//original author: Erwin Coumans
*/

#include "ImportColladaSetup.h"
#include <vector>
#include <GLInstancingRenderer.h>
#include <GLInstanceGraphicsShape.h>
#include "CommonCameraInterface.h"
#include "btBulletDynamicsCommon.h"
#include "LoadMeshFromCollada.h"
#include "Bullet3Common/b3FileUtils.h"
#include "../../Utils/b3ResourcePath.h"
#include "../../Utils/b3BulletDefaultFileIO.h"
#include <CommonRigidBodyBase.h>

class ImportColladaSetup : public CommonRigidBodyBase
{
public:
	ImportColladaSetup(struct GUIHelperInterface* helper);
	virtual ~ImportColladaSetup();

	void initPhysics(CommonCameraInterface *camera, struct GUIHelperInterface *m_guiHelper) override;
	CameraResetInfo cameraResetInfo() const override
	{
		CameraResetInfo info;
		info.camDist = 16;
		info.pitch = -28;
		info.yaw = -140;
		info.camPosX = -4;
		info.camPosY = -3;
		info.camPosZ = -3;
		info.upAxis = 1;
		return info;
	}
};

ImportColladaSetup::ImportColladaSetup(struct GUIHelperInterface* helper)
	: CommonRigidBodyBase(helper)
{
}

ImportColladaSetup::~ImportColladaSetup()
{
}

static int ColladaGraphicsInstanceSortfnc(const ColladaGraphicsInstance& a, const ColladaGraphicsInstance& b)
{
	if (a.m_shapeIndex < b.m_shapeIndex) return +1;
	if (a.m_shapeIndex > b.m_shapeIndex) return -1;
	return 0;
}

void ImportColladaSetup::initPhysics(CommonCameraInterface *camera, struct GUIHelperInterface *m_guiHelper)
{
	m_physics = new Physics;
	auto m_dynamicsWorld = m_physics->getDynamicsWorld();
	m_guiHelper->createPhysicsDebugDrawer(m_dynamicsWorld);
	m_dynamicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);

	static int fileIndex = 0;

	const char* fileNames[] = {
		"duck.dae",
		"seymourplane_triangulate.dae",
	};
	const char* fileName = fileNames[fileIndex];
	int numFiles = sizeof(fileNames) / sizeof(const char*);

	char relativeFileName[1024];

	if (!b3ResourcePath::findResourcePath(fileName, relativeFileName, 1024, 0))
		return;

	btVector3 shift(0, 0, 0);
	btVector3 scaling(1, 1, 1);
	//	int index=10;

	{
		btAlignedObjectArray<GLInstanceGraphicsShape> visualShapes;
		btAlignedObjectArray<ColladaGraphicsInstance> visualShapeInstances;

		float unitMeterScaling(1);
		btTransform upAxisTrans;
		upAxisTrans.setIdentity();

		btVector4 color(0, 0, 1, 1);

#ifdef COMPARE_WITH_ASSIMP
		static int useAssimp = 0;
		if (useAssimp)
		{
			LoadMeshFromColladaAssimp(relativeFileName, visualShapes, visualShapeInstances, upAxisTrans, unitMeterScaling);
			fileIndex++;
			if (fileIndex >= numFiles)
			{
				fileIndex = 0;
			}
			color.setValue(1, 0, 0);
		}
		else
		{
			LoadMeshFromCollada(relativeFileName, visualShapes, visualShapeInstances, upAxisTrans, unitMeterScaling);
		}
		useAssimp = 1 - useAssimp;
#else
		fileIndex++;
		if (fileIndex >= numFiles)
		{
			fileIndex = 0;
		}
		b3BulletDefaultFileIO fileIO;
		LoadMeshFromCollada(relativeFileName, visualShapes, visualShapeInstances, upAxisTrans, unitMeterScaling, cameraResetInfo().upAxis, &fileIO);
#endif  // COMPARE_WITH_ASSIMP

		//at the moment our graphics engine requires instances that share the same visual shape to be added right after registering the shape
		//so perform a sort, just to be sure
		visualShapeInstances.quickSort(ColladaGraphicsInstanceSortfnc);

		for (int i = 0; i < visualShapeInstances.size(); i++)
		{
			ColladaGraphicsInstance* instance = &visualShapeInstances[i];
			GLInstanceGraphicsShape* gfxShape = &visualShapes[instance->m_shapeIndex];
			btVector3 position(0, 0, 0);   // = scaling*btVector3(instance->m_pos[0],instance->m_pos[1],instance->m_pos[2]);
			btQuaternion orn(0, 0, 0, 1);  //instance->m_orn[0],instance->m_orn[1],instance->m_orn[2],instance->m_orn[3]);

			//sort the visualShapeInstances, then iterate etc
			//void LoadMeshFromCollada(const char* relativeFileName,
			//btAlignedObjectArray<GLInstanceGraphicsShape>& visualShapes,
			//btAlignedObjectArray<GLInstanceGraphicsInstance> visualShapeInstances);

			if (gfxShape)
			{
				//btTransform trans;
				//trans.setIdentity();
				//trans.setRotation(btQuaternion(btVector3(1,0,0),SIMD_HALF_PI));

				b3AlignedObjectArray<GLInstanceVertex> verts;
				verts.resize(gfxShape->m_vertices->size());

				for (int i = 0; i < gfxShape->m_vertices->size(); i++)
				{
					verts[i].normal[0] = gfxShape->m_vertices->at(i).normal[0];
					verts[i].normal[1] = gfxShape->m_vertices->at(i).normal[1];
					verts[i].normal[2] = gfxShape->m_vertices->at(i).normal[2];
					verts[i].uv[0] = gfxShape->m_vertices->at(i).uv[0];
					verts[i].uv[1] = gfxShape->m_vertices->at(i).uv[1];
					verts[i].xyzw[0] = gfxShape->m_vertices->at(i).xyzw[0];
					verts[i].xyzw[1] = gfxShape->m_vertices->at(i).xyzw[1];
					verts[i].xyzw[2] = gfxShape->m_vertices->at(i).xyzw[2];
					verts[i].xyzw[3] = gfxShape->m_vertices->at(i).xyzw[3];
				}

				//compensate upAxisTrans and unitMeterScaling here
				btMatrix4x4 upAxisMat;
				upAxisMat.setPureRotation(upAxisTrans.getRotation());
				btMatrix4x4 unitMeterScalingMat;
				unitMeterScalingMat.setPureScaling(btVector3(unitMeterScaling, unitMeterScaling, unitMeterScaling));
				btMatrix4x4 worldMat = unitMeterScalingMat * upAxisMat * instance->m_worldTransform;
				//btMatrix4x4 worldMat = instance->m_worldTransform;
				for (int v = 0; v < verts.size(); v++)
				{
					btVector3 pos(verts[v].xyzw[0], verts[v].xyzw[1], verts[v].xyzw[2]);
					pos = worldMat * pos;
					verts[v].xyzw[0] = float(pos[0]);
					verts[v].xyzw[1] = float(pos[1]);
					verts[v].xyzw[2] = float(pos[2]);
				}

				int shapeId = m_guiHelper->getRenderInterface()->registerShape(&verts[0].xyzw[0], gfxShape->m_numvertices, &gfxShape->m_indices->at(0), gfxShape->m_numIndices);

				//btVector3 instanceScaling(instance->m_scaling[0],instance->m_scaling[1],instance->m_scaling[2]);
				m_guiHelper->getRenderInterface()->registerGraphicsInstance(shapeId, position, orn, color, scaling);
			}
		}
	}
}

class CommonExampleInterface* ImportColladaCreateFunc(struct CommonExampleOptions& options)
{
	return new ImportColladaSetup(options.m_guiHelper);
}
