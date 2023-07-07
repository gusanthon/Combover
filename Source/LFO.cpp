/*
  ==============================================================================

    LFO.cpp
    Created: 6 Jun 2023 6:40:43pm
    Author:  Gus Anthon

  ==============================================================================
*/

#include "LFO.h"
#include "JuceHeader.h"

LFO::LFO()
{

}

LFO::~LFO()
{

}

void LFO::reset()
{
    mPhase = 0.0f;
//    juce::zeromem(modBuffer, sizeof(float) * maxDelayBufferSize);
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
    }

    lfoPosition *= inDepth;

    return lfoPosition;
}

//
//void LFO::process(Waveform waveform, float inRate, float inDepth, float inNumSamples)
//{
////    const float rate = juce::jmap(inRate, 0.0f, 1.0f, 0.0f, 20.0f);
////    std::cout<<inRate<<std::endl;
////    std::cout<<rate<<std::endl;
//    for (int i = 0; i < inNumSamples; i++)
//    {
//        mPhase = mPhase + (inRate / mSampleRate);
//
//        if (mPhase > 1.0f)
//        {
//            mPhase = mPhase - 1.0f;
//        }
//
//        float lfoPosition;
//        
//        switch (waveform)
//        {
//            case Sine:
//                lfoPosition = sinf(mPhase * juce::MathConstants<double>::twoPi );
//                break;
//            case Triangle:
//                lfoPosition = 1.0f - std::abs((mPhase - 0.5f) * 2.0f);
//                break;
//            case Square:
//                lfoPosition = mPhase < 0.5f ? 1.0f : -1.0f;
//                break;
//            case Sawtooth:
//                lfoPosition = 1.0f - mPhase * 2.0f;
//                break;
//        }
////        std::cout<<mPhase<<std::endl;
//        lfoPosition *= inDepth;
//        modBuffer[i] = lfoPosition;
//    }
//}
//
//float* LFO::getBuffer()
//{
//    return modBuffer;
//}
//
//
