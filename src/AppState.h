#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "ValueMonitor.h"
#include "Global.h"
#include "WaveformPreset.h"

struct AppState
{
public:
    struct Id
    {
        static constexpr auto loopMode = "LoopMode";
        static constexpr auto morphTime = "MorphTime";
        static constexpr auto polyphony = "Polyphony";
        static constexpr auto attack = "AttackTime";
        static constexpr auto decay = "DecayTime";
        static constexpr auto sustain = "SustainLevel";
        static constexpr auto release = "ReleaseTime";
        static constexpr auto waveformDisplayMode = "WaveformDisplayMode";
    };

    struct AudioParameters
    {
        juce::AudioParameterFloat* morphDuration;
        juce::AudioParameterFloat* attack;
        juce::AudioParameterFloat* decay;
        juce::AudioParameterFloat* sustain;
        juce::AudioParameterFloat* release;
        juce::AudioParameterBool* loopMode;
        juce::AudioParameterChoice* waveformDisplayMode;
        juce::AudioParameterInt* polyphony;
    };

    explicit AppState(juce::AudioProcessor& processorToConnectTo);

    juce::AudioProcessorValueTreeState apvts;
    AudioParameters audioParameters{};
    ValueMonitor<double> monitorMorphPosition{MONITOR_MORPH_POSITION_INTERVAL};
    std::vector<juce::AudioSampleBuffer> waveforms = {
        *WaveformPreset::getWaveformPreset(
            WaveformPreset::Type::Sine,
            WAVEFORM_SIZE),
        *WaveformPreset::getWaveformPreset(
            WaveformPreset::Type::Triangle,
            WAVEFORM_SIZE),
    };
    juce::MidiKeyboardState keyboardState;

private:
    static juce::String AppState::formatTime(float value, int maximumStringLength);
    static juce::String AppState::formatPercent(float value, int maximumStringLength);
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
};

enum class WaveformDisplayMode : int
{
    Cartesian = 0,
    Polar,
};

inline juce::StringArray waveformDisplayNames({
    "Cartesian",
    "Polar",
});
