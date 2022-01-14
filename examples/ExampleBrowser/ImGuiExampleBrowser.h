#pragma once
#include "EmptyBrowser.h"
#include <CommonExampleInterface.h>

class ImGuiExampleBrowser : public EmptyBrowser
{
public:
	ImGuiExampleBrowser(class ExampleEntries* examples);
	~ImGuiExampleBrowser();

	CommonExampleInterface* getCurrentExample();

	bool init(int argc, char* argv[]);

	void update(float deltaTime);

	void updateGraphics();

	bool requestedExit();

	void setSharedMemoryInterface(class SharedMemoryInterface* sharedMem);

	static void registerFileImporter(const char* extension, CommonExampleInterface::CreateFunc* createFunc);
};
