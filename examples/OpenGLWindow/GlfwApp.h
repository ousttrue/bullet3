#include <CommonGraphicsAppInterface.h>
#include "GLInstancingRenderer.h"
#include "GLPrimitiveRenderer.h"

class GlfwApp : public CommonGraphicsApp
{
public:
	struct SimpleInternalData* m_data;
	class GLPrimitiveRenderer* m_primRenderer;
	class GLInstancingRenderer* m_instancingRenderer;
	GlfwApp(const char* title, int width, int height, float retinaScale);
	virtual ~GlfwApp();
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
	virtual void drawGrid(DrawGridData data = DrawGridData());
	virtual void setUpAxis(int axis);
	virtual int getUpAxis() const;
	virtual void swapBuffer();
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
	virtual void drawText(const char* txt, int posX, int posY, float size, float colorRGBA[4]);
	virtual void drawText3D(const char* txt, float posX, float posY, float posZ, float size);
	virtual void drawText3D(const char* txt, float position[3], float orientation[4], float color[4], float size, int optionFlag);
	virtual void drawTexturedRect(float x0, float y0, float x1, float y1, float color[4], float u0, float v0, float u1, float v1, int useRGBA);
	virtual int registerCubeShape(float halfExtentsX, float halfExtentsY, float halfExtentsZ, int textureIndex = -1, float textureScaling = 1);
	virtual int registerGraphicsUnitSphereShape(EnumSphereLevelOfDetail lod, int textureId = -1);
	virtual void registerGrid(int xres, int yres, float color0[4], float color1[4]);
	struct sth_stash* getFontStash();
};
