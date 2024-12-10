#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

class StateHandler
{
public:
    static void getStateInformation(
        juce::MemoryBlock& destData,
        const juce::AudioSampleBuffer& waveformA,
        const juce::AudioSampleBuffer& waveformB,
        juce::AudioProcessorValueTreeState& apvts);

    static void setStateInformation(
        const void* data,
        int sizeInBytes,
        juce::AudioSampleBuffer& waveformA,
        juce::AudioSampleBuffer& waveformB,
        juce::AudioProcessorValueTreeState& apvts);

private:
    // Changing these strings will invalidate previous stored state (files or DAW)
    struct Elements
    {
        static constexpr const auto state = "State";
        static constexpr const auto appParams = "AppParams";
        static constexpr const auto waveformA = "WaveformA";
        static constexpr const auto waveformB = "WaveformB";
    };

    static std::unique_ptr<juce::XmlElement> getWaveformXml(
        const juce::String& name,
        const juce::AudioSampleBuffer& waveform);

    static void setWaveformFromXml(
        const juce::XmlElement* stateXml,
        const juce::String& name,
        juce::AudioSampleBuffer& waveform);
};
