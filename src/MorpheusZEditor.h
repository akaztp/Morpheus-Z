#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include "ui-components/WaveformWidget.h"
#include "Stylesheet.h"
#include "StylesStore.h"
#include "ui-components/EnvelopeWidget.h"

class MorpheusZProcessor;

//==============================================================================
class MorpheusZEditor final : public juce::AudioProcessorEditor
{
public:
    explicit MorpheusZEditor(MorpheusZProcessor&, juce::AudioProcessorValueTreeState&);

    ~MorpheusZEditor() override;

    void paint(juce::Graphics&) override;

    void setWaveform(int waveformNum, const juce::AudioSampleBuffer& waveform, double sampleRate);

private:
    static constexpr int numWaveforms = 2;
    int waveformSizes[numWaveforms] = {0, 0};

    juce::LookAndFeel_V4 lookAndFeel;
    StylesStore stylesStore;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MorpheusZProcessor& processorRef;

    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbnailCache{numWaveforms};

    std::unique_ptr<juce::Image> backgroundImage;
    const juce::AffineTransform backgroundImageHalved = juce::AffineTransform::scale(0.5f, 0.5f);

    std::unique_ptr<juce::MidiKeyboardComponent> keyboardComponent;
    std::unique_ptr<WaveformWidget> waveformWidget;

    std::unique_ptr<EnvelopeWidget> envelopeWidget;

    void initStylesStore();
    void initBinaries();
    void initWaveformWidget(juce::AudioProcessorValueTreeState&);
    void setWaveformWidgetBounds(int topY) const;
    void initEnvelopeWidget(juce::AudioProcessorValueTreeState&);
    void setEnvelopeWidgetBounds(int topY, int contentWidth) const;
    void initKeyboardComponent(juce::MidiKeyboardState& keyboardState);
    void setKeyboardComponentBounds(int topY, int windowWidth) const;
    void initBounds();

    void handleWaveformDraw(
        int waveformNum,
        WaveformWidget* waveform,
        const juce::Point<int>& from,
        const juce::Point<int>& to, // to.x is always equal or bigger than from.x
        void (MorpheusZProcessor::*processorCallBack)(int, int, float)) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MorpheusZEditor)
};
