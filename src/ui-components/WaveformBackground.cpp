#include "WaveformBackground.h"

WaveformBackground::WaveformBackground(const StylesStore& stylesStore)
    : stylesStore(stylesStore)
{
}

void WaveformBackground::paint(juce::Graphics& g)
{
    const auto bounds = getLocalBounds();
    g.setColour(stylesStore.getColor(StylesStore::ColorIds::WidgetBackground));
    g.fillRect(bounds);
    g.setColour(stylesStore.getColor(StylesStore::ColorIds::Outline));
    g.drawRect(bounds, 1.0f);

    g.setColour(stylesStore.getColor(StylesStore::ColorIds::WaveformDisplay_origin));
    g.drawHorizontalLine(
        bounds.getHeight() >> 1,
        0.0f,
        static_cast<float>(bounds.getWidth()) - 1.0f);
}
