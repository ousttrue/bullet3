#include "SimpleCamera.h"

#include "Bullet3Common/b3Vector3.h"
#include "Bullet3Common/b3Quaternion.h"
#include "Bullet3Common/b3Matrix3x3.h"
#include "Bullet3Common/b3Transform.h"
#include "CommonCallbacks.h"

B3_ATTRIBUTE_ALIGNED16(struct)
SimpleCameraInternalData
{
	SimpleCameraInternalData()
		: m_cameraTargetPosition(b3MakeVector3(0, 0, 0)),
		  m_cameraDistance(20),
		  m_cameraUp(b3MakeVector3(0, 1, 0)),
		  m_cameraForward(b3MakeVector3(1, 0, 0)),
		  m_cameraUpAxis(1),
		  m_yaw(20),
		  m_pitch(0),
		  m_aspect(1),
		  m_frustumZNear(0.01),
		  m_frustumZFar(1000),
		  m_enableVR(false)
	{
		b3Transform tr;
		tr.setIdentity();
		tr.getOpenGLMatrix(m_offsetTransformVR);
	}

	B3_DECLARE_ALIGNED_ALLOCATOR();

	B3_ATTRIBUTE_ALIGNED16(float)
	m_offsetTransformVR[16];
	b3Vector3 m_cameraTargetPosition;
	float m_cameraDistance;
	b3Vector3 m_cameraUp;
	b3Vector3 m_cameraForward;
	int m_cameraUpAxis;
	//the m_cameraPosition is a cached value, recomputed from other values
	b3Vector3 m_cameraPosition;
	float m_yaw;

	float m_pitch;

	float m_aspect;
	int m_screenWidth;
	int m_screenHeight;

	float m_frustumZNear;
	float m_frustumZFar;

	bool m_enableVR;
	float m_viewMatrixVR[16];
	float m_projectionMatrixVR[16];
};

SimpleCamera::SimpleCamera()
{
	m_data = new SimpleCameraInternalData;
}
SimpleCamera::~SimpleCamera()
{
	delete m_data;
}

bool SimpleCamera::mouseButtonCallback(int button, int state, float x, float y, ButtonFlags flags)
{
	switch (button)
	{
		case 0:
			m_leftMouseButton = (state == 1);
			break;
		case 1:
			m_middleMouseButton = (state == 1);
			break;
		case 2:
			m_rightMouseButton = (state == 1);
			break;
	}
	m_mouseXpos = x;
	m_mouseYpos = y;
	m_mouseInitialized = true;
	m_isAltPressed = flags & ButtonFlagsAlt;
	m_isControlPressed = flags & ButtonFlagsCtrl;
	return true;
}

bool SimpleCamera::mouseMoveCallback(float x, float y)
{
	if (m_isAltPressed || m_isControlPressed)
	{
		float xDelta = x - m_mouseXpos;
		float yDelta = y - m_mouseYpos;
		float cameraDistance = getCameraDistance();
		float pitch = getCameraPitch();
		float yaw = getCameraYaw();

		float targPos[3];
		float camPos[3];

		getCameraTargetPosition(targPos);
		getCameraPosition(camPos);

		b3Vector3 cameraPosition = b3MakeVector3(b3Scalar(camPos[0]),
												 b3Scalar(camPos[1]),
												 b3Scalar(camPos[2]));

		b3Vector3 cameraTargetPosition = b3MakeVector3(b3Scalar(targPos[0]),
													   b3Scalar(targPos[1]),
													   b3Scalar(targPos[2]));
		b3Vector3 cameraUp = b3MakeVector3(0, 0, 0);
		cameraUp[getCameraUpAxis()] = 1.f;

		if (m_leftMouseButton)
		{
			//			if (b3Fabs(xDelta)>b3Fabs(yDelta))
			//			{
			pitch -= yDelta * m_mouseMoveMultiplier;
			//			} else
			//			{
			yaw -= xDelta * m_mouseMoveMultiplier;
			//			}
		}

		if (m_middleMouseButton)
		{
			cameraTargetPosition += cameraUp * yDelta * m_mouseMoveMultiplier * 0.01;

			b3Vector3 fwd = cameraTargetPosition - cameraPosition;
			b3Vector3 side = cameraUp.cross(fwd);
			side.normalize();
			cameraTargetPosition += side * xDelta * m_mouseMoveMultiplier * 0.01;
		}
		if (m_rightMouseButton)
		{
			cameraDistance -= xDelta * m_mouseMoveMultiplier * 0.01f;
			cameraDistance -= yDelta * m_mouseMoveMultiplier * 0.01f;
			if (cameraDistance < 1)
				cameraDistance = 1;
			if (cameraDistance > 1000)
				cameraDistance = 1000;
		}
		setCameraDistance(cameraDistance);
		setCameraPitch(pitch);
		setCameraYaw(yaw);
		setCameraTargetPosition(cameraTargetPosition[0], cameraTargetPosition[1], cameraTargetPosition[2]);
	}

	m_mouseXpos = x;
	m_mouseYpos = y;
	m_mouseInitialized = true;
	return true;
}

