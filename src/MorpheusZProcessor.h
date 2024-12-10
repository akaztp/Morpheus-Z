#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "AppParams.h"
#include "SynthAudioSource.h"
#include "MorpheusZEditor.h"

//==============================================================================
class MorpheusZProcessor final : public juce::AudioProcessor
{
public:
    //==============================================================================
    MorpheusZProcessor();

    ~MorpheusZProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;

    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;

    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;

    bool producesMidi() const override;

    bool isMidiEffect() const override;

    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;

    int getCurrentProgram() override;

    void setCurrentProgram(int index) override;

    const juce::String getProgramName(int index) override;

    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;

    void setStateInformation(const void* data, int sizeInBytes) override;

    juce::MidiKeyboardState keyboardState;

    void setWaveformA(WaveformPreset preset);
    void setWaveformB(WaveformPreset preset);

    void setWaveformAValue(int index, float value);
    void setWaveformBValue(int index, float value);

private:
    juce::AudioProcessorValueTreeState apvts{
        *this,
        nullptr,
        "AppParams",
        AppParams::createParameterLayout()
    };
    int waveformSize = 512;
    juce::AudioSampleBuffer waveformA{1, waveformSize};
    juce::AudioSampleBuffer waveformB{1, waveformSize};
    SynthAudioSource synthAudioSource;

    std::unique_ptr<juce::AudioSampleBuffer> getWaveformPreset(
        WaveformPreset preset,
        int size) const;
    void refreshWaveformAEditor() const;
    void refreshWaveformBEditor() const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MorpheusZProcessor)
};
