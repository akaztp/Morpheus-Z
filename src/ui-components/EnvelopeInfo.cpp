#include "EnvelopeInfo.h"

EnvelopeInfo::EnvelopeInfo(
    juce::AudioProcessorValueTreeState& apvts,
    const StylesStore& stylesStore) : stylesStore(stylesStore)
{
    for (auto& cfg : infoConfig)
    {
        cfg.slider = std::make_unique<juce::Slider>(
            juce::Slider::SliderStyle::RotaryHorizontalDrag,
            juce::Slider::TextEntryBoxPosition::TextBoxRight);
        cfg.sliderAttachment =
            std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                apvts,
                cfg.param,
                *cfg.slider);
        addAndMakeVisible(*cfg.slider);
    }
}

void EnvelopeInfo::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    const auto layoutGutter = static_cast<int>(stylesStore.getNumber(StylesStore::NumberIds::LayoutGutter));
    const auto buttonHeight = static_cast<int>(stylesStore.getNumber(StylesStore::NumberIds::ButtonHeight));
    for (int numInfo = numEnvSteps; numInfo > 0; numInfo--)
    {
        const auto infoBounds = bounds.removeFromTop(buttonHeight);
        g.setColour(stylesStore.getColor(StylesStore::ColorIds::DefaultFill));
        g.fillRect(infoBounds);
        bounds.removeFromTop(layoutGutter);
    }
    Component::paint(g);
}

void EnvelopeInfo::resized()
{
    Component::resized();

    auto bounds = getLocalBounds();
    const auto layoutGutter = static_cast<int>(stylesStore.getNumber(StylesStore::NumberIds::LayoutGutter));
    const auto buttonHeight = static_cast<int>(stylesStore.getNumber(StylesStore::NumberIds::ButtonHeight));
    for (const auto& cfg : infoConfig)
    {
        const auto infoBounds = bounds.removeFromTop(buttonHeight);
        cfg.slider->setBounds(infoBounds);
        bounds.removeFromTop(layoutGutter);
    }
}

int EnvelopeInfo::getPreferredHeight() const
{
    const auto layoutGutter = static_cast<int>(
        stylesStore.getNumber(StylesStore::NumberIds::LayoutGutter));
    const auto buttonHeight = static_cast<int>(
        stylesStore.getNumber(StylesStore::NumberIds::ButtonHeight));
    return buttonHeight * numEnvSteps + layoutGutter * (numEnvSteps - 1);
}

void EnvelopeInfo::parentHierarchyChanged()
{
    for (const auto& cfg : infoConfig)
    {
        cfg.slider->lookAndFeelChanged();
    }
    Component::parentHierarchyChanged();
}
