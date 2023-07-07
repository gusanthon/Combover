/*
  ==============================================================================

    CustomTabbedComponent.h
    Created: 26 May 2023 5:05:59pm
    Author:  Gus Anthon

  ==============================================================================
*/

#pragma once


#include <JuceHeader.h>
#include "mParamsComponent.h"
#include "CombComponent.h"
#include "PluginProcessor.h"

class CustomTabbedComponent : public juce::TabbedComponent,
                              public juce::AudioProcessorValueTreeState::Listener
{
public:
    CustomTabbedComponent(ComboverAudioProcessor& audioProcessor)
        : juce::TabbedComponent(juce::TabbedButtonBar::TabsAtTop),
          audioProcessor(audioProcessor)
    {

        addTab("Master Params", backgroundColour, new mParamsComponent(audioProcessor), true);

        for (int i = 0; i < numCombs; ++i)
        {
            int combIndex = i + 1;
            addTab("Comb " + std::to_string(combIndex), backgroundColour, new CombComponent(audioProcessor, combIndex), true);
        }

        audioProcessor.apvts.addParameterListener("nCOMBS", this);
    }
    
    void setNumCombs(int newNCombs)
    {
        
        int nCombsToAdd = newNCombs - numCombs;
    
        if (nCombsToAdd > 0)
        {
            for (int i = 0; i < nCombsToAdd; ++i)
            {
                int combNum = i + 1 + numCombs;
                addTab("Comb " + std::to_string(combNum), backgroundColour, new CombComponent(audioProcessor, combNum), true);
            }
        }
        
        else
        {
            for (int i = numCombs; i > numCombs + nCombsToAdd; i--)
            {
                removeTab(i);

            }
        }
        numCombs += nCombsToAdd;
        updateTabSizes();
    }

    
private:
    
    void parameterChanged(const juce::String& parameterID, float newValue) override
    {
        if (parameterID == "nCOMBS")
        {
            int nCombs = audioProcessor.apvts.getRawParameterValue("nCOMBS")->load();
            setNumCombs(nCombs);
        }
        
    }
    
    void resized() override
    {
        juce::TabbedComponent::resized();
        updateTabSizes();
    }
    
    void updateTabSizes()
    {
        const int numTabs = getNumTabs();
        const int tabBarWidth = getWidth();
//        const int tabWidth = tabBarWidth / numTabs;

        setTabBarDepth(tabBarWidth * .05);

    }

    int numCombs = 6;
    static const int maxNumCombs = 12;
    ComboverAudioProcessor& audioProcessor;
    juce::Colour backgroundColour = juce::Colour::fromRGB(35, 35, 45);
//    juce::Colour backgroundColour = juce::Colours::silver;//.darker();
//    juce::Colour backgroundColour = juce::Colours::
//    juce::Colour backgroundColoour = juce::Colours::darkslategrey;
};


