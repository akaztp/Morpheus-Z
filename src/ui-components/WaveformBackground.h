#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>

#include "StyledComponent.h"
#include "../StylesStore.h"
#include "../AppState.h"

class WaveformBackground :
    public juce::Component,
    public juce::AudioProcessorParameter::Listener,
    public StyledComponent
{
public:
    explicit WaveformBackground(
        const StylesStore& stylesStore,
        AppState& appState);

    ~WaveformBackground() override;

    void paint(juce::Graphics& g) override;

private:
    juce::AudioParameterChoice* waveformDisplayModeParam = nullptr;

    void initParams(AppState& appState);
    static void paintCartOrigin(juce::Graphics& g, juce::Rectangle<int> bounds);
    static void paintPolarOrigin(juce::Graphics& g, juce::Rectangle<int> bounds);
    void parameterValueChanged(int parameterIndex, float newValue) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override;
};
