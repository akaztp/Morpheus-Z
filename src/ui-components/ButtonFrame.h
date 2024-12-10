#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "StyledComponent.h"

class ButtonFrame : public StyledComponent
{
public:
    ButtonFrame(const StylesStore& stylesStore, juce::Component &component);

    void paint(juce::Graphics& g, bool highlighted, bool down) const;
    void paint(
        juce::Graphics& g,
        bool highlighted,
        bool down,
        bool selected) const;

    void preparePaint();

private:
    juce::Component &component;
    float borderWidth;
    juce::Rectangle<int> bounds;
    juce::Rectangle<int> outlineBounds;
    float cornerRadius;
    juce::Path border;
    juce::Colour selectedColor;
};
