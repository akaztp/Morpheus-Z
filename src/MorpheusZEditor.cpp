#include <juce_core/juce_core.h>
#include "MorpheusZProcessor.h"
#include "MorpheusZEditor.h"
#include "Stylesheet.h"
#include "StylesStore.h"
#include "BinaryData.h"

MorpheusZEditor::MorpheusZEditor(
    MorpheusZProcessor& p,
    juce::AudioProcessorValueTreeState& apvts)
    : AudioProcessorEditor(&p),
      processorRef(p)
{
    initStylesStore();
    initBinaries();
    initKeyboardComponent(processorRef.keyboardState);
    initWaveformWidget(apvts);
    initSize();
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

void MorpheusZEditor::initKeyboardComponent(juce::MidiKeyboardState& keyboardState)
{
    keyboardComponent = std::make_unique<juce::MidiKeyboardComponent>(
        keyboardState,
        juce::MidiKeyboardComponent::horizontalKeyboard);
    addAndMakeVisible(*keyboardComponent);
}

void MorpheusZEditor::initWaveformWidget(juce::AudioProcessorValueTreeState& apvts)
{
    waveformWidget = std::make_unique<WaveformWidget>(
        apvts, stylesStore, thumbnailCache, formatManager,
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

void MorpheusZEditor::initSize()
{
    auto waveformWidgetBounds = calculateWaveformWidgetBounds();
    const auto keyboardHeight =
        stylesStore.getNumber(StylesStore::NumberIds::KeyboardHeight);
    const auto widgetMargin = stylesStore.getNumber(StylesStore::NumberIds::WidgetMargin);
    const auto width = waveformWidgetBounds.getWidth() + 2 * waveformWidgetBounds.getX();
    const auto height = waveformWidgetBounds.getY()
        + waveformWidgetBounds.getHeight()
        + widgetMargin
        + keyboardHeight;
    setSize(width, height);
}

void MorpheusZEditor::handleWaveformDraw(
    int waveformNum,
    WaveformWidget* waveform,
    const juce::Point<int>& from,
    const juce::Point<int>& to,
    void (MorpheusZProcessor::*processorCallBack)(int, int, float)) const
{
    jassert(to.x >= from.x);

    const int waveformSize = waveformSizes[waveformNum];
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

void MorpheusZEditor::resized()
{
    const int keyboardHeight =
        stylesStore.getNumber(StylesStore::NumberIds::KeyboardHeight);
    keyboardComponent->setBounds(
        0,
        getHeight() - keyboardHeight,
        getWidth(),
        keyboardHeight);

    waveformWidget->setBounds(calculateWaveformWidgetBounds());
}

juce::Rectangle<int> MorpheusZEditor::calculateWaveformWidgetBounds() const
{
    return juce::Rectangle<int>(
        stylesStore.getNumber(StylesStore::NumberIds::LayoutMargin),
        stylesStore.getNumber(StylesStore::NumberIds::LayoutHeaderHeight),
        waveformWidget->getPreferredWidth(),
        waveformWidget->getPreferredHeight());
}

void MorpheusZEditor::setWaveform(
    const int waveformNum,
    const juce::AudioSampleBuffer& waveform,
    double sampleRate)
{
    waveformSizes[waveformNum] = waveform.getNumSamples();
    const auto hash = waveformNum;
    thumbnailCache.removeThumb(hash);
    waveformWidget->setSource(waveformNum, waveform, sampleRate, hash);
}
