#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include "juce_audio_processors/juce_audio_processors.h"
#include "MorphSound.h"
#include "ValueMonitor.h"

class SynthAudioSource : public juce::AudioSource
{
public:
    explicit SynthAudioSource(
        juce::MidiKeyboardState& keyState,
        juce::AudioProcessorValueTreeState& apvts,
        ValueMonitor<double>& monitorMorphPosition);

    void prepareToPlay(int /*samplesPerBlockExpected*/, double sampleRate) override;

    void releaseResources() override;

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill, juce::MidiBuffer& midiMessages);

    void setWave(int waveformNum, juce::AudioSampleBuffer& waveform);

private:
    juce::MidiKeyboardState& keyboardState;
    juce::Synthesiser synth;
    juce::AudioParameterInt* polyphonyParam;
    int mostRecentActiveMorphVoiceId = -1;

    MorphSound* morphSound = new MorphSound();
};
