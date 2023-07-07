/*
  ==============================================================================

    ParameterPanel.h
    Created: 18 Jun 2023 6:22:39pm
    Author:  Gus Anthon

  ==============================================================================
*/

#pragma once

#include "CustomSlider.h"
#include "SliderWithLabel.h"


class ParameterPanel : public juce::Component
{
public:
    ParameterPanel(const juce::Array<juce::Component*>& components, bool isHorizontal)
        : components_(components), isHorizontal_(isHorizontal)
    {
        for (auto* component : components_)
        {
            addAndMakeVisible(component);
            addChildComponent(component);
            component->setLookAndFeel(&mLNF);
            
//            std::cout << typeid(*component).name() << std::endl;

        }
    }
    
    ~ParameterPanel()
    {
        for (auto* component : components_)
        {
            component->setLookAndFeel(nullptr);
        }
    }
    
    void setBackgroundColor(const juce::Colour& color)
    {
        backgroundColor_ = color;
        repaint();
    }
    
    void setTitleText(juce::String title)
    {
        titleText = title;
    }
    
    void setTitleHeight(int newHeight)
    {
        titleHeight = newHeight;
    }

    void resized() override
    {
//        std::cout<<"Here!!!!!"<<std::endl;
        if (isHorizontal_)
            layoutHorizontal();
        else
            layoutVertical();
    }

    void paint(juce::Graphics& g) override
    {
        const float cornerSize = 10.0f; // Adjust the corner size as desired

        
        g.setColour(backgroundColor_);
        g.fillRoundedRectangle(getLocalBounds().toFloat(), cornerSize);

        // Draw an outline around the entire ParameterPanel
        g.setColour(juce::Colours::black);
        g.drawRoundedRectangle(getLocalBounds().toFloat(), cornerSize, 5);

        // Draw the title text
        g.setColour(juce::Colours::navajowhite);
        g.setFont(Font("Futura", "Bold", titleHeight - 8));
        g.drawText(titleText, getLocalBounds().removeFromTop(titleHeight), juce::Justification::centred, true);
    }

private:
    juce::Colour backgroundColor_ = juce::Colours::cornflowerblue;

    
    juce::Array<juce::Component*> components_;
    bool isHorizontal_;
    
    int titleHeight = 30;
    juce::String titleText;
    
    CustomLNF mLNF;

    void layoutHorizontal()
    {
        auto bounds = getLocalBounds().reduced(0, titleHeight); // Reduce the height by the title height
        const int componentCount = components_.size();
        int componentWidth = bounds.getWidth() / componentCount;

        for (int i = 0; i < componentCount; ++i)
        {
            auto* component = components_[i];
            
            // Reduce the width of the ComboBox component
            if (dynamic_cast<ComboBoxWithLabel*>(component) != nullptr)
            {
                componentWidth = juce::jmin(componentWidth, 100); // Adjust the desired width
                
                int componentHeight = juce::jmax(component->getHeight(), 65); // Set minimum height to 50 pixels
                
                // Calculate the vertical offset to center the component
                int yOffset = (bounds.getHeight() - componentHeight) / 2;
                
                bounds = bounds.removeFromTop(componentHeight).translated(0, yOffset);
                component->setBounds(bounds);
            }
            
            else
            {
                component->setBounds(bounds.removeFromLeft(componentWidth));
            }
                
        }
    }


    void layoutVertical()
    {
        auto bounds = getLocalBounds().reduced(0, titleHeight); // Reduce the height by the title height
        const int componentCount = components_.size();
        const int componentHeight = bounds.getHeight() / componentCount;

        for (int i = 0; i < componentCount; ++i)
        {
            auto* component = components_[i];
            component->setBounds(bounds.removeFromTop(componentHeight));
        }
    }
};
