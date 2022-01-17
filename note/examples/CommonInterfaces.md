# CommonInterfaces

たくさんある `Demo` の選択器。
Gwen で実装されている。


`ExampleBrowser` に関連するインタフェース。

## ExampleBrowser

`CommonGraphicsAppInterface.h`

Application と Window を抽象化する。

```c++
struct CommonGraphicsApp
{
	virtual void dumpNextFrameToPng(const char* pngFilename) {}
	virtual void dumpFramesToVideo(const char* mp4Filename) {}
	virtual void getScreenPixels(unsigned char* rgbaBuffer, int bufferSizeInBytes, float* depthBuffer, int depthBufferSizeInBytes) {}
	virtual void setViewport(int width, int height) {}
	virtual void getBackgroundColor(float* red, float* green, float* blue) const
	virtual void setMp4Fps(int fps) {}
	virtual void setBackgroundColor(float red, float green, float blue)
	virtual void setMouseWheelMultiplier(float mult)
	virtual float getMouseWheelMultiplier() const
	virtual void setMouseMoveMultiplier(float mult)
	virtual float getMouseMoveMultiplier() const
	virtual void drawGrid(DrawGridData data = DrawGridData()) = 0;
	virtual void setUpAxis(int axis) = 0;
	virtual int getUpAxis() const = 0;
	virtual void swapBuffer() = 0;
	virtual void drawText(const char* txt, int posX, int posY)
	virtual void drawText(const char* txt, int posX, int posY, float size)
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
```

`CommonWindowInterface.h`

```c++
class CommonWindowInterface
{
public:
	virtual void createDefaultWindow(int width, int height, const char* title)
	virtual void createWindow(const b3gWindowConstructionInfo& ci) = 0;
	virtual void closeWindow() = 0;
	virtual void runMainLoop() = 0;
	virtual float getTimeInSeconds() = 0;
	virtual bool requestedExit() const = 0;
	virtual void setRequestExit() = 0;
	virtual void startRendering() = 0;
	virtual void endRendering() = 0;
	virtual bool isModifierKeyPressed(int key) = 0;
	virtual void setMouseMoveCallback(b3MouseMoveCallback mouseCallback) = 0;
	virtual b3MouseMoveCallback getMouseMoveCallback() = 0;
	virtual void setMouseButtonCallback(b3MouseButtonCallback mouseCallback) = 0;
	virtual b3MouseButtonCallback getMouseButtonCallback() = 0;
	virtual void setResizeCallback(b3ResizeCallback resizeCallback) = 0;
	virtual b3ResizeCallback getResizeCallback() = 0;
	virtual void setWheelCallback(b3WheelCallback wheelCallback) = 0;
	virtual b3WheelCallback getWheelCallback() = 0;
	virtual void setKeyboardCallback(b3KeyboardCallback keyboardCallback) = 0;
	virtual b3KeyboardCallback getKeyboardCallback() = 0;
	virtual void setRenderCallback(b3RenderCallback renderCallback) = 0;
	virtual void setWindowTitle(const char* title) = 0;
	virtual float getRetinaScale() const = 0;
	virtual void setAllowRetina(bool allow) = 0;
	virtual int getWidth() const = 0;
	virtual int getHeight() const = 0;
	virtual int fileOpenDialog(char* fileName, int maxFileNameLength) = 0;
};
```

`CommonGUIHelperInterface.h`

## CommonParameterInterface

Demo のパラメーター変更 GUI ？

`CommonParameterInterface.h`

```c++
struct CommonParameterInterface
{
	virtual ~CommonParameterInterface() {}
	virtual void registerSliderFloatParameter(SliderParams& params) = 0;
	virtual void registerButtonParameter(ButtonParams& params) = 0;
	virtual void registerComboBox(ComboBoxParams& params) = 0;

	virtual void syncParameters() = 0;
	virtual void removeAllParameters() = 0;
	virtual void setSliderValue(int sliderIndex, double sliderValue) = 0;
};
```

Demo からグラフをプロットする？

`Common2dCanvasInterface.h`

## CommonCallbacks.h
## CommonCameraInterface.h
## CommonDeformableBodyBase.h
## CommonExampleInterface.h
## CommonFileIOInterface.h
## CommonMultiBodyBase.h
## CommonRenderInterface.h
