#include "SwitchButton.h"

SwitchButton::SwitchButton(
    const StylesStore& stylesStore,
    const juce::String& buttonName,
    const juce::Path* iconPath,
    std::function<void()> onClickCallback
): FramedButton(stylesStore, buttonName, iconPath, onClickCallback)
{
    setClickingTogglesState(true);
}
