#include "MorpheusZProcessor.h"
#include "MorpheusZEditor.h"
#include "StateHandler.h"
#include "waveformPresets.h"

//==============================================================================
MorpheusZProcessor::MorpheusZProcessor()
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
				.withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
#endif
          .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      ),
      synthAudioSource(keyboardState, apvts, monitorMorphPosition)
{
    setWaveform(0, WaveformPreset::Sine);
    setWaveform(1, WaveformPreset::Triangle);
}

MorpheusZProcessor::~MorpheusZProcessor()
{
}

//==============================================================================
const juce::String MorpheusZProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MorpheusZProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
	return false;
#endif
}

bool MorpheusZProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
    return false;
#endif
}

bool MorpheusZProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
    return false;
#endif
}

double MorpheusZProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MorpheusZProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int MorpheusZProcessor::getCurrentProgram()
{
    return 0;
}

void MorpheusZProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String MorpheusZProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void MorpheusZProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void MorpheusZProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    synthAudioSource.prepareToPlay(samplesPerBlock, sampleRate);
}

std::unique_ptr<juce::AudioSampleBuffer> MorpheusZProcessor::getWaveformPreset(
    WaveformPreset preset,
    int size) const
{
    switch (preset)
    {
    case WaveformPreset::Sine:
        return waveformPresets::sine(size);
    case WaveformPreset::Square:
        return waveformPresets::square(size);
    case WaveformPreset::Triangle:
        return waveformPresets::triangle(size);
    case WaveformPreset::Sawtooth:
        return waveformPresets::sawTooth(size);
    }
    DBG(juce::String::formatted("An invalid preset wave was selected: %i", preset));
    jassertfalse;
    return waveformPresets::sine(size);
}

void MorpheusZProcessor::setWaveform(int waveformNum, const WaveformPreset preset)
{
    const std::unique_ptr<juce::AudioSampleBuffer> wave =
        getWaveformPreset(preset, WAVEFORM_SIZE);
    waveforms[waveformNum].copyFrom(0, 0, *wave, 0, 0, wave->getNumSamples());
    synthAudioSource.setWave(waveformNum, waveforms[waveformNum]);
    refreshWaveformEditor(waveformNum);
}

void MorpheusZProcessor::releaseResources()
{
    synthAudioSource.releaseResources();
}

bool MorpheusZProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused (layouts);
	return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

    return true;
#endif
}

void MorpheusZProcessor::processBlock(
    juce::AudioBuffer<float>& buffer,
    juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    juce::AudioSourceChannelInfo channelInfo(buffer);
    synthAudioSource.getNextAudioBlock(channelInfo, midiMessages);
    midiMessages.clear();
}

bool MorpheusZProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* MorpheusZProcessor::createEditor()
{
    return new MorpheusZEditor(
        *this,
        apvts,
        waveforms,
        monitorMorphPosition);
}

//==============================================================================
void MorpheusZProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    StateHandler::getStateInformation(destData, waveforms[0], waveforms[1], apvts);
}

void MorpheusZProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    StateHandler::setStateInformation(data, sizeInBytes, waveforms[0], waveforms[1], apvts);
    for (auto i = 0; i < waveforms.size(); ++i)
    {
        synthAudioSource.setWave(i, waveforms[i]);
        refreshWaveformEditor(i);
    }
}

void MorpheusZProcessor::setWaveformValue(
    const int waveformNum,
    const int index,
    const float value)
{
    waveforms[waveformNum].setSample(0, index, value);
    synthAudioSource.setWave(waveformNum, waveforms[waveformNum]);
    refreshWaveformEditor(waveformNum);
}

void MorpheusZProcessor::refreshWaveformEditor(const int waveformNum) const
{
    auto editor = dynamic_cast<MorpheusZEditor*>(getActiveEditor());
    if (editor != nullptr)
    {
        editor->waveformChanged(waveformNum);
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MorpheusZProcessor();
}
