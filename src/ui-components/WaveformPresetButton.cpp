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
    const auto borderWidth =
        stylesStore.getNumber(StylesStore::NumberIds::BorderWidth);
    const auto bounds = juce::Button::getLocalBounds();
    const auto outlineBounds = bounds.reduced(borderWidth * 0.5f);

    /* fill background */
    g.setColour(stylesStore.getColor(
        shouldDrawButtonAsDown
            ? StylesStore::ColorIds::HighlightedFill
            : StylesStore::ColorIds::WidgetBackground));
    g.fillRect(outlineBounds.reduced(borderWidth * 0.5f));

    /* draw border */
    const auto cornerRadius =
        stylesStore.getNumber(StylesStore::NumberIds::CornerRadius);
    juce::Path border = juce::Path();
    border.addRoundedRectangle(outlineBounds, 2 * cornerRadius);
    g.setColour(stylesStore.getColor(StylesStore::ColorIds::Outline));
    g.strokePath(border, juce::PathStrokeType(borderWidth));

    /* draw icon */
    const auto iconBounds = iconPath.getBounds();
    const juce::AffineTransform iconOffset = juce::AffineTransform::translation(
        0.5f * (bounds.getWidth() - iconBounds.getWidth()) - iconBounds.getX(),
        0.5f * (bounds.getHeight() - iconBounds.getHeight()) - iconBounds.getY());
    g.setColour(stylesStore.getColor(StylesStore::ColorIds::DefaultText));
    g.strokePath(iconPath, juce::PathStrokeType(2.0f), iconOffset);
}
