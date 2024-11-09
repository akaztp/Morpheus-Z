#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include "MorphSound.h"
#include "MorphVoice.h"

class SynthAudioSource : public juce::AudioSource
{
public:
	explicit SynthAudioSource(juce::MidiKeyboardState& keyState);

	void prepareToPlay(int /*samplesPerBlockExpected*/, double sampleRate) override;

	void releaseResources() override;

	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill, juce::MidiBuffer& midiMessages);

	void setWaveA(juce::AudioSampleBuffer& waveform);
	void setWaveB(juce::AudioSampleBuffer& waveform);

private:
	juce::MidiKeyboardState& keyboardState;
	juce::Synthesiser synth;
	int polyphony = 4;
	MorphSound* morphSound = new MorphSound();

};
