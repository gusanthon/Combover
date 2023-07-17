/*
  ==============================================================================

    ModDelay.cpp
    Created: 6 Jun 2023 6:40:36pm
    Author:  Gus Anthon

  ==============================================================================
*/

#include "ModDelay.h"
#include "JuceHeader.h"

inline float linear_interp(float x0, float x1, float fraction)
{
    return (1 - fraction) * x0 + fraction * x1;
}


ModDelay::ModDelay()
    : mSampleRate(-1), mFeedbackSample(0.0), mTimeSmoothed(0.0f), mDelayIndex(0)
{

}

ModDelay::~ModDelay()
{

}

void ModDelay::prepare(double inSampleRate)
{
    mSampleRate = inSampleRate;
    
    fdbackLPF.prepare(mSampleRate);
    fdbackLPF.setCutoff(1000.f);

}

void ModDelay::reset()
{
    mTimeSmoothed = 0.0f;
    juce::zeromem(mBuffer, (sizeof(double) * maxDelayBufferSize));
}


float ModDelay::processSample(float inSample, float inTime, float inFeedback, float inWetDry, float gain, float inModulation)
{
    const float wet = inWetDry;
    const float dry = 1.0f - wet;

    /**
     .003 = delay amount (center of modulation signal) = 3 ms = inTime
     .002 = amplitude of modulation signal = 2 ms = Depth
     
     @TODO : make depth param in terms of ms
     */

    
    float modAmplitude = .003f;
    
    if(inTime < modAmplitude)
        modAmplitude = .98 * inTime;

    const double delayTimeMod = (inTime + (modAmplitude * inModulation));

    mTimeSmoothed = mTimeSmoothed - SmoothingCoefficient_Fine * (mTimeSmoothed - delayTimeMod);

    const double delayTimeInSamples = (mTimeSmoothed * mSampleRate);

    const double sample = getInterpolatedSample(delayTimeInSamples);
    
//    jassert(std::abs(sample) <= 1.f);
    
    mFeedbackSample = sample;

    float wetSample = BBD_approx(sample * wet * gain);// sample * wet;
    float drySample = inSample * dry;

//    jassert(std::abs(wetSample) <= 1.0f);

    // Mix wet and dry signals
    float outSample = drySample + fdbackLPF.processSample(wetSample);

    mBuffer[mDelayIndex] = (inSample + (mFeedbackSample * inFeedback));

    mDelayIndex++;

    if (mDelayIndex >= maxDelayBufferSize)
    {
        mDelayIndex -= maxDelayBufferSize;
    }

    return outSample;
}


double ModDelay::getInterpolatedSample(float inDelayTimeInSamples)
{
    double readPosition = (double)mDelayIndex - inDelayTimeInSamples;

    if (readPosition < 0.0f)
    {
        readPosition += maxDelayBufferSize;
    }

    int index_y0 = (int)readPosition - 1;

    if (index_y0 <= 0)
    {
        index_y0 += maxDelayBufferSize;
    }

    int index_y1 = readPosition;

    if (index_y1 > maxDelayBufferSize)
    {
        index_y1 -= maxDelayBufferSize;
    }

    const float sample_y0 = mBuffer[index_y0];
    const float sample_y1 = mBuffer[index_y1];
    const float fraction = readPosition - (int)readPosition;

    double outSample = linear_interp(sample_y0, sample_y1, fraction);

    return outSample;
}
