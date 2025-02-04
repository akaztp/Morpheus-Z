#include "WaveformBackground.h"


WaveformBackground::WaveformBackground(
    const StylesStore& stylesStore,
    AppState& appState)
    : StyledComponent(stylesStore)
{
    initParams(appState);
}

WaveformBackground::~WaveformBackground()
{
    waveformDisplayModeParam->removeListener(this);
}

void WaveformBackground::initParams(AppState& appState)
{
    waveformDisplayModeParam = appState.audioParameters.waveformDisplayMode;
    waveformDisplayModeParam->addListener(this);
}

void WaveformBackground::paint(juce::Graphics& g)
{
    const auto bounds = getLocalBounds();
    g.setColour(stylesStore.getColor(StylesStore::ColorIds::WidgetBackground));
    g.fillRect(bounds);

    g.setColour(stylesStore.getColor(StylesStore::ColorIds::Outline));
    g.drawRect(bounds, stylesStore.getNumber(StylesStore::NumberIds::BorderWidth));

    g.setColour(stylesStore.getColor(StylesStore::ColorIds::WaveformDisplay_origin));

    switch (waveformDisplayModeParam->getIndex())
    {
    case WaveformDisplayMode::Cartesian:
        paintCartOrigin(g, bounds);
        break;
    case WaveformDisplayMode::Polar:
        paintPolarOrigin(g, bounds);
        break;
    default:
        jassert(false);
    }
}

void WaveformBackground::paintCartOrigin(
    juce::Graphics& g,
    const juce::Rectangle<int> bounds)
{
    g.drawHorizontalLine(
        bounds.getHeight() / 2,
        0.0f,
        static_cast<float>(bounds.getWidth()) - 1.0f);
}

void WaveformBackground::paintPolarOrigin(
    juce::Graphics& g,
    const juce::Rectangle<int> bounds)
{
    const auto width = bounds.getWidth();
    const auto height = bounds.getHeight();
    const float centerX = static_cast<float>(width) / 2.0f;
    const float centerY = static_cast<float>(height) / 2.0f;
    const float radius = std::min(centerX, centerY);
    const float halfRadius = radius / 2.0f;
    g.drawEllipse(
        centerX - halfRadius,
        centerY - halfRadius,
        radius, radius, 2.0f);
}

void WaveformBackground::parameterValueChanged(int parameterIndex, float newValue)
{
    juce::ignoreUnused(parameterIndex, newValue);
    repaint();
}

void WaveformBackground::parameterGestureChanged(int parameterIndex, bool gestureIsStarting)
{
    juce::ignoreUnused(parameterIndex, gestureIsStarting);
}
