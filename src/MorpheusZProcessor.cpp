#include "MorpheusZProcessor.h"
#include "MorpheusZEditor.h"
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
      ), synthAudioSource(keyboardState)
{
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

    setWaveformA(WaveformPreset::Sine);
    setWaveformB(WaveformPreset::Triangle);
}

std::unique_ptr<juce::AudioSampleBuffer> MorpheusZProcessor::getWaveform(WaveformPreset preset) const
{
    switch (preset)
    {
    case WaveformPreset::Sine:
        return waveformPresets::sine(waveformSize);
    case WaveformPreset::Square:
        return waveformPresets::square(waveformSize);
    case WaveformPreset::Triangle:
        return waveformPresets::triangle(waveformSize);
    case WaveformPreset::Sawtooth:
        return waveformPresets::sawTooth(waveformSize);
    }
    DBG(juce::String::formatted("An invalid preset wave was selected: %i", preset));
    jassertfalse;
    return waveformPresets::sine(waveformSize);
}

void MorpheusZProcessor::setWaveformA(const WaveformPreset preset)
{
    const std::unique_ptr<juce::AudioSampleBuffer> wave = getWaveform(preset);
    waveformA.copyFrom(0, 0, *wave, 0, 0, waveformSize);
    synthAudioSource.setWaveA(waveformA);
    refreshWaveformAEditor();
}

void MorpheusZProcessor::setWaveformB(const WaveformPreset preset)
{
    const std::unique_ptr<juce::AudioSampleBuffer> wave = getWaveform(preset);
    waveformB.copyFrom(0, 0, *wave, 0, 0, waveformSize);
    synthAudioSource.setWaveB(waveformB);
    refreshWaveformBEditor();
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
    auto editor = new MorpheusZEditor(*this);
    const auto sampleRate = getSampleRate();
    editor->setWaveformA(waveformA, sampleRate);
    editor->setWaveformB(waveformB, sampleRate);
    return editor;
}

//==============================================================================
void MorpheusZProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused(destData);
}

void MorpheusZProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused(data, sizeInBytes);
}

void MorpheusZProcessor::setWaveformAValue(int index, float value)
{
    waveformA.setSample(0, index, value);
    synthAudioSource.setWaveA(waveformA);
    refreshWaveformAEditor();
}

void MorpheusZProcessor::setWaveformBValue(int index, float value)
{
    waveformB.setSample(0, index, value);
    synthAudioSource.setWaveB(waveformB);
    refreshWaveformBEditor();
}

void MorpheusZProcessor::refreshWaveformAEditor() const
{
    auto editor = dynamic_cast<MorpheusZEditor*>(getActiveEditor());
    if (editor != nullptr)
    {
        editor->setWaveformA(waveformA, getSampleRate());
    }
}

void MorpheusZProcessor::refreshWaveformBEditor() const
{
    auto editor = dynamic_cast<MorpheusZEditor*>(getActiveEditor());
    if (editor != nullptr)
    {
        editor->setWaveformB(waveformB, getSampleRate());
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MorpheusZProcessor();
}
