#pragma once
#include "ImGuiExampleBrowser.h"

ImGuiExampleBrowser::ImGuiExampleBrowser(ExampleEntries* examples)
{
}

ImGuiExampleBrowser::~ImGuiExampleBrowser()
{
}

CommonExampleInterface* ImGuiExampleBrowser::getCurrentExample()
{
	return nullptr;
}

bool ImGuiExampleBrowser::init(int argc, char* argv[])
{
	return true;
}

void ImGuiExampleBrowser::update(float deltaTime)
{
}

void ImGuiExampleBrowser::updateGraphics()
{
}

bool ImGuiExampleBrowser::requestedExit()
{
	return false;
}

void ImGuiExampleBrowser::setSharedMemoryInterface(class SharedMemoryInterface* sharedMem)
{
}

void ImGuiExampleBrowser::registerFileImporter(const char* extension, CommonExampleInterface::CreateFunc* createFunc)
{
}
