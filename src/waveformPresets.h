#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

namespace waveformPresets
{
	std::unique_ptr<juce::AudioSampleBuffer> sine(int waveformSize);

	std::unique_ptr<juce::AudioSampleBuffer> triangle(int waveformSize);

	std::unique_ptr<juce::AudioSampleBuffer> square(int waveformSize);

	std::unique_ptr<juce::AudioSampleBuffer> sawTooth(int waveformSize);
};
