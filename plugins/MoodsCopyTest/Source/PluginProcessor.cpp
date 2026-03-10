#include "PluginProcessor.h"
#include "PluginEditor.h"

MoodsCopyTestAudioProcessor::MoodsCopyTestAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
       apvts(*this, nullptr, "Parameters", createParameterLayout())
#endif
{
}

MoodsCopyTestAudioProcessor::~MoodsCopyTestAudioProcessor() = default;

#include "WetDSP.h"

const juce::String MoodsCopyTestAudioProcessor::getName() const { return JucePlugin_Name; }
bool MoodsCopyTestAudioProcessor::acceptsMidi() const { return false; }
bool MoodsCopyTestAudioProcessor::producesMidi() const { return false; }
bool MoodsCopyTestAudioProcessor::isMidiEffect() const { return false; }
double MoodsCopyTestAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int MoodsCopyTestAudioProcessor::getNumPrograms() { return 1; }
int MoodsCopyTestAudioProcessor::getCurrentProgram() { return 0; }
void MoodsCopyTestAudioProcessor::setCurrentProgram (int index) { }
const juce::String MoodsCopyTestAudioProcessor::getProgramName (int index) { return {}; }
void MoodsCopyTestAudioProcessor::changeProgramName (int index, const juce::String& newName) { }

juce::AudioProcessorValueTreeState::ParameterLayout MoodsCopyTestAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // Global
    layout.add(std::make_unique<juce::AudioParameterFloat>("clock", "Clock Rate", 0.0f, 1.0f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("mix", "Mix", 0.0f, 1.0f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterInt>("routing", "Routing", 0, 1, 0));

    // Wet Channel
    layout.add(std::make_unique<juce::AudioParameterInt>("wet_mode", "Wet Mode", 0, 2, 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("wet_time", "Time", 0.0f, 1.0f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("wet_modify", "Modify (Wet)", 0.0f, 1.0f, 0.5f));

    // Looper Channel
    layout.add(std::make_unique<juce::AudioParameterInt>("loop_mode", "Loop Mode", 0, 2, 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("loop_length", "Length", 0.0f, 1.0f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("loop_modify", "Modify (Loop)", 0.0f, 1.0f, 0.5f));

    return layout;
}

void MoodsCopyTestAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    maxBlockSize = samplesPerBlock;
    
    smoothedClock.reset(sampleRate, 0.050);
    smoothedMix.reset(sampleRate, 0.050);

    const int channels = std::max(getTotalNumInputChannels(), getTotalNumOutputChannels());
    wetBuffer.setSize(channels, samplesPerBlock);
    loopBuffer.setSize(channels, samplesPerBlock);
    dryBuffer.setSize(channels, samplesPerBlock);

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = channels;

    delayEngine.prepare(spec);
    reverbEngine.prepare(spec);
    slipEngine.prepare(spec);
    looperEngine.prepare(spec);
}

void MoodsCopyTestAudioProcessor::releaseResources()
{
}

bool MoodsCopyTestAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void MoodsCopyTestAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
        
    const int numSamples = buffer.getNumSamples();
    const bool isSerial = (int)apvts.getRawParameterValue("routing")->load() == 1;
        
    // Update parameter smoothing
    smoothedClock.setTargetValue(apvts.getRawParameterValue("clock")->load());
    smoothedMix.setTargetValue(apvts.getRawParameterValue("mix")->load());

    // 1. Prepare buffers
    looperEngine.write(buffer); // Always listening
    
    dryBuffer.makeCopyOf(buffer, true);
    wetBuffer.clear();
    loopBuffer.clear();

    // Parameters for processing
    int wetMode = (int)apvts.getRawParameterValue("wet_mode")->load();
    float wetTime = apvts.getRawParameterValue("wet_time")->load();
    float wetMod = apvts.getRawParameterValue("wet_modify")->load();
    
    int loopMode = (int)apvts.getRawParameterValue("loop_mode")->load();
    float loopLength = apvts.getRawParameterValue("loop_length")->load();
    float loopMod = apvts.getRawParameterValue("loop_modify")->load();

    // 2. Routing Logic
    if (isSerial)
    {
        // Serial: Input -> Wet -> Loop
        wetBuffer.makeCopyOf(buffer, true);
        
        if (wetMode == 0) { // Reverb
            reverbEngine.setParameters(wetTime, wetMod);
            reverbEngine.process(wetBuffer);
        } else if (wetMode == 1) { // Delay
            delayEngine.setParameters(wetTime, wetMod);
            delayEngine.updateTime(currentSampleRate / clockScale);
            delayEngine.process(wetBuffer);
        } else if (wetMode == 2) { // Slip
            float pitch = (float)(std::pow(2.0, (double)(wetMod - 0.5f) * 2.0) * (1.0 / clockScale));
            slipEngine.process(wetBuffer, pitch);
        }
        
        loopBuffer.makeCopyOf(wetBuffer, true);
        looperEngine.process(loopBuffer, loopMode, loopLength, loopMod, clockScale);
    }
    else
    {
        // Parallel: Input -> Wet, Input -> Loop
        wetBuffer.makeCopyOf(buffer, true);
        loopBuffer.makeCopyOf(buffer, true);
        
        if (wetMode == 0) { // Reverb
            reverbEngine.setParameters(wetTime, wetMod);
            reverbEngine.process(wetBuffer);
        } else if (wetMode == 1) { // Delay
            delayEngine.setParameters(wetTime, wetMod);
            delayEngine.updateTime(currentSampleRate / clockScale);
            delayEngine.process(wetBuffer);
        } else if (wetMode == 2) { // Slip
            float pitch = (float)(std::pow(2.0, (double)(wetMod - 0.5f) * 2.0) * (1.0 / clockScale));
            slipEngine.process(wetBuffer, pitch);
        }
        
        looperEngine.process(loopBuffer, loopMode, loopLength, loopMod, clockScale);
    }

    // 3. Final Mix & Output
    buffer.clear();
    for (int sample = 0; sample < numSamples; ++sample)
    {
        float mix = smoothedMix.getNextValue();
        float clockVal = smoothedClock.getNextValue(); 
        
        // Map 0-1 clock parameter to 0.25x - 4.0x scale (logarithmic feel)
        // 0.5 is roughly 1.0x
        clockScale = std::pow(2.0, (clockVal - 0.5f) * 4.0f);
        
        for (int channel = 0; channel < totalNumOutputChannels; ++channel)
        {
            float drySample = dryBuffer.getSample(channel % dryBuffer.getNumChannels(), sample);
            float wetSample = wetBuffer.getSample(channel % wetBuffer.getNumChannels(), sample);
            float loopSample = loopBuffer.getSample(channel % loopBuffer.getNumChannels(), sample);
            
            // Basic mix logic: Dry is always present when mix is 0.
            // When mix is 1.0, we have 100% Wet/Loop.
            float wetMixed = (wetSample + loopSample) * 0.5f; // Blend of processing
            
            buffer.setSample(channel, sample, drySample * (1.0f - mix) + wetMixed * mix);
        }
    }
}

bool MoodsCopyTestAudioProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* MoodsCopyTestAudioProcessor::createEditor()
{
    return new MoodsCopyTestAudioProcessorEditor (*this);
}

void MoodsCopyTestAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void MoodsCopyTestAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState != nullptr)
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MoodsCopyTestAudioProcessor();
}

