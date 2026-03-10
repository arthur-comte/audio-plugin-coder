#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_extra/juce_gui_extra.h>
#include "PluginProcessor.h"

/**
 * NewMood Plugin Editor - WebView UI Integration
 */
class NewMoodAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    NewMoodAudioProcessorEditor (NewMoodAudioProcessor&);
    ~NewMoodAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    // Web browser component
    std::unique_ptr<juce::WebBrowserComponent> webView;

    //==============================================================================
    // Resource provider for embedded web files
    std::optional<juce::WebBrowserComponent::Resource> getResource (const juce::String& url);
    std::unique_ptr<juce::ZipFile> getZipFile();

    // Reference to processor
    NewMoodAudioProcessor& audioProcessor;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewMoodAudioProcessorEditor)
};
