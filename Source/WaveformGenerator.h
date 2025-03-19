/*
  ==============================================================================

    WaveformGenerator.h
    Created: 23 Feb 2025 6:59:25pm
    Author:  Joe Kenyon

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#define PI (3.141592653589793238L)
#define TWO_PI (2 * 3.141592653589793238L)

class WaveformGenerator
{
public:
    //WaveformGenerator();
    WaveformGenerator(float sampleRate_, float frequency_);
    
    enum WaveformType
    {
        NOISE = 0,
        SINE,
        TRIANGLE,
        SQUARE,
        SAWTOOTH,
        NUM_WAVEFORMS
    };
        
    float generateSample();
    float sine();
    float sawtooth();
    float square();
    float triangle();
    float noise();
    void setWaveform(WaveformType waveform_);
    void setFrequency(float frequency_);
    void setSampleRate(float sampleRate_);
    void setNoiseGain(float noiseGain_);
    float getNoiseGain();
    
private:
    float currentAngle;
    float angleDelta;
    float frequency;
    float sampleRate;
    float noiseGain;
      
    // array of pointers to our waveform functions
    float (WaveformGenerator::*waveformFunctions[NUM_WAVEFORMS])();
    float (WaveformGenerator::*waveformFunction)();
};
