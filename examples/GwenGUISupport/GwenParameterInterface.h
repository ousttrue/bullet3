#pragma once
#include <CommonParameterInterface.h>

struct GwenParameterInterface : public CommonParameterInterface
{
	class GwenInternalData* m_gwenInternalData;
	struct GwenParameters* m_paramInternalData;
	GwenParameterInterface(class GwenInternalData* gwenInternalData);
	~GwenParameterInterface() override;
	void registerSliderFloatParameter(SliderParams& params) override;
	void registerButtonParameter(ButtonParams& params) override;
	void registerComboBox(ComboBoxParams& params) override;
	void setSliderValue(int sliderIndex, double sliderValue) override;
	void syncParameters() override;
	void removeAllParameters() override;
};
