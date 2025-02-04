#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include "../StylesStore.h"
#include "../ValueMonitor.h"
#include "../Global.h"
#include "../AppState.h"
#include "WaveformDisplay.h"
#include "StyledComponent.h"
#include "FramedButton.h"
#include "CustomSymbols.h"
#include "SwitchButton.h"
#include "WaveformBackground.h"
#include "WaveformInput.h"
#include "../WaveformPreset.h"


struct ButtonInfo
{
    ButtonInfo(
        const WaveformPreset::Type preset,
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

    const WaveformPreset::Type preset;
    const juce::String name;
    const unsigned char* iconData;
    const size_t iconDataSize;
    std::unique_ptr<FramedButton> button;
    std::unique_ptr<juce::Path> icon;
};

class WaveformWidget :
    public StyledComponent,
    public juce::Component
{
public:
    typedef std::function<void(
        const juce::Point<int>& from,
        const juce::Point<int>& to,
        int waveformNum)> DrawCallback;
    typedef std::function<void(
        WaveformPreset::Type preset,
        int waveformNum)> PresetCallback;

    explicit WaveformWidget(
        AppState& appState,
        const StylesStore& stylesStore,
        const DrawCallback& onDrawCallback,
        const PresetCallback& onPresetCallback);

    void resized() override;

    void waveformChanged(int waveNum) const;

    int getPreferredWidth() const;
    int getPreferredHeight() const;

    int getWaveformHeight() const;
    int getWaveformWidth() const;

private:
    int selectedWaveform = -1;
    std::vector<std::unique_ptr<WaveformDisplay>> waveformDisplays;
    std::unique_ptr<WaveformDisplay> waveformMorphMonitor;
    juce::AudioSampleBuffer waveformMorph{1, WAVEFORM_SIZE};

    std::vector<std::unique_ptr<FramedButton>> waveformSelectors;
    std::unique_ptr<WaveformInput> waveformInput;
    std::unique_ptr<WaveformBackground> waveformBackground;
    std::unique_ptr<juce::Path> loopButtonIcon;
    std::unique_ptr<SwitchButton> loopButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> loopButtonAttachment;

    ButtonInfo presetButtonsConfig[4] = {
        ButtonInfo{
            WaveformPreset::Type::Sine, "Sine", CustomSymbols::sinePathData, sizeof(CustomSymbols::sinePathData)},
        ButtonInfo{
            WaveformPreset::Type::Square, "Square", CustomSymbols::squarePathData, sizeof(CustomSymbols::squarePathData)
        },
        ButtonInfo{
            WaveformPreset::Type::Triangle, "Triangle", CustomSymbols::trianglePathData,
            sizeof(CustomSymbols::trianglePathData)
        },
        ButtonInfo{
            WaveformPreset::Type::Sawtooth, "Sawtooth", CustomSymbols::sawToothFWPathData,
            sizeof(CustomSymbols::sawToothFWPathData)
        },
    };

    void initWaveformBackground(AppState& appState);
    void initWaveformSelectors(int numSelectors);
    void initWaveformDisplays(AppState& appState);
    void initWaveformMorphMonitor(AppState& appState);
    void initPresetButtons(PresetCallback onPresetCallback);
    void initLoopButton(AppState& appState);
    void initWaveformInput(const DrawCallback& onDrawCallback);
    void selectWaveform(int waveformNum);
    int getControlsHeight() const;
    void updateMorphMonitor(
        double monitorMorphPosition,
        AppState& appState);
};
