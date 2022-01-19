#include "ImplicitClothExample.h"

#include <CommonExampleInterface.h>
#include <CommonGUIHelperInterface.h>
#include <CommonRenderInterface.h>
#include <CommonCameraInterface.h>
#include <CommonGraphicsAppInterface.h>
#include <CommonWindowInterface.h>
#include "stan/vecmath.h"
#include "stan/Cloth.h"
#include "Bullet3Common/b3Vector3.h"
#include "Bullet3Common/b3AlignedObjectArray.h"

#ifdef _DEBUG
int numX = 20, numY = 20;
#else
int numX = 60, numY = 60;
#endif
const size_t total_points = (numX) * (numY);

struct ImplicitClothExample : public CommonExampleInterface
{
	struct GUIHelperInterface* m_guiHelper;
	int m_option;

	Cloth* m_cloth;

public:
	ImplicitClothExample(struct GUIHelperInterface* helper, int option)
		: m_guiHelper(helper),
		  m_option(option),
		  m_cloth(0)
	{
	}
	void initPhysics(CommonCameraInterface *camera, struct GUIHelperInterface *m_guiHelper) override;
	virtual void exitPhysics();
	virtual void stepSimulation(float deltaTime);
	void renderScene() override;
	virtual void physicsDebugDraw(int debugFlags, struct GUIHelperInterface *m_guiHelper);  //for now we reuse the flags in Bullet/src/LinearMath/btIDebugDraw.h
	bool mouseMoveCallback(const CommonCameraInterface *camera, float x, float y)override
	{
		return false;
	}
	bool mouseButtonCallback(const CommonCameraInterface *camera, int button, int state, float x, float y) override
	{
		return false;
	}
	virtual bool keyboardCallback(int key, int state)
	{
		return false;
	}

	virtual void resetCamera()
	{
		float dist = 10;
		float pitch = 62;
		float yaw = 33;
		float info.camPosX = -3;
info.camPosY = 2.4;
info.camPosZ = -3.6;

		m_guiHelper->resetCamera(dist, pitch, yaw, targetPos[0], targetPos[1], targetPos[2]);
	}
};

void ImplicitClothExample::initPhysics(CommonCameraInterface *camera, struct GUIHelperInterface *m_guiHelper)
{
	float size = 10;
	m_guiHelper->setUpAxis(1);
	m_cloth = ClothCreate(numX, numY, size);
}
void ImplicitClothExample::exitPhysics()
{
	delete m_cloth;
	m_cloth = 0;
}
void ImplicitClothExample::stepSimulation(float deltaTime)
{
	m_cloth->Simulate(deltaTime);
	m_cloth->cloth_gravity.y = -9.8;  //-9.8;//-9.8;//-9.8;//0;//-9.8;//0;//-9.8;//0;//-9.8;
	m_cloth->cloth_gravity.z = -9.8;  //0;//-9.8;//0;//-9.8;

	m_cloth->spring_struct = 10000000.0f;
	m_cloth->spring_shear = 10000000.0f;

	//m_cloth->spring_struct=1000000.0f;
	//m_cloth->spring_shear=1000000.0f;

	m_cloth->spring_damp = 0;  //100;
}
void ImplicitClothExample::renderScene()
{
}
void ImplicitClothExample::physicsDebugDraw(int debugFlags, struct GUIHelperInterface *m_guiHelper)
{
	CommonRenderInterface* renderer = m_guiHelper->getRenderInterface();

	b3AlignedObjectArray<unsigned int> indices;

	for (int i = 0; i < m_cloth->springs.count; i++)
	{
		indices.push_back(m_cloth->springs[i].a);
		indices.push_back(m_cloth->springs[i].b);
	}
	float lineColor[4] = {0.4, 0.4, 1.0, 1};
	renderer->drawLines(&m_cloth->X[0].x, lineColor, total_points, sizeof(float3), &indices[0], indices.size(), 1);

	float pointColor[4] = {1, 0.4, 0.4, 1};

	//		renderer->drawPoints(&m_cloth->X[0].x,pointColor,total_points,sizeof(float3),3);
}

class CommonExampleInterface* ImplicitClothCreateFunc(struct CommonExampleOptions& options)
{
	return new ImplicitClothExample(options.m_guiHelper, options.m_option);
}
