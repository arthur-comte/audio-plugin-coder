#pragma once

#include <JuceHeader.h>
#include <memory>
#include "WetDSP.h"
#include "LooperDSP.h"

class MoodsCopyTestAudioProcessor : public juce::AudioProcessor
{
public:
    MoodsCopyTestAudioProcessor();
    ~MoodsCopyTestAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    juce::AudioProcessorValueTreeState apvts;
    
    // Wet Engines
    MoodsDelay delayEngine;
    MoodsReverb reverbEngine;
    MoodsSlip slipEngine;
    MoodsLooper looperEngine;

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    // Core parameters smoothing values
    juce::SmoothedValue<float> smoothedClock;
    juce::SmoothedValue<float> smoothedMix;
    
    // Internal buffers for routing
    juce::AudioBuffer<float> wetBuffer;
    juce::AudioBuffer<float> loopBuffer;
    juce::AudioBuffer<float> dryBuffer;

    double currentSampleRate = 44100.0;
    double clockScale = 1.0;
    int maxBlockSize = 512;

    // Wet Engines
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoodsCopyTestAudioProcessor)
};
