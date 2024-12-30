#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include "../AppParams.h"
#include "../StylesStore.h"

class EnvelopeInfo : public juce::Component
{
public:
    EnvelopeInfo(
        juce::AudioProcessorValueTreeState& apvts,
        const StylesStore& stylesStore);

    void paint(juce::Graphics& g) override;
    void resized() override;
    int getPreferredHeight() const;
    void parentHierarchyChanged() override;

private:
    struct ParamInfo
    {
        explicit ParamInfo(const AppParams::Param param): param(param)
        {
            slider = nullptr;
            sliderAttachment = nullptr;
        }

        const AppParams::Param param;
        std::unique_ptr<juce::Slider> slider;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachment;
    };

    static constexpr int numEnvSteps = 4;
    ParamInfo infoConfig[numEnvSteps] = {
        ParamInfo{AppParams::attack},
        ParamInfo{AppParams::decay},
        ParamInfo{AppParams::sustain},
        ParamInfo{AppParams::release},
    };

    const StylesStore& stylesStore;
};
