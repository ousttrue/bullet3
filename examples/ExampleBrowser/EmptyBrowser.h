#ifndef EMPTY_BROWSER
#define EMPTY_BROWSER

#include <CommonExampleInterface.h>
#include <CommonGUIInterface.h>

extern int gSharedMemoryKey;

class EmptyBrowser
{
public:
	EmptyExample m_emptyExample;

	virtual CommonExampleInterface* getCurrentExample()
	{
		return &m_emptyExample;
	}

	EmptyBrowser(class ExampleEntries* = nullptr)
	{
	}

	virtual ~EmptyBrowser()
	{
	}

	virtual bool init(int /*argc*/, char* argv[], const CommonGUIInterface::Factory& factory = {})
	{
		return true;
	}

	virtual void update(float deltaTime)
	{
		m_emptyExample.stepSimulation(deltaTime);
	}

	virtual void updateGraphics()
	{
	}

	virtual bool requestedExit()
	{
		return false;
	}

	virtual void setSharedMemoryInterface(class SharedMemoryInterface* sharedMem)
	{
	}

	virtual void registerFileImporter(const char* extension, const CommonExampleInterface::CreateFunc& createFunc)
	{
	}
};

#endif  //EMPTY_BROWSER
