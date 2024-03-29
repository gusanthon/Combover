/*
  ==============================================================================

    LFO.cpp
    Created: 6 Jun 2023 6:40:43pm
    Author:  Gus Anthon

  ==============================================================================
*/

#include "LFO.h"
#include "JuceHeader.h"


void LFO::reset()
{
    mPhase = 0.0f;
}

void LFO::setSampleRate(double inSampleRate)
{
    mSampleRate = inSampleRate;
}

float LFO::process(Waveform waveform, float inRate, float inDepth)
{
    mPhase = mPhase + (inRate / mSampleRate);

    if (mPhase > 1.0f)
    {
        mPhase = mPhase - 1.0f;
    }

    float lfoPosition;
    
    switch (waveform)
    {
        case Sine:
            lfoPosition = sinf(mPhase * juce::MathConstants<double>::twoPi);
            break;
        case Triangle:
            lfoPosition = 1.0f - std::abs((mPhase - 0.5f) * 2.0f);
            break;
        case Square:
            lfoPosition = mPhase < 0.5f ? 1.0f : -1.0f;
            break;
        case Sawtooth:
            lfoPosition = 1.0f - mPhase * 2.0f;
            break;
        case Random:
            lfoPosition = juce::Random::getSystemRandom().nextFloat();
            break;
    }

    lfoPosition *= inDepth;

    return lfoPosition;
}