bool SimpleCamera::wheelCallback(float deltax, float deltay)
{
	b3Vector3 cameraTargetPosition, cameraPosition, cameraUp = b3MakeVector3(0, 0, 0);
	cameraUp[getCameraUpAxis()] = 1;
	getCameraPosition(cameraPosition);
	getCameraTargetPosition(cameraTargetPosition);

	if (!m_leftMouseButton)
	{
		float cameraDistance = getCameraDistance();
		if (deltay < 0 || cameraDistance > 1)
		{
			cameraDistance -= deltay * m_wheelMultiplier;
			if (cameraDistance < 1)
				cameraDistance = 1;
			setCameraDistance(cameraDistance);
		}
		else
		{
			b3Vector3 fwd = cameraTargetPosition - cameraPosition;
			fwd.normalize();
			cameraTargetPosition += fwd * deltay * m_wheelMultiplier;  //todo: expose it in the GUI?
		}
	}
	else
	{
		if (b3Fabs(deltax) > b3Fabs(deltay))
		{
			b3Vector3 fwd = cameraTargetPosition - cameraPosition;
			b3Vector3 side = cameraUp.cross(fwd);
			side.normalize();
			cameraTargetPosition += side * deltax * m_wheelMultiplier;
		}
		else
		{
			cameraTargetPosition -= cameraUp * deltay * m_wheelMultiplier;
		}
	}

	setCameraTargetPosition(cameraTargetPosition[0], cameraTargetPosition[1], cameraTargetPosition[2]);
	return true;
}

void SimpleCamera::setVRCamera(const float viewMat[16], const float projectionMatrix[16])
{
	m_data->m_enableVR = true;

	b3Matrix3x3 vm;
	vm.setValue(viewMat[0], viewMat[4], viewMat[8],
				viewMat[1], viewMat[5], viewMat[9],
				viewMat[2], viewMat[6], viewMat[10]);

	b3Vector3 vp = b3MakeVector3(viewMat[12], viewMat[13], viewMat[14]);
	b3Transform tr;
	tr.setBasis(vm);
	tr.setOrigin(vp);
	b3Transform cp = tr.inverse();
	m_data->m_cameraPosition = cp.getOrigin();

	for (int i = 0; i < 16; i++)
	{
		m_data->m_viewMatrixVR[i] = viewMat[i];
		m_data->m_projectionMatrixVR[i] = projectionMatrix[i];
		m_data->m_frustumZNear = m_data->m_projectionMatrixVR[14] / (m_data->m_projectionMatrixVR[10] - 1);
		m_data->m_frustumZFar = m_data->m_projectionMatrixVR[14] / (m_data->m_projectionMatrixVR[10] + 1);
	}
}

