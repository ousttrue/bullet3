#ifndef EMPTY_BROWSER
#define EMPTY_BROWSER

#include "EmptyExample.h"

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

	virtual bool init(int /*argc*/, char* argv[])
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
};

#endif  //EMPTY_BROWSER
