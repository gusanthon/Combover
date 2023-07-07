/*
  ==============================================================================

    RCLowPass.h
    Created: 23 Jun 2023 3:11:00pm
    Author:  Gus Anthon

  ==============================================================================
*/

#pragma once
#include "chowdsp_wdf.h"

namespace wdft = chowdsp::wdft;

class RCLowpass {
    
public:
    RCLowpass() = default;
    
    wdft::ResistorT<double> r1 { 1.e3 };     // 1 KOhm Resistor
    wdft::CapacitorT<double> c1 { Cap };   // 1 uF capacitor
    
    wdft::WDFSeriesT<double, decltype (r1), decltype (c1)> s1 { r1, c1 };   // series connection of r1 and c1
    wdft::PolarityInverterT<float, decltype(s1)> i1 { s1 };                 // invert polarity
    wdft::IdealVoltageSourceT<double, decltype (s1)> vs { s1 };             // input voltage source
    
    // prepare the WDF model here...
    void prepare (double sampleRate) {
        c1.prepare (sampleRate);
    }
    
    // use the WDF model to process one sample of data
    inline double processSample (double x) {
        vs.setVoltage (x);

        vs.incident(i1.reflected());
        i1.incident(vs.reflected());

        return wdft::voltage<double> (c1);
    }
    
    void setCutoff(double newCutoff)
    {
        if (newCutoff != cutoff)
        {
            cutoff = newCutoff;
            double Res = 1.f / (twopi * Cap * cutoff);
            r1.setResistanceValue(Res);
        }
    }
    

private:
    constexpr static double twopi = 6.28318530718;
    constexpr static double Cap = 1.e-6;
    double cutoff;
};
