#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewMoodAudioProcessor::NewMoodAudioProcessor()
    : AudioProcessor (BusesProperties()
                     .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                     .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      parameters (*this, nullptr, juce::Identifier ("NewMood"), createParameterLayout())
{
    // Initialize loop buffer (10 seconds at 48kHz max)
    bufferSize = 48000 * 10;
    loopBuffer.resize(bufferSize, 0.0f);
}

NewMoodAudioProcessor::~NewMoodAudioProcessor()
{
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout NewMoodAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // Core parameters
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { "time", 1 },
        "Time",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.5f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String (static_cast<int>(value * 100)); }
    ));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { "speed", 1 },
        "Speed",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.5f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String (static_cast<int>(value * 100)); }
    ));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { "mix", 1 },
        "Mix",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.5f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String (static_cast<int>(value * 100)); }
    ));

    // Fine-tune parameters
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { "feedback", 1 },
        "Feedback",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.3f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String (static_cast<int>(value * 100)); }
    ));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { "grainSize", 1 },
        "GrainSize",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.3f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String (static_cast<int>(value * 100)); }
    ));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { "drift", 1 },
        "Drift",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.0f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String (static_cast<int>(value * 100)); }
    ));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { "spread", 1 },
        "Spread",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.5f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String (static_cast<int>(value * 100)); }
    ));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { "tone", 1 },
        "Tone",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.5f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String (static_cast<int>(value * 100)); }
    ));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { "level", 1 },
        "Level",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.75f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String (static_cast<int>(value * 100)); }
    ));

    // Sync parameters
    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { "syncEnabled", 1 },
        "Sync",
        false
    ));

    layout.add (std::make_unique<juce::AudioParameterInt> (
        juce::ParameterID { "subdivision", 1 },
        "Subdivision",
        0, 7, 2
    ));

    // Recording controls
    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { "record", 1 },
        "Record",
        false
    ));

    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { "overdub", 1 },
        "Overdub",
        false
    ));

    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { "reverse", 1 },
        "Reverse",
        false
    ));

    return layout;
}

//==============================================================================
const juce::String NewMoodAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NewMoodAudioProcessor::acceptsMidi() const
{
    return false;
}

bool NewMoodAudioProcessor::producesMidi() const
{
    return false;
}

bool NewMoodAudioProcessor::isMidiEffect() const
{
    return false;
}

double NewMoodAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NewMoodAudioProcessor::getNumPrograms()
{
    return 1;
}

int NewMoodAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NewMoodAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String NewMoodAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void NewMoodAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void NewMoodAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    
    // Resize buffer for current sample rate
    bufferSize = static_cast<int>(sampleRate * 10); // 10 seconds max
    loopBuffer.resize(bufferSize, 0.0f);
    writePosition = 0;
    readPosition = 0;

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32> (samplesPerBlock);
    spec.numChannels = 2;

    inputGain.prepare (spec);
    outputGain.prepare (spec);
    toneFilter.prepare (spec);

    // Set ramping
    inputGain.setRampDurationSeconds (0.010);
    outputGain.setRampDurationSeconds (0.010);
}

void NewMoodAudioProcessor::releaseResources()
{
}

bool NewMoodAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void NewMoodAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                           juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear extra output channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Handle zero-length buffers
    if (buffer.getNumSamples() == 0)
        return;

    // Get parameter values
    const float mix = parameters.getRawParameterValue ("mix")->load();
    const float level = parameters.getRawParameterValue ("level")->load();
    const float feedback = parameters.getRawParameterValue ("feedback")->load();
    const float tone = parameters.getRawParameterValue ("tone")->load();
    
    // Recording controls
    const bool record = parameters.getRawParameterValue ("record")->load() > 0.5f;
    const bool overdub = parameters.getRawParameterValue ("overdub")->load() > 0.5f;
    const bool reverse = parameters.getRawParameterValue ("reverse")->load() > 0.5f;
    
    // Calculate loop length from time parameter
    const float timeParam = parameters.getRawParameterValue ("time")->load();
    const int loopLength = static_cast<int>(timeParam * bufferSize);
    
    // Set output level
    outputGain.setGainLinear (level);
    
    // Simple pass-through for now (granular processing to be implemented)
    // This is a placeholder that passes audio through
    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> context (block);
    
    // Apply gain and pass through
    inputGain.setGainLinear (mix);
    inputGain.process (context);
    outputGain.process (context);
}

//==============================================================================
bool NewMoodAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* NewMoodAudioProcessor::createEditor()
{
    return new NewMoodAudioProcessorEditor (*this);
}

//==============================================================================
void NewMoodAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void NewMoodAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NewMoodAudioProcessor();
}
