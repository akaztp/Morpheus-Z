#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include "../StylesStore.h"
#include "../AppState.h"
#include "StyledComponent.h"


class WaveformDisplay :
    public StyledComponent,
    public juce::Component,
    public juce::AudioProcessorParameter::Listener
{
public:
    WaveformDisplay(
        const StylesStore& stylesStore,
        AppState& appState,
         const juce::AudioSampleBuffer& waveform);

    ~WaveformDisplay() override;

    void paint(juce::Graphics&) override;

    void setWaveFormColorId(const StylesStore::ColorIds colorId)
    {
        waveformColorId = colorId;
        repaint();
    }

private:
    const juce::AudioSampleBuffer& waveform;
    StylesStore::ColorIds waveformColorId;
    juce::AudioParameterChoice* waveformDisplayModeParam = nullptr;

    void initParams(AppState& appState);
    void paintCartesian(juce::Graphics& g, int width, int height) const;
    void paintPolar(juce::Graphics& g, int width, int height) const;
    void parameterValueChanged(int parameterIndex, float newValue) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override;
};
