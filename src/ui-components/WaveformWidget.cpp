#include "WaveformWidget.h"


WaveformWidget::WaveformWidget(
    const StylesStore& stylesStore,
    juce::AudioThumbnailCache& thumbnailCache,
    juce::AudioFormatManager& formatManager,
    WaveformDisplay::DrawCallback onDrawCallback,
    std::function<void(WaveformPreset preset)> onClickCallback)
    : StyledComponent(stylesStore),
      waveformDisplay(stylesStore, thumbnailCache, formatManager, onDrawCallback)
{
    addAndMakeVisible(waveformDisplay);
    for (auto& buttonInfo : presetButtonsConfig)
    {
        buttonInfo.path = std::make_unique<juce::Path>();
        buttonInfo.path->loadPathFromData(buttonInfo.iconData, buttonInfo.iconDataSize);
        buttonInfo.button = std::make_unique<WaveformPresetButton>(
            stylesStore,
            buttonInfo.name,
            *buttonInfo.path,
            [onClickCallback, &buttonInfo] { onClickCallback(buttonInfo.preset); }
        );
        addAndMakeVisible(dynamic_cast<juce::Button*>(buttonInfo.button.get()));
    }
}

void WaveformWidget::resized()
{
    auto bounds = getLocalBounds();
    const int buttonsHeight = stylesStore.getNumber(StylesStore::NumberIds::ButtonHeight);
    const int layoutGutter = stylesStore.getNumber(StylesStore::NumberIds::LayoutGutter);
    auto bottom = bounds.removeFromBottom(buttonsHeight + layoutGutter);
    waveformDisplay.setBounds(bounds);

    bottom.removeFromTop(layoutGutter);

    const int buttonWidth = stylesStore.getNumber(StylesStore::NumberIds::ButtonHeight);
    for (const auto& buttonInfo : presetButtonsConfig)
    {
        const auto buttonBounds = bottom.removeFromLeft(buttonWidth);
        dynamic_cast<juce::Button*>(buttonInfo.button.get())->setBounds(buttonBounds);
        bottom.removeFromLeft(layoutGutter);
    }
}

int WaveformWidget::getWaveformHeight() const
{
    return waveformDisplay.getHeight();
}

int WaveformWidget::getWaveformWidth() const
{
    return waveformDisplay.getWidth();
}

void WaveformWidget::setSource(
    const juce::AudioBuffer<float>& newSource,
    const double sampleRate,
    const juce::int64 hashCode)
{
    waveformDisplay.setSource(newSource, sampleRate, hashCode);
}
