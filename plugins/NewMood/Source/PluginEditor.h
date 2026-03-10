#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"

/**
 * NewMood Plugin Editor - Simple JUCE UI with controls
 */
class NewMoodAudioProcessorEditor : public juce::AudioProcessorEditor,
                                    private juce::Timer
{
public:
    NewMoodAudioProcessorEditor (NewMoodAudioProcessor&);
    ~NewMoodAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void createSlider (const juce::String& label, const juce::String& paramId, int x, int y);
    void timerCallback() override;
    
    //==============================================================================
    // UI Elements
    juce::Label titleLabel;
    juce::OwnedArray<juce::Slider> sliders;
    juce::TextButton recordButton;

    // Reference to processor
    NewMoodAudioProcessor& audioProcessor;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewMoodAudioProcessorEditor)
};
