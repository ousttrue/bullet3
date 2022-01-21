#pragma once
#include <Bullet3Common/b3Vector3.h>
#include "CommonRenderInterface.h"
#include "CommonCameraInterface.h"
#include "CommonCallbacks.h"
#include <memory>
#include <list>

struct b3gWindowConstructionInfo
{
	int m_width = 1024;
	int m_height = 768;
	const char* m_title;
	bool m_fullscreen = false;
	int m_colorBitsPerPixel = 32;
	void* m_windowHandle = nullptr;
	int m_openglVersion = 3;
	int m_renderDevice = -1;
};

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
	struct CommonRenderInterface* m_renderer = nullptr;
	struct CommonParameterInterface* m_parameterInterface = nullptr;
	struct Common2dCanvasInterface* m_2dCanvasInterface = nullptr;

	float m_backgroundColorRGB[3] = {0.7f, 0.7f, 0.8f};

	virtual ~CommonGraphicsApp()
	{
	}

	virtual bool createWindow(const b3gWindowConstructionInfo& ci) = 0;

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
	virtual void drawGrid(DrawGridData data = DrawGridData()) = 0;
	
	virtual void drawTexturedRect(float x0, float y0, float x1, float y1, float color[4], float u0, float v0, float u1, float v1, int useRGBA) = 0;
	virtual int registerCubeShape(float halfExtentsX, float halfExtentsY, float halfExtentsZ, int textureIndex = -1, float textureScaling = 1) = 0;
	virtual int registerGraphicsUnitSphereShape(EnumSphereLevelOfDetail lod, int textureId = -1) = 0;
	virtual void registerGrid(int xres, int yres, float color0[4], float color1[4]) = 0;

	std::list<b3ResizeCallback> resizeCallback;
	std::list<b3MouseMoveCallback> mouseMoveCallback;
	std::list<b3MouseButtonCallback> mouseButtonCallback;
	std::list<b3WheelCallback> wheelCallback;
	std::list<b3KeyboardCallback> keyboardCallback;

	virtual float getTimeInSeconds() = 0;
	virtual bool requestedExit() const = 0;
	virtual void setRequestExit() = 0;
	virtual void startRendering() = 0;
	virtual void endRendering() = 0;
	virtual bool isModifierKeyPressed(int key) = 0;
	virtual void setRenderCallback(b3RenderCallback renderCallback) = 0;
	virtual void setWindowTitle(const char* title) = 0;
	virtual float getRetinaScale() const = 0;
	virtual void setAllowRetina(bool allow) = 0;
	virtual int getWidth() const = 0;
	virtual int getHeight() const = 0;
	virtual int fileOpenDialog(char* fileName, int maxFileNameLength) = 0;
};
