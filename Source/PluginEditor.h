/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomTabbedComponent.h"
#include "TopBarComponent.h"

//==============================================================================
/**
*/
class ComboverAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ComboverAudioProcessorEditor (ComboverAudioProcessor&);
    ~ComboverAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ComboverAudioProcessor& audioProcessor;
    
    bool keyPressed(const KeyPress& press) override;
    
    TopBarComponent topBar;
    CustomTabbedComponent tabs;
    
    static const int topBarHeight = 40;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ComboverAudioProcessorEditor)
};