bool SimpleCamera::getVRCamera(float viewMat[16], float projectionMatrix[16])
{
	if (m_data->m_enableVR)
	{
		for (int i = 0; i < 16; i++)
		{
			viewMat[i] = m_data->m_viewMatrixVR[i];
			projectionMatrix[i] = m_data->m_projectionMatrixVR[i];
		}
	}
	return false;
}

void SimpleCamera::disableVRCamera()
{
	m_data->m_enableVR = false;
}

bool SimpleCamera::isVRCamera() const
{
	return m_data->m_enableVR;
}

static void b3CreateFrustum(
	float left,
	float right,
	float bottom,
	float top,
	float nearVal,
	float farVal,
	float frustum[16])
{
	frustum[0 * 4 + 0] = (float(2) * nearVal) / (right - left);
	frustum[0 * 4 + 1] = float(0);
	frustum[0 * 4 + 2] = float(0);
	frustum[0 * 4 + 3] = float(0);

	frustum[1 * 4 + 0] = float(0);
	frustum[1 * 4 + 1] = (float(2) * nearVal) / (top - bottom);
	frustum[1 * 4 + 2] = float(0);
	frustum[1 * 4 + 3] = float(0);

	frustum[2 * 4 + 0] = (right + left) / (right - left);
	frustum[2 * 4 + 1] = (top + bottom) / (top - bottom);
	frustum[2 * 4 + 2] = -(farVal + nearVal) / (farVal - nearVal);
	frustum[2 * 4 + 3] = float(-1);

	frustum[3 * 4 + 0] = float(0);
	frustum[3 * 4 + 1] = float(0);
	frustum[3 * 4 + 2] = -(float(2) * farVal * nearVal) / (farVal - nearVal);
	frustum[3 * 4 + 3] = float(0);
}

#if 0
static void b3CreateDiagonalMatrix(float value, float result[4][4])
{
	for (int i=0;i<4;i++)
	{
		for (int j=0;j<4;j++)
		{
			if (i==j)
			{
				result[i][j] = value;
			} else
			{
				result[i][j] = 0.f;
			}
		}
	}
}
static void b3CreateOrtho(float left, float right, float bottom, float top, float zNear, float zFar, float result[4][4])
{
	b3CreateDiagonalMatrix(1.f,result);

	result[0][0] = 2.f / (right - left);
	result[1][1] = 2.f / (top - bottom);
	result[2][2] = - 2.f / (zFar - zNear);
	result[3][0] = - (right + left) / (right - left);
	result[3][1] = - (top + bottom) / (top - bottom);
	result[3][2] = - (zFar + zNear) / (zFar - zNear);
}
#endif
static void b3CreateLookAt(const b3Vector3& eye, const b3Vector3& center, const b3Vector3& up, float result[16])
{
	b3Vector3 f = (center - eye).normalized();
	b3Vector3 u = up.normalized();
	b3Vector3 s = (f.cross(u)).normalized();
	u = s.cross(f);

	result[0 * 4 + 0] = s.x;
	result[1 * 4 + 0] = s.y;
	result[2 * 4 + 0] = s.z;

	result[0 * 4 + 1] = u.x;
	result[1 * 4 + 1] = u.y;
	result[2 * 4 + 1] = u.z;

	result[0 * 4 + 2] = -f.x;
	result[1 * 4 + 2] = -f.y;
	result[2 * 4 + 2] = -f.z;

	result[0 * 4 + 3] = 0.f;
	result[1 * 4 + 3] = 0.f;
	result[2 * 4 + 3] = 0.f;

	result[3 * 4 + 0] = -s.dot(eye);
	result[3 * 4 + 1] = -u.dot(eye);
	result[3 * 4 + 2] = f.dot(eye);
	result[3 * 4 + 3] = 1.f;
}

void SimpleCamera::setCameraUpAxis(int upAxis)
{
	m_data->m_cameraUpAxis = upAxis;

	update();
}

