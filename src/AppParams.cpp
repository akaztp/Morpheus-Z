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
            4));

    layout.add(
        std::make_unique<juce::AudioParameterBool>(
            AppParams::loopMode,
            "Loop Mode",
            false));

    layout.add(
        std::make_unique<juce::AudioParameterFloat>(
            AppParams::morphDuration,
            "Morph Duration (sec)",
            0.0f,
            20.0f,
            2.0f));

    return layout;
}

// constexpr const char* AppParamsArray::operator[](AppParam id) const
// {
//     if (id >= AppParam::Count)
//     {
//         throw std::out_of_range("Invalid id");
//     }
//     return IdNames[static_cast<size_t>(id)];
// }
