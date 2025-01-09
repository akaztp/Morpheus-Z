#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include "Stylesheet.h"
#include "StylesStore.h"

const std::map<StylesStore::ColorIds, juce::uint32> Stylesheet::styleColours =
{
    {StylesStore::ColorIds::WindowBackground, 0xff1A1D29},
    {StylesStore::ColorIds::WidgetBackground, 0xff101010},
    {StylesStore::ColorIds::MenuBackground, 0xff6E4D4D},
    {StylesStore::ColorIds::Outline, 0xff817B9B},
    {StylesStore::ColorIds::DefaultText, 0xffC4B0CC},
    {StylesStore::ColorIds::DefaultFill, 0xff4E4A60},
    {StylesStore::ColorIds::HighlightedText, 0xffC4B0CC},
    {StylesStore::ColorIds::HighlightedFill, 0xffF3D4FF},
    {StylesStore::ColorIds::MenuText, 0xffC4B0CC},
    {StylesStore::ColorIds::SelectedFill, 0xff817B9B},
    {StylesStore::ColorIds::WaveformDisplay_SelectedWave, 0xffF3D4FF},
    {StylesStore::ColorIds::WaveformDisplay_UnselectedWave, 0x40F3D4FF},
    {StylesStore::ColorIds::WaveformDisplay_origin, 0x40817B9B},
};

const std::map<StylesStore::NumberIds, float> Stylesheet::styleNumbers =
{
    {StylesStore::NumberIds::LayoutMargin, 64.0f},
    {StylesStore::NumberIds::LayoutGutter, 6.0f},
    {StylesStore::NumberIds::LayoutHeaderHeight, 120.0f},
    {StylesStore::NumberIds::WidgetMargin, 32.0f},
    {StylesStore::NumberIds::CornerRadius, 0.0f},
    {StylesStore::NumberIds::KeyboardHeight, 54.0f},
    {StylesStore::NumberIds::ButtonHeight, 32.0f},
    {StylesStore::NumberIds::BorderWidth, 1.0f},
    {StylesStore::NumberIds::IconStrokeWidth, 1.0f},
    {StylesStore::NumberIds::ButtonFontHeight, 15.0f},
    {StylesStore::NumberIds::EnvelopeHandleOpacity, 0.8f},
};

const std::map<int, juce::uint32> Stylesheet::lookAndFeel =
{
    {juce::KeyboardComponentBase::ColourIds::upDownButtonBackgroundColourId, 0x604E4A60},
    {juce::KeyboardComponentBase::ColourIds::upDownButtonArrowColourId, 0xA0817B9B},

    {juce::MidiKeyboardComponent::ColourIds::whiteNoteColourId, 0xff101010},
    {juce::MidiKeyboardComponent::ColourIds::blackNoteColourId, 0xff000000},
    {juce::MidiKeyboardComponent::ColourIds::keySeparatorLineColourId, 0xff4E4A60},
    {juce::MidiKeyboardComponent::ColourIds::mouseOverKeyOverlayColourId, 0xff817B9B},
    {juce::MidiKeyboardComponent::ColourIds::keyDownOverlayColourId, 0xff817B9B},
    {juce::MidiKeyboardComponent::ColourIds::textLabelColourId, 0xff4E4A60},
    {juce::MidiKeyboardComponent::ColourIds::shadowColourId, 0xff101010},

    {juce::Slider::ColourIds::textBoxOutlineColourId, 0},
};
