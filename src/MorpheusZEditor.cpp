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
    initParams(apvts);
    initBinaries();
    initWaveformWidget(apvts, monitorMorphPosition);
    initSideControls(apvts);
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

void MorpheusZEditor::initParams(juce::AudioProcessorValueTreeState& apvts)
{
    waveformDisplayModeParam = dynamic_cast<juce::AudioParameterChoice*>(
        apvts.getParameter(AppParams::waveformDisplayMode));
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

void MorpheusZEditor::initSideControls(juce::AudioProcessorValueTreeState& apvts)
{
    sideControls = std::make_unique<SideControls>(apvts, stylesStore);
    addAndMakeVisible(*sideControls);
}

void MorpheusZEditor::setSideControlsBounds(const int left, const int top) const
{
    sideControls->setBounds(
        left,
        top,
        sideControls->getPreferredWidth(),
        sideControls->getPreferredHeight());
}

void MorpheusZEditor::initWaveformWidget(
    juce::AudioProcessorValueTreeState& apvts,
    ValueMonitor<double>& monitorMorphPosition)
{
    waveformWidget = std::make_unique<WaveformWidget>(
        apvts, stylesStore, waveforms, monitorMorphPosition,
        [this](const juce::Point<int>& former, const juce::Point<int>& later, const int waveformNum)
        {
            this->handleWaveformDraw(
                waveformNum,
                waveformWidget.get(),
                former,
                later,
                &MorpheusZProcessor::setWaveformValue);
        },
        [this](WaveformPreset preset, int waveformNum)
        {
            this->processorRef.setWaveform(waveformNum, preset);
        }
    );
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
    const auto layoutGutter = static_cast<int>(
        stylesStore.getNumber(StylesStore::NumberIds::LayoutGutter));
    const auto layoutHeaderHeight = static_cast<int>(
        stylesStore.getNumber(StylesStore::NumberIds::LayoutHeaderHeight));

    setWaveformWidgetBounds(layoutHeaderHeight);
    setSideControlsBounds(
        waveformWidget->getBounds().getRight() + layoutGutter,
        layoutHeaderHeight);

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
    const int waveformNum,
    const WaveformWidget* waveform,
    const juce::Point<int>& former,
    const juce::Point<int>& later,
    void (MorpheusZProcessor::*processorCallBack)(int, int, float)) const
{
    switch (waveformDisplayModeParam->getIndex())
    {
    case WaveformDisplayMode::Cartesian:
        handleWaveformCartDraw(waveformNum, waveform, former, later, processorCallBack);
        break;
    case WaveformDisplayMode::Polar:
        handleWaveformPolarDraw(waveformNum, waveform, former, later, processorCallBack);
        break;
    default:
        jassert(false);
    }
}

void MorpheusZEditor::handleWaveformCartDraw(
    const int waveformNum,
    const WaveformWidget* widget,
    const juce::Point<int>& former,
    const juce::Point<int>& later,
    void (MorpheusZProcessor::*processorCallBack)(int, int, float)) const
{
    juce::Point<int> from = former;
    juce::Point<int> to = later;
    if (to.x < from.x)
    {
        from = later;
        to = former;
    }

    const int numSamples = waveforms[waveformNum].getNumSamples();
    const auto height = static_cast<double>(widget->getWaveformHeight());
    const auto width = static_cast<double>(widget->getWaveformWidth());
    const auto fromY = static_cast<double>(from.getY());
    const auto fromX = static_cast<double>(from.getX());
    const auto startIndex = static_cast<int>(std::floor(numSamples * fromX / width));
    const auto endIndex = static_cast<int>(std::ceil(numSamples * to.getX() / width));
    double startValue = 1.0 - (fromY / height) * 2.0;
    const double endValue = 1.0 - (to.getY() / height) * 2.0;
    const auto yStep = (endValue - startValue) / (endIndex - startIndex);
    for (int sampleIndex = startIndex; sampleIndex <= endIndex; ++sampleIndex)
    {
        (processorRef.*processorCallBack)(waveformNum, sampleIndex, static_cast<float>(startValue));
        startValue += yStep;
    }
}

void MorpheusZEditor::handleWaveformPolarDraw(
    const int waveformNum,
    const WaveformWidget* widget,
    const juce::Point<int>& former,
    const juce::Point<int>& later,
    void (MorpheusZProcessor::*processorCallBack)(int, int, float)) const
{
    const auto height = widget->getWaveformHeight();
    const auto width = widget->getWaveformWidth();
    const auto center = juce::Point<int>(
        static_cast<int>(width / 2.0),
        static_cast<int>(height / 2.0));
    const double radius = std::min(center.x, center.y);
    auto fromPolar = cartToPolar(former, center, radius);
    auto toPolar = cartToPolar(later, center, radius);
    if (fromPolar.y - toPolar.y > juce::MathConstants<double>::pi)
    {
        toPolar.y += juce::MathConstants<double>::twoPi;
    }
    else if (toPolar.y - fromPolar.y > juce::MathConstants<double>::pi)
    {
        fromPolar.y += juce::MathConstants<double>::twoPi;
    }

    const int numSamples = waveforms[waveformNum].getNumSamples();
    int sampleFrom = static_cast<int>(numSamples * fromPolar.y / juce::MathConstants<double>::twoPi);
    const int sampleTo = static_cast<int>(numSamples * toPolar.y / juce::MathConstants<double>::twoPi);
    const double valueStep = (toPolar.x - fromPolar.x) / (sampleTo - sampleFrom);
    if (sampleFrom <= sampleTo)
    {
        for (; sampleFrom <= sampleTo; ++sampleFrom)
        {
            (processorRef.*processorCallBack)(
                waveformNum,
                sampleFrom % numSamples,
                trimSample(static_cast<float>(fromPolar.x)));
            fromPolar.x += valueStep;
        }
    }
    else
    {
        for (; sampleFrom > sampleTo; --sampleFrom)
        {
            (processorRef.*processorCallBack)(
                waveformNum,
                sampleFrom % numSamples,
                trimSample(static_cast<float>(fromPolar.x)));
            fromPolar.x -= valueStep;
        }
    }
}

float MorpheusZEditor::trimSample(const float value)
{
    return value > 1.0f ? 1.0f : value < -1.0f ? -1.0f : value;
}

juce::Point<double> MorpheusZEditor::cartToPolar(
    const juce::Point<int> pos,
    const juce::Point<int> center,
    const double radius)
{
    const double distanceToCenterFrom = std::sqrt(
        (pos.getX() - center.getX()) * (pos.getX() - center.getX())
        + (pos.getY() - center.getY()) * (pos.getY() - center.getY()));
    const double value = (distanceToCenterFrom / radius) * 2.0 - 1.0;
    const double angle = std::atan2(
        center.getY() - pos.getY(),
        center.getX() - pos.getX());
    const juce::Point<double> polarPoint(
        value,
        angle >= 0.0 ? angle : angle + juce::MathConstants<double>::twoPi);
    return polarPoint;
}

void MorpheusZEditor::paint(juce::Graphics& g)
{
    g.drawImageTransformed(*backgroundImage, backgroundImageHalved);
}

void MorpheusZEditor::waveformChanged(const int waveformNum) const
{
    waveformWidget->waveformChanged(waveformNum);
}
