/*
  ==============================================================================

    DelayLine.cpp
    Created: 17 Feb 2025 3:49:02pm
    Author:  Joe Kenyon
  ==============================================================================
*/

#include "DelayLine.h"
DelayLine::DelayLine()
{

}
DelayLine::DelayLine(float maxDelayTime_, float delayTime_, float sampleRate_)
{
    // set member variables
    sampleRate = sampleRate_;
    maxDelayTime = maxDelayTime_;
    delayBufferLen = maxDelayTime * sampleRate;
    delayTime = delayTime_;
    delayTimeSamples = delayTime * sampleRate;

    // allocate memory for our delay buffer
    // assumes only 1 channels
    delayBuffer.setSize(1, delayBufferLen);
    delayBuffer.clear();
    writePosition = 0;
    setDelayTime(delayTime);
}
void DelayLine::setSampleRate(float sampleRate_)
{
    sampleRate = sampleRate_;
}
void DelayLine::setDelayTime(float delayTime_)
{
    jassert(delayTime_ < maxDelayTime);
    delayTime = delayTime_;
    delayTimeSamples = delayTime * sampleRate;
    readPosition = (writePosition-(delayTimeSamples)+delayBufferLen) % delayBufferLen;
}
float DelayLine::readSample()
{
    return delayBuffer.getWritePointer(0)[readPosition];
}
void DelayLine::writeSample(float sample)
{
    delayBuffer.getWritePointer(0)[writePosition] = sample;
}
void DelayLine::advance()
{
    writePosition = (writePosition >= delayBufferLen) ? 0 : writePosition + 1;
    readPosition  = (readPosition >= delayBufferLen)  ? 0 : readPosition  + 1;
}
void DelayLine::clear()
{
    delayBuffer.clear();
}
