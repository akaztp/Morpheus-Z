#include <juce_audio_utils/juce_audio_utils.h>
#include "WaveformDisplay.h"
#include "../StylesStore.h"
#include "../AppParams.h"


WaveformDisplay::WaveformDisplay(
    const StylesStore& stylesStore,
    juce::AudioProcessorValueTreeState& apvts,
    const juce::AudioSampleBuffer& waveform)
    : StyledComponent(stylesStore),
      waveform(waveform)
{
    initParams(apvts);
}

WaveformDisplay::~WaveformDisplay()
{
    waveformDisplayModeParam->removeListener(this);
}

void WaveformDisplay::initParams(juce::AudioProcessorValueTreeState& apvts)
{
    waveformDisplayModeParam = dynamic_cast<juce::AudioParameterChoice*>(
        apvts.getParameter(AppParams::waveformDisplayMode));
    waveformDisplayModeParam->addListener(this);
}

void WaveformDisplay::paint(juce::Graphics& g)
{
    if (isVisible())
    {
        const auto width = getWidth();
        const auto height = getHeight();
        g.setColour(stylesStore.getColor(waveformColorId));
        switch (waveformDisplayModeParam->getIndex())
        {
        case WaveformDisplayMode::Cartesian:
            paintCartesian(g, width, height);
            break;
        case WaveformDisplayMode::Polar:
            paintPolar(g, width, height);
            break;
        default:
            jassert(false);
        }
    }
}

void WaveformDisplay::paintCartesian(
    juce::Graphics& g,
    const int width,
    const int height) const
{
    const float halfHeight = static_cast<float>(height) / 2.0f;
    const auto numSamples = waveform.getNumSamples();
    juce::Path path;
    path.startNewSubPath(
        0,
        halfHeight - waveform.getSample(0, 0) * halfHeight);
    for (int i = 1; i < numSamples; ++i)
    {
        path.lineTo(
            static_cast<float>(width) * i / numSamples,
            halfHeight - waveform.getSample(0, i) * halfHeight);
    }
    g.strokePath(
        path,
        juce::PathStrokeType(
            stylesStore.getNumber(StylesStore::NumberIds::waveformThickness)));
}

void WaveformDisplay::paintPolar(
    juce::Graphics& g,
    const int width,
    const int height) const
{
    const double centerX = static_cast<float>(width) / 2.0f;
    const double centerY = static_cast<float>(height) / 2.0f;
    const double radius = std::min(centerX, centerY) / 2.0f;
    const auto numSamples = waveform.getNumSamples();
    const auto step = juce::MathConstants<double>::twoPi / numSamples;
    const auto samples = waveform.getReadPointer(0);
    juce::Path path;
    path.startNewSubPath(
        static_cast<float>(centerX - 1 * radius * (samples[0] + 1.0)),
        static_cast<float>(centerY));
    for (double angle = step; angle < juce::MathConstants<double>::twoPi; angle += step)
    {
        const auto sample =  1.0f + samples[static_cast<int>(
            numSamples * angle / juce::MathConstants<double>::twoPi)];
        path.lineTo(
            static_cast<float>(centerX - std::cos(angle) * radius * sample),
            static_cast<float>(centerY - std::sin(angle) * radius * sample));
    }
    path.closeSubPath();
    g.strokePath(
        path,
        juce::PathStrokeType(
            stylesStore.getNumber(StylesStore::NumberIds::waveformThickness)));
}

void WaveformDisplay::parameterValueChanged(int parameterIndex, float newValue)
{
    juce::ignoreUnused(parameterIndex, newValue);
    repaint();
}

void WaveformDisplay::parameterGestureChanged(int parameterIndex, bool gestureIsStarting)
{
    juce::ignoreUnused(parameterIndex, gestureIsStarting);
}
