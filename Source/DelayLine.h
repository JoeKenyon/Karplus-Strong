/*
  ==============================================================================

    DelayLine.h
    Created: 17 Feb 2025 3:49:02pm
    Author:  Joe Kenyon

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class DelayLine
{
public:
    // delay in seconds
    DelayLine();
    DelayLine(float maxDelayTime, float delayTime_,float sampleRate_);
    void setDelayTime(float delayTime);
    void setSampleRate(float sampleRate_);
    float readSample();
    void writeSample(float sample);
    void clear();
    void advance();

private:
    juce::AudioSampleBuffer delayBuffer;
    unsigned int delayBufferLen;  // maxDelayTime * sampleRate
    float delayTime;
    float maxDelayTime;
    int delayTimeSamples;
    float sampleRate;
    unsigned int  readPosition;
    unsigned int writePosition;
};
