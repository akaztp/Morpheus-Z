#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "ButtonFrame.h"
#include "FramedButton.h"

class SwitchButton : public FramedButton
{
public:
    SwitchButton(
    const StylesStore& stylesStore,
    const juce::String& buttonName,
    const juce::Path* iconPath,
    std::function<void()> onClickCallback);
};
