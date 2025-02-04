#pragma once
#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "StyledComponent.h"
#include "../StylesStore.h"

class EnvelopeHandle : public juce::Component, public StyledComponent
{
public:
    EnvelopeHandle(
        const StylesStore& stylesStore,
        std::function<void()> onDragStartCallback,
        std::function<void()> onDragEndCallback,
        std::function<void (int, int)> onDragCallback);

    void paint(juce::Graphics& g) override;

    void mouseDown(const juce::MouseEvent& event) override;

    void mouseDrag(const juce::MouseEvent& event) override;

    void mouseUp(const juce::MouseEvent& event) override;

private:
    juce::ComponentDragger dragger;
    bool isBeingDragged = false;

    std::function<void()> onDragStartCallback;
    std::function<void()> onDragEndCallback;
    std::function<void(int, int)> onDragCallback;
};
