#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include "../StylesStore.h"
#include "WaveformDisplay.h"
#include "StyledComponent.h"
#include "WaveformPresetButton.h"
#include "CustomSymbols.h"

enum class WaveformPreset
{
    Sine = 0,
    Square,
    Triangle,
    Sawtooth,
};

struct ButtonInfo
{
    ButtonInfo(
        const WaveformPreset preset,
        const juce::String& name,
        const unsigned char* iconData,
        const size_t iconDataSize)
        : preset(preset),
          name(name),
          iconData(iconData),
          iconDataSize(iconDataSize),
          button(nullptr),
          path(nullptr)
    {
    }

    const WaveformPreset preset;
    const juce::String name;
    const unsigned char* iconData;
    const size_t iconDataSize;
    std::unique_ptr<WaveformPresetButton> button;
    std::unique_ptr<juce::Path> path;
};

class WaveformWidget : public StyledComponent, public juce::Component
{
public:
    explicit WaveformWidget(
        const StylesStore& stylesStore,
        juce::AudioThumbnailCache& thumbnailCache,
        juce::AudioFormatManager& formatManager,
        WaveformDisplay::DrawCallback onDrawCallback,
        std::function<void(WaveformPreset preset)> onClickCallback);

    void resized() override;

    void setSource(const juce::AudioBuffer<float>& newSource, double sampleRate, juce::int64 hashCode);

    int getWaveformHeight() const;

    int getWaveformWidth() const;

protected:
    WaveformDisplay waveformDisplay;
    ButtonInfo presetButtonsConfig[4] = {
        ButtonInfo{WaveformPreset::Sine, "Sine", CustomSymbols::sinePathData, sizeof(CustomSymbols::sinePathData)},
        ButtonInfo{WaveformPreset::Square, "Square", CustomSymbols::squarePathData, sizeof(CustomSymbols::squarePathData)},
        ButtonInfo{WaveformPreset::Triangle, "Triangle", CustomSymbols::trianglePathData, sizeof(CustomSymbols::trianglePathData)},
        ButtonInfo{WaveformPreset::Sawtooth, "Sawtooth", CustomSymbols::sawToothFWPathData, sizeof(CustomSymbols::sawToothFWPathData)},
    };
};
