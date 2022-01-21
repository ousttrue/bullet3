#include <CommonGraphicsAppInterface.h>
#include "GLInstancingRenderer.h"
#include <memory>

struct GlfwApp : public CommonGraphicsApp
{
private:
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

	class GLInstancingRenderer* m_instancingRenderer;
	float m_retinaScale = 1.0f;
	struct GLFWwindow* m_window = nullptr;

public:
	GlfwApp();
	GlfwApp(const GlfwApp&) = delete;
	GlfwApp& operator=(const GlfwApp&) = delete;

	~GlfwApp() override;
	class GLInstancingRenderer* getRenderer() { return m_instancingRenderer; }
	bool createWindow(const b3gWindowConstructionInfo& ci) override;
	void dumpNextFrameToPng(const char* filename) override;
	void dumpFramesToVideo(const char* mp4Filename) override;
	void getScreenPixels(unsigned char* rgbaBuffer, int bufferSizeInBytes, float* depthBuffer, int depthBufferSizeInBytes) override;
	void setMp4Fps(int fps) override;
	void setViewport(int width, int height) override;
	void setBackgroundColor(float red, float green, float blue) override;
	void drawGrid(DrawGridData data = DrawGridData()) override;

	int registerCubeShape(float halfExtentsX, float halfExtentsY, float halfExtentsZ, int textureIndex = -1, float textureScaling = 1) override;
	int registerGraphicsUnitSphereShape(EnumSphereLevelOfDetail lod, int textureId = -1) override;
	void registerGrid(int xres, int yres, float color0[4], float color1[4]) override;

public:
	float getTimeInSeconds() override;
	bool requestedExit() const override;
	void setRequestExit() override;
	void startRendering() override;
	void endRendering() override;
	void setWindowTitle(const char* title) override {}
	float getRetinaScale() const override { return m_retinaScale; }
	void setAllowRetina(bool /*allowRetina*/) override{};
	int getWidth() const override;
	int getHeight() const override;
	int fileOpenDialog(char* fileName, int maxFileNameLength) override;
};
