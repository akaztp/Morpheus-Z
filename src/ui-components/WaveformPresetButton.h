#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "StyledComponent.h"

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

protected:
    const juce::Path& iconPath;

private:
    float borderWidth;
    juce::Rectangle<int> bounds;
    juce::Rectangle<int> outlineBounds;
    float cornerRadius;
    juce::Path border;
    juce::Rectangle<float> iconBounds;
    juce::AffineTransform iconOffset;

    void preparePaint();
};
