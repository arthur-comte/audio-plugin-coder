#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewMoodAudioProcessorEditor::NewMoodAudioProcessorEditor (NewMoodAudioProcessor& p)
    : AudioProcessorEditor (p), audioProcessor (p)
{
    setSize (800, 600);

    // Create web view - use default backend (WKWebView on macOS)
    webView = std::make_unique<juce::WebBrowserComponent> (
        juce::WebBrowserComponent::Options{}
            .withNativeIntegrationEnabled());

    addAndMakeVisible (*webView);
    
    // Load the web UI from the app bundle resources
    // For standalone: use bundle path. For plugin: use a placeholder HTML
    webView->goToURL ("about:blank");
    
    // Inject the HTML directly
    auto html = R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>NewMood</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body {
            background: linear-gradient(180deg, #1a1816 0%, #2d2a26 100%);
            color: #e8e4df;
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
            min-height: 100vh;
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
        }
        h1 { font-size: 2.5rem; margin-bottom: 1rem; color: #ff6b35; }
        p { font-size: 1.1rem; opacity: 0.8; }
    </style>
</head>
<body>
    <h1>NewMood</h1>
    <p>Chase Bliss inspired granular looper</p>
</body>
</html>
)";
    
    webView->loadHTMLString (html, "");
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
