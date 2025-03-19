/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class KarplusStrongAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener
{
public:
    KarplusStrongAudioProcessorEditor (KarplusStrongAudioProcessor&);
    ~KarplusStrongAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider) override;

private:
    juce::Slider width;
    juce::Slider decay;
    juce::Slider delay;
    juce::Slider cutOff;
    juce::TextButton pluck;
    juce::ComboBox source;
    
    juce::Label widthLabel;
    juce::Label decayLabel;
    juce::Label delayLabel;
    juce::Label cutOffLabel;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> widthAttachement;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayAttachement;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayAttachement;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutOffAttachement;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> pluckAttachement;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> sourceAttachement;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    KarplusStrongAudioProcessor& audioProcessor;
    juce::MidiKeyboardComponent myKeyboard;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KarplusStrongAudioProcessorEditor)
};
