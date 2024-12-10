#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

struct AppParams
{
    static constexpr auto loopMode = "LoopMode";
    static constexpr auto morphDuration = "MorphDuration";
    static constexpr auto polyphony = "Polyphony";

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
};
