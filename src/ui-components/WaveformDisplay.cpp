#include <juce_audio_utils/juce_audio_utils.h>
#include "WaveformDisplay.h"
#include "../StylesStore.h"

WaveformDisplay::WaveformDisplay(
    const StylesStore& stylesStore,
    const juce::AudioSampleBuffer& waveform)
    : StyledComponent(stylesStore),
      waveform(waveform)
{
}

WaveformDisplay::~WaveformDisplay()
{
}


void WaveformDisplay::paint(juce::Graphics& g)
{
    if (isVisible())
    {
        const auto width = getWidth();
        const auto height = getHeight();

        g.setColour(stylesStore.getColor(waveformColorId));
        paintWaveform(g, width, height);
    }
}


void WaveformDisplay::paintWaveform(
    juce::Graphics& g,
    const int width,
    const int height) const
{
    const auto halfHeight = height / 2;
    const auto numSamples = waveform.getNumSamples();
    juce::Path path;
    path.startNewSubPath(
        0,
        halfHeight - waveform.getSample(0, 0) * halfHeight);
    for (int i = 1; i < numSamples; ++i)
    {
        path.lineTo(
            width * i / numSamples,
            halfHeight - waveform.getSample(0, i) * halfHeight);
    }
    g.strokePath(
        path,
        juce::PathStrokeType(
            stylesStore.getNumber(StylesStore::NumberIds::waveformThickness)));
}
