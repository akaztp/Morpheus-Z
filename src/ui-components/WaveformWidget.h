#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include "../StylesStore.h"
#include "WaveformDisplay.h"
#include "StyledComponent.h"
#include "FramedButton.h"
#include "CustomSymbols.h"
#include "SwitchButton.h"
#include "WaveformBackground.h"
#include "WaveformInput.h"


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
          icon(nullptr)
    {
    }

    const WaveformPreset preset;
    const juce::String name;
    const unsigned char* iconData;
    const size_t iconDataSize;
    std::unique_ptr<FramedButton> button;
    std::unique_ptr<juce::Path> icon;
};

class WaveformWidget : public StyledComponent, public juce::Component
{
public:
    typedef std::function<void(
        const juce::Point<int>& from,
        const juce::Point<int>& to,
        int waveformNum)> DrawCallback;
    typedef std::function<void(
        WaveformPreset preset,
        int waveformNum)> PresetCallback;

    explicit WaveformWidget(
        juce::AudioProcessorValueTreeState& apvts,
        const StylesStore& stylesStore,
        juce::AudioThumbnailCache& thumbnailCache,
        juce::AudioFormatManager& formatManager,
        DrawCallback onDrawCallback,
        PresetCallback onPresetCallback);

    void resized() override;

    void setSource(
        int waveNum,
        const juce::AudioBuffer<float>& newSource,
        double sampleRate,
        juce::int64 hashCode) const;

    int getWaveformHeight() const;

    int getWaveformWidth() const;

private:
    static constexpr int numWaveforms = 2;
    int selectedWaveform = -1;
    std::unique_ptr<WaveformDisplay> waveformDisplays[numWaveforms];
    std::unique_ptr<FramedButton> waveformSelectors[numWaveforms];
    std::unique_ptr<WaveformInput> waveformInput;
    std::unique_ptr<WaveformBackground> waveformBackground;
    std::unique_ptr<juce::Path> loopButtonIcon;
    std::unique_ptr<SwitchButton> loopButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> loopButtonAttachment;

    ButtonInfo presetButtonsConfig[4] = {
        ButtonInfo{WaveformPreset::Sine, "Sine", CustomSymbols::sinePathData, sizeof(CustomSymbols::sinePathData)},
        ButtonInfo{
            WaveformPreset::Square, "Square", CustomSymbols::squarePathData, sizeof(CustomSymbols::squarePathData)
        },
        ButtonInfo{
            WaveformPreset::Triangle, "Triangle", CustomSymbols::trianglePathData,
            sizeof(CustomSymbols::trianglePathData)
        },
        ButtonInfo{
            WaveformPreset::Sawtooth, "Sawtooth", CustomSymbols::sawToothFWPathData,
            sizeof(CustomSymbols::sawToothFWPathData)
        },
    };

    void initWaveformBackground();
    void initWaveformSelectors();
    void initWaveformDisplays(juce::AudioThumbnailCache& thumbnailCache, juce::AudioFormatManager& formatManager);
    void initPresetButtons(PresetCallback onPresetCallback);
    void initLoopButton(juce::AudioProcessorValueTreeState& apvts);
    void initWaveformInput(DrawCallback onDrawCallback);
    void selectWaveform(int waveformNum);
};
