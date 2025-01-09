#include "EnvelopeDisplay.h"
#include "../AppParams.h"

EnvelopeDisplay::EnvelopeDisplay(
    juce::AudioProcessorValueTreeState& apvts,
    const StylesStore& stylesStore) : stylesStore(stylesStore)
{
    initParams(apvts);
    initEnvelopeHandles();
}

void EnvelopeDisplay::initParams(juce::AudioProcessorValueTreeState& apvts)
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

void EnvelopeDisplay::initEnvelopeHandles()
{
    attackHandle = std::make_unique<EnvelopeHandle>(
        stylesStore,
        [this]() { attackParam->beginChangeGesture(); },
        [this]() { attackParam->endChangeGesture(); },
        [this](const int x, int)
        {
            attackParam->setValueNotifyingHost(juce::jmap<float>(
                x,
                attackDisplayRange.getStart(), attackDisplayRange.getEnd(),
                0.0f, 1.0f));
        });
    addAndMakeVisible(*attackHandle);

    decayHandle = std::make_unique<EnvelopeHandle>(
        stylesStore,
        [this]()
        {
            decayParam->beginChangeGesture();
            sustainParam->beginChangeGesture();
        },
        [this]()
        {
            decayParam->endChangeGesture();
            sustainParam->endChangeGesture();
        },
        [this](const int x, const int y)
        {
            decayParam->setValueNotifyingHost(juce::jmap<float>(
                x,
                decayDisplayRange.getStart(), decayDisplayRange.getEnd(),
                0.0f, 1.0f));
            sustainParam->setValueNotifyingHost(juce::jmap<float>(
                y,
                sustainDisplayRange.getEnd(), sustainDisplayRange.getStart(),
                0.0f, 1.0f));
        });
    addAndMakeVisible(*decayHandle);

    sustainHandle = std::make_unique<EnvelopeHandle>(
        stylesStore,
        [this]() { sustainParam->beginChangeGesture(); },
        [this]() { sustainParam->endChangeGesture(); },
        [this](int, const int y)
        {
            sustainParam->setValueNotifyingHost(juce::jmap<float>(
                y,
                sustainDisplayRange.getEnd(), sustainDisplayRange.getStart(),
                0.0f, 1.0f));
        });
    addAndMakeVisible(*sustainHandle);

    releaseHandle = std::make_unique<EnvelopeHandle>(
        stylesStore,
        [this]() { releaseParam->beginChangeGesture(); },
        [this]() { releaseParam->endChangeGesture(); },
        [this](const int x, int)
        {
            releaseParam->setValueNotifyingHost(juce::jmap<float>(
                x,
                releaseDisplayRange.getStart(), releaseDisplayRange.getEnd(),
                0.0f, 1.0f));
        });
    addAndMakeVisible(*releaseHandle);
}

EnvelopeDisplay::~EnvelopeDisplay()
{
    attackParam->removeListener(this);
    decayParam->removeListener(this);
    sustainParam->removeListener(this);
    releaseParam->removeListener(this);
}

void EnvelopeDisplay::resized()
{
    Component::resized();
    precalculate();
    calculateDisplayRanges();
    updateHandlesPositions();
}

juce::Rectangle<int> EnvelopeDisplay::getUsefulBounds() const
{
    return getLocalBounds().reduced(graphMargin);
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

void EnvelopeDisplay::paintGraph(juce::Graphics& g) const
{
    const auto bounds = getUsefulBounds();
    const auto bottom = static_cast<float>(bounds.getBottom());
    const auto left = static_cast<float>(bounds.getX());

    juce::Path path;
    path.startNewSubPath(left, bottom);
    path.lineTo(attackHandle->getBounds().getCentre().toFloat());
    path.lineTo(decayHandle->getBounds().getCentre().toFloat());
    path.lineTo(sustainHandle->getBounds().getCentre().toFloat());
    path.lineTo(releaseHandle->getBounds().getCentre().toFloat());
    g.setColour(stylesStore.getColor(StylesStore::ColorIds::DefaultText));
    g.strokePath(
        path,
        juce::PathStrokeType(stylesStore.getNumber(StylesStore::NumberIds::BorderWidth)));
}

void EnvelopeDisplay::precalculate()
{
    const auto bounds = getUsefulBounds();
    const auto width = static_cast<float>(bounds.getWidth());
    const auto left = static_cast<float>(bounds.getX());
    usefulThirdOfWidth = width / 3.0f;
    sustainPosX = left + usefulThirdOfWidth + usefulThirdOfWidth;
    maxAttackDecayLength = usefulThirdOfWidth - width / 16.0f;
}

void EnvelopeDisplay::calculateDisplayRanges()
{
    const auto bounds = getUsefulBounds();
    const auto top = static_cast<float>(bounds.getY());
    const auto bottom = static_cast<float>(bounds.getBottom());
    const auto left = static_cast<float>(bounds.getX());
    const auto right = static_cast<float>(bounds.getRight());

    const auto attackPosX = juce::jmap<float>(
        attackRange->convertTo0to1(attackParam->get()),
        left,
        left + maxAttackDecayLength);

    attackDisplayRange = juce::Range<float>(left, left + maxAttackDecayLength);
    decayDisplayRange = juce::Range<float>(attackPosX, attackPosX + maxAttackDecayLength);
    sustainDisplayRange = juce::Range<float>(top, bottom);
    releaseDisplayRange = juce::Range<float>(sustainPosX, right);
}

void EnvelopeDisplay::updateHandlesPositions() const
{
    const auto bounds = getUsefulBounds();
    const auto top = static_cast<float>(bounds.getY());
    const auto bottom = static_cast<float>(bounds.getBottom());

    const float sustainLevel = 1.0f - sustainRange->convertTo0to1(sustainParam->get());
    setHandleBounds(
        attackHandle.get(),
        attackDisplayRange,
        juce::Range<float>(top, top),
        attackRange->convertTo0to1(attackParam->get()),
        0
    );
    setHandleBounds(
        decayHandle.get(),
        decayDisplayRange,
        sustainDisplayRange,
        decayRange->convertTo0to1(decayParam->get()),
        sustainLevel);
    setHandleBounds(
        sustainHandle.get(),
        juce::Range<float>(sustainPosX, sustainPosX),
        sustainDisplayRange,
        decayRange->convertTo0to1(decayParam->get()),
        sustainLevel);
    setHandleBounds(
        releaseHandle.get(),
        releaseDisplayRange,
        juce::Range<float>(bottom, bottom),
        releaseRange->convertTo0to1(releaseParam->get()),
        0);
}

void EnvelopeDisplay::setHandleBounds(
    EnvelopeHandle* handle,
    const juce::Range<float>& hRange,
    const juce::Range<float>& vRange,
    const float xPosition, // 0 to 1
    const float yPosition // 0 to 1
)
{
    const auto x = juce::jmap<float>(xPosition, hRange.getStart(), hRange.getEnd());
    const auto y = juce::jmap<float>(yPosition, vRange.getStart(), vRange.getEnd());
    handle->setBounds(x - handleSize / 2.0f, y - handleSize / 2.0f, handleSize, handleSize);
}

void EnvelopeDisplay::parameterValueChanged(int parameterIndex, float newValue)
{
    calculateDisplayRanges();
    updateHandlesPositions();
    repaint();
}

void EnvelopeDisplay::parameterGestureChanged(int parameterIndex, bool gestureIsStarting)
{
}
