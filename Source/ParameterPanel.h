/*
  ==============================================================================

    ParameterPanel.h
    Created: 18 Jun 2023 6:22:39pm
    Author:  Gus Anthon

  ==============================================================================
*/

#pragma once

#include "LabeledComponent.h"



class ParameterPanel : public LabeledComponent
{
public:
    ParameterPanel(const juce::Array<juce::Component*>& components, bool isHorizontal = true)
        : components_(components), isHorizontal_(isHorizontal)
    {
        
        flexBox.flexDirection = isHorizontal_ ? FlexBox::Direction::row : FlexBox::Direction::column;
        
        labelHeight = 30;
        
        addComponentsToFlexBox();
    }
    
    ~ParameterPanel()
    {
        for (auto* component : components_)
            component->setLookAndFeel(nullptr);
        
    }
    
    void addComponentsToFlexBox()
    {
        flexBox.items.clear();
        
        for (auto* component : components_)
        {
            jassert(component != nullptr);
            addAndMakeVisible(component);
            if (isHorizontal_)
                flexBox.items.add(FlexItem(*component).withFlex(1.0).withMargin({ 0, 7.5, 0, 7.5 }));
            else
                flexBox.items.add(FlexItem(*component).withFlex(1.0).withMargin({5,0,5,0}));
        }
    }
    
    void setBackgroundColor(const juce::Colour& color)
    {
        backgroundColor_ = color;
        repaint();
    }

    void resized() override
    {
        auto b = getLocalBounds().reduced(0, labelHeight);
        flexBox.performLayout(b);
    }


    void paint(juce::Graphics& g) override
    {
        const float cornerSize = 10.0f;

        g.setColour(backgroundColor_);
        g.fillRoundedRectangle(getLocalBounds().toFloat(), cornerSize);

        g.setColour(juce::Colours::black);
        g.drawRoundedRectangle(getLocalBounds().toFloat(), cornerSize, 2);

        g.setColour(juce::Colours::white);
        g.setFont(Font("Futura", "Bold", labelHeight - 8));
        g.drawText(labelText, getLocalBounds().removeFromTop(labelHeight), juce::Justification::centred, true);
    }
    
    juce::Array<juce::Component*>& getComponents()
    {
        return components_;
    }
    
    void setOrientation(bool isHorizontal)
    {
        if (isHorizontal == isHorizontal_) return;
        isHorizontal_ = isHorizontal;
        flexBox.flexDirection = isHorizontal_ ? FlexBox::Direction::row : FlexBox::Direction::column;
        
        addComponentsToFlexBox();
        resized();
    }

protected:

    juce::Colour backgroundColor_ = juce::Colour::fromHSV(.65, .82, .52, 1);
    juce::FlexBox flexBox;

    juce::Array<juce::Component*> components_;
    bool isHorizontal_;

};



