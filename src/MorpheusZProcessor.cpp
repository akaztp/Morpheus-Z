#include "MorpheusZProcessor.h"
#include "MorpheusZEditor.h"
#include "StateSerializer.h"

//==============================================================================
MorpheusZProcessor::MorpheusZProcessor()
    : AudioProcessor(BusesProperties()
          .withOutput("Output", juce::AudioChannelSet::stereo(), true)
      ),
      synthAudioSource(appState)
{
}

MorpheusZProcessor::~MorpheusZProcessor() = default;

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
        appState);
}

//==============================================================================
void MorpheusZProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    StateSerializer::getStateInformation(destData, appState);
}

void MorpheusZProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    StateSerializer::setStateInformation(data, sizeInBytes, appState);
    for (auto i = 0; i < appState.waveforms.size(); ++i)
    {
        auto editor = dynamic_cast<MorpheusZEditor*>(getActiveEditor());
        if (editor != nullptr)
        {
            editor->waveformChanged(i);
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MorpheusZProcessor();
}
