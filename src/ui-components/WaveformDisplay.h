#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include "../StylesStore.h"
#include "StyledComponent.h"

class WaveformDisplay :
    public StyledComponent,
    public juce::Component,
    juce::ChangeListener
{
public:
    WaveformDisplay(
        const StylesStore& stylesStore,
        juce::AudioThumbnailCache& thumbnailCache,
        juce::AudioFormatManager& formatManager);

    ~WaveformDisplay() override;

    void paint(juce::Graphics&) override;

    void setSource(const juce::AudioBuffer<float>& newSource, double sampleRate, juce::int64 hashCode);

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    void setWaveFormColorId(const StylesStore::ColorIds colorId)
    {
        waveformColorId = colorId;
        repaint();
    }

private:
    StylesStore::ColorIds waveformColorId;
    juce::AudioThumbnail thumbnail;
};
