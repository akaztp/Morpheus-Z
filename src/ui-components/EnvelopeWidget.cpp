#include "EnvelopeWidget.h"

EnvelopeWidget::EnvelopeWidget(
    AppState& appState,
    const StylesStore& stylesStore)
    : StyledComponent(stylesStore)
{
    initEnvelopeDisplay(appState);
    initEnvelopeInfo(appState);
}

void EnvelopeWidget::initEnvelopeDisplay(AppState& appState)
{
    envelopeDisplay = std::make_unique<EnvelopeDisplay>(appState, stylesStore);
    addAndMakeVisible(*envelopeDisplay);
}

void EnvelopeWidget::initEnvelopeInfo(AppState& appState)
{
    envelopeInfo = std::make_unique<EnvelopeInfo>(appState, stylesStore);
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
