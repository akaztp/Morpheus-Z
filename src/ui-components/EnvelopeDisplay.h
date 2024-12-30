#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include "../StylesStore.h"

class EnvelopeDisplay :
    public juce::Component,
    public juce::AudioProcessorParameter::Listener
{
public:
    EnvelopeDisplay(
        juce::AudioProcessorValueTreeState& apvts,
        const StylesStore& stylesStore);

    ~EnvelopeDisplay() override;

    void paint(juce::Graphics& g) override;
    void paintGraph(juce::Graphics& g);
    void parameterValueChanged(int parameterIndex, float newValue) override;
    void parameterGestureChanged (int parameterIndex, bool gestureIsStarting) override;

private:
    static constexpr int graphMargin = 4;
    juce::AudioParameterFloat* attackParam = nullptr;
    const juce::NormalisableRange<float>* attackRange = nullptr;
    float attackX = 0.0f;
    juce::AudioParameterFloat* decayParam = nullptr;
    const juce::NormalisableRange<float>* decayRange = nullptr;
    float decayX = 0.0f;
    juce::AudioParameterFloat* sustainParam = nullptr;
    const juce::NormalisableRange<float>* sustainRange = nullptr;
    float sustainY = 0.0f;
    juce::AudioParameterFloat* releaseParam = nullptr;
    const juce::NormalisableRange<float>* releaseRange = nullptr;
    float releaseX = 0.0f;

    const StylesStore& stylesStore;
};
