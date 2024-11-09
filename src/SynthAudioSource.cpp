#include "SynthAudioSource.h"

// @formatter:off
SynthAudioSource::SynthAudioSource(juce::MidiKeyboardState & a)
		: keyboardState(a)
{
		for (auto i = 0; i < polyphony; ++i)
		{
			synth.addVoice(new MorphVoice());
		}
		synth.addSound(morphSound);
}
// @formatter:on

void SynthAudioSource::prepareToPlay(int /*samplesPerBlockExpected*/, double sampleRate)
{
	synth.setCurrentPlaybackSampleRate(sampleRate);
}

void SynthAudioSource::releaseResources()
{
}

void SynthAudioSource::setWaveA(juce::AudioSampleBuffer& waveform)
{
	morphSound->setWaveA(waveform);
}

void SynthAudioSource::setWaveB(juce::AudioSampleBuffer& waveform)
{
	morphSound->setWaveB(waveform);
}

void SynthAudioSource::getNextAudioBlock(
		const juce::AudioSourceChannelInfo& bufferToFill)
{
	juce::MidiBuffer emptyMidiBuffer;
	getNextAudioBlock(bufferToFill, emptyMidiBuffer);
}

void SynthAudioSource::getNextAudioBlock(
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