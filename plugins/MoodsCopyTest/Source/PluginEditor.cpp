#include <memory>
#include "PluginProcessor.h"
#include "PluginEditor.h"

MoodsCopyTestAudioProcessorEditor::MoodsCopyTestAudioProcessorEditor (MoodsCopyTestAudioProcessor& p)
    : VisagePluginEditor(p), audioProcessor(p)
{
    setSize(600, 400);
}

MoodsCopyTestAudioProcessorEditor::~MoodsCopyTestAudioProcessorEditor() = default;

void MoodsCopyTestAudioProcessorEditor::onInit()
{
    mainView = std::make_unique<VisageMainView>();
    addFrameToCanvas(mainView.get());
    setEventRoot(mainView.get());
    mainView->setBounds(0, 0, getWidth(), getHeight());
    mainView->redrawAll();

    auto& apvts = audioProcessor.apvts;

    // Helper to find parameter pointer
    auto getParam = [&](juce::String id) -> juce::RangedAudioParameter* {
        return apvts.getParameter(id);
    };

    // Bind Knobs
    mainView->clock_knob.on_change = [this, getParam](float v) {
        if (auto* p = getParam("clock")) p->setValueNotifyingHost(v);
    };
    mainView->mix_knob.on_change = [this, getParam](float v) {
        if (auto* p = getParam("mix")) p->setValueNotifyingHost(v);
    };
    mainView->wet_time.on_change = [this, getParam](float v) {
        if (auto* p = getParam("wet_time")) p->setValueNotifyingHost(v);
    };
    mainView->wet_mod.on_change = [this, getParam](float v) {
        if (auto* p = getParam("wet_modify")) p->setValueNotifyingHost(v);
    };
    mainView->loop_len.on_change = [this, getParam](float v) {
        if (auto* p = getParam("loop_length")) p->setValueNotifyingHost(v);
    };
    mainView->loop_mod.on_change = [this, getParam](float v) {
        if (auto* p = getParam("loop_modify")) p->setValueNotifyingHost(v);
    };

    // Initialize values
    mainView->clock_knob.setValue(apvts.getRawParameterValue("clock")->load());
    mainView->mix_knob.setValue(apvts.getRawParameterValue("mix")->load());
    mainView->wet_time.setValue(apvts.getRawParameterValue("wet_time")->load());
    mainView->wet_mod.setValue(apvts.getRawParameterValue("wet_modify")->load());
    mainView->loop_len.setValue(apvts.getRawParameterValue("loop_length")->load());
    mainView->loop_mod.setValue(apvts.getRawParameterValue("loop_modify")->load());
}

void MoodsCopyTestAudioProcessorEditor::onRender()
{
    if (mainView) {
        audioProcessor.looperEngine.getWaveformSummary(mainView->waveform.data, 512);
        mainView->waveform.redraw();
    }
}

void MoodsCopyTestAudioProcessorEditor::onDestroy()
{
    if (mainView) {
        removeFrameFromCanvas(mainView.get());
        mainView.reset();
    }
}

void MoodsCopyTestAudioProcessorEditor::onResize(int w, int h)
{
    if (mainView) {
        mainView->setBounds(0, 0, w, h);
        mainView->redraw();
    }
}
