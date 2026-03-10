#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewMoodAudioProcessorEditor::NewMoodAudioProcessorEditor (NewMoodAudioProcessor& p)
    : AudioProcessorEditor (p), audioProcessor (p)
{
    setSize (800, 500);

    // Title
    titleLabel.setText ("NewMood", juce::dontSendNotification);
    titleLabel.setFont (juce::Font (28.0f, juce::Font::bold));
    titleLabel.setColour (juce::Label::textColourId, juce::Colour (0xFFFF6B35));
    titleLabel.setBounds (320, 20, 160, 35);
    addAndMakeVisible (titleLabel);

    // Create sliders for each parameter
    createSlider ("Time", "time", 50, 80);
    createSlider ("Speed", "speed", 200, 80);
    createSlider ("Mix", "mix", 350, 80);
    createSlider ("Feedback", "feedback", 500, 80);
    createSlider ("GrainSize", "grainSize", 650, 80);
    
    createSlider ("Drift", "drift", 50, 200);
    createSlider ("Spread", "spread", 200, 200);
    createSlider ("Tone", "tone", 350, 200);
    createSlider ("Level", "level", 500, 200);
    
    // Record button
    recordButton.setButtonText ("REC");
    recordButton.setBounds (650, 200, 80, 30);
    recordButton.setColour (juce::TextButton::buttonColourId, juce::Colour (0xFF444444));
    addAndMakeVisible (recordButton);
    
    // Connect button to parameter
    recordButton.onClick = [this] { 
        auto* param = audioProcessor.parameters.getRawParameterValue ("record");
        param->setValueNotifyingHost (recordButton.getToggleState() ? 1.0f : 0.0f);
    };
}

void NewMoodAudioProcessorEditor::createSlider (const juce::String& label, const juce::String& paramId, int x, int y)
{
    // Label
    auto* l = new juce::Label();
    l->setText (label, juce::dontSendNotification);
    l->setFont (juce::Font (12.0f));
    l->setColour (juce::Label::textColourId, juce::Colour (0xFFAAAAAA));
    l->setBounds (x, y, 80, 20);
    addAndMakeVisible (l);
    
    // Slider
    auto* s = new juce::Slider();
    s->setBounds (x, y + 22, 120, 24);
    s->setRange (0.0, 1.0);
    s->setValue (0.5);
    s->setColour (juce::Slider::trackColourId, juce::Colour (0xFF444444));
    s->setColour (juce::Slider::thumbColourId, juce::Colour (0xFFFF6B35));
    addAndMakeVisible (s);
    
    // Connect to parameter
    s->onValueChange = [this, paramId, s]() {
        auto* param = audioProcessor.parameters.getRawParameterValue (paramId);
        if (param)
            param->setValueNotifyingHost ((float)s->getValue());
    };
    
    sliders.add (s);
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
