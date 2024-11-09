#include "MorphSound.h"

void MorphSound::setWaveA(juce::AudioSampleBuffer& waveform)
{
	delete waveformA;
	waveformASize = waveform.getNumSamples();
	waveformA = wrapWaveform(waveform);
}

void MorphSound::setWaveB(juce::AudioSampleBuffer& waveform)
{
	delete waveformB;
	waveformBSize = waveform.getNumSamples();
	waveformB = wrapWaveform(waveform);
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
	delete waveformA;
	delete waveformB;
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
