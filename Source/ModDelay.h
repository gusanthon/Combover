/*
  ==============================================================================

    ModDelay.h
    Created: 6 Jun 2023 6:40:36pm
    Author:  Gus Anthon

  ==============================================================================
*/

#pragma once

#define maxDelayBufferSize 48000
#define SmoothingCoefficient_Fine 0.002


#include "LFO.h"
#include "RCLowPass.h"
#include "JuceHeader.h"

class ModDelay
{
public:

    ModDelay();
    ~ModDelay();

    void prepare(double inSampleRate);

    void reset();

    void setWaveShaper(int choice);

    float processSample(float inSample, float inTime, float inFeedback, float inWetDry, float gain, float inModulation);
    
    RCLowpass fdbackLPF;

private:

    double getInterpolatedSample(float inDelayTimeInSamples);
    
    double mSampleRate;
    double mBuffer[maxDelayBufferSize];
    int mBufferSize;

    double mFeedbackSample;

    float mTimeSmoothed;

    int mDelayIndex;
   
    
    static inline float BBD_approx(float inputSample) {
        return (inputSample - (pow(inputSample, 2) / 8.0f) - (pow(inputSample, 3) / 16.0f));// + 0.125f);
    }

    
};
