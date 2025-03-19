/*
  ==============================================================================

    LowPassFilter.h
    Created: 17 Feb 2025 3:49:46pm
    Author:  Joe Kenyon

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class LowPassFilter
{
public:
    LowPassFilter();
    LowPassFilter(float cutOffFreq_, float sampleRate_);
    void setCutoff(float cutOffFreq_);
    float processSample(float sample);
private:
    float cutOffFreq;
    float sampleRate;
    juce::dsp::IIR::Filter<float> iirFilter;
};