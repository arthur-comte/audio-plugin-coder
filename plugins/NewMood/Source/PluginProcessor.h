#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

//==============================================================================
class NewMoodAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    NewMoodAudioProcessor();
    ~NewMoodAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    juce::AudioProcessorValueTreeState parameters;

private:
    //==============================================================================
    // DSP Components
    // Circular buffer for loop recording
    std::vector<float> loopBuffer;
    int writePosition = 0;
    int readPosition = 0;
    int bufferSize = 44100 * 10; // 10 seconds max
    
    // Processing
    juce::dsp::Gain<float> inputGain;
    juce::dsp::Gain<float> outputGain;
    
    // Filters
    using FilterType = juce::dsp::IIR::Filter<float>;
    using StereoFilter = juce::dsp::ProcessorDuplicator<FilterType, juce::dsp::IIR::Coefficients<float>>;
    StereoFilter toneFilter;
    
    // State
    double currentSampleRate = 44100.0;
    bool isRecording = false;
    bool isOverdubbing = false;
    bool isReversed = false;
    
    //==============================================================================
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewMoodAudioProcessor)
};
