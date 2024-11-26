#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "StyledComponent.h"


class WaveformPresetButton : public juce::Button, protected StyledComponent
{
public:
    WaveformPresetButton(
        const StylesStore& stylesStore,
        const juce::String& buttonName,
        const juce::Path& iconPath);

    void paintButton(
        juce::Graphics& g,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown) override;

protected:
    const juce::Path& iconPath;
};
