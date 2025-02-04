#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include "EnvelopeDisplay.h"
#include "EnvelopeInfo.h"
#include "../StylesStore.h"
#include "../AppState.h"

class EnvelopeWidget : public juce::Component, public StyledComponent
{
public:
    explicit EnvelopeWidget(
        AppState& appState,
        const StylesStore& stylesStore);

    void resized() override;
    int getPreferredHeight() const;

private:
    std::unique_ptr<EnvelopeDisplay> envelopeDisplay;
    std::unique_ptr<EnvelopeInfo> envelopeInfo;

    void initEnvelopeDisplay(AppState& appState);
    void initEnvelopeInfo(AppState& appState);
};
