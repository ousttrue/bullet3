#include "OpenGLExampleBrowser.h"
#include "ExampleEntries.h"
#include <GwenImpl.h>
#include <Bullet3Common/b3CommandLineArgs.h>
#include <b3Clock.h>
#include <ImportObjDemo/ImportObjExample.h>
#include <ImportBsp/ImportBspExample.h>
#include <ImportColladaDemo/ImportColladaSetup.h>
#include <ImportSTLDemo/ImportSTLSetup.h>
#include <ImportURDFDemo/ImportURDFSetup.h>
#include <ImportSDFDemo/ImportSDFSetup.h>
#include <ImportSTLDemo/ImportSTLSetup.h>
#include <ImportBullet/SerializeSetup.h>

int main(int argc, char* argv[])
{
	b3CommandLineArgs args(argc, argv);
	double gMinUpdateTimeMicroSecs = 1000.;
	args.GetCmdLineArgument("minUpdateTimeMicroSecs", gMinUpdateTimeMicroSecs);

	ExampleEntriesAll examples;
	examples.initExampleEntries();

	OpenGLExampleBrowser exampleBrowser(&examples);
	if (!exampleBrowser.init(argc, argv, GwenImpl::Create))
	{
		return 1;
	}
	exampleBrowser.registerFileImporter(".urdf", ImportURDFCreateFunc);
	exampleBrowser.registerFileImporter(".sdf", ImportSDFCreateFunc);
	exampleBrowser.registerFileImporter(".obj", ImportObjCreateFunc);
	exampleBrowser.registerFileImporter(".stl", ImportSTLCreateFunc);
	exampleBrowser.registerFileImporter(".bullet", SerializeBulletCreateFunc);

	b3Clock clock;
	while (!exampleBrowser.requestedExit())
	{
		float deltaTimeInSeconds = clock.getTimeMicroseconds() / 1000000.f;
		if (deltaTimeInSeconds > 0.1)
		{
			deltaTimeInSeconds = 0.1;
		}
		if (deltaTimeInSeconds < (gMinUpdateTimeMicroSecs / 1e6))
		{
			b3Clock::usleep(gMinUpdateTimeMicroSecs / 10.);
		}
		else
		{
			clock.reset();
			exampleBrowser.update(deltaTimeInSeconds);
		}
	}

#ifdef BT_DEBUG_MEMORY_ALLOCATIONS
	int numBytesLeaked = btDumpMemoryLeaks();
	btAssert(numBytesLeaked == 0);
#endif  //BT_DEBUG_MEMORY_ALLOCATIONS

	return 0;
}
