#include "SynthAudioSource.h"

#include "AppParams.h"
#include "MorphVoice.h"

SynthAudioSource::SynthAudioSource(
    juce::MidiKeyboardState& a,
    juce::AudioProcessorValueTreeState& apvts,
    ValueMonitor<double>& monitorMorphPosition)
    : keyboardState(a)
{
    polyphonyParam = dynamic_cast<juce::AudioParameterInt*>(
        apvts.getParameter(AppParams::polyphony));
    for (auto i = 0; i < polyphonyParam->get(); ++i)
    {
        synth.addVoice(new MorphVoice(
            i,
            apvts,
            monitorMorphPosition,
            mostRecentActiveMorphVoiceId));
    }
    synth.addSound(morphSound);
}

void SynthAudioSource::prepareToPlay(int /*samplesPerBlockExpected*/, double sampleRate)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
}

void SynthAudioSource::releaseResources()
{
}

void SynthAudioSource::setWave(const int waveformNum, juce::AudioSampleBuffer& waveform)
{
    morphSound->setWave(waveformNum, waveform);
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
