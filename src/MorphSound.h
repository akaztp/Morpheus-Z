#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

class MorphSound : public juce::SynthesiserSound
{
public:
	MorphSound() = default;

    void setWave(int waveformNum, juce::AudioSampleBuffer& waveform);

    bool appliesToNote(int) override;

    bool appliesToChannel(int) override;

    /**
     * This method assumes the waveform size has been increased and wrapped by one sample
     * @param {number} position - The position of the sample to get in range [0, 1[
     * @returns {number} The value of the sample at the given position
     */
    forcedinline float getSample(
        const double waveformPosition,
        const double morphPosition) const noexcept
    {
        const auto a = getWaveformSample(waveformPosition, waveforms[0], waveformSizes[0]);
        const auto b = getWaveformSample(waveformPosition, waveforms[1], waveformSizes[1]);
        return static_cast<float>((1.0 - morphPosition) * a + morphPosition * b);
    }

    forcedinline static float getWaveformSample(
        const double position,
        const juce::AudioSampleBuffer* waveform,
        const int waveformSize) noexcept
    {
        if (waveform == nullptr)
        {
            return 0.0f;
        }

        const auto currentIndex = position * waveformSize;
        const auto index0 = static_cast<unsigned int>(currentIndex);
        const auto index1 = index0 + 1;

        auto* wave = waveform->getReadPointer(0);
        const auto value0 = wave[index0];
        const auto value1 = wave[index1];

        const auto frac = currentIndex - static_cast<double>(index0);
        const auto currentSample = static_cast<double>(value0) + frac * static_cast<double>(value1 - value0);
        // interpolation
        return static_cast<float>(currentSample);
    }

    ~MorphSound() override;

private:
    static juce::AudioSampleBuffer* wrapWaveform(juce::AudioSampleBuffer& waveform);
    static constexpr int numWaveforms = 2;

    // waveform duplicated from the original but wrapped
    juce::AudioSampleBuffer* waveforms[numWaveforms] = {nullptr, nullptr};
    int waveformSizes[numWaveforms] = {0, 0}; // size of the waveform unwrapped
};
