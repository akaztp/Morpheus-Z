#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../StylesStore.h"

class StyledComponent : public juce::Component
{
public:
	explicit StyledComponent(const StylesStore& stylesStore);

protected:
	const StylesStore& stylesStore;
};
