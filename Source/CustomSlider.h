/*
  ==============================================================================

    CustomSlider.h
    Created: 18 Jun 2023 3:31:23pm
    Author:  Gus Anthon

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class CustomSlider : public juce::Slider
{
public:
    CustomSlider()
    {
        setNumDecimalPlacesToDisplay(2);
    }

    juce::String getTextFromValue(double value) override
    {
        juce::String text = juce::String(value, getNumDecimalPlacesToDisplay());
        text += suffix_; // Append the suffix
        return text;
    }

    void setSuffix(const juce::String& suffix)
    {
        suffix_ = " " + suffix;
        updateText();
    }
    
private:
    juce::String suffix_;
};


 
