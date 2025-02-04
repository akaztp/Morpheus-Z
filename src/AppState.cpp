#include <juce_audio_processors/juce_audio_processors.h>
#include "AppState.h"
#include "WaveformPreset.h"

AppState::AppState(
    juce::AudioProcessor& processorToConnectTo
) : apvts(processorToConnectTo, nullptr, "AppParams", createParameterLayout())
{
    audioParameters.loopMode = dynamic_cast<juce::AudioParameterBool*>(
        apvts.getParameter(Id::loopMode));
    jassert(audioParameters.loopMode != nullptr);

    audioParameters.morphDuration = dynamic_cast<juce::AudioParameterFloat*>(
        apvts.getParameter(Id::morphTime));
    jassert(audioParameters.morphDuration != nullptr);

    audioParameters.attack = dynamic_cast<juce::AudioParameterFloat*>(
        apvts.getParameter(Id::attack));
    jassert(audioParameters.attack != nullptr);

    audioParameters.decay = dynamic_cast<juce::AudioParameterFloat*>(
        apvts.getParameter(Id::decay));
    jassert(audioParameters.decay != nullptr);

    audioParameters.sustain = dynamic_cast<juce::AudioParameterFloat*>(
        apvts.getParameter(Id::sustain));
    jassert(audioParameters.sustain != nullptr);

    audioParameters.release = dynamic_cast<juce::AudioParameterFloat*>(
        apvts.getParameter(Id::release));
    jassert(audioParameters.release != nullptr);

    audioParameters.waveformDisplayMode = dynamic_cast<juce::AudioParameterChoice*>(
        apvts.getParameter(Id::waveformDisplayMode));
    jassert(audioParameters.waveformDisplayMode != nullptr);

    audioParameters.polyphony = dynamic_cast<juce::AudioParameterInt*>(
        apvts.getParameter(Id::polyphony));
    jassert(audioParameters.polyphony != nullptr);
}

juce::AudioProcessorValueTreeState::ParameterLayout AppState::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(
        std::make_unique<juce::AudioParameterInt>(
            Id::polyphony,
            "Polyphony",
            1,
            32,
            32));

    layout.add(
        std::make_unique<juce::AudioParameterBool>(
            Id::loopMode,
            "Loop Mode",
            false));

    layout.add(
        std::make_unique<juce::AudioParameterFloat>(
            Id::morphTime,
            "Morph Time (sec)",
            0.0f,
            20.0f,
            2.0f));

    layout.add(
        std::make_unique<juce::AudioParameterFloat>(
            Id::attack,
            "Attack (sec)",
            juce::NormalisableRange<float>(0.0f, 10.0f, 0.0f, 0.3f),
            0.1f,
            "",
            juce::AudioProcessorParameter::genericParameter,
            AppState::formatTime));

    layout.add(
        std::make_unique<juce::AudioParameterFloat>(
            Id::decay,
            "Decay (sec)",
            juce::NormalisableRange<float>(0.0f, 10.0f, 0.0f, 0.3f),
            0.1f,
            "",
            juce::AudioProcessorParameter::genericParameter,
            AppState::formatTime));

    layout.add(
        std::make_unique<juce::AudioParameterFloat>(
            Id::sustain,
            "Sustain Level",
            juce::NormalisableRange<float>(0.0f, 1.0f, 0.0f, 0.5f),
            0.5f,
            "",
            juce::AudioProcessorParameter::genericParameter,
            AppState::formatPercent));

    layout.add(
        std::make_unique<juce::AudioParameterFloat>(
            Id::release,
            "Release (sec)",
            juce::NormalisableRange<float>(0.0f, 10.0f, 0.0f, 0.3f),
            0.100f,
            "",
            juce::AudioProcessorParameter::genericParameter,
            AppState::formatTime));

    layout.add(
        std::make_unique<juce::AudioParameterChoice>(
            Id::waveformDisplayMode,
            "Waveform Display Mode",
            waveformDisplayNames,
            static_cast<int>(WaveformDisplayMode::Polar)
        ));

    return layout;
}

juce::String AppState::formatTime(const float value, int maximumStringLength)
{
    juce::ignoreUnused(maximumStringLength);
    return juce::String(static_cast<int>(value * 1000.0f)) + "ms";
}

juce::String AppState::formatPercent(const float value, int maximumStringLength)
{
    juce::ignoreUnused(maximumStringLength);
    return juce::String(static_cast<int>(value * 100.0f)) + "%";
}
