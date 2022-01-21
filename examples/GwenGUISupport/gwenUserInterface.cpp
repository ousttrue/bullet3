
#include "gwenUserInterface.h"
#include "gwenInternalData.h"
#include "GwenOpenGL3CoreRenderer.h"
#include "GwenParameterInterface.h"
#include <CommonGUIInterface.h>
#include <Common2dCanvasInterface.h>
#include <CommonExampleInterface.h>
#include <LinearMath/btHashMap.h>
#include <OpenGLInclude.h>

GwenUserInterface::GwenUserInterface(GlfwApp* s_app, int width, int height, float retinaScale)
{
	m_data = new GwenInternalData(s_app, width, height, retinaScale);
}

GwenUserInterface::~GwenUserInterface()
{
	delete m_data;
}

std::tuple<CommonGUIInterface*, int> GwenUserInterface::Create(GlfwApp* app, int width, int height, float reginaScale,
															   ExampleEntries* gAllExamples, const char* demoNameFromCommandOption,
															   const std::function<void()>& onB, const std::function<void()>& onD, const std::function<void(int)>& _onE)
{
	auto m_gwen = new GwenUserInterface(app, width, height, reginaScale);
	auto onE = [_onE, m_gwen, gAllExamples](int id)
	{
		_onE(id);
		auto desc = gAllExamples->getExampleDescription(id);
		m_gwen->SetDescription(desc);
	};

	auto firstAvailableDemoIndex = m_gwen->getInternalData()->setup(gAllExamples, demoNameFromCommandOption, onB, onD, onE);

	return {m_gwen, firstAvailableDemoIndex};
}

void GwenUserInterface::SetStatusbarMessage(const char* message, bool isLeft)
{
	Gwen::UnicodeString msg = Gwen::Utility::StringToUnicode(message);
	m_data->setStatusBarMessage(message, isLeft);
}

void GwenUserInterface::RegisterFileOpen(const std::function<void()>& callback)
{
	m_data->registerFileOpen(callback);
}

void GwenUserInterface::RegisterQuit(const std::function<void()>& callback)
{
	m_data->registerQuit(callback);
}

bool GwenUserInterface::OnMouseMove(int x, int y)
{
	return m_data->onMouseMove(x, y);
}

bool GwenUserInterface::OnKeyboard(int bulletKey, int state)
{
	return m_data->onKeyboard(bulletKey, state);
}

bool GwenUserInterface::OnMouseButton(int button, int state, int x, int y)
{
	return m_data->onMouseButton(button, state, x, y);
}

CommonParameterInterface* GwenUserInterface::CreateCommonParameterInterface()
{
	return new GwenParameterInterface(getInternalData());
}

Common2dCanvasInterface* GwenUserInterface::CreateCommon2dCanvasInterface()
{
	return m_data->createCommon2dCanvasInterface();
}

void GwenUserInterface::SetDescription(const char* description)
{
	SetStatusbarMessage("Status: OK", false);
	m_data->setExampleDescription(description);
}

void GwenUserInterface::ShowMessage(const char* msg)
{
	m_data->textOutput(msg);
	ForceUpdateScrollBars();
}

void GwenUserInterface::ShowErrorMessage(const char* msg)
{
	bool isLeft = false;
	SetStatusbarMessage(msg, isLeft);
	m_data->textOutput(msg);
	ForceUpdateScrollBars();
}

void GwenUserInterface::Render(int w, int h)
{
	m_data->render(w, h);
}

void GwenUserInterface::ForceUpdateScrollBars()
{
	m_data->forceUpdateScrollBars();
}
