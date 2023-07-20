/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ComboverAudioProcessor::ComboverAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParameters())
#endif
{
    for (int i =0; i < maxNumCombs; i++) {
        mDelays[i] = std::make_unique<ModDelay>();
        mDelays[i]->prepare(getSampleRate());
        mLFOs[i] = std::make_unique<LFO>();
        mLFOs[i]->setSampleRate(getSampleRate());
    }
    apvts.state = juce::ValueTree("savedParams");
    
}

juce::AudioProcessorValueTreeState::ParameterLayout ComboverAudioProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout params;
    
    params.add(std::make_unique<juce::AudioParameterFloat>(ParameterID{"mDELAY", 1}, "mDELAY", 0.f, 200.f, 100.f));
    
    params.add(std::make_unique<juce::AudioParameterFloat>(ParameterID{"mFEEDBACK", 1}, "mFEEDBACK", 0.f, 200.f, 100.f));
    
    params.add(std::make_unique<juce::AudioParameterFloat>(ParameterID{"mCUTOFF", 1}, "mCUTOFF", 0.f, 100.f, 100.f));
    
    params.add(std::make_unique<juce::AudioParameterFloat>(ParameterID{"mDEPTH", 1}, "mDEPTH", 0.f, 200.f, 100.f));
    
    params.add(std::make_unique<juce::AudioParameterFloat>(ParameterID{"mRATE", 1}, "mRATE", 0.f, 200.f, 100.f));
    
    params.add(std::make_unique<juce::AudioParameterFloat>(ParameterID{"mWIDTH", 1}, "mWIDTH", 0.f, 200.f, 100.f));
    
    params.add(std::make_unique<juce::AudioParameterFloat>(ParameterID{"mMIX", 1}, "mMIX", 0.f, 100.f, 100.f));
    
    params.add(std::make_unique<juce::AudioParameterInt>(ParameterID{"nCOMBS", 1}, "nCOMBS", 2, 12, 6));
    
    for (int i = 0; i < maxNumCombs; i++)
    {
        int combIndex = i+1;
        
        juce::NormalisableRange<float> delayRange(1.f,200.f);
        delayRange.setSkewForCentre(10);
        
        params.add(std::make_unique<AudioParameterFloat>(ParameterID{std::string("DELAY_") + std::to_string(combIndex), 1}, "DELAY_" + std::to_string(combIndex), delayRange, 5.f));
        
        params.add(std::make_unique<AudioParameterFloat>(ParameterID{std::string("FEEDBACK_") + std::to_string(combIndex), 1}, "FEEDBACK_" + std::to_string(combIndex), -100., 100., 50.f));
    
        params.add(std::make_unique<AudioParameterFloat>(ParameterID{std::string("CUTOFF_") + std::to_string(combIndex), 1}, "CUTOFF_" + std::to_string(combIndex), 0,100, 0.f));
        
        params.add(std::make_unique<AudioParameterFloat>(ParameterID{std::string("DEPTH_") + std::to_string(combIndex), 1}, "DEPTH_" + std::to_string(combIndex), 0, 100, 100.f));
        
        juce::NormalisableRange<float> rateRange(0.0001f, 20.f, 0.f, 1.f, true);
        rateRange.setSkewForCentre(1.f);
        params.add(std::make_unique<AudioParameterFloat>(ParameterID{std::string("RATE_") + std::to_string(combIndex), 1}, "RATE_" + std::to_string(combIndex), rateRange, .5f));
        
        params.add(std::make_unique<AudioParameterFloat>(ParameterID{std::string("PAN_") + std::to_string(combIndex), 1}, "PAN_" + std::to_string(combIndex), -100, 100, 0.f));
        
        params.add(std::make_unique<AudioParameterFloat>(ParameterID{std::string("SATURATION_") + std::to_string(combIndex), 1}, "SATURATION_" + std::to_string(combIndex), 0,100, 0.f));
        
        params.add(std::make_unique<AudioParameterFloat>(ParameterID{std::string("MIX_") + std::to_string(combIndex), 1}, "MIX_" + std::to_string(combIndex), 0, 100, 100.f));
        
        params.add(std::make_unique<AudioParameterChoice>(ParameterID{std::string("SHAPE_") + std::to_string(combIndex), 1}, "SHAPE_" + std::to_string(combIndex), juce::StringArray("Sine", "Triangle", "Square", "Sawtooth", "Random"), 0));
    
    }
    
    return params;
    
    
    
}

ComboverAudioProcessor::~ComboverAudioProcessor()
{
}

//==============================================================================
const juce::String ComboverAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ComboverAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ComboverAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ComboverAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ComboverAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ComboverAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ComboverAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ComboverAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ComboverAudioProcessor::getProgramName (int index)
{
    return {};
}

void ComboverAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ComboverAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    for (int i=0; i < maxNumCombs; i++) {
        mDelays[i]->prepare(sampleRate);
        mLFOs[i]->setSampleRate(sampleRate);
    }
}

void ComboverAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ComboverAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ComboverAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    int currentNumCombs = apvts.getRawParameterValue("nCOMBS")->load();
    
    // initialize arrays to store each comb's param values
    std::vector<float> delayValues(currentNumCombs);
    std::vector<float> feedbackValues(currentNumCombs);
    std::vector<float> cutoffValues(currentNumCombs);
    std::vector<float> rateValues(currentNumCombs);
    std::vector<float> depthValues(currentNumCombs);
    std::vector<float> saturationValues(currentNumCombs);
    std::vector<float> panValues(currentNumCombs);
    std::vector<float> levelValues(currentNumCombs);
    std::vector<int> shapeChoiceValues(currentNumCombs);

    float mDelayValue = apvts.getRawParameterValue("mDELAY")->load() / 100.f;
    float mFeedbackValue = apvts.getRawParameterValue("mFEEDBACK")->load()/ 100.f;
    float mDampingValue = apvts.getRawParameterValue("mCUTOFF")->load() / 100.f;
    float mRateValue = apvts.getRawParameterValue("mRATE")->load() / 100.f;
    float mDepthValue = apvts.getRawParameterValue("mDEPTH")->load() / 100.f;
    float mWidthValue = apvts.getRawParameterValue("mWIDTH")->load() / 100.f;
    float mMixValue = apvts.getRawParameterValue("mMIX")->load() / 100.f;
    
    for (int numComb = 0; numComb < currentNumCombs; ++numComb)
    {
        int combIndex = numComb + 1;
        
        delayValues[numComb] = std::clamp((apvts.getRawParameterValue("DELAY_" + std::to_string(combIndex))->load() / 1000.f) * mDelayValue, 0.0f, .003f);
        feedbackValues[numComb] = std::clamp((apvts.getRawParameterValue("FEEDBACK_" + std::to_string(combIndex))->load() / 100) * mFeedbackValue, -10.0f, 10.0f);

        float cutoff = apvts.getRawParameterValue("CUTOFF_" + std::to_string(combIndex))->load() * mDampingValue;
        cutoffValues[numComb] = ((20.e3 + (minDampingFreq - 20.e3) * (cutoff / 100.0f)));
        rateValues[numComb] = (apvts.getRawParameterValue("RATE_" + std::to_string(combIndex))->load() * mRateValue);
        depthValues[numComb] = (apvts.getRawParameterValue("DEPTH_" + std::to_string(combIndex))->load() / 100.f) * mDepthValue;
        saturationValues[numComb] = apvts.getRawParameterValue("SATURATION_" + std::to_string(combIndex))->load() / 100.f;
        panValues[numComb] = (apvts.getRawParameterValue("PAN_" + std::to_string(combIndex))->load() / 100.f) * mWidthValue;
        levelValues[numComb] = (apvts.getRawParameterValue("MIX_" + std::to_string(combIndex))->load() / 100.f) * mMixValue;
        shapeChoiceValues[numComb] = apvts.getRawParameterValue("SHAPE_" + std::to_string(combIndex))->load() + 1;
    }

    const float maxGain = 15 ; //db
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            const float inSample = channelData[sample];
            
            float sum = 0;
            int count = 0;
            
            for (int i = channel; i < currentNumCombs; i+=2)
            {
                
                mDelays[i]->fdbackLPF.setCutoff(cutoffValues[i]);
                
                const float modulationSample = mLFOs[i]->process(shapeChoiceValues[i], rateValues[i], depthValues[i]);
                
                float currentSample = mDelays[i]->processSample(inSample, delayValues[i], feedbackValues[i], levelValues[i], juce::Decibels::decibelsToGain(saturationValues[i] * maxGain), modulationSample);
                
                const float panValue = panValues[i];
                
                float panLeft = (panValue <= 0.f) ? 1.f : 1.f - (panValue);
                float panRight = (panValue >= 0.f) ? 1.f : 1.f + (panValue);

                currentSample *= (channel == 0) ? panLeft : panRight;
                
                sum += currentSample;
                
                count++;
                
            }
            
            const float outSample = sum / count;
            
            channelData[sample] = outSample;
            
        }
    }
}

//==============================================================================
bool ComboverAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ComboverAudioProcessor::createEditor()
{
    return new ComboverAudioProcessorEditor (*this);
}

//==============================================================================
void ComboverAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    std::unique_ptr<juce::XmlElement> xml (apvts.state.createXml());
    copyXmlToBinary(*xml, destData);
}

void ComboverAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> stateInfo (getXmlFromBinary(data, sizeInBytes));
    
    if (stateInfo != nullptr && stateInfo->hasTagName(apvts.state.getType()))
    {
        apvts.state = juce::ValueTree::fromXml(*stateInfo);
    }
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ComboverAudioProcessor();
}

