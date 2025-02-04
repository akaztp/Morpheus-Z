#include "SynthAudioSource.h"
#include "MorphVoice.h"


SynthAudioSource::SynthAudioSource(AppState& appState)
    : appState(appState)
{
    const int numVoices = appState.audioParameters.polyphony->get();
    for (auto i = 0; i < numVoices; ++i)
    {
        synth.addVoice(new MorphVoice(
            i,
            appState,
            mostRecentActiveMorphVoiceId));
    }
    synth.addSound(new MorphSound(appState));
}

void SynthAudioSource::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    juce::ignoreUnused(samplesPerBlockExpected);
    synth.setCurrentPlaybackSampleRate(sampleRate);
}

void SynthAudioSource::releaseResources()
{
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

    appState.keyboardState.processNextMidiBuffer(
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
