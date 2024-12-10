#include "WaveformPresetButton.h"

#include <utility>

WaveformPresetButton::WaveformPresetButton(
    const StylesStore& stylesStore,
    const juce::String& buttonName,
    const juce::Path& iconPath,
    std::function<void()> onClickCallback)
    : juce::Button(buttonName),
      StyledComponent(stylesStore),
      buttonFrame(stylesStore, *this),
      iconPath(iconPath)
{
    onClick = std::move(onClickCallback);
}

void WaveformPresetButton::paintButton(
    juce::Graphics& g,
    const bool shouldDrawButtonAsHighlighted,
    const bool shouldDrawButtonAsDown)
{
    buttonFrame.paint(g, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

    /* draw icon */
    g.setColour(stylesStore.getColor(StylesStore::ColorIds::DefaultText));
    g.strokePath(iconPath, juce::PathStrokeType(2.0f), iconOffset);
}

void WaveformPresetButton::lookAndFeelChanged()
{
    Button::lookAndFeelChanged();
    preparePaint();
}

void WaveformPresetButton::resized()
{
    Button::resized();
    preparePaint();
}


void WaveformPresetButton::preparePaint()
{
    buttonFrame.preparePaint();
    bounds = getLocalBounds();
    iconBounds = iconPath.getBounds();
    iconOffset = juce::AffineTransform::translation(
        0.5f * (bounds.getWidth() - iconBounds.getWidth()) - iconBounds.getX(),
        0.5f * (bounds.getHeight() - iconBounds.getHeight()) - iconBounds.getY());
}
