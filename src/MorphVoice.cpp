#include "AppParams.h"
#include "MorphVoice.h"
#include "MorphSound.h"

MorphVoice::MorphVoice(juce::AudioProcessorValueTreeState& apvts)
{
    loopModeParam = dynamic_cast<juce::AudioParameterBool*>(
        apvts.getParameter(AppParams::loopMode));

    morphDurationParam = dynamic_cast<juce::AudioParameterFloat*>(
        apvts.getParameter(AppParams::morphDuration));
}

bool MorphVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<MorphSound*>(sound) != nullptr;
}

void MorphVoice::startNote(
    int midiNoteNumber,
    float velocity,
    juce::SynthesiserSound* sound,
    int currentPitchWheelPosition)
{
    morphSound = dynamic_cast<MorphSound*>(sound);
    if (morphSound != nullptr)
    {
        auto waveformsPerSecond = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        double sampleRate = getSampleRate();
        waveformDelta = waveformsPerSecond / sampleRate;
        waveformPosition = 0.0;

        morphPosition = 0.0;
        morphDelta = 1.0 / (sampleRate * morphDurationParam->get());

        level = velocity * maxLevel;
    }
}

void MorphVoice::stopNote(float velocity, bool allowTailOff)
{
    clearCurrentNote();
    waveformDelta = 0.0;
    waveformPosition = 0.0;
    morphPosition = 0.0;
    morphDelta = 0.0;
}

void MorphVoice::renderNextBlock(
    juce::AudioSampleBuffer& outputBuffer,
    int startSample,
    int numSamples)
{
    if (waveformDelta != 0.0)
    {
        double loopMode = loopModeParam->get() ? -1 : 0;
        while (--numSamples >= 0)
        {
            auto currentSample = morphSound->getSample(waveformPosition, morphPosition) * level;

            for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                outputBuffer.addSample(i, startSample, currentSample);

            waveformPosition += waveformDelta;
            if (waveformPosition >= 1.0)
            {
                waveformPosition -= 1.0;
            }

            if (morphPosition < 0.0)
            {
                morphPosition = 0.0;
                morphDelta *= loopMode;
            }
            else if (morphPosition > 1.0)
            {
                morphPosition = 1.0;
                morphDelta *= loopMode;
            }
            else
            {
                morphPosition += morphDelta;
            }
            ++startSample;
        }
    }
}

void MorphVoice::pitchWheelMoved(int newPitchWheelValue)
{
}

void MorphVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
}
