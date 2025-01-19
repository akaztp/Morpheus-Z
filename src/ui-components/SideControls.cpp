#include "SideControls.h"
#include "CustomSymbols.h"
#include "../AppParams.h"

SideControls::SideControls(
    juce::AudioProcessorValueTreeState& apvts,
    const StylesStore& stylesStore):
    StyledComponent(stylesStore)
{
    initParams(apvts);
    initDisplayModeSelectors();
    updateButtonsState();
}

SideControls::~SideControls()
{
    waveformDisplayModeParam->removeListener(this);
}

int SideControls::getPreferredWidth() const
{
    const auto buttonWidth = static_cast<int>(
        stylesStore.getNumber(StylesStore::NumberIds::ButtonHeight));
    return buttonWidth;
}

int SideControls::getPreferredHeight() const
{
    const auto layoutGutter = static_cast<int>(
        stylesStore.getNumber(StylesStore::NumberIds::LayoutGutter));
    const auto buttonWidth = static_cast<int>(
        stylesStore.getNumber(StylesStore::NumberIds::ButtonHeight));
    return 2 * buttonWidth + layoutGutter;
}

void SideControls::initDisplayModeSelectors()
{
    cartesianButtonIcon = CustomSymbols::createPathFromData(
        CustomSymbols::cartesianPathData,
        sizeof(CustomSymbols::cartesianPathData));
    cartesianButton = std::make_unique<FramedButton>(
        stylesStore,
        "Cartesian View",
        cartesianButtonIcon.get(),
        nullptr);
    cartesianButton->onClick = [this]
    {
        this->selectDisplayMode(static_cast<int>(WaveformDisplayMode::Cartesian));
    };
    addAndMakeVisible(*cartesianButton);

    polarButtonIcon = CustomSymbols::createPathFromData(
        CustomSymbols::polarPathData,
        sizeof(CustomSymbols::polarPathData));
    polarButton = std::make_unique<FramedButton>(
        stylesStore,
        "Polar view",
        polarButtonIcon.get(),
        nullptr);
    polarButton->onClick = [this]
    {
        this->selectDisplayMode(static_cast<int>(WaveformDisplayMode::Polar));
    };
    addAndMakeVisible(*polarButton);
}

void SideControls::initParams(juce::AudioProcessorValueTreeState& apvts)
{
    waveformDisplayModeParam = dynamic_cast<juce::AudioParameterChoice*>(
        apvts.getParameter(AppParams::waveformDisplayMode));
    waveformDisplayModeParam->addListener(this);
}

void SideControls::resized()
{
    auto bounds = getLocalBounds();
    const auto layoutGutter = static_cast<int>(
        stylesStore.getNumber(StylesStore::NumberIds::LayoutGutter));
    const auto buttonWidth = static_cast<int>(
        stylesStore.getNumber(StylesStore::NumberIds::ButtonHeight));

    cartesianButton->setBounds(
        bounds.getX(),
        bounds.getY(),
        buttonWidth,
        buttonWidth);
    polarButton->setBounds(
        bounds.getX(),
        bounds.getY() + buttonWidth + layoutGutter,
        buttonWidth,
        buttonWidth);
}

void SideControls::selectDisplayMode(const int displayMode) const
{
    waveformDisplayModeParam->setValueNotifyingHost(
        static_cast<float>(displayMode));
}

void SideControls::updateButtonsState() const
{
    const auto mode = waveformDisplayModeParam->getIndex();
    cartesianButton->setToggleState(
        mode == static_cast<int>(WaveformDisplayMode::Cartesian),
        juce::NotificationType::dontSendNotification);
    polarButton->setToggleState(
        mode == static_cast<int>(WaveformDisplayMode::Polar),
        juce::NotificationType::dontSendNotification);
}

void SideControls::parameterValueChanged(int parameterIndex, float newValue)
{
    juce::ignoreUnused(parameterIndex, newValue);
    updateButtonsState();
    repaint();
}

void SideControls::parameterGestureChanged(int parameterIndex, bool gestureIsStarting)
{
    juce::ignoreUnused(parameterIndex, gestureIsStarting);
}
