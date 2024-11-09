#pragma once

#include "juce_audio_basics/juce_audio_basics.h"
#include "MorphSound.h"

class MorphVoice : public juce::SynthesiserVoice
{
public:
	MorphVoice() = default;

	bool canPlaySound(juce::SynthesiserSound* sound) override;

	void startNote(
			int midiNoteNumber,
			float velocity,
			juce::SynthesiserSound* sound,
			int currentPitchWheelPosition) override;

	void stopNote(float velocity, bool allowTailOff) override;

	void pitchWheelMoved(int newPitchWheelValue) override;

	void controllerMoved(int controllerNumber, int newControllerValue) override;

	void renderNextBlock(
			juce::AudioSampleBuffer& outputBuffer,
			int startSample,
			int numSamples) override;

private:
	// current progress in ratio of the waveform from [0, 1[
	double waveformPosition = 0.0;
	// ratio of the waveform to progress on each sample output
	// >0.0 means sound is being output
	double waveformDelta = 0.0;

	// current progress in ratio of the morph from [0, 1]
	double morphPosition = 0.0f;
	// ratio of the morph to progress on each sample output
	double morphDelta = 0.0f;

	// output level from a note on
	float level = 0.0f;
	// absolute max level
	float maxLevel = 0.25f;

	double morphDuration = 2; // seconds

	MorphSound* morphSound = nullptr;
};
