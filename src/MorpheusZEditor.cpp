#include "MorpheusZProcessor.h"
#include <juce_core/juce_core.h>
#include "MorpheusZEditor.h"
#include "Stylesheet.h"
#include "StylesStore.h"

MorpheusZEditor::MorpheusZEditor(MorpheusZProcessor& p, juce::AudioProcessorValueTreeState& apvts)
    : AudioProcessorEditor(&p),
      processorRef(p),
      keyboardComponent(processorRef.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard),
      waveformWidgetA(stylesStore, thumbnailCache, formatManager,
                      [this](const juce::Point<int>& from, const juce::Point<int>& to)
                      {
                          this->handleWaveformDraw(waveformWidgetA, waveformASize, from, to,
                                                   &MorpheusZProcessor::setWaveformAValue);
                      },
                      [this](WaveformPreset preset)
                      {
                          this->processorRef.setWaveformA(preset);
                      }),
      waveformWidgetB(stylesStore, thumbnailCache, formatManager,
                      [this](const juce::Point<int>& from, const juce::Point<int>& to)
                      {
                          this->handleWaveformDraw(waveformWidgetB, waveformBSize, from, to,
                                                   &MorpheusZProcessor::setWaveformBValue);
                      },
                      [this](WaveformPreset preset)
                      {
                          this->processorRef.setWaveformB(preset);
                      })
{
    stylesStore.fillStore(
        Stylesheet::styleColours,
        Stylesheet::styleNumbers,
        lookAndFeel);
    setLookAndFeel(&lookAndFeel);

    addAndMakeVisible(keyboardComponent);
    addAndMakeVisible(waveformWidgetA);
    addAndMakeVisible(waveformWidgetB);

    loopButton.setButtonText("Loop");
    loopButtonAttachment.reset(
        new juce::AudioProcessorValueTreeState::ButtonAttachment(
            apvts,
            AppParams::loopMode,
            loopButton));
    addAndMakeVisible(loopButton);

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

    const auto buttonWidth = stylesStore.getNumber(StylesStore::NumberIds::ButtonWidth);
    const auto layoutMargin = stylesStore.getNumber(StylesStore::NumberIds::LayoutMargin);
    const auto gutter = stylesStore.getNumber(StylesStore::NumberIds::LayoutGutter);
    const auto middlePanelWidth = buttonWidth + 2 * gutter;
    const auto waveformWidgetWidth = 0.5 * (getWidth() - middlePanelWidth - 2 * layoutMargin);
    const auto waveformWidgetALeft = layoutMargin;
    const auto middlePanelLeft = layoutMargin + waveformWidgetWidth;
    const auto waveformWidgetBLeft = middlePanelLeft + middlePanelWidth;
    const int waveformWidgetHeight = getHeight() - keyboardHeight - 2 * layoutMargin;

    waveformWidgetA.setBounds(waveformWidgetALeft, layoutMargin, waveformWidgetWidth, waveformWidgetHeight);
    waveformWidgetB.setBounds(waveformWidgetBLeft, layoutMargin, waveformWidgetWidth, waveformWidgetHeight);
    loopButton.setBounds(
        middlePanelLeft + gutter,
        layoutMargin,
        buttonWidth,
        stylesStore.getNumber(StylesStore::NumberIds::ButtonHeight));
}

void MorpheusZEditor::setWaveformA(
    const juce::AudioSampleBuffer& waveform,
    double sampleRate)
{
    waveformASize = waveform.getNumSamples();
    const auto hash = 0;
    thumbnailCache.removeThumb(hash);
    waveformWidgetA.setSource(waveform, sampleRate, hash);
}

void MorpheusZEditor::setWaveformB(
    const juce::AudioSampleBuffer& waveform,
    double sampleRate)
{
    waveformBSize = waveform.getNumSamples();
    const auto hash = 1;
    thumbnailCache.removeThumb(hash);
    waveformWidgetB.setSource(waveform, sampleRate, hash);
}
