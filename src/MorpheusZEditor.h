#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include "MorpheusZProcessor.h"
#include "ui-components/WaveformWidget.h"
#include "Stylesheet.h"
#include "StylesStore.h"


class MorpheusZProcessor;

//==============================================================================
class MorpheusZEditor final : public juce::AudioProcessorEditor
{
public:
	explicit MorpheusZEditor(MorpheusZProcessor&);

	~MorpheusZEditor();

	void paint(juce::Graphics&) override;

	void resized() override;

	void setWaveformA(const juce::AudioSampleBuffer& waveform, double sampleRate);

	void setWaveformB(const juce::AudioSampleBuffer& waveform, double sampleRate);

private:
	const int numWaveforms = 2;
	int waveformASize = 0;
	int waveformBSize = 0;

	juce::LookAndFeel_V4 lookAndFeel;
	StylesStore stylesStore;

	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	MorpheusZProcessor& processorRef;

	juce::AudioFormatManager formatManager;
	juce::AudioThumbnailCache thumbnailCache{ numWaveforms };

	juce::MidiKeyboardComponent keyboardComponent;
	WaveformWidget waveformWidgetA;
	WaveformWidget waveformWidgetB;

	enum WaveformPosition
	{
		Left = 0,
		Right = 1
	};

	void handleWaveformDraw(
			const WaveformWidget& waveform,
			int waveformSize,
			const juce::Point<int>& from,
			const juce::Point<int>& to, // to.x is always equal or bigger than from.x
			void (MorpheusZProcessor::*processorCallBack)(int, float));

	void resizeWaveform(WaveformWidget& waveformWidget, int position);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MorpheusZEditor)
};
