#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include "EnvelopeDisplay.h"
#include "EnvelopeInfo.h"
#include "../StylesStore.h"


class EnvelopeWidget : public juce::Component
{
public:
    EnvelopeWidget(
        juce::AudioProcessorValueTreeState& apvts,
        const StylesStore& stylesStore
    );

    void resized() override;
    int getPreferredHeight() const;

private:
    const StylesStore& stylesStore;

    std::unique_ptr<EnvelopeDisplay> envelopeDisplay;
    std::unique_ptr<EnvelopeInfo> envelopeInfo;

    void initEnvelopeDisplay(juce::AudioProcessorValueTreeState& apvts);
    void initEnvelopeInfo(juce::AudioProcessorValueTreeState& apvts);
};
