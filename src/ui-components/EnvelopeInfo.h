#pragma once

#include <juce_audio_utils/juce_audio_utils.h>

#include "StyledComponent.h"
#include "../AppState.h"
#include "../StylesStore.h"

class EnvelopeInfo : public juce::Component, public StyledComponent
{
public:
    EnvelopeInfo(
        AppState& appState,
        const StylesStore& stylesStore);

    void paint(juce::Graphics& g) override;
    void resized() override;
    int getPreferredHeight() const;
    void parentHierarchyChanged() override;

private:
    struct ParamInfo
    {
        explicit ParamInfo(const char* paramId): paramId(paramId)
        {
            slider = nullptr;
            sliderAttachment = nullptr;
        }

        const char* paramId;
        std::unique_ptr<juce::Slider> slider;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachment;
    };

    static constexpr int numEnvSteps = 4;
    ParamInfo infoConfig[numEnvSteps] = {
        ParamInfo{AppState::Id::attack},
        ParamInfo{AppState::Id::decay},
        ParamInfo{AppState::Id::sustain},
        ParamInfo{AppState::Id::release},
    };
};
