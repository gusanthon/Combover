/*
  ==============================================================================

    TopBarComponent.h
    Created: 19 Jun 2023 2:19:44am
    Author:  Gus Anthon

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "CustomLNF.h"

class TopBarComponent : public Component
{
public:
    TopBarComponent(juce::String pluginName, CustomTabbedComponent& tabs, juce::AudioProcessorValueTreeState& apvts) : tabs_(tabs), apvts_(apvts)
    {
        
        pluginNameLabel.setText(pluginName, dontSendNotification);
        pluginNameLabel.setFont(Font("Futura", 20, juce::Font::plain));
        pluginNameLabel.setJustificationType(Justification::centredLeft);
        addAndMakeVisible(pluginNameLabel);

        presetButton.setButtonText("Manage Presets");
        addAndMakeVisible(presetButton);

        randomizeButton.setButtonText("Randomize");
        addAndMakeVisible(randomizeButton);
        randomizeButton.onClick = [this] { randomizeParameters(); };

        resized();
    }

    ~TopBarComponent()
    {
        pluginNameLabel.setLookAndFeel(nullptr);
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colour::fromRGB(35, 35, 45).darker().darker());

        juce::Colour buttonColor = juce::Colour::fromRGB(255, 255, 255); 
        juce::Rectangle<int> randomizeButtonBounds = randomizeButton.getBounds();
        g.setColour(buttonColor);
        g.fillRoundedRectangle(randomizeButtonBounds.toFloat(), 5.0f);

        g.setColour(juce::Colours::black);
        g.setFont(Font("Futura", 14, juce::Font::plain));
        
        g.drawText(randomizeButton.getButtonText(), randomizeButtonBounds, juce::Justification::centred);
    }


    void resized() override
    {
        int margin = 10;
        int logoSize = 20;
        int presetButtonWidth = 140;

        int labelX = margin + logoSize + margin;
        int labelWidth = getWidth() - labelX - presetButtonWidth - margin;
        pluginNameLabel.setBounds(labelX, margin, labelWidth, logoSize);

        int presetButtonX = getWidth() - presetButtonWidth - margin;
        presetButton.setBounds(presetButtonX, margin, presetButtonWidth, logoSize);

        int randomizeButtonWidth = 100;
        int randomizeButtonX = presetButtonX - margin - randomizeButtonWidth;
        randomizeButton.setBounds(randomizeButtonX, margin, randomizeButtonWidth, logoSize);
    }


    void randomizeParameters()
    {
        
        auto* mParamsContent = dynamic_cast<mParamsComponent*>(tabs_.getTabContentComponent(0));
        
        std::vector<SliderWithLabel*> mSliders = mParamsContent->getSlideres();
        
        for (SliderWithLabel* slider : mSliders)
        {

            float minVal = slider->getSlider().getMinimum();
            float maxVal = slider->getSlider().getMaximum();
            float range = maxVal - minVal;
            
            float randomFloat = juce::Random::getSystemRandom().nextFloat();
            float randomVal = (range * randomFloat) + minVal;
            
            slider->getSlider().setValue(randomVal);
            
        }
        
        int numCombs = apvts_.getRawParameterValue("nCOMBS")->load();
        
        for (int i = 1; i < numCombs; ++i)
        {
            auto* combContent = dynamic_cast<CombComponent*>(tabs_.getTabContentComponent(i));

            std::vector<SliderWithLabel*> sliders = combContent->getSliders();

            for (auto* slider : sliders)
            {
                
                float minVal = slider->getSlider().getMinimum();
                float maxVal = slider->getSlider().getMaximum();
                float range = maxVal - minVal;
                
                float randomFloat = juce::Random::getSystemRandom().nextFloat();
                float randomVal = (range * randomFloat) + minVal;
                
                slider->getSlider().setValue(randomVal);

            }
            combContent->randomizeShape();
        }

    }

private:
    juce::String pluginName;

    Label pluginNameLabel;
    TextButton presetButton;
    TextButton randomizeButton;
    CustomLNF mLNF;
    CustomTabbedComponent& tabs_;
    juce::AudioProcessorValueTreeState& apvts_;
};
