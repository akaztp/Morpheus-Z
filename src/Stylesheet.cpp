#pragma once

#include "Stylesheet.h"
#include "StylesStore.h"

const std::map<StylesStore::ColorIds, juce::uint32> Stylesheet::styleColours =
		{
				{ StylesStore::ColorIds::WindowBackground,      0xff594C4C },
				{ StylesStore::ColorIds::WidgetBackground,      0xff95423D },
				{ StylesStore::ColorIds::MenuBackground,        0xff594C4C },
				{ StylesStore::ColorIds::Outline,               0xff242424 },
				{ StylesStore::ColorIds::DefaultText,           0xff060606 },
				{ StylesStore::ColorIds::DefaultFill,           0xff95423D },
				{ StylesStore::ColorIds::HighlightedText,       0xff060606 },
				{ StylesStore::ColorIds::HighlightedFill,       0xff95423D },
				{ StylesStore::ColorIds::MenuText,              0xff060606 },
				{ StylesStore::ColorIds::WaveformDisplay_signal,     0xffFF97CE },
				{ StylesStore::ColorIds::WaveformDisplay_zero,       0xff555555 },
				{ StylesStore::ColorIds::WaveformDisplay_background, 0xff242424 },
		};

const std::map<StylesStore::NumberIds, float> Stylesheet::styleNumbers =
		{
				{ StylesStore::NumberIds::LayoutMargin,   24.0f },
				{ StylesStore::NumberIds::LayoutGutter,   3.0f },
				{ StylesStore::NumberIds::CornerRadius,   2.0f },
				{ StylesStore::NumberIds::KeyboardHeight, 54.0f },
		};
