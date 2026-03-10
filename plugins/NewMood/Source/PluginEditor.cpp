#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewMoodAudioProcessorEditor::NewMoodAudioProcessorEditor (NewMoodAudioProcessor& p)
    : AudioProcessorEditor (p), audioProcessor (p)
{
    setSize (800, 600);

    // Title label
    titleLabel.setText ("NewMood", juce::dontSendNotification);
    titleLabel.setFont (juce::Font (32.0f, juce::Font::bold));
    titleLabel.setColour (juce::Label::textColourId, juce::Colour (0xFFFF6B35));
    titleLabel.setBounds (300, 50, 200, 40);
    addAndMakeVisible (titleLabel);

    // Subtitle
    subtitleLabel.setText ("Chase Bliss inspired granular looper", juce::dontSendNotification);
    subtitleLabel.setFont (juce::Font (16.0f));
    subtitleLabel.setColour (juce::Label::textColourId, juce::Colour (0xFFE8E4DF));
    subtitleLabel.setBounds (220, 100, 360, 24);
    addAndMakeVisible (subtitleLabel);
    
    // Placeholder text
    infoLabel.setText ("VST3/AU Plugin - Build Complete!", juce::dontSendNotification);
    infoLabel.setFont (juce::Font (14.0f));
    infoLabel.setColour (juce::Label::textColourId, juce::Colour (0xFFAAAAAA));
    infoLabel.setBounds (280, 200, 240, 20);
    addAndMakeVisible (infoLabel);
}

NewMoodAudioProcessorEditor::~NewMoodAudioProcessorEditor()
{
}

//==============================================================================
void NewMoodAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xFF2D2A26));
}

void NewMoodAudioProcessorEditor::resized()
{
}
