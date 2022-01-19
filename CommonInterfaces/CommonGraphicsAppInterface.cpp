#include "CommonGraphicsAppInterface.h"

void CommonGraphicsApp::defaultMouseButtonCallback(int button, int state, float x, float y)
{
	if (button == 0)
		m_leftMouseButton = (state == 1);
	if (button == 1)
		m_middleMouseButton = (state == 1);

	if (button == 2)
		m_rightMouseButton = (state == 1);

	m_mouseXpos = x;
	m_mouseYpos = y;
	m_mouseInitialized = true;
}

void CommonGraphicsApp::defaultMouseMoveCallback(float x, float y)
{
	if (m_window && m_renderer)
	{
		CommonCameraInterface* camera = m_renderer->getActiveCamera();

		bool isAltPressed = m_window->isModifierKeyPressed(B3G_ALT);
		bool isControlPressed = m_window->isModifierKeyPressed(B3G_CONTROL);

		if (isAltPressed || isControlPressed)
		{
			float xDelta = x - m_mouseXpos;
			float yDelta = y - m_mouseYpos;
			float cameraDistance = camera->getCameraDistance();
			float pitch = camera->getCameraPitch();
			float yaw = camera->getCameraYaw();

			float targPos[3];
			float camPos[3];

			camera->getCameraTargetPosition(targPos);
			camera->getCameraPosition(camPos);

			b3Vector3 cameraPosition = b3MakeVector3(b3Scalar(camPos[0]),
													 b3Scalar(camPos[1]),
													 b3Scalar(camPos[2]));

			b3Vector3 cameraTargetPosition = b3MakeVector3(b3Scalar(targPos[0]),
														   b3Scalar(targPos[1]),
														   b3Scalar(targPos[2]));
			b3Vector3 cameraUp = b3MakeVector3(0, 0, 0);
			cameraUp[camera->getCameraUpAxis()] = 1.f;

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
			camera->setCameraDistance(cameraDistance);
			camera->setCameraPitch(pitch);
			camera->setCameraYaw(yaw);
			camera->setCameraTargetPosition(cameraTargetPosition[0], cameraTargetPosition[1], cameraTargetPosition[2]);
		}

	}  //m_window &&  m_renderer

	m_mouseXpos = x;
	m_mouseYpos = y;
	m_mouseInitialized = true;
}

void CommonGraphicsApp::defaultWheelCallback(float deltax, float deltay)
{
	if (m_renderer)
	{
		b3Vector3 cameraTargetPosition, cameraPosition, cameraUp = b3MakeVector3(0, 0, 0);
		CommonCameraInterface* camera = m_renderer->getActiveCamera();
		cameraUp[camera->getCameraUpAxis()] = 1;
		camera->getCameraPosition(cameraPosition);
		camera->getCameraTargetPosition(cameraTargetPosition);

		if (!m_leftMouseButton)
		{
			float cameraDistance = camera->getCameraDistance();
			if (deltay < 0 || cameraDistance > 1)
			{
				cameraDistance -= deltay * m_wheelMultiplier;
				if (cameraDistance < 1)
					cameraDistance = 1;
				camera->setCameraDistance(cameraDistance);
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

		camera->setCameraTargetPosition(cameraTargetPosition[0], cameraTargetPosition[1], cameraTargetPosition[2]);
	}
}
