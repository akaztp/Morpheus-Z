#pragma once

#include <juce_audio_utils/juce_audio_utils.h>

class WaveformInput : public juce::Component
{
public:
    typedef std::function<void(const juce::Point<int>& from, const juce::Point<int>& to)> DrawCallback;

    explicit WaveformInput(DrawCallback onDrawCallback);

    void mouseDown(const juce::MouseEvent& event) override;

    void mouseDrag(const juce::MouseEvent& event) override;


private:
    std::function<void(const juce::Point<int>& from, const juce::Point<int>& to)> onDraw;
    juce::Point<int> lastMousePosition;

    bool forceHorizontalBounds(juce::Point<int>& pos) const;
    void forceVerticalBounds(juce::Point<int>& pos) const;
};
