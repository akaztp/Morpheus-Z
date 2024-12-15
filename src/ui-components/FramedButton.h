#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "ButtonFrame.h"

class FramedButton : public juce::Button, protected StyledComponent
{
public:
    FramedButton(
        const StylesStore& stylesStore,
        const juce::String& buttonName,
        const juce::Path* iconPath,
        std::function<void()> onClickCallback);

    void lookAndFeelChanged() override;
    void resized() override;

protected:
    void paintButton(
        juce::Graphics& g,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown) override;

private:
    ButtonFrame buttonFrame;
    const juce::Path* iconPath = nullptr;

    juce::Rectangle<int> bounds;
    juce::Rectangle<float> iconBounds;
    juce::AffineTransform iconOffset;
    float fontSize = 0.0f;

    void preparePaint();
};
