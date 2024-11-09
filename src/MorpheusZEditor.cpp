#include "MorpheusZProcessor.h"
#include "MorpheusZEditor.h"
#include <juce_core/juce_core.h>

MorpheusZEditor::MorpheusZEditor(MorpheusZProcessor& p)
		: AudioProcessorEditor(&p),
		  processorRef(p),
		  keyboardComponent(processorRef.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard),
		  waveformAUI(thumbnailCache, formatManager,
				  std::move([this](const juce::Point<int>& from, const juce::Point<int>& to)
				  {
					  this->handleWaveformDraw(waveformAUI, waveformASize, from, to,
							  &MorpheusZProcessor::setWaveformAValue);
				  })),
		  waveformBUI(thumbnailCache, formatManager,
				  [this](const juce::Point<int>& from, const juce::Point<int>& to)
				  {
					  this->handleWaveformDraw(waveformBUI, waveformBSize, from, to,
							  &MorpheusZProcessor::setWaveformBValue);
				  })
{
	addAndMakeVisible(keyboardComponent);
	addAndMakeVisible(waveformAUI);
	addAndMakeVisible(waveformBUI);
	setSize(600, 250);
}

void MorpheusZEditor::handleWaveformDraw(
		const WaveformUI& waveform,
		int waveformSize,
		const juce::Point<int>& from,
		const juce::Point<int>& to,
		void (MorpheusZProcessor::*processorCallBack)(int, float))
{
	jassert(to.x >= from.x);

	const auto height = static_cast<double>(waveform.getHeight());
	const auto width = static_cast<double>(waveform.getWidth());
	const auto fromY = static_cast<double>(from.getY());
	const auto fromX = static_cast<double>(from.getX());
	const auto startIndex = static_cast<int>(std::floor(waveformSize * fromX / width));
	const auto endIndex = static_cast<int>(std::ceil(waveformSize * to.getX() / width));
	double startValue = 1.0 - (fromY / height) * 2.0;
	const auto endValue = 1.0 - (static_cast<double>(to.getY()) / height) * 2.0;
	const auto yStep = (endValue - startValue) / (endIndex - startIndex);
	for (int sampleIndex = startIndex; sampleIndex <= endIndex; ++sampleIndex)
	{
		(processorRef.*processorCallBack)(sampleIndex, static_cast<float>(startValue));
		startValue += yStep;
	}
}

void MorpheusZEditor::paint(juce::Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MorpheusZEditor::resized()
{
	keyboardComponent.setBounds(0, getHeight() - keyboardHeight, getWidth(), keyboardHeight);
	resizeWaveform(waveformAUI, 0);
	resizeWaveform(waveformBUI, 1);
}

void MorpheusZEditor::resizeWaveform(WaveformUI& waveformUI, int position)
{
	auto width = (getWidth() - margin * 3) / 2;
	auto left = margin + position * width + margin * position;
	int height = getHeight() - keyboardHeight - 2 * margin;
	waveformUI.setBounds(left, margin, width, height);
}

void MorpheusZEditor::setWaveformA(
		const juce::AudioSampleBuffer& waveform,
		double sampleRate)
{
	waveformASize = waveform.getNumSamples();
	const auto hash = 0;
	thumbnailCache.removeThumb(hash);
	waveformAUI.setSource(waveform, sampleRate, hash);
}

void MorpheusZEditor::setWaveformB(
		const juce::AudioSampleBuffer& waveform,
		double sampleRate)
{
	waveformBSize = waveform.getNumSamples();
	const auto hash = 1;
	thumbnailCache.removeThumb(hash);
	waveformBUI.setSource(waveform, sampleRate, hash);
}
