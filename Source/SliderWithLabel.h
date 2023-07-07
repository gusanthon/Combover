/*
  ==============================================================================

    SliderWithLabel.h
    Created: 18 Jun 2023 3:31:50pm
    Author:  Gus Anthon

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CustomLNF.h"
#include "CustomSlider.h"

class SliderWithLabel : public juce::Component
{
public:
    SliderWithLabel(const juce::String& parameterLabel)
    {
//        setOpaque(false);
        slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 72, 32);
        slider.setLookAndFeel(&mLNF);
//        slider.setRange(0.f, 100.f, 1.f);
//        slider.setNumDecimalPlacesToDisplay(0);
        addAndMakeVisible(slider);

        label.setLookAndFeel(&mLNF);
        label.setText(parameterLabel, juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(label);
    }

    ~SliderWithLabel()
    {
        slider.setLookAndFeel(nullptr);
        label.setLookAndFeel(nullptr);
    }

    void resized() override
    {
        auto bounds = getLocalBounds();
        label.setBounds(bounds.removeFromTop(labelHeight).reduced(0, 10));
        slider.setBounds(bounds);
    }
    
    void setRange(float newMin, float newMax, double newInt)
    {
        slider.setRange(newMin, newMax, newInt);
    }
    
    void setNumDecimalPlacesToDisplay(int numDecimals)
    {
        slider.setNumDecimalPlacesToDisplay(numDecimals);
    }
    
    CustomSlider& getSlider()
    {
        return slider;
    }
    
    void setSuffix(juce::String newSuffix)
    {
        slider.setSuffix(newSuffix);
    }
    

protected:
//    juce::Slider slider;
    CustomSlider slider;
    juce::Label label;
    int labelHeight = 30;
    CustomLNF mLNF;
};



class ComboBoxWithLabel : public juce::Component
{
public:
    ComboBoxWithLabel(const juce::String& parameterLabel)
    {
        comboBox.setLookAndFeel(&mLNF);
        addAndMakeVisible(comboBox);

        label.setText(parameterLabel, juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        label.setLookAndFeel(&mLNF);
        addAndMakeVisible(label);
//        comboBox.setSelectedId(1);
    }

    ~ComboBoxWithLabel()
    {
        comboBox.setLookAndFeel(nullptr);
        label.setLookAndFeel(nullptr);
    }
    
    
    void resized() override
    {
        auto bounds = getLocalBounds();
        label.setBounds(bounds.removeFromTop(labelHeight).reduced(0, 10));
        comboBox.setBounds(bounds);
    }

    void addItem(const juce::String &newItemText, int newItemId)
    {
        comboBox.addItem(newItemText, newItemId);
    }
    

    juce::ComboBox& getComboBox()
    {
        return comboBox;
    }

private:
    juce::ComboBox comboBox;
    juce::Label label;
    int labelHeight = 30;
    CustomLNF mLNF;
};
