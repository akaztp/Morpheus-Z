#include "StylesStore.h"
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

void StylesStore::fillStore(
		const std::map<StylesStore::ColorIds, juce::uint32>& styleColours,
		const std::map<StylesStore::NumberIds, float>& styleNumbers,
		juce::LookAndFeel_V4& laf)
{
	for (const auto& [colourId, colourVal]: styleColours)
	{
		setColor(colourId, juce::Colour(colourVal));
	}

	for (const auto& [numberId, numberVal]: styleNumbers)
	{
		setNumber(numberId, numberVal);
	}

	auto colourScheme = laf.getCurrentColourScheme();
	for (int i = juce::LookAndFeel_V4::ColourScheme::UIColour::numColours; --i >= 0;)
	{
		colourScheme.setUIColour(
				static_cast<juce::LookAndFeel_V4::ColourScheme::UIColour>(i),
				juce::Colour(styleColours.at(static_cast<StylesStore::ColorIds>(i))));
	}
	laf.setColourScheme(colourScheme); // to re-apply the colour scheme
}

[[nodiscard]] juce::Colour StylesStore::getColor(ColorIds id) const
{
	return { (colors[static_cast<int>(id)]) };
}

void StylesStore::setColor(ColorIds id, const juce::Colour& value)
{
	colors[static_cast<int>(id)] = juce::Colour(value);
}

[[nodiscard]] float StylesStore::getNumber(NumberIds id) const
{
	return numbers[static_cast<int>(id)];
}

void StylesStore::setNumber(NumberIds id, float value)
{
	numbers[static_cast<int>(id)] = value;
}

[[nodiscard]] juce::String StylesStore::getTextual(TextualIds id) const
{
	return { (textuals[static_cast<int>(id)]) };
}

void StylesStore::setTextual(TextualIds id, juce::String& value)
{
	textuals[static_cast<int>(id)] = juce::String(value);
}
