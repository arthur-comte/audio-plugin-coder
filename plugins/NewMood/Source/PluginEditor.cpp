#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewMoodAudioProcessorEditor::NewMoodAudioProcessorEditor (NewMoodAudioProcessor& p)
    : AudioProcessorEditor (p), audioProcessor (p)
{
    setSize (600, 400);

    // Create web view with basic options
    webView = std::make_unique<juce::WebBrowserComponent> (
        juce::WebBrowserComponent::Options{}
            .withBackend (juce::WebBrowserComponent::Options::Backend::webview2)
            .withNativeIntegrationEnabled());

    addAndMakeVisible (*webView);
    
    // Load the embedded web UI
    webView->goToURL (juce::WebBrowserComponent::getResourceProviderRoot());
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
    webView->setBounds (getBounds());
}

//==============================================================================
std::optional<juce::WebBrowserComponent::Resource> NewMoodAudioProcessorEditor::getResource (const juce::String& url)
{
    return std::nullopt;
}

std::unique_ptr<juce::ZipFile> NewMoodAudioProcessorEditor::getZipFile()
{
    return nullptr;
}
