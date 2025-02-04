#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include "EnvelopeHandle.h"
#include "StyledComponent.h"
#include "../StylesStore.h"
#include "../AppState.h"


class EnvelopeDisplay :
    public juce::Component,
    public juce::AudioProcessorParameter::Listener,
    public StyledComponent
{
public:
    EnvelopeDisplay(
        AppState& appState,
        const StylesStore& stylesStore);

    ~EnvelopeDisplay() override;

    void resized() override;
    void paint(juce::Graphics& g) override;
    void parameterValueChanged(int parameterIndex, float newValue) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override;

private:
    static constexpr int graphMargin = 4;
    static constexpr float handleSize = 8.0f;

    float usefulThirdOfWidth;
    float sustainPosX;
    float maxAttackDecayLength;

    std::unique_ptr<EnvelopeHandle> attackHandle, decayHandle, sustainHandle, releaseHandle;
    juce::Range<float> attackDisplayRange;
    juce::Range<float> decayDisplayRange;
    juce::Range<float> sustainDisplayRange;
    juce::Range<float> releaseDisplayRange;

    void initParams(AppState& appState);
    void initEnvelopeHandles();
    juce::Rectangle<int> getUsefulBounds() const;
    void paintGraph(juce::Graphics& g) const;
    void precalculate();
    void calculateDisplayRanges();
    void updateHandlesPositions() const;
    static void setHandleBounds(EnvelopeHandle* handle,
                                const juce::Range<float>& hRange, const juce::Range<float>& vRange,
                                float xPosition, float yPosition);

    juce::AudioParameterFloat* attackParam = nullptr;
    const juce::NormalisableRange<float>* attackRange = nullptr;
    juce::AudioParameterFloat* decayParam = nullptr;
    const juce::NormalisableRange<float>* decayRange = nullptr;
    juce::AudioParameterFloat* sustainParam = nullptr;
    const juce::NormalisableRange<float>* sustainRange = nullptr;
    juce::AudioParameterFloat* releaseParam = nullptr;
    const juce::NormalisableRange<float>* releaseRange = nullptr;
};
