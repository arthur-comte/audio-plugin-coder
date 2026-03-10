#include "PluginProcessor.h"
#include "PluginEditor.h"

NewMoodAudioProcessorEditor::NewMoodAudioProcessorEditor (NewMoodAudioProcessor& p)
    : VisagePluginEditor(p), audioProcessor(p)
{
    setSize(600, 400);
}

NewMoodAudioProcessorEditor::~NewMoodAudioProcessorEditor() = default;

void NewMoodAudioProcessorEditor::onInit()
{
    mainView = std::make_unique<VisageMainView>();
    addFrameToCanvas(mainView.get());
    mainView->setBounds(0, 0, getWidth(), getHeight());
}

void NewMoodAudioProcessorEditor::onRender()
{
    // Optional: drive animations or dynamic redraws here.
}

void NewMoodAudioProcessorEditor::onDestroy()
{
    if (mainView) {
        removeFrameFromCanvas(mainView.get());
        mainView.reset();
    }
}

void NewMoodAudioProcessorEditor::onResize(int w, int h)
{
    if (mainView) {
        mainView->setBounds(0, 0, w, h);
        mainView->redraw();
    }
}
