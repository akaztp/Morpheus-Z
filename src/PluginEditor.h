#pragma once

#include "PluginProcessor.h"
#include "juce_audio_utils/juce_audio_utils.h"

//==============================================================================
class AudioPluginAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
	explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor&);

	~AudioPluginAudioProcessorEditor() override;

	//==============================================================================
	void paint(juce::Graphics&) override;

	void resized() override;

private:
	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	AudioPluginAudioProcessor& processorRef;
	juce::MidiKeyboardComponent keyboardComponent;

	int keyboardHeight = 80;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
