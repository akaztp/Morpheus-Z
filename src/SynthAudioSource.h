#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include "AppState.h"


class SynthAudioSource : public juce::AudioSource
{
public:
    explicit SynthAudioSource(AppState& appState);

    void prepareToPlay(int /*samplesPerBlockExpected*/, double sampleRate) override;

    void releaseResources() override;

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill, juce::MidiBuffer& midiMessages);

private:
    juce::Synthesiser synth;
    AppState& appState;
    int mostRecentActiveMorphVoiceId = -1;
};
