#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include "ui-components/WaveformWidget.h"
#include "Stylesheet.h"
#include "StylesStore.h"
#include "ui-components/EnvelopeWidget.h"
#include "ui-components/SideControls.h"
#include "AppState.h"


class MorpheusZProcessor;

//==============================================================================
class MorpheusZEditor final : public juce::AudioProcessorEditor
{
public:
    explicit MorpheusZEditor(
        MorpheusZProcessor& processor,
        AppState& appState);

    ~MorpheusZEditor() override;

    void paint(juce::Graphics&) override;

    void waveformChanged(int waveformNum) const;

private:
    AppState& appState;
    juce::LookAndFeel_V4 lookAndFeel;
    StylesStore stylesStore;

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
    void initBinaries();
    void initSideControls(AppState& appState);
    void setSideControlsBounds(int left, int top) const;
    void initWaveformWidget(AppState& appState);
    void setWaveformWidgetBounds(int topY) const;
    void initEnvelopeWidget(AppState& appState);
    void setEnvelopeWidgetBounds(int topY, int contentWidth) const;
    void initKeyboard(AppState& appState);
    void setKeyboardComponentBounds(int topY, int windowWidth) const;
    void initBounds();

    void setPresetWaveform(int waveformNum, WaveformPreset::Type preset) const;
    void setWaveformValue(int waveformNum, int index, float value) const;
    void handleWaveformDraw(
        int waveformNum,
        const WaveformWidget* waveform,
        const juce::Point<int>& former,
        const juce::Point<int>& later,
        AppState& appState);
    void handleWaveformCartDraw(
        int waveformNum,
        const WaveformWidget* widget,
        const juce::Point<int>& former,
        const juce::Point<int>& later);
    void handleWaveformPolarDraw(
        int waveformNum,
        const WaveformWidget* widget,
        const juce::Point<int>& former,
        const juce::Point<int>& later);
    static float trimSample(float value);
    static juce::Point<double> cartToPolar(
        juce::Point<int> pos,
        juce::Point<int> center,
        double radius);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MorpheusZEditor)
};
