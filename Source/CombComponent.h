/*
  ==============================================================================

    mParamsComponent.h
    Created: 24 Jun 2023 5:04:42pm
    Author:  Gus Anthon

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "SliderWithLabel.h"
#include "ParameterPanel.h"

class CombComponent : public juce::Component
{
public:
    CombComponent(ComboverAudioProcessor& proc, int combNum) :
        DelaySlider("DELAY " + std::to_string(combNum)),
        FeedbackSlider("FEEDBACK " + std::to_string(combNum)),
        CutoffSlider("DAMPING " + std::to_string(combNum)),
        RateSlider("RATE " + std::to_string(combNum)),
        DepthSlider("DEPTH " + std::to_string(combNum)),
        PanSlider("PAN " + std::to_string(combNum)),
        MixSlider("LEVEL " + std::to_string(combNum)),
        SaturationSlider("SATURATION " + std::to_string(combNum)),
        ShapeComboBox("SHAPE "+ std::to_string(combNum)),
        DelayPanel(juce::Array<juce::Component*>{&DelaySlider, &FeedbackSlider, &CutoffSlider}, true),
        LFOPanel(juce::Array<juce::Component*>{&RateSlider, &DepthSlider, &ShapeComboBox}, true),
        OutPanel(juce::Array<juce::Component*>{&SaturationSlider, &PanSlider, &MixSlider}, true)
    {
        
        delayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(proc.apvts, "DELAY_" + std::to_string(combNum), DelaySlider.getSlider());
        feedbackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(proc.apvts, "FEEDBACK_" + std::to_string(combNum), FeedbackSlider.getSlider());
        cutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(proc.apvts, "CUTOFF_" + std::to_string(combNum), CutoffSlider.getSlider());
        depthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(proc.apvts, "DEPTH_" + std::to_string(combNum), DepthSlider.getSlider());
        panAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(proc.apvts, "PAN_" + std::to_string(combNum), PanSlider.getSlider());
        levelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(proc.apvts, "MIX_" + std::to_string(combNum), MixSlider.getSlider());

        rateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(proc.apvts, "RATE_" + std::to_string(combNum), RateSlider.getSlider());

        saturationAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(proc.apvts, "SATURATION_" + std::to_string(combNum), SaturationSlider.getSlider());
        
        shapeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(proc.apvts, "SHAPE_" + std::to_string(combNum), ShapeComboBox.getComboBox());
        
        
        
//        DelaySlider.getSlider().setValue(10 * combNum);
        
        DelayPanel.setTitleText("DELAY "+ std::to_string(combNum));
        DelaySlider.setRange(.01f, 300.f, .01f);
        DelaySlider.setSuffix("ms");
        
        FeedbackSlider.setRange(-100.f, 100.f, .1f);
        FeedbackSlider.setSuffix("%");
        FeedbackSlider.setNumDecimalPlacesToDisplay(1);
        
        CutoffSlider.setRange(0.f, 100.f, .1f);
        CutoffSlider.setSuffix("%");
        
        LFOPanel.setTitleText("LFO "+ std::to_string(combNum));
        DepthSlider.setNumDecimalPlacesToDisplay(1);
        DepthSlider.setRange(0.f, 100.f, .1f);
        DepthSlider.setSuffix("%");
        
        RateSlider.setRange(0.01f, 20.f, .1f);
        RateSlider.setSuffix("Hz");
        RateSlider.setNumDecimalPlacesToDisplay(1);
        
        PanSlider.setRange(-100.f, 100.f, .1f);
        PanSlider.setNumDecimalPlacesToDisplay(1);
        MixSlider.setRange(0.f, 100.f, .1f);
        MixSlider.setNumDecimalPlacesToDisplay(1);
        SaturationSlider.setRange(0, 100, .1);
        SaturationSlider.setNumDecimalPlacesToDisplay(1);
        OutPanel.setTitleText("OUT");
        
        DelayPanel.setBackgroundColor(juce::Colours::darkblue.brighter());
        LFOPanel.setBackgroundColor(juce::Colours::crimson);
        OutPanel.setBackgroundColor(juce::Colours::darkgreen);

        
//        addAndMakeVisible(&mDelaySlider);
        addAndMakeVisible(DelayPanel);
        addAndMakeVisible(LFOPanel);
        addAndMakeVisible(OutPanel);
    }
    
//    void resized() override
//    {
//        mDelayPanel.setBounds(getLocalBounds());
//        mLFOPanel.setBounds(getLocalBounds());
//        mDelayPanel.resized();
//    }
    
    void resized() override
    {

        // Add space between the panels
        const int panelSpacing = 30;

        const int panelWidth = (getWidth() - 3 * panelSpacing) / 2; // Subtract spacing from total width
        const int panelHeight = (getHeight() - 100) / 2;
        
        const int padding = 20;
        const int topBarHeight = 0;

        DelayPanel.setBounds(padding, topBarHeight + padding, panelWidth, panelHeight);
        
        // Add space between the panels
//        const int panelSpacing = 40;
        const int panelOffsetX = panelWidth + 2 * panelSpacing;

        LFOPanel.setBounds(panelOffsetX, topBarHeight + padding, panelWidth, panelHeight);
        OutPanel.setBounds(getWidth() / 2 - panelWidth / 2, panelHeight + topBarHeight + padding * 2, panelWidth, panelHeight);


    }
    
    std::vector<SliderWithLabel*> getParameters()
    {
        std::vector<SliderWithLabel*> parameters;

        parameters.push_back(&DelaySlider);
        parameters.push_back(&FeedbackSlider);
        parameters.push_back(&CutoffSlider);
        parameters.push_back(&RateSlider);
        parameters.push_back(&DepthSlider);
        parameters.push_back(&SaturationSlider);
        parameters.push_back(&PanSlider);
        parameters.push_back(&MixSlider);

        return parameters;
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
