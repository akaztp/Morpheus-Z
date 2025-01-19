#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include "../StylesStore.h"
#include "StyledComponent.h"
#include "FramedButton.h"

class SideControls :
    public StyledComponent,
    public juce::Component,
    public juce::AudioProcessorParameter::Listener
{
public:
    SideControls(
        juce::AudioProcessorValueTreeState& apvts,
        const StylesStore& stylesStore);

    ~SideControls() override;

    void resized() override;
    void selectDisplayMode(int displayMode) const;
    void updateButtonsState() const;

    int getPreferredWidth() const;
    int getPreferredHeight() const;

private:
    std::unique_ptr<juce::Path> cartesianButtonIcon;
    std::unique_ptr<FramedButton> cartesianButton;

    std::unique_ptr<juce::Path> polarButtonIcon;
    std::unique_ptr<FramedButton> polarButton;

    juce::AudioParameterChoice* waveformDisplayModeParam = nullptr;

    void initDisplayModeSelectors();
    void initParams(juce::AudioProcessorValueTreeState& apvts);
    void parameterValueChanged(int parameterIndex, float newValue) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override;
};
