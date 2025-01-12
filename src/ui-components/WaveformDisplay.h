#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include "../StylesStore.h"
#include "StyledComponent.h"

class WaveformDisplay :
    public StyledComponent,
    public juce::Component
{
public:
    WaveformDisplay(
        const StylesStore& stylesStore,
        const juce::AudioSampleBuffer& waveform);

    ~WaveformDisplay() override;

    void paint(juce::Graphics&) override;

    void setWaveFormColorId(const StylesStore::ColorIds colorId)
    {
        waveformColorId = colorId;
        repaint();
    }

private:
    const juce::AudioSampleBuffer& waveform;
    StylesStore::ColorIds waveformColorId;

    void paintWaveform(juce::Graphics& g, int width, int height) const;
};
