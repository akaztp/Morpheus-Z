#include "MorpheusZProcessor.h"
#include <juce_core/juce_core.h>
#include "MorpheusZEditor.h"
#include "Stylesheet.h"
#include "StylesStore.h"

MorpheusZEditor::MorpheusZEditor(MorpheusZProcessor& p)
		: AudioProcessorEditor(&p),
		  processorRef(p),
		  keyboardComponent(processorRef.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard),
		  waveformWithControlsUIA(stylesStore, thumbnailCache, formatManager,
				  [this](const juce::Point<int>& from, const juce::Point<int>& to)
				  {
					  this->handleWaveformDraw(waveformWithControlsUIA, waveformASize, from, to,
							  &MorpheusZProcessor::setWaveformAValue);
				  }),
		  waveformWithControlsUIB(stylesStore, thumbnailCache, formatManager,
				  [this](const juce::Point<int>& from, const juce::Point<int>& to)
				  {
					  this->handleWaveformDraw(waveformWithControlsUIB, waveformBSize, from, to,
							  &MorpheusZProcessor::setWaveformBValue);
				  })
{
	stylesStore.fillStore(
			Stylesheet::styleColours,
			Stylesheet::styleNumbers,
			lookAndFeel);
	setLookAndFeel(&lookAndFeel);

	addAndMakeVisible(keyboardComponent);
	addAndMakeVisible(waveformWithControlsUIA);
	addAndMakeVisible(waveformWithControlsUIB);
	setSize(600, 250);
}

MorpheusZEditor::~MorpheusZEditor()
{
	setLookAndFeel(nullptr);
}

void MorpheusZEditor::handleWaveformDraw(
		const WaveformWidget& waveform,
		int waveformSize,
		const juce::Point<int>& from,
		const juce::Point<int>& to,
		void (MorpheusZProcessor::*processorCallBack)(int, float))
{
	jassert(to.x >= from.x);

	const auto height = static_cast<double>(waveform.getWaveformHeight());
	const auto width = static_cast<double>(waveform.getWaveformWidth());
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
	const int keyboardHeight =
			stylesStore.getNumber(StylesStore::NumberIds::KeyboardHeight);
	keyboardComponent.setBounds(
			0,
			getHeight() - keyboardHeight,
			getWidth(),
			keyboardHeight);
	resizeWaveform(waveformWithControlsUIA, MorpheusZEditor::WaveformPosition::Left);
	resizeWaveform(waveformWithControlsUIB, MorpheusZEditor::WaveformPosition::Right);
}

void MorpheusZEditor::resizeWaveform(
		WaveformWidget& waveformWithControlsUI,
		int position)
{
	const int layoutMargin = stylesStore.getNumber(StylesStore::NumberIds::LayoutMargin);
	const int layoutGutter = stylesStore.getNumber(StylesStore::NumberIds::LayoutGutter);
	const int keyboardHeight = stylesStore.getNumber(StylesStore::NumberIds::KeyboardHeight);
	const auto width = (getWidth() - layoutMargin * 2 - layoutGutter) / 2;
	const auto left = layoutMargin + position * (width + layoutGutter);
	const int height = getHeight() - keyboardHeight - 2 * layoutMargin;
	waveformWithControlsUI.setBounds(left, layoutMargin, width, height);
}

void MorpheusZEditor::setWaveformA(
		const juce::AudioSampleBuffer& waveform,
		double sampleRate)
{
	waveformASize = waveform.getNumSamples();
	const auto hash = 0;
	thumbnailCache.removeThumb(hash);
	waveformWithControlsUIA.setSource(waveform, sampleRate, hash);
}

void MorpheusZEditor::setWaveformB(
		const juce::AudioSampleBuffer& waveform,
		double sampleRate)
{
	waveformBSize = waveform.getNumSamples();
	const auto hash = 1;
	thumbnailCache.removeThumb(hash);
	waveformWithControlsUIB.setSource(waveform, sampleRate, hash);
}
