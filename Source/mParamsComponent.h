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


class mParamsComponent : public juce::Component
{
public:
    mParamsComponent(ComboverAudioProcessor& proc) :
        mDelaySlider("DELAY"),
        mFeedbackSlider("FEEDBACK"),
        mCutoffSlider("DAMPING"),
        mRateSlider("RATE"),
        mDepthSlider("DEPTH"),
        mWidthSlider("WIDTH"),
        mMixSlider("MIX"),
        nCombsSlider("# OF COMBS"),
        mDelayPanel(juce::Array<juce::Component*>{&mDelaySlider, &mFeedbackSlider, &mCutoffSlider}, true),
        mLFOPanel(juce::Array<juce::Component*>{&mRateSlider, &mDepthSlider}, true),
        mOutPanel(juce::Array<juce::Component*>{&nCombsSlider, &mWidthSlider, &mMixSlider}, true)
    {
        mDelayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(proc.apvts, "mDELAY", mDelaySlider.getSlider());
        mFeedbackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(proc.apvts, "mFEEDBACK", mFeedbackSlider.getSlider());
        mCutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(proc.apvts, "mCUTOFF", mCutoffSlider.getSlider());
        mDepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(proc.apvts, "mDEPTH", mDepthSlider.getSlider());
        mWidthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(proc.apvts, "mWIDTH", mWidthSlider.getSlider());
        mMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(proc.apvts, "mMIX", mMixSlider.getSlider());
        mRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(proc.apvts, "mRATE", mRateSlider.getSlider());
        nCombsAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(proc.apvts, "nCOMBS", nCombsSlider.getSlider());
        
        
        mDelayPanel.setLabelText("mDELAY");
        
        mDelaySlider.getSlider().setRange(.0f, 200.f, .1f);
        mDelaySlider.getSlider().setTextValueSuffix(" %");
        mDelaySlider.getSlider().setNumDecimals(1);
        
        mFeedbackSlider.getSlider().setRange(-100.f, 120.f, .1f);
        mFeedbackSlider.getSlider().setTextValueSuffix("%");
        mFeedbackSlider.getSlider().setNumDecimals(1);
        
        mCutoffSlider.getSlider().setRange(0.f, 100.f, .1f);
        mCutoffSlider.getSlider().setTextValueSuffix(" %");;
        mCutoffSlider.getSlider().setNumDecimals(1);
        
        mLFOPanel.setLabelText("mLFO");
        
        mDepthSlider.getSlider().setNumDecimalPlacesToDisplay(1);
        mDepthSlider.getSlider().setRange(0.f, 100.f, .1f);
        mDepthSlider.getSlider().setTextValueSuffix(" %");
        
        mRateSlider.getSlider().setRange(0.f, 200.f, .1f);
        mRateSlider.getSlider().setTextValueSuffix(" %");
        mRateSlider.getSlider().setNumDecimalPlacesToDisplay(1);
        
        mWidthSlider.getSlider().setRange(0.f, 200.f, .1f);
        mWidthSlider.getSlider().setNumDecimalPlacesToDisplay(1);
        mWidthSlider.getSlider().setTextValueSuffix(" %");
        
        mMixSlider.getSlider().setRange(0.f, 100.f, .1f);
        mMixSlider.getSlider().setTextValueSuffix(" %");
        
        mMixSlider.getSlider().setNumDecimalPlacesToDisplay(1);
        nCombsSlider.getSlider().setRange(2, 12, 1);
        nCombsSlider.getSlider().setNumDecimalPlacesToDisplay(0);
        
        mOutPanel.setLabelText("mOUT");

        mDelayPanel.setBackgroundColor(juce::Colours::darkblue.brighter());
        mLFOPanel.setBackgroundColor(juce::Colours::crimson);
        mOutPanel.setBackgroundColor(juce::Colours::darkgreen);
        
        addAndMakeVisible(mDelayPanel);
        addAndMakeVisible(mLFOPanel);
        addAndMakeVisible(mOutPanel);
    }
    

    void resized() override
    {


        // Add space between the panels
        const int panelSpacing = 30;

        const int panelWidth = (getWidth() - 3 * panelSpacing) / 2; // Subtract spacing from total width
        const int panelHeight = (getHeight() - 100) / 2;
        
        const int padding = 20;
        const int topBarHeight = 0;

        mDelayPanel.setBounds(padding, topBarHeight + padding, panelWidth, panelHeight);
        
        // Add space between the panels
        const int panelOffsetX = panelWidth + 2 * panelSpacing;

        mLFOPanel.setBounds(panelOffsetX, topBarHeight + padding, panelWidth, panelHeight);
        mOutPanel.setBounds(getWidth() / 2 - panelWidth / 2, panelHeight + topBarHeight + padding * 2, panelWidth, panelHeight);


    }
    
    std::vector<SliderWithLabel*> getSlideres()
    {
        std::vector<SliderWithLabel*> parameters;

        parameters.push_back(&mDelaySlider);
        parameters.push_back(&mFeedbackSlider);
        parameters.push_back(&mCutoffSlider);
        parameters.push_back(&mRateSlider);
        parameters.push_back(&mDepthSlider);
        parameters.push_back(&mWidthSlider);
        parameters.push_back(&mMixSlider);
        parameters.push_back(&nCombsSlider);

        return parameters;
    }

    
private:
        
    SliderWithLabel mDelaySlider;
    SliderWithLabel mFeedbackSlider;
    SliderWithLabel mCutoffSlider;
    
    SliderWithLabel mRateSlider;
    SliderWithLabel mDepthSlider;
    
    SliderWithLabel mWidthSlider;
    SliderWithLabel mMixSlider;
    SliderWithLabel nCombsSlider;
    
    ParameterPanel mDelayPanel;
    ParameterPanel mLFOPanel;
    ParameterPanel mOutPanel;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mDelayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mFeedbackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mCutoffAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mRateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mDepthAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mWidthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mMixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> nCombsAttachment;
    
};
