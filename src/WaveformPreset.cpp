#include "WaveformPreset.h"

std::unique_ptr<juce::AudioSampleBuffer> WaveformPreset::sine(int waveformSize)
{
    std::unique_ptr<juce::AudioSampleBuffer> waveform =
        std::make_unique<juce::AudioSampleBuffer>(1, waveformSize);
    for (int i = 0; i < waveformSize; ++i)
    {
        float value = (float)std::sin(juce::MathConstants<double>::twoPi * i / waveformSize);
        waveform->setSample(0, i, value);
    }
    return waveform;
}

std::unique_ptr<juce::AudioSampleBuffer> WaveformPreset::triangle(int waveformSize)
{
    std::unique_ptr<juce::AudioSampleBuffer> waveform =
        std::make_unique<juce::AudioSampleBuffer>(1, waveformSize);
    int halfIndex = waveformSize / 2;
    int lastIndex = waveformSize - 1;
    for (int i = 0; i < halfIndex; ++i)
    {
        float t = static_cast<float>(i) / lastIndex;
        waveform->setSample(0, i, 1.0f - 4.0f * std::abs(t - 0.25f));
    }
    for (int i = halfIndex; i < waveformSize; ++i)
    {
        float t = static_cast<float>(i) / lastIndex;
        waveform->setSample(0, i, -1.0f + 4.0f * std::abs(t - 0.75f));
    }
    return waveform;
}

std::unique_ptr<juce::AudioSampleBuffer> WaveformPreset::square(int waveformSize)
{
    std::unique_ptr<juce::AudioSampleBuffer> waveform =
        std::make_unique<juce::AudioSampleBuffer>(1, waveformSize);
    int halfIndex = waveformSize / 2;
    for (int i = 0; i < halfIndex; ++i)
    {
        waveform->setSample(0, i, 1.0f);
    }
    for (int i = halfIndex; i < waveformSize; ++i)
    {
        waveform->setSample(0, i, -1.0f);
    }
    return waveform;
}

std::unique_ptr<juce::AudioSampleBuffer> WaveformPreset::sawTooth(int waveformSize)
{
    std::unique_ptr<juce::AudioSampleBuffer> waveform =
        std::make_unique<juce::AudioSampleBuffer>(1, waveformSize);
    int halfIndex = waveformSize / 2;
    int lastIndex = waveformSize - 1;
    for (int i = 0; i < halfIndex; ++i)
    {
        float t = static_cast<float>(i) / lastIndex;
        waveform->setSample(0, i, t * 2.0f);
    }
    for (int i = halfIndex; i < waveformSize; ++i)
    {
        float t = static_cast<float>(i) / lastIndex;
        waveform->setSample(0, i, t * 2.0f - 2.0f);
    }
    return waveform;
}

std::unique_ptr<juce::AudioSampleBuffer> WaveformPreset::getWaveformPreset(
    WaveformPreset::Type preset,
    int size)
{
    switch (preset)
    {
    case WaveformPreset::Type::Sine:
        return WaveformPreset::sine(size);
    case WaveformPreset::Type::Square:
        return WaveformPreset::square(size);
    case WaveformPreset::Type::Triangle:
        return WaveformPreset::triangle(size);
    case WaveformPreset::Type::Sawtooth:
        return WaveformPreset::sawTooth(size);
    }
    DBG(juce::String::formatted("An invalid preset wave was requested: %i", preset));
    jassertfalse;
    return WaveformPreset::sine(size);
}
