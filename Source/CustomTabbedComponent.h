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
        auto mParams = std::make_unique<mParamsComponent>(audioProcessor);

        addTab("Master Params", backgroundColour, mParams.release(), true);
        
        for (int combIndex = 1; combIndex < numCombs + 1; ++combIndex)
        {
            auto combComponent = std::make_unique<CombComponent>(audioProcessor, combIndex);
            addTab("Comb " + std::to_string(combIndex), backgroundColour, combComponent.release(), true);

        }

        audioProcessor.apvts.addParameterListener("nCOMBS", this);
        setNumCombs(audioProcessor.apvts.getRawParameterValue("nCOMBS")->load());
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
    
    void setNumCombs(int newNCombs)
    {
        int nCombsToAdd = newNCombs - numCombs;
        
        int newNumCombs = nCombsToAdd + numCombs;

        if (getCurrentTabIndex() > newNumCombs)
            setCurrentTabIndex(newNumCombs);
    
        if (nCombsToAdd > 0)
        {
            for (int i = 1; i < nCombsToAdd + 1; ++i)
            {
                int combNum = i + numCombs;
                auto newComb = std::make_unique<CombComponent>(audioProcessor, combNum);
                addTab("Comb " + std::to_string(combNum), backgroundColour, newComb.release(), true);
            }
        }
        
        else
        {
            for (int i = numCombs; i > newNumCombs; i--)
            {
                removeTab(i);
            }
        }
        
        numCombs = newNumCombs;
        
        updateTabSizes();
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

        setTabBarDepth(tabBarWidth * .05);

    }

    int numCombs = 6;
    ComboverAudioProcessor& audioProcessor;
    juce::Colour backgroundColour = juce::Colour::fromRGB(35, 35, 45);

};


