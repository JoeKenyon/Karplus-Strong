/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
KarplusStrongAudioProcessorEditor::KarplusStrongAudioProcessorEditor (KarplusStrongAudioProcessor& p)
: AudioProcessorEditor (&p), audioProcessor (p), myKeyboard(audioProcessor.keyboardState, juce::MidiKeyboardComponent::Orientation::horizontalKeyboard)
{
    setSize(500, 350);
    width.setSliderStyle(juce::Slider::LinearHorizontal);
    width.setTextBoxStyle(juce::Slider::TextBoxRight, false, 72, 32);
    width.setRange(0.0, 20.0, 8.0);
    width.setTextValueSuffix(" ms");
    addAndMakeVisible(width);
    
    delay.setSliderStyle(juce::Slider::LinearHorizontal);
    delay.setTextBoxStyle(juce::Slider::TextBoxRight, false, 72, 32);
    delay.setRange(0.0, 20.0, 8.0);
    delay.setTextValueSuffix(" ms");
    delay.addListener(this);
    addAndMakeVisible(delay);
    
    decay.setSliderStyle(juce::Slider::LinearHorizontal);
    decay.setTextBoxStyle(juce::Slider::TextBoxRight, false, 72, 32);
    decay.setRange(0.8, 0.99, 0.8);
    addAndMakeVisible(decay);
    
    source.addItem("Noise", 1);
    source.addItem("Sine", 2);
    source.addItem("Triangle", 3);
    source.addItem("Square", 4);
    source.addItem("Sawtooth", 5);
    addAndMakeVisible(source);
    
    pluck.setClickingTogglesState(true);
    pluck.setButtonText("Pluck Sting");
    addAndMakeVisible(pluck);
    
    cutOff.setSliderStyle(juce::Slider::LinearHorizontal);
    cutOff.setTextBoxStyle(juce::Slider::TextBoxRight, false, 72, 32);
    cutOff.setRange(10.0, 1000.0, 50.0);
    cutOff.setTextValueSuffix(" Hz");
    addAndMakeVisible(cutOff);
    
    myKeyboard.setAvailableRange(32,32+(12*4));
    myKeyboard.setKeyWidth(25);
    myKeyboard.setKeyPressBaseOctave(3);
    addAndMakeVisible(&myKeyboard);
    
    widthLabel.setText ("Width", juce::dontSendNotification);
    delayLabel.setText ("Delay", juce::dontSendNotification);
    decayLabel.setText ("Decay", juce::dontSendNotification);
    cutOffLabel.setText("CutOff", juce::dontSendNotification);
    widthLabel.attachToComponent(&width, true);
    delayLabel.attachToComponent(&delay, true);
    decayLabel.attachToComponent(&decay, true);
    cutOffLabel.attachToComponent(&cutOff, true);
    addAndMakeVisible(widthLabel);
    addAndMakeVisible(delayLabel);
    addAndMakeVisible(decayLabel);
    addAndMakeVisible(cutOffLabel);
    
    widthAttachement  = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.pvms, "width", width);
    delayAttachement  = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.pvms, "delay", delay);
    decayAttachement  = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.pvms, "decay", decay);
    sourceAttachement = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.pvms, "source", source);
    pluckAttachement  = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.pvms, "pluck", pluck);
    cutOffAttachement  = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.pvms, "cutOff", cutOff);
    
    setResizable(false, false);
}

KarplusStrongAudioProcessorEditor::~KarplusStrongAudioProcessorEditor()
{
}

//==============================================================================
void KarplusStrongAudioProcessorEditor::paint (juce::Graphics& g)
{
}

void KarplusStrongAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto bounds = getLocalBounds();
    const int size = (bounds.getHeight() / 8);
    auto labelSize = 45;

    width.setBounds(bounds.removeFromTop(size).removeFromRight(bounds.getWidth() - labelSize));
    delay.setBounds(bounds.removeFromTop(size).removeFromRight(bounds.getWidth() - labelSize));
    decay.setBounds(bounds.removeFromTop(size).removeFromRight(bounds.getWidth() - labelSize));
    source.setBounds(bounds.removeFromTop(size));
    pluck.setBounds(bounds.removeFromTop(size));
    cutOff.setBounds(bounds.removeFromTop(size).removeFromRight(bounds.getWidth() - labelSize));
    myKeyboard.setBounds(bounds.removeFromTop(size*2).removeFromRight(bounds.getWidth() - 10).removeFromLeft(bounds.getWidth() - 20));
}
void KarplusStrongAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &delay) audioProcessor.setDelayTime(delay.getValue()/1000.0f);
}
