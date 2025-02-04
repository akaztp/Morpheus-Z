#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include "AppState.h"

class MorphSound : public juce::SynthesiserSound
{
public:
	explicit MorphSound(AppState& appState);

    ~MorphSound() override;

    bool appliesToNote(int) override;

    bool appliesToChannel(int) override;

    /**
     * @param {number} position - The position of the sample to get in range [0, 1[
     * @returns {number} The value of the sample at the given position
     */
    forcedinline float getMorphedSample(
        const double waveformPosition,
        const double morphPosition) const noexcept
    {
        const auto a = getWaveformSample(waveformPosition, waveforms[0], waveformSizes[0]);
        const auto b = getWaveformSample(waveformPosition, waveforms[1], waveformSizes[1]);
        return static_cast<float>((1.0 - morphPosition) * a + morphPosition * b);
    }

private:
    static constexpr int numWaveforms = 2;
    juce::AudioSampleBuffer* waveforms[numWaveforms] = {nullptr, nullptr};
    int waveformSizes[numWaveforms] = {0, 0}; // size of the waveforms

    forcedinline static float getWaveformSample(
        const double position,
        const juce::AudioSampleBuffer* waveform,
        const int waveformSize) noexcept
    {
        const auto currentIndex = position * waveformSize;
        const auto index0 = static_cast<unsigned int>(currentIndex); // = floor(currentIndex)
        const auto index1 = index0 + 1;

        auto* wave = waveform->getReadPointer(0);
        const auto value0 = wave[index0];
        const auto value1 = (index1 != waveformSize) ? wave[index1] : wave[0];

        // interpolating between the two samples
        const auto frac = currentIndex - static_cast<double>(index0);
        const auto currentSample = static_cast<double>(value0) + frac * static_cast<double>(value1 - value0);

        return static_cast<float>(currentSample);
    }
};
