#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include "../StylesStore.h"
#include "StyledComponent.h"
#include "FramedButton.h"
#include "../AppState.h"


class SideControls :
    public StyledComponent,
    public juce::Component,
    public juce::AudioProcessorParameter::Listener
{
public:
    explicit SideControls(AppState& appState, const StylesStore& stylesStore);

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
    void initParams(AppState& appState);
    void parameterValueChanged(int parameterIndex, float newValue) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override;
};
