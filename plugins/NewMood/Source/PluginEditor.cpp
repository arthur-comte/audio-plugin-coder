#include "PluginProcessor.h"
#include "PluginEditor.h"

// Include the generated binary data
#include "newmood_WebUI/juce_newmood_WebUI.h"

//==============================================================================
NewMoodAudioProcessorEditor::NewMoodAudioProcessorEditor (NewMoodAudioProcessor& p)
    : AudioProcessorEditor (p), audioProcessor (p)
{
    setSize (800, 600);

    // Create web view - use default backend (WKWebView on macOS)
    webView = std::make_unique<juce::WebBrowserComponent> (
        juce::WebBrowserComponent::Options{}
            .withNativeIntegrationEnabled());

    // Set up resource provider to serve embedded files
    webView->setResourceProvider ([this](const juce::String& url) -> std::optional<juce::WebBrowserComponent::Resource> {
        return getResource(url);
    });

    addAndMakeVisible (*webView);
    
    // Load the embedded web UI
    webView->goToURL ("index.html");
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
    // Remove query parameters if any
    juce::String path = url.fromFirstOccurrenceOf("?", false, false);
    
    // Check for each embedded file
    if (path == "index.html" || path == "/index.html")
    {
        return juce::WebBrowserComponent::Resource (
            newmood_WebUI::index_html,
            newmood_WebUI::index_htmlSize,
            "text/html");
    }
    else if (path == "js/index.js" || path == "/js/index.js")
    {
        return juce::WebBrowserComponent::Resource (
            newmood_WebUI::index_js,
            newmood_WebUI::index_jsSize,
            "application/javascript");
    }
    
    return std::nullopt;
}
