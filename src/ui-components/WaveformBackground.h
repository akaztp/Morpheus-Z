#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../StylesStore.h"


class WaveformBackground : public juce::Component
{
public:
    explicit WaveformBackground(const StylesStore& stylesStore);
    void paint(juce::Graphics& g) override;

private:
    const StylesStore& stylesStore;
};
