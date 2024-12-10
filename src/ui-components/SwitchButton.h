#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "ButtonFrame.h"

class SwitchButton : public juce::ToggleButton
{
public:
    explicit SwitchButton(const StylesStore& stylesStore);

    void lookAndFeelChanged() override;
    void resized() override;

protected:
    ButtonFrame buttonFrame;

    void paintButton(
        juce::Graphics& g,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown) override;
};
