#pragma once

#include "juce_audio_basics/juce_audio_basics.h"
#include "SineWaveVoice.h"
#include "SineWaveSound.h"

class SynthAudioSource : public juce::AudioSource
{
public:
	SynthAudioSource(juce::MidiKeyboardState& keyState)
			: keyboardState(keyState)
	{
		for (auto i = 0; i < 4; ++i)                // [1]
			synth.addVoice(new SineWaveVoice());

		synth.addSound(new SineWaveSound());       // [2]
	}

	void prepareToPlay(int /*samplesPerBlockExpected*/, double sampleRate) override
	{
		synth.setCurrentPlaybackSampleRate(sampleRate); // [3]
	}

	void releaseResources() override
	{
	}

	void getNextAudioBlock(
			const juce::AudioSourceChannelInfo& bufferToFill) override
	{
		juce::MidiBuffer emptyMidiBuffer;
		getNextAudioBlock(bufferToFill, emptyMidiBuffer);
	}

	void getNextAudioBlock(
			const juce::AudioSourceChannelInfo& bufferToFill,
			juce::MidiBuffer& midiMessages)
	{
		bufferToFill.clearActiveBufferRegion();

		keyboardState.processNextMidiBuffer(
				midiMessages,
				bufferToFill.startSample,
				bufferToFill.numSamples,
				true);
		synth.renderNextBlock(
				*bufferToFill.buffer,
				midiMessages,
				bufferToFill.startSample,
				bufferToFill.numSamples);
	}

private:
	juce::MidiKeyboardState& keyboardState;
	juce::Synthesiser synth;
};
