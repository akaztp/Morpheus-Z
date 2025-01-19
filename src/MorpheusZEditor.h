#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include "ui-components/WaveformWidget.h"
#include "Stylesheet.h"
#include "StylesStore.h"
#include "ui-components/EnvelopeWidget.h"
#include "ui-components/SideControls.h"


class MorpheusZProcessor;

//==============================================================================
class MorpheusZEditor final : public juce::AudioProcessorEditor
{
public:
    explicit MorpheusZEditor(
        MorpheusZProcessor&,
        juce::AudioProcessorValueTreeState&,
        const std::vector<juce::AudioSampleBuffer>& waveforms,
        ValueMonitor<double>& monitorMorphPosition);

    ~MorpheusZEditor() override;

    void paint(juce::Graphics&) override;

    void waveformChanged(int waveformNum) const;

private:
    const std::vector<juce::AudioSampleBuffer>& waveforms;
    juce::LookAndFeel_V4 lookAndFeel;
    StylesStore stylesStore;
    juce::AudioParameterChoice* waveformDisplayModeParam = nullptr;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MorpheusZProcessor& processorRef;

    std::unique_ptr<juce::Image> backgroundImage;
    const juce::AffineTransform backgroundImageHalved = juce::AffineTransform::scale(0.5f, 0.5f);

    std::unique_ptr<juce::MidiKeyboardComponent> keyboardComponent;
    std::unique_ptr<WaveformWidget> waveformWidget;

    std::unique_ptr<EnvelopeWidget> envelopeWidget;

    std::unique_ptr<SideControls> sideControls;

    void initStylesStore();
    void initParams(juce::AudioProcessorValueTreeState& apvts);
    void initBinaries();
    void initSideControls(juce::AudioProcessorValueTreeState& apvts);
    void setSideControlsBounds(int left, int top) const;
    void initWaveformWidget(
        juce::AudioProcessorValueTreeState&,
        ValueMonitor<double>& monitorMorphPosition);
    void setWaveformWidgetBounds(int topY) const;
    void initEnvelopeWidget(juce::AudioProcessorValueTreeState&);
    void setEnvelopeWidgetBounds(int topY, int contentWidth) const;
    void initKeyboardComponent(juce::MidiKeyboardState& keyboardState);
    void setKeyboardComponentBounds(int topY, int windowWidth) const;
    void initBounds();

    void handleWaveformDraw(
        int waveformNum,
        const WaveformWidget* waveform,
        const juce::Point<int>& former,
        const juce::Point<int>& later,
        void (MorpheusZProcessor::*processorCallBack)(int, int, float)) const;
    void handleWaveformCartDraw(
        int waveformNum,
        const WaveformWidget* widget,
        const juce::Point<int>& former,
        const juce::Point<int>& later,
        void (MorpheusZProcessor::*processorCallBack)(int, int, float)) const;
    void handleWaveformPolarDraw(
        int waveformNum,
        const WaveformWidget* widget,
        const juce::Point<int>& former,
        const juce::Point<int>& later,
        void (MorpheusZProcessor::*processorCallBack)(int, int, float)) const;
    static float trimSample(float value);
    static juce::Point<double> cartToPolar(
        juce::Point<int> pos,
        juce::Point<int> center,
        double radius);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MorpheusZEditor)
};
