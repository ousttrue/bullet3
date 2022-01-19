#pragma once
#include "EmptyBrowser.h"
#include <CommonExampleInterface.h>

class OpenGLExampleBrowser : public EmptyBrowser
{
	class OpenGLExampleBrowserInternalData* m_internalData;

public:
	OpenGLExampleBrowser(class ExampleEntries* examples);
	~OpenGLExampleBrowser() override;
	CommonExampleInterface* getCurrentExample() override;
	bool init(int argc, char* argv[], const CommonGUIInterface::Factory& factory) override;
	void update(float deltaTime) override;
	void updateGraphics() override;
	bool requestedExit() override;
	void setSharedMemoryInterface(class SharedMemoryInterface* sharedMem) override;
	void registerFileImporter(const char* extension, const CommonExampleInterface::CreateFunc& createFunc) override;
};
