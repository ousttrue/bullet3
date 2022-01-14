#pragma once
#include "EmptyBrowser.h"
#include <CommonExampleInterface.h>

class OpenGLExampleBrowser : public EmptyBrowser
{
	class OpenGLExampleBrowserInternalData* m_internalData;

public:
	OpenGLExampleBrowser(class ExampleEntries* examples);
	~OpenGLExampleBrowser();

	CommonExampleInterface* getCurrentExample();

	bool init(int argc, char* argv[]);

	void update(float deltaTime);

	void updateGraphics();

	bool requestedExit();

	void setSharedMemoryInterface(class SharedMemoryInterface* sharedMem);

	void registerFileImporter(const char* extension, CommonExampleInterface::CreateFunc* createFunc);
};
