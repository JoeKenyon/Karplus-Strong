/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define NOISE 0
#define SINE 1
#define TRIANGLE 2
#define SQUARE 3
#define SAWTOOTH 4



//==============================================================================
KarplusStrongAudioProcessor::KarplusStrongAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), pvms(*this, nullptr, "params", createParameterLayout())
                        ,waveformGenerator(getSampleRate(), 800)
#endif
{
}

KarplusStrongAudioProcessor::~KarplusStrongAudioProcessor()
{
}

//==============================================================================
const juce::String KarplusStrongAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool KarplusStrongAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool KarplusStrongAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool KarplusStrongAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double KarplusStrongAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int KarplusStrongAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int KarplusStrongAudioProcessor::getCurrentProgram()
{
    return 0;
}

void KarplusStrongAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String KarplusStrongAudioProcessor::getProgramName (int index)
{
    return {};
}

void KarplusStrongAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void KarplusStrongAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    delayTime = pvms.getRawParameterValue("delay")->load()/1000.0f;
    
    delayLine = DelayLine(2.0f, delayTime, sampleRate);
    
    waveformGenerator.setSampleRate(sampleRate);
    waveformGenerator.setFrequency(800);
    
    lowPassFilter = LowPassFilter(1000.0f, sampleRate);
}

void KarplusStrongAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool KarplusStrongAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void KarplusStrongAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    auto numSamples = buffer.getNumSamples();
    
    keyboardState.processNextMidiBuffer(midiMessages, 0, numSamples, true);
    
    auto sampleRate   = juce::AudioProcessor::getSampleRate();
    float widthTime   = pvms.getRawParameterValue("width")->load()/1000.0f;
    float decay       = pvms.getRawParameterValue("decay")->load();
    auto  noiseSource = static_cast<WaveformGenerator::WaveformType>(pvms.getRawParameterValue("source")->load());
    bool  pluck       = pvms.getRawParameterValue("pluck")->load();
    float cutOff      = pvms.getRawParameterValue("cutOff")->load();

    lowPassFilter.setCutoff(cutOff);
    
    // if pluck not already enabled check for midi
    if(!pluck)
        for (const auto metadata : midiMessages)
        {
            const auto msg = metadata.getMessage();
            if (msg.isNoteOn())
            {
                float hz = msg.getMidiNoteInHertz(msg.getNoteNumber());
                delayTime = 1.0f/hz;
                pluck = true;
                break;
            }
        }
    else
    {
        delayTime = pvms.getRawParameterValue("delay")->load()/1000.0f;
    }
    
    waveformGenerator.setWaveform(noiseSource);
    delayLine.setDelayTime(delayTime);
    
    if (pluck)
    {
        pvms.getParameter("pluck")->setValueNotifyingHost(0);
        waveformGenerator.setNoiseGain(1.0f);
        delayLine.clear();
    }
    for (int sample = 0; sample < numSamples; ++sample)
    {
        // get output buffers
        float* channelDataL = buffer.getWritePointer(0);
        float* channelDataR = buffer.getWritePointer(1);

        // generate our noise sample
        float noiseSample = 0.0;
        float noiseGain = waveformGenerator.getNoiseGain();
        
        if (noiseGain > 0.0f)
        {
            noiseSample = waveformGenerator.generateSample();
            noiseGain = fmax(0.0f, noiseGain - 1.0f / (widthTime * sampleRate));
            waveformGenerator.setNoiseGain(noiseGain);
        }

        // get a delayed sample
        float delayedSample = delayLine.readSample();

        // filter it and put and get some feedback going
        delayedSample = lowPassFilter.processSample(delayedSample);
        float out = noiseSample + delayedSample * decay;
        delayLine.writeSample(out);

        // send this to our output buffer
        channelDataL[sample] = out;
        channelDataR[sample] = out;
        
        // advance delay pointers
        delayLine.advance();
    }
}

//==============================================================================
bool KarplusStrongAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* KarplusStrongAudioProcessor::createEditor()
{
    return new KarplusStrongAudioProcessorEditor(*this);
}

//==============================================================================
void KarplusStrongAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void KarplusStrongAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new KarplusStrongAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout KarplusStrongAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    auto sources = {"Noise","Sine","Triangle","Square","Sawtooth"};
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"width",1}, "Width (ms)", 0.0f, 20.0f,10.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"delay",1}, "Delay (ms)", 0.0f, 20.0f,10.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"decay",1}, "Decay", 0.8f, 0.99f,0.90f));
    params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{"source",1}, "Source", sources, 0));
    params.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{"pluck",1}, "Pluck string", 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"cutOff",1}, "CutOff (Hz)", 10.0f, 20000.0f,1000.0f));
    return { params.begin(), params.end() };
}

