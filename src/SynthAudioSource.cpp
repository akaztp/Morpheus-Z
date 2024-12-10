#include "SynthAudioSource.h"

#include "AppParams.h"
#include "MorphVoice.h"

// @formatter:off
SynthAudioSource::SynthAudioSource(
    juce::MidiKeyboardState& a,
    juce::AudioProcessorValueTreeState& apvts)
		: keyboardState(a)
{
    polyphonyParam = dynamic_cast<juce::AudioParameterInt*>(
        apvts.getParameter(AppParams::polyphony));
	for (auto i = 0; i < polyphonyParam->get(); ++i)
	{
		synth.addVoice(new MorphVoice(apvts));
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
