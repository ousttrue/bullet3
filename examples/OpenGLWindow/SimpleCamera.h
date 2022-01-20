#ifndef SIMPLE_CAMERA_H
#define SIMPLE_CAMERA_H

#include <CommonCameraInterface.h>
#include "CommonCallbacks.h"

struct SimpleCamera : public CommonCameraInterface
{
	struct SimpleCameraInternalData* m_data;
	bool m_leftMouseButton = false;
	bool m_middleMouseButton = false;
	bool m_rightMouseButton = false;
	float m_wheelMultiplier = 0.01f;
	float m_mouseMoveMultiplier = 0.4f;
	float m_mouseXpos = 0;
	float m_mouseYpos = 0;
	bool m_mouseInitialized = false;
	bool m_isAltPressed = false;
	bool m_isControlPressed = false;

	SimpleCamera();
	~SimpleCamera() override;
	bool mouseButtonCallback(int button, int state, float x, float y, ButtonFlags flags) override;
	bool mouseMoveCallback(float x, float y) override;
	bool wheelCallback(float deltax, float deltay) override;

	void update();

	void getCameraProjectionMatrix(float m[16]) const override;
	void getCameraViewMatrix(float m[16]) const override;

	// VRCamera
	void setVRCamera(const float viewMat[16], const float projectionMatrix[16]) override;
	bool getVRCamera(float viewMat[16], float projectionMatrix[16]);
	void setVRCameraOffsetTransform(const float offset[16]) override;
	void disableVRCamera() override;
	bool isVRCamera() const override;

	virtual void getCameraTargetPosition(float pos[3]) const;
	virtual void getCameraPosition(float pos[3]) const;

	virtual void getCameraTargetPosition(double pos[3]) const;
	virtual void getCameraPosition(double pos[3]) const;

	virtual void setCameraTargetPosition(float x, float y, float z);
	virtual void setCameraDistance(float dist);
	virtual float getCameraDistance() const;

	virtual void setCameraUpVector(float x, float y, float z);
	void getCameraUpVector(float up[3]) const;

	void getCameraForwardVector(float fwd[3]) const;

	///the setCameraUpAxis will call the 'setCameraUpVector' and 'setCameraForwardVector'
	virtual void setCameraUpAxis(int axis);
	virtual int getCameraUpAxis() const;

	virtual void setCameraYaw(float yaw);
	virtual float getCameraYaw() const;

	virtual void setCameraPitch(float pitch);
	virtual float getCameraPitch() const;

	virtual void setAspectRatio(float ratio);
	virtual float getAspectRatio() const;

	virtual float getCameraFrustumFar() const;
	virtual float getCameraFrustumNear() const;

	virtual void setCameraFrustumFar(float far);
	virtual void setCameraFrustumNear(float near);

	int getScreenWidth() const override;
	int getScreenHeight() const override;
	void resize(int width, int height) override;

	btVector3 getRayTo(int x, int y) const override;
	void resetCamera(const CameraResetInfo& info) override;
};

#endif  //SIMPLE_CAMERA_H
