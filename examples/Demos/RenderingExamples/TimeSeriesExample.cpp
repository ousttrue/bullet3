#include "TimeSeriesExample.h"
#include <CommonGraphicsAppInterface.h>
// #include <CommonRenderInterface.h>
// #include <Common2dCanvasInterface.h>
// #include "LinearMath/btAlignedObjectArray.h"
// #include "btBulletCollisionCommon.h"
// #include <CommonGUIHelperInterface.h>
#include "TimeSeriesCanvas.h"

struct TimeSeriesExampleInternalData
{
	TimeSeriesCanvas* m_timeSeriesCanvas;

	TimeSeriesExampleInternalData()
		: m_timeSeriesCanvas(0)
	{
	}
};

TimeSeriesExample::TimeSeriesExample(struct CommonGraphicsApp* app)
{
	m_app = app;
	m_internalData = new TimeSeriesExampleInternalData;
}

TimeSeriesExample::~TimeSeriesExample()
{
	delete m_internalData->m_timeSeriesCanvas;
	delete m_internalData;
}

void TimeSeriesExample::initPhysics(CommonCameraInterface* camera, struct GUIHelperInterface* m_guiHelper)
{
	//request a visual bitma/texture we can render to

	m_internalData->m_timeSeriesCanvas = new TimeSeriesCanvas(m_app->m_2dCanvasInterface, 512, 512, "Test");
	m_internalData->m_timeSeriesCanvas->setupTimeSeries(3, 100, 0);
	m_internalData->m_timeSeriesCanvas->addDataSource("Some sine wave", 255, 0, 0);
	m_internalData->m_timeSeriesCanvas->addDataSource("Some cosine wave", 0, 255, 0);
	m_internalData->m_timeSeriesCanvas->addDataSource("Delta Time (*10)", 0, 0, 255);
	m_internalData->m_timeSeriesCanvas->addDataSource("Tan", 255, 0, 255);
	m_internalData->m_timeSeriesCanvas->addDataSource("Some cosine wave2", 255, 255, 0);
	m_internalData->m_timeSeriesCanvas->addDataSource("Empty source2", 255, 0, 255);
}

void TimeSeriesExample::stepSimulation(float deltaTime)
{
	float time = m_internalData->m_timeSeriesCanvas->getCurrentTime();
	float v = sinf(time);
	m_internalData->m_timeSeriesCanvas->insertDataAtCurrentTime(v, 0, true);
	v = cosf(time);
	m_internalData->m_timeSeriesCanvas->insertDataAtCurrentTime(v, 1, true);
	v = tanf(time);
	m_internalData->m_timeSeriesCanvas->insertDataAtCurrentTime(v, 3, true);
	m_internalData->m_timeSeriesCanvas->insertDataAtCurrentTime(deltaTime * 10, 2, true);

	m_internalData->m_timeSeriesCanvas->nextTick();
}
