#include "Global.h"
#include "AppState.h"
#include "MorphVoice.h"
#include "MorphSound.h"

MorphVoice::MorphVoice(
    const int id,
    AppState& appState,
    int& mostRecentActiveId):
    id(id),
    appState(appState),
    mostRecentActiveId(mostRecentActiveId)
{
    adsr.reset();
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
    juce::ignoreUnused(currentPitchWheelPosition);

    morphSound = dynamic_cast<MorphSound*>(sound);
    if (morphSound != nullptr)
    {
        auto waveformsPerSecond = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        double sampleRate = getSampleRate();
        waveformDelta = waveformsPerSecond / sampleRate;
        waveformPosition = 0.0;

        morphPosition = 0.0;
        morphDelta = 1.0 / (sampleRate * appState.audioParameters.morphDuration->get());

        level = velocity * maxLevel;

        triggerADSR();

        mostRecentActiveId = id;
    }
}

void MorphVoice::triggerADSR()
{
    adsr.reset();
    adsr.setSampleRate(getSampleRate());
    adsr.setParameters(
        juce::ADSR::Parameters(
            appState.audioParameters.attack->get(),
            appState.audioParameters.decay->get(),
            appState.audioParameters.sustain->get(),
            appState.audioParameters.release->get()
        ));
    adsr.noteOn();
}

void MorphVoice::stopNote(float velocity, bool allowTailOff)
{
    juce::ignoreUnused(velocity);
    adsr.noteOff();
    if (!allowTailOff)
    {
        stopVoice();
    }
}

void MorphVoice::stopVoice()
{
    adsr.reset();
    clearCurrentNote();
    if (mostRecentActiveId == id)
    {
        appState.monitorMorphPosition.setValue(MONITOR_MORPH_POSITION_OFF);
    }
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
    if (!adsr.isActive())
    {
        if (waveformDelta != 0.0)
        {
            // is not active but waveform is still sounding, so it just became inactive
            stopVoice();
        }
        return;
    }

    double loopMode = appState.audioParameters.loopMode->get() ? -1 : 0;
    while (--numSamples >= 0)
    {
        const auto currentSample = morphSound->getMorphedSample(waveformPosition, morphPosition) * level;
        auto currentADSR = adsr.getNextSample();

        for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
            outputBuffer.addSample(i, startSample, currentSample * currentADSR);

        if (mostRecentActiveId == id)
        {
            appState.monitorMorphPosition.setValue(morphPosition);
        }

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

void MorphVoice::pitchWheelMoved(int newPitchWheelValue)
{
    juce::ignoreUnused(newPitchWheelValue);
}

void MorphVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
    juce::ignoreUnused(controllerNumber, newControllerValue);
}
