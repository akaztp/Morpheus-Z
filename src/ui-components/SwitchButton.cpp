#include "SwitchButton.h"

SwitchButton::SwitchButton(
    const StylesStore& stylesStore)
    : juce::ToggleButton(),
      buttonFrame(stylesStore, *this)
{
}

void SwitchButton::paintButton(
    juce::Graphics& g,
    bool shouldDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown)
{
    buttonFrame.paint(
        g,
        shouldDrawButtonAsHighlighted,
        shouldDrawButtonAsDown,
        getToggleState());

    auto fontSize = juce::jmin(15.0f, static_cast<float>(getHeight()) * 0.75f);
    g.setColour(findColour(textColourId));
    g.setFont(fontSize);
    g.drawFittedText(getButtonText(),
                     getLocalBounds(),
                     juce::Justification::centred, 10);
}

void SwitchButton::lookAndFeelChanged()
{
    ToggleButton::lookAndFeelChanged();
    buttonFrame.preparePaint();
}

void SwitchButton::resized()
{
    ToggleButton::resized();
    buttonFrame.preparePaint();
}
