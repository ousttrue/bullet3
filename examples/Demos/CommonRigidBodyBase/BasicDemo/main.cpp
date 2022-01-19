#include "BasicExample.h"
#include <memory>

int main(int argc, char* argv[])
{
	auto example = std::make_unique<BasicExample>();
	example->initPhysics({}, {});
	example->stepSimulation(1.f / 60.f);
	example->exitPhysics();
	return 0;
}
