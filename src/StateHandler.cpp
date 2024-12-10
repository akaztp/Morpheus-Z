#include "StateHandler.h"

void StateHandler::getStateInformation(
    juce::MemoryBlock& destData,
    const juce::AudioSampleBuffer& waveformA,
    const juce::AudioSampleBuffer& waveformB,
    juce::AudioProcessorValueTreeState& apvts)
{
    /* get the app state into XML */
    auto appParamsXml = apvts.copyState().createXml();

    /* Get the waveform data into XML */
    auto waveformAXml = getWaveformXml(Elements::waveformA, waveformA);
    auto waveformBXml = getWaveformXml(Elements::waveformB, waveformB);

    /* combine the XMLs */
    auto stateXml = std::make_unique<juce::XmlElement>(Elements::state);
    stateXml->addChildElement(appParamsXml.release());
    stateXml->addChildElement(waveformAXml.release());
    stateXml->addChildElement(waveformBXml.release());

    juce::AudioPluginInstance::copyXmlToBinary(*stateXml, destData);
}

std::unique_ptr<juce::XmlElement> StateHandler::getWaveformXml(
    const juce::String& name,
    const juce::AudioSampleBuffer& waveform)
{
    const float* waveformData = waveform.getReadPointer(0);
    juce::MemoryBlock waveformBlock{
        waveformData,
        waveform.getNumSamples() * sizeof(float)};
    auto waveformEncoded = waveformBlock.toBase64Encoding();

    auto waveformXml = std::make_unique<juce::XmlElement>(name);
    waveformXml->addTextElement(waveformEncoded);
    return waveformXml;
}

void StateHandler::setStateInformation(
    const void* data,
    const int sizeInBytes,
    juce::AudioSampleBuffer& waveformA,
    juce::AudioSampleBuffer& waveformB,
    juce::AudioProcessorValueTreeState& apvts)
{
    const auto stateXml = juce::AudioPluginInstance::getXmlFromBinary(data, sizeInBytes);
    if (stateXml == nullptr)
    {
        DBG("Failed to decode XML from state data");
        return;
    }

    setWaveformFromXml(stateXml.get(), Elements::waveformA, waveformA);
    setWaveformFromXml(stateXml.get(), Elements::waveformB, waveformB);

    const auto appParamsName = apvts.state.getType();
    juce::XmlElement* appParamsXml = stateXml->getChildByName(appParamsName);
    if (appParamsXml == nullptr)
    {
        DBG("Failed to find app params XML element: " << appParamsName);
        return;
    }
    apvts.replaceState(juce::ValueTree::fromXml(*appParamsXml));
}

void StateHandler::setWaveformFromXml(
    const juce::XmlElement* stateXml,
    const juce::String& name,
    juce::AudioSampleBuffer& waveform)
{
    const juce::XmlElement* waveformXml = stateXml->getChildByName(name);
    if (waveformXml == nullptr)
    {
        DBG("Failed to find waveform XML: " << name);
        return;
    }

    juce::MemoryBlock waveformBlock;
    auto success = waveformBlock.fromBase64Encoding(waveformXml->getAllSubText());
    if (!success)
    {
        DBG("Failed to decode waveform base64 data for waveform: " << name);
        return;
    }
    const int numSamples = waveformBlock.getSize() / sizeof(float);
    waveform.setSize(1, numSamples);
    float* writePointer = waveform.getWritePointer(0);
    std::copy_n(
        static_cast<const float*>(waveformBlock.getData()),
        numSamples,
        writePointer);
}
