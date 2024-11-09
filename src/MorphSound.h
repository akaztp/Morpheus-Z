#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

class MorphSound : public juce::SynthesiserSound
{
public:
	MorphSound() = default;

	void setWaveA(juce::AudioSampleBuffer& waveform);

	void setWaveB(juce::AudioSampleBuffer& waveform);

	bool appliesToNote(int) override;

	bool appliesToChannel(int) override;

	/**
	 * This method assumes the waveform has been increased and wrapped by one sample
	 * @param {number} position - The position of the sample to get in range [0, 1[
	 * @returns {number} The value of the sample at the given position
	 */
	forcedinline float getSample(double waveformPosition, double morphPosition) const noexcept
	{
		auto a = getWaveformSample(waveformPosition, waveformA, waveformASize);
		auto b = getWaveformSample(waveformPosition, waveformB, waveformBSize);
		return (float)((1.0 - morphPosition) * a +  morphPosition * b);
	}

	forcedinline static float getWaveformSample(
			double position,
			juce::AudioSampleBuffer* waveform,
			int waveformSize) noexcept
	{
		if (waveform == nullptr)
		{ return 0.0f; }

		auto currentIndex = position * waveformSize;
		auto index0 = (unsigned int)currentIndex;
		auto index1 = index0 + 1;

		auto* wave = waveform->getReadPointer(0);
		auto value0 = wave[index0];
		auto value1 = wave[index1];

		auto frac = currentIndex - (double)index0;
		auto currentSample = (double)value0 + frac * (double)(value1 - value0); // interpolation
		return (float)currentSample;
	}

	~MorphSound() override;

private:

	static juce::AudioSampleBuffer* wrapWaveform(juce::AudioSampleBuffer& waveform);

	// waveform duplicated from the original but wrapped
	juce::AudioSampleBuffer* waveformA = nullptr;
	int waveformASize = 0; // size of the waveform unwrapped

	// waveform duplicated from the original but wrapped
	juce::AudioSampleBuffer* waveformB = nullptr;
	int waveformBSize = 0; // size of the waveform unwrapped
};
