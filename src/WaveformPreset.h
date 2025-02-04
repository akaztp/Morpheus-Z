#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

class WaveformPreset
{
public:
    enum class Type
    {
        Sine = 0,
        Square,
        Triangle,
        Sawtooth,
    };

    static std::unique_ptr<juce::AudioSampleBuffer> getWaveformPreset(
        Type preset,
        int size);

private:
    static std::unique_ptr<juce::AudioSampleBuffer> sine(int waveformSize);
    static std::unique_ptr<juce::AudioSampleBuffer> triangle(int waveformSize);
    static std::unique_ptr<juce::AudioSampleBuffer> square(int waveformSize);
    static std::unique_ptr<juce::AudioSampleBuffer> sawTooth(int waveformSize);
};
