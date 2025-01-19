#include <juce_audio_processors/juce_audio_processors.h>
#include "AppParams.h"

juce::AudioProcessorValueTreeState::ParameterLayout AppParams::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(
        std::make_unique<juce::AudioParameterInt>(
            AppParams::polyphony,
            "Polyphony",
            1,
            32,
            32));

    layout.add(
        std::make_unique<juce::AudioParameterBool>(
            AppParams::loopMode,
            "Loop Mode",
            false));

    layout.add(
        std::make_unique<juce::AudioParameterFloat>(
            AppParams::morphTime,
            "Morph Time (sec)",
            0.0f,
            20.0f,
            2.0f));

    layout.add(
        std::make_unique<juce::AudioParameterFloat>(
            AppParams::attack,
            "Attack (sec)",
            juce::NormalisableRange<float>(0.0f, 10.0f, 0.0f, 0.3f),
            0.1f,
            "",
            juce::AudioProcessorParameter::genericParameter,
            AppParams::formatTime));

    layout.add(
        std::make_unique<juce::AudioParameterFloat>(
            AppParams::decay,
            "Decay (sec)",
            juce::NormalisableRange<float>(0.0f, 10.0f, 0.0f, 0.3f),
            0.1f,
            "",
            juce::AudioProcessorParameter::genericParameter,
            AppParams::formatTime));

    layout.add(
        std::make_unique<juce::AudioParameterFloat>(
            AppParams::sustain,
            "Sustain Level",
            juce::NormalisableRange<float>(0.0f, 1.0f, 0.0f, 0.5f),
            0.5f,
            "",
            juce::AudioProcessorParameter::genericParameter,
            AppParams::formatPercent));

    layout.add(
        std::make_unique<juce::AudioParameterFloat>(
            AppParams::release,
            "Release (sec)",
            juce::NormalisableRange<float>(0.0f, 10.0f, 0.0f, 0.3f),
            0.100f,
            "",
            juce::AudioProcessorParameter::genericParameter,
            AppParams::formatTime));

    layout.add(
        std::make_unique<juce::AudioParameterChoice>(
            AppParams::waveformDisplayMode,
            "Waveform Display Mode",
            waveformDisplayNames,
            static_cast<int>(WaveformDisplayMode::Polar)
        ));

    return layout;
}

juce::String AppParams::formatTime(const float value, int maximumStringLength)
{
    juce::ignoreUnused(maximumStringLength);
    return juce::String(static_cast<int>(value * 1000.0f)) + "ms";
}

juce::String AppParams::formatPercent(const float value, int maximumStringLength)
{
    juce::ignoreUnused(maximumStringLength);
    return juce::String(static_cast<int>(value * 100.0f)) + "%";
}
