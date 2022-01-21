#include <CommonGraphicsAppInterface.h>
#include "GLInstancingRenderer.h"
#include "GLPrimitiveRenderer.h"
#include <memory>

struct GlfwApp : public CommonGraphicsApp
{
private:
	unsigned int m_fontTextureId = 0;
	unsigned int m_largeFontTextureId = 0;
	std::shared_ptr<class FontStash> m_fontStash;
	std::shared_ptr<class FontStash> m_fontStash2;
	struct RenderCallbacks* m_renderCallbacks = nullptr;
	struct RenderCallbacks* m_renderCallbacks2 = nullptr;
	int m_droidRegular = 0;
	int m_droidRegular2 = 0;
	int m_textureId = -1;

	const char* m_frameDumpPngFileName = nullptr;
	FILE* m_ffmpegFile = nullptr;
	class GLRenderToTexture* m_renderTexture = nullptr;
	void* m_userPointer = nullptr;
	int m_customViewPortWidth = -1;
	int m_customViewPortHeight = -1;
	int m_mp4Fps = 60;

	class GLPrimitiveRenderer* m_primRenderer;
	class GLInstancingRenderer* m_instancingRenderer;
	float m_retinaScale = 1.0f;

public:
	GlfwApp();
	GlfwApp(const GlfwApp&) = delete;
	GlfwApp& operator=(const GlfwApp&) = delete;

	~GlfwApp() override;
	class GLPrimitiveRenderer* getPrimRenderer() { return m_primRenderer; }
	class GLInstancingRenderer* getRenderer() { return m_instancingRenderer; }
	std::shared_ptr<CommonWindowInterface> createWindow(const b3gWindowConstructionInfo& ci) override;
	void dumpNextFrameToPng(const char* filename) override;
	void dumpFramesToVideo(const char* mp4Filename) override;
	void getScreenPixels(unsigned char* rgbaBuffer, int bufferSizeInBytes, float* depthBuffer, int depthBufferSizeInBytes) override;
	void setMp4Fps(int fps) override;
	void setViewport(int width, int height) override;
	void setBackgroundColor(float red, float green, float blue) override;
	void drawGrid(DrawGridData data = DrawGridData()) override;
	void drawText(const char* txt, int posX, int posY, float size, float colorRGBA[4]) override;
	void drawText3D(const char* txt, float posX, float posY, float posZ, float size) override;
	void drawText3D(const char* txt, float position[3], float orientation[4], float color[4], float size, int optionFlag) override;
	void drawTexturedRect(float x0, float y0, float x1, float y1, float color[4], float u0, float v0, float u1, float v1, int useRGBA) override;
	int registerCubeShape(float halfExtentsX, float halfExtentsY, float halfExtentsZ, int textureIndex = -1, float textureScaling = 1) override;
	int registerGraphicsUnitSphereShape(EnumSphereLevelOfDetail lod, int textureId = -1) override;
	void registerGrid(int xres, int yres, float color0[4], float color1[4]) override;
	std::shared_ptr<class FontStash> getFontStash();
};
