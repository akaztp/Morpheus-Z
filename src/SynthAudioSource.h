#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include "juce_audio_processors/juce_audio_processors.h"
#include "MorphSound.h"

class SynthAudioSource : public juce::AudioSource
{
public:
    explicit SynthAudioSource(
        juce::MidiKeyboardState& keyState,
        juce::AudioProcessorValueTreeState& apvts);

    void prepareToPlay(int /*samplesPerBlockExpected*/, double sampleRate) override;

    void releaseResources() override;

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill, juce::MidiBuffer& midiMessages);

    void setWaveA(juce::AudioSampleBuffer& waveform);
    void setWaveB(juce::AudioSampleBuffer& waveform);

private:
    juce::MidiKeyboardState& keyboardState;
    juce::Synthesiser synth;
    juce::AudioParameterInt* polyphonyParam;

    MorphSound* morphSound = new MorphSound();
};
