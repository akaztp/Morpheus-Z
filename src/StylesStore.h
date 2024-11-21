#pragma once

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

class StylesStore
{
public:
	enum class ColorIds
	{
		// These must come all from juce::LookAndFeel_V4::ColourScheme::UIColour
		WindowBackground = 0,
		WidgetBackground,
		MenuBackground,
		Outline,
		DefaultText,
		DefaultFill,
		HighlightedText,
		HighlightedFill,
		MenuText,

		WaveformDisplay_signal,
		WaveformDisplay_zero,
		WaveformDisplay_background,

		numIds,
	};

	enum class NumberIds
	{
		LayoutMargin = 0,
		LayoutGutter,
		CornerRadius,
		KeyboardHeight,

		numIds
	};

	enum class TextualIds
	{
		FontName = 0,

		numIds
	};

	void fillStore(
			const std::map<StylesStore::ColorIds, juce::uint32>& styleColours,
			const std::map<StylesStore::NumberIds, float>& styleNumbers,
			juce::LookAndFeel_V4& laf);

	[[nodiscard]] juce::Colour getColor(ColorIds id) const;

	[[nodiscard]] float getNumber(NumberIds id) const;

	[[nodiscard]] juce::String getTextual(TextualIds id) const;

private:
	void setColor(ColorIds id, const juce::Colour& value);

	void setNumber(NumberIds id, float value);

	void setTextual(TextualIds id, juce::String& value);

	juce::Colour colors[static_cast<int>(ColorIds::numIds)]{};
	float numbers[static_cast<int>(NumberIds::numIds)]{};
	juce::String textuals[static_cast<int>(TextualIds::numIds)]{};
};
