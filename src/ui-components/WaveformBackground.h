#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include "../StylesStore.h"


class WaveformBackground :
    public juce::Component,
    public juce::AudioProcessorParameter::Listener
{
public:
    explicit WaveformBackground(
        const StylesStore& stylesStore,
        juce::AudioProcessorValueTreeState& apvts);

    ~WaveformBackground() override;

    void paint(juce::Graphics& g) override;

private:
    const StylesStore& stylesStore;
    juce::AudioParameterChoice* waveformDisplayModeParam = nullptr;

    void initParams(juce::AudioProcessorValueTreeState& apvts);
    static void paintCartOrigin(juce::Graphics& g, juce::Rectangle<int> bounds);
    static void paintPolarOrigin(juce::Graphics& g, juce::Rectangle<int> bounds);
    void parameterValueChanged(int parameterIndex, float newValue) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override;
};
