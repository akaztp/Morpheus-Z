#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

struct AppParams
{
    typedef const char* Param;

    static constexpr Param loopMode = "LoopMode";
    static constexpr Param morphTime = "Morph Time";
    static constexpr Param polyphony = "Polyphony";
    static constexpr Param attack = "Attack Time";
    static constexpr Param decay = "Decay Time";
    static constexpr Param sustain = "Sustain Level";
    static constexpr Param release = "Release Time";

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:
    static juce::String AppParams::formatTime(float value, int maximumStringLength);

    static juce::String AppParams::formatPercent(float value, int maximumStringLength);
};
