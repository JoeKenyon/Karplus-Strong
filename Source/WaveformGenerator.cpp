/*
  ==============================================================================

    WaveformGenerator.cpp
    Created: 23 Feb 2025 6:59:25pm
    Author:  Joe Kenyon

  ==============================================================================
*/

#include "WaveformGenerator.h"
WaveformGenerator::WaveformGenerator(float sampleRate_, float frequency_)
{
    sampleRate = sampleRate_;
    frequency = frequency_;
    
    currentAngle = 0.0;
    angleDelta = TWO_PI * frequency / sampleRate;
    
    // init function pointer array
    waveformFunctions[NOISE] = &WaveformGenerator::noise;
    waveformFunctions[SINE] = &WaveformGenerator::sine;
    waveformFunctions[TRIANGLE] = &WaveformGenerator::triangle;
    waveformFunctions[SQUARE] = &WaveformGenerator::square;
    waveformFunctions[SAWTOOTH] = &WaveformGenerator::sawtooth;
}
float WaveformGenerator::generateSample()
{
    float sample = (this->*waveformFunction)();
    
    // advance phase and wrap around
    currentAngle += (currentAngle >= TWO_PI) ? -TWO_PI : angleDelta;
    
    return sample;
}
void WaveformGenerator::setFrequency(float frequency_)
{
    frequency = frequency_;
    angleDelta = TWO_PI * frequency / sampleRate;
}
void WaveformGenerator::setSampleRate(float sampleRate_)
{
    sampleRate = sampleRate_;
    angleDelta = TWO_PI * frequency / sampleRate;
}
void WaveformGenerator::setWaveform(WaveformType waveformType_)
{
    waveformFunction = waveformFunctions[waveformType_];
}
float WaveformGenerator::sine()
{
    return noiseGain * (std::sin(currentAngle));
}
float WaveformGenerator::sawtooth()
{
    return noiseGain * (2.0f * (currentAngle / TWO_PI) - 1.0f);
}
float WaveformGenerator::square()
{
    return noiseGain * (2.0f * (std::sin(currentAngle) >= 0.0f) - 1.0f);
}
float WaveformGenerator::triangle()
{
    return noiseGain * (2.0f * std::fabs((currentAngle / PI) - 1.0f) - 1.0f);
}
float WaveformGenerator::noise()
{
    return 2.f * noiseGain * ((double)rand() / RAND_MAX) - static_cast<double>(1.0);
}
void WaveformGenerator::setNoiseGain(float noiseGain_)
{
    noiseGain = noiseGain_;
}
float WaveformGenerator::getNoiseGain()
{
    return noiseGain;
}
