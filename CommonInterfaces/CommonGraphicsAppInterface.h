#pragma once
#include <Bullet3Common/b3Vector3.h>
#include "CommonRenderInterface.h"
#include "CommonWindowInterface.h"
#include "CommonCameraInterface.h"

struct DrawGridData
{
	int gridSize;
	float upOffset;
	int upAxis;
	float gridColor[4];

	DrawGridData(int upAx = 1)
		: gridSize(10),
		  upOffset(0.001f),
		  upAxis(upAx)
	{
		gridColor[0] = 0.6f;
		gridColor[1] = 0.6f;
		gridColor[2] = 0.6f;
		gridColor[3] = 1.f;
	}
};

enum EnumSphereLevelOfDetail
{
	SPHERE_LOD_POINT_SPRITE = 0,
	SPHERE_LOD_LOW,
	SPHERE_LOD_MEDIUM,
	SPHERE_LOD_HIGH,

};

struct CommonGraphicsApp
{
	enum drawText3DOption
	{
		eDrawText3D_OrtogonalFaceCamera = 1,
		eDrawText3D_TrueType = 2,
		eDrawText3D_TrackObject = 4,
	};
	class CommonWindowInterface* m_window = nullptr;
	struct CommonRenderInterface* m_renderer = nullptr;
	struct CommonParameterInterface* m_parameterInterface = nullptr;
	struct Common2dCanvasInterface* m_2dCanvasInterface = nullptr;
	bool m_leftMouseButton = false;
	bool m_middleMouseButton = false;
	bool m_rightMouseButton = false;
	float m_wheelMultiplier = 0.01f;
	float m_mouseMoveMultiplier = 0.4f;
	float m_mouseXpos = 0;
	float m_mouseYpos = 0;
	bool m_mouseInitialized = false;
	float m_backgroundColorRGB[3] = {0.7f, 0.7f, 0.8f};

	virtual ~CommonGraphicsApp()
	{
	}

	virtual CommonWindowInterface* createWindow(const b3gWindowConstructionInfo& ci) = 0;

	virtual void dumpNextFrameToPng(const char* pngFilename) {}
	virtual void dumpFramesToVideo(const char* mp4Filename) {}
	virtual void getScreenPixels(unsigned char* rgbaBuffer, int bufferSizeInBytes, float* depthBuffer, int depthBufferSizeInBytes) {}
	virtual void setViewport(int width, int height) {}
	virtual void getBackgroundColor(float* red, float* green, float* blue) const
	{
		if (red)
			*red = m_backgroundColorRGB[0];
		if (green)
			*green = m_backgroundColorRGB[1];
		if (blue)
			*blue = m_backgroundColorRGB[2];
	}
	virtual void setMp4Fps(int fps) {}
	virtual void setBackgroundColor(float red, float green, float blue)
	{
		m_backgroundColorRGB[0] = red;
		m_backgroundColorRGB[1] = green;
		m_backgroundColorRGB[2] = blue;
	}
	virtual void setMouseWheelMultiplier(float mult)
	{
		m_wheelMultiplier = mult;
	}
	virtual float getMouseWheelMultiplier() const
	{
		return m_wheelMultiplier;
	}
	virtual void setMouseMoveMultiplier(float mult)
	{
		m_mouseMoveMultiplier = mult;
	}
	virtual float getMouseMoveMultiplier() const
	{
		return m_mouseMoveMultiplier;
	}
	virtual void drawGrid(DrawGridData data = DrawGridData()) = 0;
	virtual void setUpAxis(int axis) = 0;
	virtual int getUpAxis() const = 0;
	virtual void swapBuffer() = 0;
	virtual void drawText(const char* txt, int posX, int posY)
	{
		float size = 1;
		float colorRGBA[4] = {0, 0, 0, 1};
		drawText(txt, posX, posY, size, colorRGBA);
	}
	virtual void drawText(const char* txt, int posX, int posY, float size)
	{
		float colorRGBA[4] = {0, 0, 0, 1};
		drawText(txt, posX, posY, size, colorRGBA);
	}
	virtual void drawText(const char* txt, int posX, int posY, float size, float colorRGBA[4]) = 0;
	virtual void drawText3D(const char* txt, float posX, float posY, float posZ, float size) = 0;
	virtual void drawText3D(const char* txt, float position[3], float orientation[4], float color[4], float size, int optionFlag) = 0;
	virtual void drawTexturedRect(float x0, float y0, float x1, float y1, float color[4], float u0, float v0, float u1, float v1, int useRGBA) = 0;
	virtual int registerCubeShape(float halfExtentsX, float halfExtentsY, float halfExtentsZ, int textureIndex = -1, float textureScaling = 1) = 0;
	virtual int registerGraphicsUnitSphereShape(EnumSphereLevelOfDetail lod, int textureId = -1) = 0;
	virtual void registerGrid(int xres, int yres, float color0[4], float color1[4]) = 0;
	void defaultMouseButtonCallback(int button, int state, float x, float y);
	void defaultMouseMoveCallback(float x, float y);
	void defaultWheelCallback(float deltax, float deltay);
};