int SimpleCamera::getCameraUpAxis() const
{
	return m_data->m_cameraUpAxis;
}

void SimpleCamera::update()
{
	b3Scalar yawRad = m_data->m_yaw * b3Scalar(0.01745329251994329547);      // rads per deg
	b3Scalar pitchRad = m_data->m_pitch * b3Scalar(0.01745329251994329547);  // rads per deg
	b3Scalar rollRad = 0.0;
	b3Quaternion eyeRot;

	int forwardAxis(-1);
	switch (m_data->m_cameraUpAxis)
	{
		case 1:
			forwardAxis = 2;
			m_data->m_cameraUp = b3MakeVector3(0, 1, 0);
			//gLightPos = b3MakeVector3(-50.f,100,30);
			eyeRot.setEulerZYX(rollRad, yawRad, -pitchRad);
			break;
		case 2:
			forwardAxis = 1;
			m_data->m_cameraUp = b3MakeVector3(0, 0, 1);
			//gLightPos = b3MakeVector3(-50.f,30,100);
			eyeRot.setEulerZYX(yawRad, rollRad, pitchRad);
			break;
		default:
		{
			//b3Assert(0);
			return;
		}
	};

	b3Vector3 eyePos = b3MakeVector3(0, 0, 0);
	eyePos[forwardAxis] = -m_data->m_cameraDistance;
	eyePos = b3Matrix3x3(eyeRot) * eyePos;

	m_data->m_cameraPosition = eyePos;

	m_data->m_cameraPosition += m_data->m_cameraTargetPosition;

	m_data->m_cameraForward = m_data->m_cameraTargetPosition - m_data->m_cameraPosition;
	if (m_data->m_cameraForward.length2() < B3_EPSILON)
	{
		m_data->m_cameraForward.setValue(1.f, 0.f, 0.f);
	}
	else
	{
		m_data->m_cameraForward.normalize();
	}
}

void SimpleCamera::getCameraProjectionMatrix(float projectionMatrix[16]) const
{
	if (m_data->m_enableVR)
	{
		for (int i = 0; i < 16; i++)
		{
			projectionMatrix[i] = m_data->m_projectionMatrixVR[i];
		}
	}
	else
	{
		b3CreateFrustum(-m_data->m_aspect * m_data->m_frustumZNear, m_data->m_aspect * m_data->m_frustumZNear, -m_data->m_frustumZNear, m_data->m_frustumZNear, m_data->m_frustumZNear, m_data->m_frustumZFar, projectionMatrix);
	}
}
void SimpleCamera::setVRCameraOffsetTransform(const float offset[16])
{
	for (int i = 0; i < 16; i++)
	{
		m_data->m_offsetTransformVR[i] = offset[i];
	}
}
void SimpleCamera::getCameraViewMatrix(float viewMatrix[16]) const
{
	if (m_data->m_enableVR)
	{
		for (int i = 0; i < 16; i++)
		{
			b3Transform tr;
			tr.setFromOpenGLMatrix(m_data->m_viewMatrixVR);
			b3Transform shift = b3Transform::getIdentity();
			shift.setFromOpenGLMatrix(m_data->m_offsetTransformVR);
			tr = tr * shift;
			tr.getOpenGLMatrix(viewMatrix);
			//viewMatrix[i] = m_data->m_viewMatrixVR[i];
		}
	}
	else
	{
		b3CreateLookAt(m_data->m_cameraPosition, m_data->m_cameraTargetPosition, m_data->m_cameraUp, viewMatrix);
	}
}

void SimpleCamera::getCameraTargetPosition(double pos[3]) const
{
	pos[0] = m_data->m_cameraTargetPosition[0];
	pos[1] = m_data->m_cameraTargetPosition[1];
	pos[2] = m_data->m_cameraTargetPosition[2];
}

