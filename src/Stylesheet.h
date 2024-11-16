#pragma once

#include <juce_core/juce_core.h>
#include "StylesStore.h"

struct Stylesheet
{
	const static std::map<StylesStore::ColorIds, juce::uint32> styleColours;
	const static std::map<StylesStore::NumberIds, float> styleNumbers;
};
