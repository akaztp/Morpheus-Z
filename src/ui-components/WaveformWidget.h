#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include "../StylesStore.h"
#include "WaveformDisplay.h"
#include "StyledComponent.h"
#include "WaveformPresetButton.h"
#include "CustomSymbols.h"


struct ButtonInfo
{
    ButtonInfo(
        const juce::String& name,
        const unsigned char* iconData,
        const size_t iconDataSize)
        : name(name),
          iconData(iconData),
          iconDataSize(iconDataSize),
          button(nullptr),
          path(nullptr)
    {
    }

    const juce::String name;
    const unsigned char* iconData;
    const size_t iconDataSize;
    std::unique_ptr<WaveformPresetButton> button;
    std::unique_ptr<juce::Path> path;
};

class WaveformWidget : public StyledComponent
{
public:
    explicit WaveformWidget(
        const StylesStore& stylesStore,
        juce::AudioThumbnailCache& thumbnailCache,
        juce::AudioFormatManager& formatManager,
        DrawCallback onDrawCallback);

    void resized() override;

    void setSource(const juce::AudioBuffer<float>& newSource, double sampleRate, juce::int64 hashCode);

    int getWaveformHeight() const;

    int getWaveformWidth() const;

protected:
    WaveformDisplay waveformDisplay;
    ButtonInfo presetButtonsConfig[5] = {
        ButtonInfo{"Sinewave", CustomSymbols::sinePathData, sizeof(CustomSymbols::sinePathData)},
        ButtonInfo{"Square", CustomSymbols::squarePathData, sizeof(CustomSymbols::squarePathData)},
        ButtonInfo{"Triangle", CustomSymbols::trianglePathData, sizeof(CustomSymbols::trianglePathData)},
        ButtonInfo{"Sawtooth F", CustomSymbols::sawToothFWPathData, sizeof(CustomSymbols::sawToothFWPathData)},
        ButtonInfo{"Sawtooth B", CustomSymbols::sawToothBWPathData, sizeof(CustomSymbols::sawToothBWPathData)}
    };
};