void SimpleCamera::getCameraPosition(double pos[3]) const
{
	pos[0] = m_data->m_cameraPosition[0];
	pos[1] = m_data->m_cameraPosition[1];
	pos[2] = m_data->m_cameraPosition[2];
}

void SimpleCamera::getCameraTargetPosition(float pos[3]) const
{
	pos[0] = m_data->m_cameraTargetPosition[0];
	pos[1] = m_data->m_cameraTargetPosition[1];
	pos[2] = m_data->m_cameraTargetPosition[2];
}
void SimpleCamera::getCameraPosition(float pos[3]) const
{
	pos[0] = m_data->m_cameraPosition[0];
	pos[1] = m_data->m_cameraPosition[1];
	pos[2] = m_data->m_cameraPosition[2];
}

void SimpleCamera::setCameraTargetPosition(float x, float y, float z)
{
	m_data->m_cameraTargetPosition.setValue(x, y, z);
	update();
}
float SimpleCamera::getCameraDistance() const
{
	return m_data->m_cameraDistance;
}

void SimpleCamera::setCameraDistance(float dist)
{
	m_data->m_cameraDistance = dist;
	update();
}
void SimpleCamera::setCameraUpVector(float x, float y, float z)
{
	m_data->m_cameraUp.setValue(x, y, z);
	update();
}

void SimpleCamera::getCameraUpVector(float up[3]) const
{
	if (m_data->m_enableVR)
	{
		float viewMatTotal[16];
		getCameraViewMatrix(viewMatTotal);
		up[0] = viewMatTotal[0];
		up[1] = viewMatTotal[4];
		up[2] = viewMatTotal[8];
	}
	else
	{
		up[0] = float(m_data->m_cameraUp[0]);
		up[1] = float(m_data->m_cameraUp[1]);
		up[2] = float(m_data->m_cameraUp[2]);
	}
}

void SimpleCamera::getCameraForwardVector(float fwd[3]) const
{
	if (m_data->m_enableVR)
	{
		float viewMatTotal[16];
		getCameraViewMatrix(viewMatTotal);
		fwd[0] = viewMatTotal[2];
		fwd[1] = viewMatTotal[6];
		fwd[2] = viewMatTotal[10];
	}
	else
	{
		fwd[0] = float(m_data->m_cameraForward[0]);
		fwd[1] = float(m_data->m_cameraForward[1]);
		fwd[2] = float(m_data->m_cameraForward[2]);
	}
}

void SimpleCamera::setCameraYaw(float yaw)
{
	m_data->m_yaw = yaw;
	update();
}

float SimpleCamera::getCameraYaw() const
{
	return m_data->m_yaw;
}

void SimpleCamera::setCameraPitch(float pitch)
{
	m_data->m_pitch = pitch;
	update();
}

void SimpleCamera::setAspectRatio(float ratio)
{
	m_data->m_aspect = ratio;
	update();
}

float SimpleCamera::getCameraPitch() const
{
	return m_data->m_pitch;
}
float SimpleCamera::getAspectRatio() const
{
	return m_data->m_aspect;
}

float SimpleCamera::getCameraFrustumFar() const
{
	return m_data->m_frustumZFar;
}

float SimpleCamera::getCameraFrustumNear() const
{
	return m_data->m_frustumZNear;
}

void SimpleCamera::setCameraFrustumFar(float far)
{
	m_data->m_frustumZFar = far;
}

void SimpleCamera::setCameraFrustumNear(float near)
{
	m_data->m_frustumZNear = near;
}

int SimpleCamera::getScreenWidth() const
{
	return m_data->m_screenWidth;
}
int SimpleCamera::getScreenHeight() const
{
	return m_data->m_screenHeight;
}
void SimpleCamera::resize(int width, int height)
{
	m_data->m_screenWidth = width;
	m_data->m_screenHeight = height;
	setAspectRatio((float)m_data->m_screenWidth / (float)m_data->m_screenHeight);
}

