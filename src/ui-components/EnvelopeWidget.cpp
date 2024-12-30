#include "EnvelopeWidget.h"

EnvelopeWidget::EnvelopeWidget(
    juce::AudioProcessorValueTreeState& apvts,
    const StylesStore& stylesStore
): stylesStore(stylesStore)
{
    initEnvelopeDisplay(apvts);
    initEnvelopeInfo(apvts);
}

void EnvelopeWidget::initEnvelopeDisplay(
    juce::AudioProcessorValueTreeState& apvts)
{
    envelopeDisplay = std::make_unique<EnvelopeDisplay>(apvts, stylesStore);
    addAndMakeVisible(*envelopeDisplay);
}

void EnvelopeWidget::initEnvelopeInfo(
    juce::AudioProcessorValueTreeState& apvts)
{
    envelopeInfo = std::make_unique<EnvelopeInfo>(apvts, stylesStore);
    addAndMakeVisible(*envelopeInfo);
}

void EnvelopeWidget::resized()
{
    Component::resized();

    auto bounds = getLocalBounds();
    envelopeInfo->setBounds(bounds.removeFromRight(bounds.getWidth() / 4));
    const auto layoutGutter = static_cast<int>(
        stylesStore.getNumber(StylesStore::NumberIds::LayoutGutter));
    bounds.removeFromRight(layoutGutter);
    envelopeDisplay->setBounds(bounds);
}

int EnvelopeWidget::getPreferredHeight() const
{
    return envelopeInfo->getPreferredHeight();
};
