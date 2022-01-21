#pragma once
#include <CommonExampleInterface.h>
struct TimeSeriesExample : public CommonExampleInterface
{
	struct CommonGraphicsApp* m_app;
	struct TimeSeriesExampleInternalData* m_internalData;

	TimeSeriesExample(struct CommonGraphicsApp* app);
	~TimeSeriesExample() override;
	void initPhysics(CommonCameraInterface* camera, struct GUIHelperInterface* m_guiHelper) override;
	void stepSimulation(float deltaTime) override;
};