btVector3 SimpleCamera::getRayTo(int x, int y) const
{
	float top = 1.f;
	float bottom = -1.f;
	float nearPlane = 1.f;
	float tanFov = (top - bottom) * 0.5f / nearPlane;
	float fov = btScalar(2.0) * btAtan(tanFov);

	btVector3 camPos, camTarget;

	getCameraPosition(camPos);
	getCameraTargetPosition(camTarget);

	btVector3 rayFrom = camPos;
	btVector3 rayForward = (camTarget - camPos);
	rayForward.normalize();
	float farPlane = 10000.f;
	rayForward *= farPlane;

	btVector3 rightOffset;
	btVector3 cameraUp = btVector3(0, 0, 0);
	cameraUp[getCameraUpAxis()] = 1;

	btVector3 vertical = cameraUp;

	btVector3 hor;
	hor = rayForward.cross(vertical);
	hor.safeNormalize();
	vertical = hor.cross(rayForward);
	vertical.safeNormalize();

	float tanfov = tanf(0.5f * fov);

	hor *= 2.f * farPlane * tanfov;
	vertical *= 2.f * farPlane * tanfov;

	btScalar aspect;
	float width = float(getScreenWidth());
	float height = float(getScreenHeight());

	aspect = width / height;

	hor *= aspect;

	btVector3 rayToCenter = rayFrom + rayForward;
	btVector3 dHor = hor * 1.f / width;
	btVector3 dVert = vertical * 1.f / height;

	btVector3 rayTo = rayToCenter - 0.5f * hor + 0.5f * vertical;
	rayTo += btScalar(x) * dHor;
	rayTo -= btScalar(y) * dVert;
	return rayTo;
}
// btVector3 getRayTo(int x, int y)
// {
// 	CommonRenderInterface* renderer = m_guiHelper->getRenderInterface();

// 	if (!renderer)
// 	{
// 		btAssert(0);
// 		return btVector3(0, 0, 0);
// 	}

// 	float top = 1.f;
// 	float bottom = -1.f;
// 	float nearPlane = 1.f;
// 	float tanFov = (top - bottom) * 0.5f / nearPlane;
// 	float fov = btScalar(2.0) * btAtan(tanFov);

// 	btVector3 camPos, camTarget;
// 	renderer->getActiveCamera()->getCameraPosition(camPos);
// 	renderer->getActiveCamera()->getCameraTargetPosition(camTarget);

// 	btVector3 rayFrom = camPos;
// 	btVector3 rayForward = (camTarget - camPos);
// 	rayForward.normalize();
// 	float farPlane = 10000.f;
// 	rayForward *= farPlane;

// 	btVector3 rightOffset;
// 	btVector3 cameraUp = btVector3(0, 0, 0);
// 	cameraUp[m_guiHelper->getAppInterface()->getUpAxis()] = 1;

// 	btVector3 vertical = cameraUp;

// 	btVector3 hor;
// 	hor = rayForward.cross(vertical);
// 	hor.normalize();
// 	vertical = hor.cross(rayForward);
// 	vertical.normalize();

// 	float tanfov = tanf(0.5f * fov);

// 	hor *= 2.f * farPlane * tanfov;
// 	vertical *= 2.f * farPlane * tanfov;

// 	btScalar aspect;
// 	float width = float(renderer->getScreenWidth());
// 	float height = float(renderer->getScreenHeight());

// 	aspect = width / height;

// 	hor *= aspect;

// 	btVector3 rayToCenter = rayFrom + rayForward;
// 	btVector3 dHor = hor * 1.f / width;
// 	btVector3 dVert = vertical * 1.f / height;

// 	btVector3 rayTo = rayToCenter - 0.5f * hor + 0.5f * vertical;
// 	rayTo += btScalar(x) * dHor;
// 	rayTo -= btScalar(y) * dVert;
// 	return rayTo;
// }

// btVector3 getRayTo(int x, int y)
// {
// 	CommonRenderInterface* renderer = m_guiHelper->getRenderInterface();

