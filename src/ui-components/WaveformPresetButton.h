#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "ButtonFrame.h"

class WaveformPresetButton : public juce::Button, protected StyledComponent
{
public:
    WaveformPresetButton(
        const StylesStore& stylesStore,
        const juce::String& buttonName,
        const juce::Path& iconPath,
        std::function<void()> onClickCallback);

    void paintButton(
        juce::Graphics& g,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown) override;

    void lookAndFeelChanged() override;
    void resized() override;

private:
    const juce::Path& iconPath;
    ButtonFrame buttonFrame;

    juce::Rectangle<int> bounds;
    juce::Rectangle<float> iconBounds;
    juce::AffineTransform iconOffset;

    void preparePaint();
};
