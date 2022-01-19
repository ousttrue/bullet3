#pragma once
#include "CommonExampleInterface.h"

class EmptyExample : public CommonExampleInterface
{
public:
	EmptyExample() {}
	~EmptyExample() override {}

	static CommonExampleInterface* CreateFunc(struct CommonExampleOptions& /* unusedOptions*/)
	{
		return new EmptyExample;
	}

	void initPhysics() override {}
	void exitPhysics() override {}
	void stepSimulation(float deltaTime) override {}
	void physicsDebugDraw(int debugFlags) override {}
	bool mouseMoveCallback(const CommonCameraInterface* camera, float x, float y) override { return false; }
	bool mouseButtonCallback(const CommonCameraInterface* camera, int button, int state, float x, float y, ButtonFlags flags) override { return false; }
	bool keyboardCallback(int key, int state) override { return false; }
};