// 	if (!renderer)
// 	{
// 		btAssert(0);
// 		return btVector3(0, 0, 0);
// 	}

// 	float top = 1.f;
// 	float bottom = -1.f;
// 	float nearPlane = 1.f;
// 	float tanFov = (top - bottom) * 0.5f / nearPlane;
// 	float fov = btScalar(2.0) * btAtan(tanFov);

// 	btVector3 camPos, camTarget;

// 	renderer->getActiveCamera()->getCameraPosition(camPos);
// 	renderer->getActiveCamera()->getCameraTargetPosition(camTarget);

// 	btVector3 rayFrom = camPos;
// 	btVector3 rayForward = (camTarget - camPos);
// 	rayForward.normalize();
// 	float farPlane = 10000.f;
// 	rayForward *= farPlane;

// 	btVector3 rightOffset;
// 	btVector3 cameraUp = btVector3(0, 0, 0);
// 	cameraUp[m_guiHelper->getAppInterface()->getUpAxis()] = 1;

// 	btVector3 vertical = cameraUp;

// 	btVector3 hor;
// 	hor = rayForward.cross(vertical);
// 	hor.safeNormalize();
// 	vertical = hor.cross(rayForward);
// 	vertical.safeNormalize();

// 	float tanfov = tanf(0.5f * fov);

// 	hor *= 2.f * farPlane * tanfov;
// 	vertical *= 2.f * farPlane * tanfov;

// 	btScalar aspect;
// 	float width = float(renderer->getScreenWidth());
// 	float height = float(renderer->getScreenHeight());

// 	aspect = width / height;

// 	hor *= aspect;

// 	btVector3 rayToCenter = rayFrom + rayForward;
// 	btVector3 dHor = hor * 1.f / width;
// 	btVector3 dVert = vertical * 1.f / height;

// 	btVector3 rayTo = rayToCenter - 0.5f * hor + 0.5f * vertical;
// 	rayTo += btScalar(x) * dHor;
// 	rayTo -= btScalar(y) * dVert;
// 	return rayTo;
// }

// btVector3 PhysicsServerExample::getRayTo(int x, int y)
// {
// 	CommonRenderInterface* renderer = m_guiHelper->getRenderInterface();

// 	if (!renderer)
// 	{
// 		btAssert(0);
// 		return btVector3(0, 0, 0);
// 	}

// 	float top = 1.f;
// 	float bottom = -1.f;
// 	float nearPlane = 1.f;
// 	float tanFov = (top - bottom) * 0.5f / nearPlane;
// 	float fov = btScalar(2.0) * btAtan(tanFov);

// 	btVector3 camPos, camTarget;
// 	renderer->getActiveCamera()->getCameraPosition(camPos);
// 	renderer->getActiveCamera()->getCameraTargetPosition(camTarget);

// 	btVector3 rayFrom = camPos;
// 	btVector3 rayForward = (camTarget - camPos);
// 	rayForward.normalize();
// 	float farPlane = 10000.f;
// 	rayForward *= farPlane;

// 	btVector3 rightOffset;
// 	btVector3 cameraUp = btVector3(0, 0, 0);
// 	cameraUp[m_guiHelper->getAppInterface()->getUpAxis()] = 1;

// 	btVector3 vertical = cameraUp;

// 	btVector3 hor;
// 	hor = rayForward.cross(vertical);
// 	hor.safeNormalize();
// 	vertical = hor.cross(rayForward);
// 	vertical.safeNormalize();

// 	float tanfov = tanf(0.5f * fov);

// 	hor *= 2.f * farPlane * tanfov;
// 	vertical *= 2.f * farPlane * tanfov;

// 	btScalar aspect;
// 	float width = float(renderer->getScreenWidth());
// 	float height = float(renderer->getScreenHeight());

// 	aspect = width / height;

// 	hor *= aspect;

