#include "WaveformWidget.h"

#include <utility>


WaveformWidget::WaveformWidget(
    AppState& appState,
    const StylesStore& stylesStore,
    const DrawCallback& onDrawCallback,
    const PresetCallback& onPresetCallback)
    : StyledComponent(stylesStore)
{
    initWaveformBackground(appState);
    initWaveformDisplays(appState);
    initWaveformMorphMonitor(appState);
    initWaveformInput(std::move(onDrawCallback));
    initWaveformSelectors(appState.waveforms.size());
    selectWaveform(0);
    initPresetButtons(std::move(onPresetCallback));
    initLoopButton(appState);

    appState.monitorMorphPosition.addObserver([this, &appState](double value)
    {
        this->updateMorphMonitor(value, appState);
    });
}

void WaveformWidget::updateMorphMonitor(
    const double monitorMorphPosition,
    AppState& appState)
{
    const auto mixFactor = monitorMorphPosition;
    if (mixFactor == MONITOR_MORPH_POSITION_OFF)
    {
        if (!waveformMorph.hasBeenCleared())
        {
            waveformMorph.clear();
            waveformMorphMonitor->setVisible(false);
            waveformMorphMonitor->repaint();
        }
    }
    else
    {
        const auto size = waveformMorph.getNumSamples();
        jassert(size == appState.waveforms[0].getNumSamples());
        jassert(size == appState.waveforms[1].getNumSamples());
        for (int i = 0; i < size; ++i)
        {
            waveformMorph.setSample(
                0,
                i,
                appState.waveforms[0].getSample(0, i) * (1.0 - mixFactor)
                + appState.waveforms[1].getSample(0, i) * mixFactor);
        }
        waveformMorphMonitor->setVisible(true);
        waveformMorphMonitor->repaint();
    }
}


void WaveformWidget::initWaveformBackground(
    AppState& appState)
{
    waveformBackground =
        std::make_unique<WaveformBackground>(stylesStore, appState);
    addAndMakeVisible(*waveformBackground);
}


void WaveformWidget::initWaveformSelectors(const int numSelectors)
{
    waveformSelectors.resize(numSelectors);
    for (auto i = 0; i < numSelectors; ++i)
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

void WaveformWidget::initWaveformDisplays(AppState& appState)
{
    waveformDisplays.resize(appState.waveforms.size());
    for (auto i = 0; i < appState.waveforms.size(); ++i)
    {
        waveformDisplays[i] = std::make_unique<WaveformDisplay>(
            stylesStore,
            appState,
            appState.waveforms[i]
        );
        addAndMakeVisible(*waveformDisplays[i]);
    }
}

void WaveformWidget::initWaveformMorphMonitor(AppState& appState)
{
    waveformMorphMonitor = std::make_unique<WaveformDisplay>(
        stylesStore,
        appState,
        waveformMorph);
    waveformMorphMonitor->setWaveFormColorId(
        StylesStore::ColorIds::WaveformMonitor);
    addAndMakeVisible(*waveformMorphMonitor);
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

void WaveformWidget::initLoopButton(AppState& appState)
{
    loopButtonIcon = CustomSymbols::createPathFromData(
        CustomSymbols::infinitePathData,
        sizeof(CustomSymbols::infinitePathData));
    loopButton = std::make_unique<SwitchButton>(
        stylesStore,
        "",
        loopButtonIcon.get(),
        nullptr);
    loopButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        appState.apvts,
        AppState::Id::loopMode,
        *loopButton);
    addAndMakeVisible(*loopButton);
}

void WaveformWidget::initWaveformInput(const DrawCallback& onDrawCallback)
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
    for (auto i = 0; i < waveformSelectors.size(); ++i)
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

    const auto waveformBounds = bounds.reduced(borderWidth);
    waveformInput->setBounds(waveformBounds);
    for (const auto& waveformDisplay : waveformDisplays)
    {
        waveformDisplay->setBounds(waveformBounds);
    }
    waveformMorphMonitor->setBounds(waveformBounds);

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
    const auto borderWidth = static_cast<int>(
        stylesStore.getNumber(StylesStore::NumberIds::BorderWidth));
    return waveformInput->getPreferredWidth() + 2 * borderWidth;
}

int WaveformWidget::getPreferredHeight() const
{
    const auto borderWidth = static_cast<int>(
        stylesStore.getNumber(StylesStore::NumberIds::BorderWidth));
    return waveformInput->getPreferredHeight() +
        2 * borderWidth + getControlsHeight();
}

int WaveformWidget::getControlsHeight() const
{
    const auto buttonHeight = static_cast<int>(
        stylesStore.getNumber(StylesStore::NumberIds::ButtonHeight));
    const auto layoutGutter = static_cast<int>(
        stylesStore.getNumber(StylesStore::NumberIds::LayoutGutter));
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

void WaveformWidget::waveformChanged(const int waveNum) const
{
    waveformDisplays[waveNum]->repaint();
}
