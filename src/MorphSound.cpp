#include "MorphSound.h"

void MorphSound::setWave(const int waveformNum, juce::AudioSampleBuffer& waveform)
{
    waveformsLock.enter();
    delete waveforms[waveformNum];
    waveformSizes[waveformNum] = waveform.getNumSamples();
    waveforms[waveformNum] = wrapWaveform(waveform);
    waveformsLock.exit();
}

bool MorphSound::appliesToNote(int)
{
    return true;
}

bool MorphSound::appliesToChannel(int)
{
    return true;
}

MorphSound::~MorphSound()
{
    for (auto const waveform : waveforms)
    {
        delete waveform;
    }
}

juce::AudioSampleBuffer* MorphSound::wrapWaveform(juce::AudioSampleBuffer& waveform)
{
    auto* copiedWaveform = new juce::AudioSampleBuffer(waveform);
    auto size = copiedWaveform->getNumSamples();
    copiedWaveform->setSize(copiedWaveform->getNumChannels(), size + 1, true);
    auto* samples = copiedWaveform->getWritePointer(0);
    samples[size] = samples[0];
    return copiedWaveform;
}
