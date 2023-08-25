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
    Sine = 1,
    Triangle,
    Square,
    Sawtooth,
    Random
};


class LFO
{
public:

    LFO();

    ~LFO();

    void reset();

    void setSampleRate(double inSampleRate);
    
    float process(Waveform waveform, float inRate, float inDepth);

private:

    double mSampleRate;

    float mPhase;
    
};
