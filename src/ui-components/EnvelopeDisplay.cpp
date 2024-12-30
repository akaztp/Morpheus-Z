#include "EnvelopeDisplay.h"
#include "../AppParams.h"

EnvelopeDisplay::EnvelopeDisplay(
    juce::AudioProcessorValueTreeState& apvts,
    const StylesStore& stylesStore) : stylesStore(stylesStore)
{
    attackParam = dynamic_cast<juce::AudioParameterFloat*>(
        apvts.getParameter(AppParams::attack));
    attackRange = &attackParam->getNormalisableRange();
    attackParam->addListener(this);

    decayParam = dynamic_cast<juce::AudioParameterFloat*>(
        apvts.getParameter(AppParams::decay));
    decayRange = &decayParam->getNormalisableRange();
    decayParam->addListener(this);

    sustainParam = dynamic_cast<juce::AudioParameterFloat*>(
        apvts.getParameter(AppParams::sustain));
    sustainRange = &sustainParam->getNormalisableRange();
    sustainParam->addListener(this);

    releaseParam = dynamic_cast<juce::AudioParameterFloat*>(
        apvts.getParameter(AppParams::release));
    releaseRange = &releaseParam->getNormalisableRange();
    releaseParam->addListener(this);
}

EnvelopeDisplay::~EnvelopeDisplay()
{
    attackParam->removeListener(this);
    decayParam->removeListener(this);
    sustainParam->removeListener(this);
    releaseParam->removeListener(this);
}

void EnvelopeDisplay::paint(juce::Graphics& g)
{
    const auto bounds = getLocalBounds();
    g.setColour(stylesStore.getColor(StylesStore::ColorIds::WidgetBackground));
    g.fillRect(bounds);
    g.setColour(stylesStore.getColor(StylesStore::ColorIds::Outline));
    g.drawRect(bounds, static_cast<int>(stylesStore.getNumber(StylesStore::NumberIds::BorderWidth)));

    paintGraph(g);
}

void EnvelopeDisplay::paintGraph(juce::Graphics& g)
{
    const auto bounds = getLocalBounds().reduced(graphMargin);
    const auto bottom = static_cast<float>(bounds.getBottom());
    const auto top = static_cast<float>(bounds.getY());
    const auto left = static_cast<float>(bounds.getX());
    const auto width = static_cast<float>(bounds.getWidth());
    const float third = width / 3.0f;
    const float releaseStartX = left + third + third;
    const float maxAttackLength = third - width / 16.0f;
    const float maxDecayLength = maxAttackLength;

    attackX = juce::jmap<float>(
        attackParam->get(),
        attackRange->start,
        attackRange->end,
        left,
        left + maxAttackLength);

    decayX = juce::jmap<float>(
        decayParam->get(),
        decayRange->start,
        decayRange->end,
        attackX,
        attackX + maxDecayLength);

    sustainY = juce::jmap<float>(
        sustainParam->get(),
        sustainRange->start,
        sustainRange->end,
        bottom,
        top
    );

    releaseX = juce::jmap<float>(
        releaseParam->get(),
        releaseRange->start,
        releaseRange->end,
        releaseStartX,
        width
    );

    g.setColour(stylesStore.getColor(StylesStore::ColorIds::DefaultText));
    g.drawLine(left, bottom, attackX, top);
    g.drawLine(attackX, top, decayX, sustainY);
    g.drawLine(decayX, sustainY, releaseStartX, sustainY);
    g.drawLine(releaseStartX, sustainY, releaseX, bottom);
}

void EnvelopeDisplay::parameterValueChanged(int parameterIndex, float newValue)
{
    repaint();
}

void EnvelopeDisplay::parameterGestureChanged(int parameterIndex, bool gestureIsStarting)
{
}