// 	btVector3 rayToCenter = rayFrom + rayForward;
// 	btVector3 dHor = hor * 1.f / width;
// 	btVector3 dVert = vertical * 1.f / height;

// 	btVector3 rayTo = rayToCenter - 0.5f * hor + 0.5f * vertical;
// 	rayTo += btScalar(x) * dHor;
// 	rayTo -= btScalar(y) * dVert;
// 	return rayTo;
// }

void SimpleCamera::resetCamera(const CameraResetInfo& info)
{
	setCameraDistance(info.camDist);
	setCameraPitch(info.pitch);
	setCameraYaw(info.yaw);
	setCameraTargetPosition(info.camPosX, info.camPosY, info.camPosZ);
	switch (info.upAxis)
	{
		case 0:
			setCameraUpAxis(0);
			break;
		case 1:
			setCameraUpAxis(1);
			break;
		case 2:
			setCameraUpAxis(2);
			break;
	}
}

// bool OpenGLGuiHelper::getCameraInfo(int* width, int* height, float viewMatrix[16], float projectionMatrix[16], float camUp[3], float camForward[3], float hor[3], float vert[3], float* yaw, float* pitch, float* camDist, float cameraTarget[3]) const
// {
// 	if (getRenderInterface() && getRenderInterface()->getActiveCamera())
// 	{
// 		*width = m_data->m_glApp->m_window->getWidth();
// 		*height = m_data->m_glApp->m_window->getHeight();
// 		getRenderInterface()->getActiveCamera()->getCameraViewMatrix(viewMatrix);
// 		getRenderInterface()->getActiveCamera()->getCameraProjectionMatrix(projectionMatrix);
// 		getRenderInterface()->getActiveCamera()->getCameraUpVector(camUp);
// 		getRenderInterface()->getActiveCamera()->getCameraForwardVector(camForward);

// 		float top = 1.f;
// 		float bottom = -1.f;
// 		float tanFov = (top - bottom) * 0.5f / 1;
// 		float fov = btScalar(2.0) * btAtan(tanFov);
// 		btVector3 camPos, camTarget;
// 		getRenderInterface()->getActiveCamera()->getCameraPosition(camPos);
// 		getRenderInterface()->getActiveCamera()->getCameraTargetPosition(camTarget);
// 		btVector3 rayFrom = camPos;
// 		btVector3 rayForward = (camTarget - camPos);
// 		rayForward.normalize();
// 		float farPlane = 10000.f;
// 		rayForward *= farPlane;

// 		btVector3 rightOffset;
// 		btVector3 cameraUp = btVector3(camUp[0], camUp[1], camUp[2]);
// 		btVector3 vertical = cameraUp;
// 		btVector3 hori;
// 		hori = rayForward.cross(vertical);
// 		hori.normalize();
// 		vertical = hori.cross(rayForward);
// 		vertical.normalize();
// 		float tanfov = tanf(0.5f * fov);
// 		hori *= 2.f * farPlane * tanfov;
// 		vertical *= 2.f * farPlane * tanfov;
// 		btScalar aspect = float(*width) / float(*height);
// 		hori *= aspect;
// 		//compute 'hor' and 'vert' vectors, useful to generate raytracer rays
// 		hor[0] = hori[0];
// 		hor[1] = hori[1];
// 		hor[2] = hori[2];
// 		vert[0] = vertical[0];
// 		vert[1] = vertical[1];
// 		vert[2] = vertical[2];

// 		*yaw = getRenderInterface()->getActiveCamera()->getCameraYaw();
// 		*pitch = getRenderInterface()->getActiveCamera()->getCameraPitch();
// 		*camDist = getRenderInterface()->getActiveCamera()->getCameraDistance();
// 		cameraTarget[0] = camTarget[0];
// 		cameraTarget[1] = camTarget[1];
// 		cameraTarget[2] = camTarget[2];
// 		return true;
// 	}
// 	return false;
// }
