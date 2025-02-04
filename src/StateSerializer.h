#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include "AppState.h"

class StateSerializer
{
public:
    static void getStateInformation(
        juce::MemoryBlock& destData,
        AppState& appState);

    static void setStateInformation(
        const void* data,
        int sizeInBytes,
        AppState& appState);

private:
    // Changing these strings will invalidate previous stored state (files or DAW)
    struct Elements
    {
        static constexpr auto state = "State";
        static constexpr auto appParams = "AppParams";
        static constexpr auto waveformA = "WaveformA";
        static constexpr auto waveformB = "WaveformB";
    };

    static std::unique_ptr<juce::XmlElement> getWaveformXml(
        const juce::String& name,
        const juce::AudioSampleBuffer& waveform);

    static void setWaveformFromXml(
        const juce::XmlElement* stateXml,
        const juce::String& name,
        juce::AudioSampleBuffer& waveform);
};
