/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "VisageControls.h"
#include "VisageJuceHost.h"

//==============================================================================
class NewMoodAudioProcessorEditor : public VisagePluginEditor
{
public:
    NewMoodAudioProcessorEditor (NewMoodAudioProcessor&);
    ~NewMoodAudioProcessorEditor() override;

    void onInit() override;
    void onRender() override;
    void onDestroy() override;
    void onResize(int w, int h) override;

private:
    NewMoodAudioProcessor& audioProcessor;
    std::unique_ptr<VisageMainView> mainView;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewMoodAudioProcessorEditor)
};
