#include "WaveformPresetButton.h"

WaveformPresetButton::WaveformPresetButton(
    const StylesStore& stylesStore,
    const juce::String& buttonName,
    const juce::Path& iconPath)
    : StyledComponent(stylesStore),
      juce::Button(buttonName),
      iconPath(iconPath)
{
}

void WaveformPresetButton::paintButton(
    juce::Graphics& g,
    bool shouldDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown)
{
    /* fill background */
    g.setColour(stylesStore.getColor(
        shouldDrawButtonAsDown
            ? StylesStore::ColorIds::HighlightedFill
            : StylesStore::ColorIds::WidgetBackground));
    g.fillRect(outlineBounds.reduced(borderWidth * 0.5f));

    /* draw border */
    g.setColour(stylesStore.getColor(StylesStore::ColorIds::Outline));
    g.strokePath(border, juce::PathStrokeType(borderWidth));

    /* draw icon */
    g.setColour(stylesStore.getColor(StylesStore::ColorIds::DefaultText));
    g.strokePath(iconPath, juce::PathStrokeType(2.0f), iconOffset);
}

void WaveformPresetButton::lookAndFeelChanged()
{
    preparePaint();
}

void WaveformPresetButton::resized()
{
    preparePaint();
}

void WaveformPresetButton::preparePaint()
{
    borderWidth = stylesStore.getNumber(StylesStore::NumberIds::BorderWidth);
    bounds = juce::Button::getLocalBounds();
    outlineBounds = bounds.reduced(borderWidth * 0.5f);

    cornerRadius =
        stylesStore.getNumber(StylesStore::NumberIds::CornerRadius);
    border = juce::Path();
    border.addRoundedRectangle(outlineBounds, 2 * cornerRadius);

    iconBounds = iconPath.getBounds();
    iconOffset = juce::AffineTransform::translation(
        0.5f * (bounds.getWidth() - iconBounds.getWidth()) - iconBounds.getX(),
        0.5f * (bounds.getHeight() - iconBounds.getHeight()) - iconBounds.getY());
}
