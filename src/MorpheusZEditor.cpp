#include <juce_core/juce_core.h>
#include "MorpheusZProcessor.h"
#include "MorpheusZEditor.h"
#include "Stylesheet.h"
#include "StylesStore.h"
#include "BinaryData.h"

MorpheusZEditor::MorpheusZEditor(
    MorpheusZProcessor& p,
    juce::AudioProcessorValueTreeState& apvts,
    const std::vector<juce::AudioSampleBuffer>& waveforms,
    ValueMonitor<double>& monitorMorphPosition)
    : AudioProcessorEditor(&p),
      processorRef(p),
      waveforms(waveforms)
{
    initStylesStore();
    initBinaries();
    initWaveformWidget(apvts, monitorMorphPosition);
    initEnvelopeWidget(apvts);
    initKeyboardComponent(processorRef.keyboardState);
    initBounds();
}

MorpheusZEditor::~MorpheusZEditor()
{
    setLookAndFeel(nullptr);
}

void MorpheusZEditor::initStylesStore()
{
    stylesStore.fillStore(
        Stylesheet::styleColours,
        Stylesheet::styleNumbers,
        Stylesheet::lookAndFeel,
        lookAndFeel);
    setLookAndFeel(&lookAndFeel);
}

void MorpheusZEditor::initBinaries()
{
    int binarySize = 0;
    auto binaryData =
        BinaryData::getNamedResource("background", binarySize);
    backgroundImage = std::make_unique<juce::Image>(
        juce::ImageFileFormat::loadFrom(binaryData, binarySize)
    );
    jassert(backgroundImage->isValid());
}

void MorpheusZEditor::initWaveformWidget(
    juce::AudioProcessorValueTreeState& apvts,
    ValueMonitor<double>& monitorMorphPosition)
{
    waveformWidget = std::make_unique<WaveformWidget>(
        apvts, stylesStore, waveforms, monitorMorphPosition,
        [this](const juce::Point<int>& from, const juce::Point<int>& to, const int waveformNum)
        {
            this->handleWaveformDraw(
                waveformNum,
                waveformWidget.get(),
                from,
                to,
                &MorpheusZProcessor::setWaveformValue);
        },
        [this](WaveformPreset preset, int waveformNum)
        {
            this->processorRef.setWaveform(waveformNum, preset);
        });
    addAndMakeVisible(*waveformWidget);
}

void MorpheusZEditor::setWaveformWidgetBounds(const int topY) const
{
    waveformWidget->setBounds(
        static_cast<int>(
            stylesStore.getNumber(StylesStore::NumberIds::LayoutMargin)),
        topY,
        waveformWidget->getPreferredWidth(),
        waveformWidget->getPreferredHeight());
}

void MorpheusZEditor::initEnvelopeWidget(juce::AudioProcessorValueTreeState& apvts)
{
    envelopeWidget = std::make_unique<EnvelopeWidget>(apvts, stylesStore);
    addAndMakeVisible(*envelopeWidget);
}

void MorpheusZEditor::setEnvelopeWidgetBounds(
    const int topY,
    const int contentWidth) const
{
    envelopeWidget->setBounds(
        static_cast<int>(
            stylesStore.getNumber(StylesStore::NumberIds::LayoutMargin)),
        topY,
        contentWidth,
        envelopeWidget->getPreferredHeight());
}

void MorpheusZEditor::initKeyboardComponent(juce::MidiKeyboardState& keyboardState)
{
    keyboardComponent = std::make_unique<juce::MidiKeyboardComponent>(
        keyboardState,
        juce::MidiKeyboardComponent::horizontalKeyboard);
    addAndMakeVisible(*keyboardComponent);
}

void MorpheusZEditor::setKeyboardComponentBounds(
    const int topY,
    const int windowWidth) const
{
    keyboardComponent->setBounds(
        0,
        topY,
        windowWidth,
        static_cast<int>(
            stylesStore.getNumber(StylesStore::NumberIds::KeyboardHeight)));
}

void MorpheusZEditor::initBounds()
{
    jassert(waveformWidget.get() != nullptr);
    jassert(envelopeWidget.get() != nullptr);
    jassert(keyboardComponent.get() != nullptr);

    const auto layoutMargin = static_cast<int>(
        stylesStore.getNumber(StylesStore::NumberIds::LayoutMargin));
    const auto widgetMargin = static_cast<int>(
        stylesStore.getNumber(StylesStore::NumberIds::WidgetMargin));
    const auto layoutHeaderHeight = static_cast<int>(
        stylesStore.getNumber(StylesStore::NumberIds::LayoutHeaderHeight));

    setWaveformWidgetBounds(layoutHeaderHeight);

    const auto contentWidth = waveformWidget->getBounds().getWidth();
    const auto windowWidth = contentWidth + (layoutMargin * 2);

    setEnvelopeWidgetBounds(
        waveformWidget->getBounds().getBottom() + widgetMargin,
        contentWidth);

    setKeyboardComponentBounds(
        envelopeWidget->getBounds().getBottom() + widgetMargin,
        windowWidth);

    setSize(windowWidth, keyboardComponent->getBounds().getBottom() + 1);
}

void MorpheusZEditor::handleWaveformDraw(
    int waveformNum,
    WaveformWidget* waveform,
    const juce::Point<int>& from,
    const juce::Point<int>& to,
    void (MorpheusZProcessor::*processorCallBack)(int, int, float)) const
{
    jassert(to.x >= from.x);

    const int waveformSize = waveforms[waveformNum].getNumSamples();
    const auto height = static_cast<double>(waveform->getWaveformHeight());
    const auto width = static_cast<double>(waveform->getWaveformWidth());
    const auto fromY = static_cast<double>(from.getY());
    const auto fromX = static_cast<double>(from.getX());
    const auto startIndex = static_cast<int>(std::floor(waveformSize * fromX / width));
    const auto endIndex = static_cast<int>(std::ceil(waveformSize * to.getX() / width));
    double startValue = 1.0 - (fromY / height) * 2.0;
    const auto endValue = 1.0 - (static_cast<double>(to.getY()) / height) * 2.0;
    const auto yStep = (endValue - startValue) / (endIndex - startIndex);
    for (int sampleIndex = startIndex; sampleIndex <= endIndex; ++sampleIndex)
    {
        (processorRef.*processorCallBack)(waveformNum, sampleIndex, static_cast<float>(startValue));
        startValue += yStep;
    }
}

void MorpheusZEditor::paint(juce::Graphics& g)
{
    g.drawImageTransformed(*backgroundImage, backgroundImageHalved);
}

void MorpheusZEditor::waveformChanged(const int waveformNum) const
{
    waveformWidget->waveformChanged(waveformNum);
}