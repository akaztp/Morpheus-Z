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
    if (onDraw == nullptr)
    {
        return;
    }

    auto pos = event.getPosition();
    forceVerticalBounds(pos);
    const auto forcedHorizontally = forceHorizontalBounds(pos);
    const auto x = pos.getX();
    const auto y = pos.getY();
    const auto lastX = lastMousePosition.getX();
    const auto lastY = lastMousePosition.getY();
    if (x == lastX)
    {
        if (y != lastY && !forcedHorizontally)
        {
            onDraw(pos, pos);
        }
    }
    else
    {
        if (x > lastX)
        {
            onDraw(lastMousePosition, pos);
        }
        else
        {
            onDraw(pos, lastMousePosition);
        }
    }

    lastMousePosition = pos;
}

bool WaveformInput::forceHorizontalBounds(juce::Point<int>& pos) const
{
    auto forced = false;
    const auto x = pos.getX();
    if (x >= getWidth())
    {
        pos.x = getWidth() - 1;
        forced = true;
    }
    else if (x < 0)
    {
        pos.x = 0;
        forced = true;
    }
    return forced;
}

void WaveformInput::forceVerticalBounds(juce::Point<int>& pos) const
{
    const auto y = pos.getY();
    if (y >= getHeight())
    {
        pos.y = getHeight() - 1;
    }
    else if (y < 0)
    {
        pos.y = 0;
    }
}

int WaveformInput::getPreferredWidth() const
{
    return 510;
}

int WaveformInput::getPreferredHeight() const
{
    return 255;
}