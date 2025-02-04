#include "EnvelopeHandle.h"

EnvelopeHandle::EnvelopeHandle(
    const StylesStore& stylesStore,
    std::function<void()> onDragStartCallback,
    std::function<void()> onDragEndCallback,
    std::function<void(int, int)> onDragCallback) :
    StyledComponent(stylesStore),
    onDragStartCallback(std::move(onDragStartCallback)),
    onDragEndCallback(std::move(onDragEndCallback)),
    onDragCallback(std::move(onDragCallback))
{
}

void EnvelopeHandle::paint(juce::Graphics& g)
{
    StylesStore::ColorIds colorId;
    if (isBeingDragged)
        colorId = StylesStore::ColorIds::SelectedFill;
    else if (isMouseOver())
        colorId = StylesStore::ColorIds::HighlightedFill;
    else
        colorId = StylesStore::ColorIds::DefaultFill;
    g.setColour(stylesStore.getColor(colorId));
    g.setOpacity(stylesStore.getNumber(
        StylesStore::NumberIds::EnvelopeHandleOpacity));
    g.fillRect(getLocalBounds());
}

void EnvelopeHandle::mouseDown(const juce::MouseEvent& event)
{
    dragger.startDraggingComponent(this, event);
    onDragStartCallback();
}

void EnvelopeHandle::mouseDrag(const juce::MouseEvent& event)
{
    dragger.dragComponent(this, event, nullptr);
    const auto center = getBounds().getCentre();
    onDragCallback(center.x, center.y);
    isBeingDragged = true;
    repaint();
}

void EnvelopeHandle::mouseUp(const juce::MouseEvent& event)
{
    isBeingDragged = false;
    onDragEndCallback();
    repaint();
}
