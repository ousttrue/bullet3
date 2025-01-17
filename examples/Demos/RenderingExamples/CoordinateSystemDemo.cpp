
#include "CoordinateSystemDemo.h"
#include <CommonGraphicsAppInterface.h>
#include <LinearMath/btTransform.h>
#include <assert.h>

CoordinateSystemDemo::CoordinateSystemDemo(CommonGraphicsApp* app)
	: m_app(app),
	  m_x(0),
	  m_y(0),
	  m_z(0)
{
	assert(m_app);

	int boxId = m_app->registerCubeShape(0.1, 0.1, 0.1);
	btVector3 pos(0, 0, 0);
	btQuaternion orn(0, 0, 0, 1);
	btVector4 color(0.3, 0.3, 0.3, 1);
	btVector3 scaling(1, 1, 1);
	m_app->m_renderer->registerGraphicsInstance(boxId, pos, orn, color, scaling);
	m_app->m_renderer->writeTransforms();
}

CameraResetInfo CoordinateSystemDemo::cameraResetInfo() const
{
	CameraResetInfo info;
	info.camDist = 3.5;
	info.pitch = -32;
	info.yaw = 136;
	info.camPosX = 0;
	info.camPosY = 0;
	info.camPosZ = 0;
	info.upAxis = 2;
	return info;
}

void CoordinateSystemDemo::drawArc(const btVector3& center, const btVector3& normal, const btVector3& axis, btScalar radiusA, btScalar radiusB, btScalar minAngle, btScalar maxAngle,
								   const btVector3& color, bool drawSect, btScalar stepDegrees)
{
	btScalar lineWidth = 3;
	const btVector3& vx = axis;
	btVector3 vy = normal.cross(axis);
	btScalar step = stepDegrees * SIMD_RADS_PER_DEG;
	int nSteps = (int)btFabs((maxAngle - minAngle) / step);
	if (!nSteps) nSteps = 1;
	btVector3 prev = center + radiusA * vx * btCos(minAngle) + radiusB * vy * btSin(minAngle);
	if (drawSect)
	{
		m_app->m_renderer->drawLine(center, prev, color, lineWidth);
	}
	for (int i = 1; i <= nSteps; i++)
	{
		btScalar angle = minAngle + (maxAngle - minAngle) * btScalar(i) / btScalar(nSteps);
		btVector3 next = center + radiusA * vx * btCos(angle) + radiusB * vy * btSin(angle);
		m_app->m_renderer->drawLine(prev, next, color, lineWidth);
		prev = next;
	}
	if (drawSect)
	{
		m_app->m_renderer->drawLine(center, prev, color, lineWidth);
	}
}

void CoordinateSystemDemo::physicsDebugDraw(int debugDrawFlags)
{
	btVector3 xUnit(1, 0, 0);
	btVector3 yUnit(0, 1, 0);
	btVector3 zUnit(0, 0, 1);

	btScalar lineWidth = 3;

	btQuaternion rotAroundX(xUnit, m_x);
	btQuaternion rotAroundY(yUnit, m_y);
	btQuaternion rotAroundZ(zUnit, m_z);

	btScalar radius = 0.5;
	btVector3 toX = radius * quatRotate(rotAroundX, yUnit);
	btVector3 toY = radius * quatRotate(rotAroundY, xUnit);
	btVector3 toZ = radius * quatRotate(rotAroundZ, xUnit);

	m_app->m_renderer->drawLine(xUnit + toX + quatRotate(rotAroundX, btVector3(0, 0.1, -0.2)), xUnit + toX, xUnit, lineWidth);
	m_app->m_renderer->drawLine(xUnit + toX + quatRotate(rotAroundX, btVector3(0, -0.2, -0.2)), xUnit + toX, xUnit, lineWidth);
	//draw the letter 'x' on the x-axis
	//m_app->m_renderer->drawLine(xUnit-0.1*zUnit+0.1*yUnit,xUnit+0.1*zUnit-0.1*yUnit,xUnit,lineWidth);
	//m_app->m_renderer->drawLine(xUnit+0.1*zUnit+0.1*yUnit,xUnit-0.1*zUnit-0.1*yUnit,xUnit,lineWidth);

	m_app->m_renderer->drawLine(xUnit + toX + quatRotate(rotAroundX, btVector3(0, -0.2, -0.2)), xUnit + toX, xUnit, lineWidth);

	m_app->m_renderer->drawLine(yUnit + toY + quatRotate(rotAroundY, btVector3(-0.2, 0, 0.2)), yUnit + toY, yUnit, lineWidth);
	m_app->m_renderer->drawLine(yUnit + toY + quatRotate(rotAroundY, btVector3(0.1, 0, 0.2)), yUnit + toY, yUnit, lineWidth);
	m_app->m_renderer->drawLine(zUnit + toZ + quatRotate(rotAroundZ, btVector3(0.1, -0.2, 0)), zUnit + toZ, zUnit, lineWidth);
	m_app->m_renderer->drawLine(zUnit + toZ + quatRotate(rotAroundZ, btVector3(-0.2, -0.2, 0)), zUnit + toZ, zUnit, lineWidth);

	drawArc(xUnit, xUnit, toX.normalized(), radius, radius, 0.4, SIMD_2_PI, xUnit, false);
	drawArc(yUnit, yUnit, toY.normalized(), radius, radius, 0.4, SIMD_2_PI, yUnit, false);
	drawArc(zUnit, zUnit, toZ.normalized(), radius, radius, 0.4, SIMD_2_PI, zUnit, false);
}
