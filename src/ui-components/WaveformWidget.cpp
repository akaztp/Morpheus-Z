#include "WaveformWidget.h"

#include <memory>
#include "../AppParams.h"

WaveformWidget::WaveformWidget(
    juce::AudioProcessorValueTreeState& apvts,
    const StylesStore& stylesStore,
    juce::AudioThumbnailCache& thumbnailCache,
    juce::AudioFormatManager& formatManager,
    DrawCallback onDrawCallback,
    PresetCallback onPresetCallback)
    : StyledComponent(stylesStore)
{
    initWaveformBackground();
    initWaveformDisplays(thumbnailCache, formatManager);
    initWaveformInput(onDrawCallback);
    initWaveformSelectors();
    selectWaveform(0);
    initPresetButtons(onPresetCallback);
    initLoopButton(apvts);
}

void WaveformWidget::initWaveformBackground()
{
    waveformBackground = std::make_unique<WaveformBackground>(stylesStore);
    addAndMakeVisible(*waveformBackground);
}

void WaveformWidget::initWaveformSelectors()
{
    for (auto i = 0; i < numWaveforms; ++i)
    {
        waveformSelectors[i] = std::make_unique<FramedButton>(
            stylesStore,
            juce::String(i + 1),
            nullptr,
            nullptr);
        waveformSelectors[i]->onClick = [this, i] { this->selectWaveform(i); };
        addAndMakeVisible(*waveformSelectors[i]);
    }
}

void WaveformWidget::initWaveformDisplays(
    juce::AudioThumbnailCache& thumbnailCache,
    juce::AudioFormatManager& formatManager)
{
    for (auto i = 0; i < numWaveforms; ++i)
    {
        waveformDisplays[i] = std::make_unique<WaveformDisplay>(
            stylesStore,
            thumbnailCache,
            formatManager
        );
        addAndMakeVisible(*waveformDisplays[i]);
    }
}

void WaveformWidget::initPresetButtons(PresetCallback onPresetCallback)
{
    for (int i = sizeof(presetButtonsConfig) / sizeof(ButtonInfo) - 1; i >= 0; i--)
    {
        auto& buttonInfo = presetButtonsConfig[i];
        buttonInfo.icon = CustomSymbols::createPathFromData(
            buttonInfo.iconData,
            buttonInfo.iconDataSize);
        buttonInfo.button = std::make_unique<FramedButton>(
            stylesStore,
            buttonInfo.name,
            buttonInfo.icon.get(),
            [onPresetCallback, &buttonInfo, this]
            {
                onPresetCallback(buttonInfo.preset, this->selectedWaveform);
            }
        );
        addAndMakeVisible(*buttonInfo.button);
    }
}

void WaveformWidget::initLoopButton(juce::AudioProcessorValueTreeState& apvts)
{
    loopButtonIcon = CustomSymbols::createPathFromData(
        CustomSymbols::infinitePathData,
        sizeof(CustomSymbols::infinitePathData));
    loopButton = std::make_unique<SwitchButton>(stylesStore, "", loopButtonIcon.get(), nullptr);
    loopButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        apvts,
        AppParams::loopMode,
        *loopButton);
    addAndMakeVisible(*loopButton);
}

void WaveformWidget::initWaveformInput(DrawCallback onDrawCallback)
{
    waveformInput = std::make_unique<WaveformInput>(
        [onDrawCallback, this](const juce::Point<int>& from, const juce::Point<int>& to)
        {
            onDrawCallback(from, to, this->selectedWaveform);
        });
    addAndMakeVisible(*waveformInput);
}

void WaveformWidget::selectWaveform(const int waveformNum)
{
    selectedWaveform = waveformNum;
    for (auto i = 0; i < numWaveforms; ++i)
    {
        waveformSelectors[i]->setToggleState(
            i == selectedWaveform,
            juce::NotificationType::dontSendNotification);
        waveformDisplays[i]->setWaveFormColorId(
            i == selectedWaveform
                ? StylesStore::ColorIds::WaveformDisplay_SelectedWave
                : StylesStore::ColorIds::WaveformDisplay_UnselectedWave
        );
    }
}

void WaveformWidget::resized()
{
    auto bounds = getLocalBounds();
    const auto layoutGutter = static_cast<int>(stylesStore.getNumber(StylesStore::NumberIds::LayoutGutter));
    const auto borderWidth = static_cast<int>(stylesStore.getNumber(StylesStore::NumberIds::BorderWidth));

    auto controls = bounds.removeFromBottom(getControlsHeight());
    waveformBackground->setBounds(bounds);

    const auto waveformBounds = bounds.reduced(borderWidth * 2);
    waveformInput->setBounds(waveformBounds);
    for (const auto& waveformDisplay : waveformDisplays)
    {
        waveformDisplay->setBounds(waveformBounds);
    }

    controls.removeFromTop(layoutGutter);
    auto bottomRight = controls.removeFromRight(controls.getWidth() / 2);
    const auto buttonWidth = static_cast<int>(stylesStore.getNumber(StylesStore::NumberIds::ButtonHeight));
    for (int i = sizeof(presetButtonsConfig) / sizeof(ButtonInfo) - 1; i >= 0; i--)
    {
        presetButtonsConfig[i].button->setBounds(bottomRight.removeFromRight(buttonWidth));
        bottomRight.removeFromRight(layoutGutter);
    }

    for (const auto& waveformSelector : waveformSelectors)
    {
        waveformSelector->setBounds(controls.removeFromLeft(buttonWidth));
    }

    controls.removeFromLeft(stylesStore.getNumber(StylesStore::NumberIds::LayoutMargin));
    loopButton->setBounds(controls.removeFromLeft(buttonWidth));
}

int WaveformWidget::getPreferredWidth() const
{
    const auto borderWidth = static_cast<int>(stylesStore.getNumber(StylesStore::NumberIds::BorderWidth));
    return waveformInput->getPreferredWidth() + 2 * borderWidth;
}

int WaveformWidget::getPreferredHeight() const
{
    const auto borderWidth = static_cast<int>(stylesStore.getNumber(StylesStore::NumberIds::BorderWidth));
    return waveformInput->getPreferredHeight() + 2 * borderWidth + getControlsHeight();
}

int WaveformWidget::getControlsHeight() const
{
    const auto buttonHeight =  static_cast<int>(stylesStore.getNumber(StylesStore::NumberIds::ButtonHeight));
    const auto layoutGutter =  static_cast<int>(stylesStore.getNumber(StylesStore::NumberIds::LayoutGutter));
    return layoutGutter + buttonHeight;
}

int WaveformWidget::getWaveformHeight() const
{
    return waveformInput->getHeight();
}

int WaveformWidget::getWaveformWidth() const
{
    return waveformInput->getWidth();
}

void WaveformWidget::setSource(
    const int waveNum,
    const juce::AudioBuffer<float>& newSource,
    const double sampleRate,
    const juce::int64 hashCode) const
{
    waveformDisplays[waveNum]->setSource(newSource, sampleRate, hashCode);
}
