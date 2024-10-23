#pragma once

#include "juce_audio_basics/juce_audio_basics.h"

struct SineWaveSound   : public juce::SynthesiserSound
{
	SineWaveSound() {}

	bool appliesToNote    (int) override        { return true; }
	bool appliesToChannel (int) override        { return true; }
};
