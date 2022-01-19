#pragma once
#include <LinearMath/btVector3.h>

struct CameraResetInfo
{
	float camDist;
	float yaw;
	float pitch;
	float camPosX;
	float camPosY;
	float camPosZ;
};

struct CommonCameraInterface
{
	virtual ~CommonCameraInterface() {}
	virtual void getCameraProjectionMatrix(float m[16]) const = 0;
	virtual void getCameraViewMatrix(float m[16]) const = 0;

	virtual void setVRCamera(const float viewMat[16], const float projectionMatrix[16]) = 0;
	virtual void disableVRCamera() = 0;
	virtual bool isVRCamera() const = 0;
	virtual void setVRCameraOffsetTransform(const float offset[16]) = 0;

	virtual void getCameraTargetPosition(float pos[3]) const = 0;
	virtual void getCameraPosition(float pos[3]) const = 0;

	virtual void getCameraTargetPosition(double pos[3]) const = 0;
	virtual void getCameraPosition(double pos[3]) const = 0;

	virtual void setCameraTargetPosition(float x, float y, float z) = 0;
	virtual void setCameraDistance(float dist) = 0;
	virtual float getCameraDistance() const = 0;

	virtual void setCameraUpVector(float x, float y, float z) = 0;
	virtual void getCameraUpVector(float up[3]) const = 0;
	virtual void getCameraForwardVector(float fwd[3]) const = 0;

	///the setCameraUpAxis will call the 'setCameraUpVector' and 'setCameraForwardVector'
	virtual void setCameraUpAxis(int axis) = 0;
	virtual int getCameraUpAxis() const = 0;

	virtual void setCameraYaw(float yaw) = 0;
	virtual float getCameraYaw() const = 0;

	virtual void setCameraPitch(float pitch) = 0;
	virtual float getCameraPitch() const = 0;

	virtual void setAspectRatio(float ratio) = 0;
	virtual float getAspectRatio() const = 0;

	virtual float getCameraFrustumFar() const = 0;
	virtual float getCameraFrustumNear() const = 0;

	virtual int getScreenWidth() const = 0;
	virtual int getScreenHeight() const = 0;
	virtual void resize(int width, int height) = 0;

	virtual btVector3 getRayTo(int x, int y) const = 0;
};
