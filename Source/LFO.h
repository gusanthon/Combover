/*
  ==============================================================================

    LFO.h
    Created: 6 Jun 2023 6:40:43pm
    Author:  Gus Anthon

  ==============================================================================
*/

#pragma once


enum Waveform
{
    Sine,
    Triangle,
    Square,
    Sawtooth
};


class LFO
{
public:

    LFO();

    ~LFO();

    void reset();

    void setSampleRate(double inSampleRate);

    void process(Waveform waveform, float inRate, float inDepth, float inNumSamples);
    
    float process(Waveform waveform, float inRate, float inDepth);

private:

    double mSampleRate;

    float mPhase;
    
};
