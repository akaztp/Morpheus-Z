#include "MorphSound.h"

MorphSound::MorphSound(AppState& appState)
{
    jassert(numWaveforms == appState.waveforms.size());
    for (int i = 0; i < numWaveforms; ++i)
    {
        waveformSizes[i] = appState.waveforms[i].getNumSamples();
        waveforms[i] = &appState.waveforms[i];
    }
}

bool MorphSound::appliesToNote(int)
{
    return true;
}

bool MorphSound::appliesToChannel(int)
{
    return true;
}

MorphSound::~MorphSound() = default;
