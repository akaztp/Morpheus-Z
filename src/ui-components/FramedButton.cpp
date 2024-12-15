#include "FramedButton.h"

#include <utility>

FramedButton::FramedButton(
    const StylesStore& stylesStore,
    const juce::String& buttonName,
    const juce::Path* iconPath,
    std::function<void()> onClickCallback)
    : juce::Button(buttonName),
      StyledComponent(stylesStore),
      buttonFrame(stylesStore, *this),
      iconPath(iconPath)
{
    onClick = std::move(onClickCallback);
}

void FramedButton::lookAndFeelChanged()
{
    Button::lookAndFeelChanged();
    preparePaint();
}

void FramedButton::resized()
{
    Button::resized();
    preparePaint();
}

void FramedButton::preparePaint()
{
    buttonFrame.preparePaint();
    bounds = getLocalBounds();
    if (iconPath != nullptr)
    {
        iconBounds = iconPath->getBounds();
        iconOffset = juce::AffineTransform::translation(
            0.5f * (bounds.getWidth() - iconBounds.getWidth()) - iconBounds.getX(),
            0.5f * (bounds.getHeight() - iconBounds.getHeight()) - iconBounds.getY());
    }
    fontSize = juce::jmin(
        stylesStore.getNumber(StylesStore::NumberIds::ButtonFontHeight),
        static_cast<float>(getHeight()) * 0.75f);
}

void FramedButton::paintButton(
    juce::Graphics& g,
    const bool shouldDrawButtonAsHighlighted,
    const bool shouldDrawButtonAsDown)
{
    buttonFrame.paint(
        g,
        shouldDrawButtonAsHighlighted,
        shouldDrawButtonAsDown,
        getToggleState());

    g.setColour(stylesStore.getColor(StylesStore::ColorIds::DefaultText));
    if (iconPath != nullptr)
    {
        /* draw icon */
        g.strokePath(
            *iconPath,
            juce::PathStrokeType(stylesStore.getNumber(StylesStore::NumberIds::IconStrokeWidth)),
            iconOffset);
    }
    else
    {
        /* draw text */
        g.setFont(fontSize);
        g.drawFittedText(getButtonText(),
                         bounds,
                         juce::Justification::centred, 10);
    }
}
