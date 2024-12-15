#include "ButtonFrame.h"
#include "StyledComponent.h"

ButtonFrame::ButtonFrame(const StylesStore& stylesStore, juce::Component& component)
    : StyledComponent(stylesStore), component(component)
{
    preparePaint();
}

void ButtonFrame::paint(
    juce::Graphics& g,
    const bool highlighted,
    const bool down) const
{
    paint(g, highlighted, down, false);
}

void ButtonFrame::paint(
    juce::Graphics& g,
    const bool highlighted,
    const bool down,
    const bool selected) const
{
    juce::ignoreUnused(highlighted);

    /* fill background */
    g.setColour(stylesStore.getColor(
        down
            ? StylesStore::ColorIds::HighlightedFill
            : (selected ? StylesStore::ColorIds::SelectedFill : StylesStore::ColorIds::DefaultFill)));
    g.fillRect(outlineBounds.reduced(borderWidth * 0.5f));

    /* draw border */
    g.setColour(stylesStore.getColor(StylesStore::ColorIds::Outline));
    g.strokePath(border, juce::PathStrokeType(borderWidth));
}

void ButtonFrame::preparePaint()
{
    borderWidth = stylesStore.getNumber(StylesStore::NumberIds::BorderWidth);
    bounds = component.getLocalBounds();
    outlineBounds = bounds.reduced(borderWidth * 0.5f);

    cornerRadius =
        stylesStore.getNumber(StylesStore::NumberIds::CornerRadius);
    border = juce::Path();
    border.addRoundedRectangle(outlineBounds, 2 * cornerRadius);
}
