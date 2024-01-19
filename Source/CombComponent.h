/*
  ==============================================================================

    mParamsComponent.h
    Created: 24 Jun 2023 5:04:42pm
    Author:  Gus Anthon

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "ParameterPanel.h"
#include "LabeledComponent.h"


class CombComponent : public juce::Component
{
public:
    CombComponent(ComboverAudioProcessor& proc, int combNum) :
        DelaySlider("DELAY " + std::to_string(combNum)),
        FeedbackSlider("FEEDBACK " + std::to_string(combNum)),
        CutoffSlider("DAMPING " + std::to_string(combNum)),
        RateSlider("RATE " + std::to_string(combNum)),
        DepthSlider("DEPTH " + std::to_string(combNum)),
        ShapeComboBox("SHAPE "+ std::to_string(combNum)),
        SaturationSlider("SATURATION " + std::to_string(combNum)),
        PanSlider("PAN " + std::to_string(combNum)),
        MixSlider("LEVEL " + std::to_string(combNum)),
        DelayPanel(juce::Array<juce::Component*>{&DelaySlider, &FeedbackSlider, &CutoffSlider}, true),
        LFOPanel(juce::Array<juce::Component*>{&RateSlider, &DepthSlider, &ShapeComboBox}, true),
        OutPanel(juce::Array<juce::Component*>{&SaturationSlider, &PanSlider, &MixSlider}, true)
    {
        
        ShapeComboBox.addItem(juce::String("Sine"), 1);
        ShapeComboBox.addItem(juce::String("Triangle"), 2);
        ShapeComboBox.addItem(juce::String("Square"), 3);
        ShapeComboBox.addItem(juce::String("Sawtooth"), 4);
        ShapeComboBox.addItem(juce::String("Random"), 5);
        ShapeComboBox.getComboBox().setSelectedId(1);
        
        delayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(proc.apvts, "DELAY_" + std::to_string(combNum), DelaySlider.getSlider());
        feedbackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(proc.apvts, "FEEDBACK_" + std::to_string(combNum), FeedbackSlider.getSlider());
        cutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(proc.apvts, "CUTOFF_" + std::to_string(combNum), CutoffSlider.getSlider());
        depthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(proc.apvts, "DEPTH_" + std::to_string(combNum), DepthSlider.getSlider());
        panAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(proc.apvts, "PAN_" + std::to_string(combNum), PanSlider.getSlider());
        levelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(proc.apvts, "MIX_" + std::to_string(combNum), MixSlider.getSlider());

        rateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(proc.apvts, "RATE_" + std::to_string(combNum), RateSlider.getSlider());

        saturationAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(proc.apvts, "SATURATION_" + std::to_string(combNum), SaturationSlider.getSlider());
        
        shapeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(proc.apvts, "SHAPE_" + std::to_string(combNum), ShapeComboBox.getComboBox());

        
        DelayPanel.setLabelText("DELAY "+ std::to_string(combNum));
        
        DelaySlider->setTextValueSuffix(" ms");
        DelaySlider->setRange(.01f, 300.f, .01f);
        DelaySlider->setNumDecimals(2);

        
        FeedbackSlider.getSlider().setRange(-100.f, 100.f, .1f);
        FeedbackSlider.getSlider().setTextValueSuffix("%");
        FeedbackSlider->setNumDecimals(1);
        
        CutoffSlider.getSlider().setRange(0.f, 100.f, .1f);
        CutoffSlider.getSlider().setTextValueSuffix("%");
        
        LFOPanel.setLabelText("LFO "+ std::to_string(combNum));
        
        DepthSlider.getSlider().setRange(0.f, 100.f, .1f);
        DepthSlider.getSlider().setTextValueSuffix("%");;
        DepthSlider->setNumDecimals(1);
        
        RateSlider.getSlider().setTextValueSuffix("Hz");
        RateSlider->setNumDecimals(2);
              
        PanSlider.getSlider().setRange(-100.f, 100.f, .1f);

        MixSlider.getSlider().setRange(0.f, 100.f, .1f);
        MixSlider->setTextValueSuffix("%");
        MixSlider->setNumDecimals(1);
        
        SaturationSlider.getSlider().setRange(0, 100, .1);
        SaturationSlider->setNumDecimals(1);
        SaturationSlider.getSlider().setTextValueSuffix("%");
        OutPanel.setLabelText("OUT");
        
        DelayPanel.setBackgroundColor(juce::Colours::darkblue.brighter());
        LFOPanel.setBackgroundColor(juce::Colours::crimson);
        OutPanel.setBackgroundColor(juce::Colours::darkgreen);

        addAndMakeVisible(DelayPanel);
        addAndMakeVisible(LFOPanel);
        addAndMakeVisible(OutPanel);

    }

    
    void resized() override
    {

        // Add space between the panels
        const int panelSpacing = 30;

        const int panelWidth = (getWidth() - 3 * panelSpacing) / 2; // Subtract spacing from total width
        const int panelHeight = (getHeight() - 100) / 2;
        
        const int padding = 20;
        const int topBarHeight = 0;

        DelayPanel.setBounds(padding, topBarHeight + padding, panelWidth, panelHeight);
        
        const int panelOffsetX = panelWidth + 2 * panelSpacing;

        LFOPanel.setBounds(panelOffsetX, topBarHeight + padding, panelWidth, panelHeight);
        OutPanel.setBounds(getWidth() / 2 - panelWidth / 2, panelHeight + topBarHeight + padding * 2, panelWidth, panelHeight);

    }
    
    std::vector<SliderWithLabel*> getSliders()
    {
        std::vector<SliderWithLabel*> sliders;

        sliders.push_back(&DelaySlider);
        sliders.push_back(&FeedbackSlider);
        sliders.push_back(&CutoffSlider);
        sliders.push_back(&RateSlider);
        sliders.push_back(&DepthSlider);
        sliders.push_back(&SaturationSlider);
        sliders.push_back(&PanSlider);
        sliders.push_back(&MixSlider);

        return sliders;
    }
    
    void randomizeShape()
    {
        float randomFloat = juce::Random::getSystemRandom().nextFloat();

        int range = ShapeComboBox.getComboBox().getNumItems();
        int randomChoice = randomFloat * range + 1;
        ShapeComboBox.getComboBox().setSelectedId(randomChoice);
    
    }

    
      
private:
    
    SliderWithLabel DelaySlider;
    SliderWithLabel FeedbackSlider;
    SliderWithLabel CutoffSlider;
    
    SliderWithLabel RateSlider;
    SliderWithLabel DepthSlider;
    ComboBoxWithLabel ShapeComboBox;
    
    SliderWithLabel SaturationSlider;
    SliderWithLabel PanSlider;
    SliderWithLabel MixSlider;
    
//    SliderWithLabel slider1, slider2, slider3;
    
    ParameterPanel DelayPanel;
    ParameterPanel LFOPanel;
    ParameterPanel OutPanel;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> feedbackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> depthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> shapeAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> panAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> levelAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> saturationAttachment;
    
};
