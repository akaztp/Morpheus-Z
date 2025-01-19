#include "WaveformInput.h"

WaveformInput::WaveformInput(DrawCallback onDrawCallback)
    : onDraw(std::move(onDrawCallback))
{
}


void WaveformInput::mouseDown(const juce::MouseEvent& event)
{
    if (onDraw == nullptr)
    {
        return;
    }

    const auto pos = event.getPosition();
    onDraw(pos, pos);

    lastMousePosition = pos;
}

void WaveformInput::mouseDrag(const juce::MouseEvent& event)
{
    if (onDraw == nullptr) { return; }

    auto pos = event.getPosition();
    pos.x = forceHighLimit(pos.x, getWidth() - 1);
    pos.y = forceHighLimit(pos.y, getHeight() - 1);
    onDraw(lastMousePosition, pos);
    lastMousePosition = pos;
}

int WaveformInput::forceHighLimit(const int value, const int hiLimit)
{
    return value > hiLimit ? hiLimit : (value < 0 ? 0 : value);
}

int WaveformInput::getPreferredWidth()
{
    return 510;
}

int WaveformInput::getPreferredHeight()
{
    return 255;
}
