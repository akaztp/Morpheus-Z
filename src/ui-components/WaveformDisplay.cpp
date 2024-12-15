#include <juce_audio_utils/juce_audio_utils.h>
#include "WaveformDisplay.h"
#include "../StylesStore.h"

WaveformDisplay::WaveformDisplay(
    const StylesStore& stylesStore,
    juce::AudioThumbnailCache& thumbnailCache,
    juce::AudioFormatManager& formatManager)
    : StyledComponent(stylesStore),
      thumbnail(1, formatManager, thumbnailCache)
{
    thumbnail.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
    thumbnail.removeChangeListener(this);
}

void WaveformDisplay::setSource(
    const juce::AudioBuffer<float>& newSource,
    const double sampleRate,
    const juce::int64 hashCode)
{
    thumbnail.setSource(&newSource, sampleRate, hashCode);
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    juce::ignoreUnused(source);
    repaint();
}

void WaveformDisplay::paint(juce::Graphics& g)
{
    const auto width = getWidth();
    const auto height = getHeight();
    const juce::Rectangle<int> thumbnailBounds(0, 0, width, height);

    g.setColour(stylesStore
                      .getColor(waveformColorId));
    thumbnail.drawChannel(g,
                          thumbnailBounds,
                          0.0,
                          thumbnail.getTotalLength(),
                          0,
                          1.0f);
}
