/*
  ==============================================================================

    LowPassFilter.cpp
    Created: 17 Feb 2025 3:49:46pm
    Author:  Joe Kenyon

  ==============================================================================
*/

#include "LowPassFilter.h"

LowPassFilter::LowPassFilter()
{

}
LowPassFilter::LowPassFilter(float cutOffFreq_, float sampleRate_)
{
    sampleRate = sampleRate_;
    cutOffFreq = cutOffFreq_;
    iirFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeFirstOrderLowPass(sampleRate, cutOffFreq);
}
void LowPassFilter::setCutoff(float cutOffFreq_)
{
    cutOffFreq = cutOffFreq_;
    iirFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeFirstOrderLowPass(sampleRate, cutOffFreq);
}
float LowPassFilter::processSample(float sample)
{
    return iirFilter.processSample(sample);
}