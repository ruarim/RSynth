/*
  ==============================================================================

    Chorus.cpp
    Created: 16 Jan 2021 7:07:59pm
    Author:  Ruari

  ==============================================================================
*/

#include "Chorus.h"

Chorus::Chorus(RSynth1AudioProcessor& p) :
    processor(p) 
{
    setSize(200, 100);

    chorusMix.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    chorusMix.setValue(0.1f);
    chorusMix.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(&chorusMix);
    addAndMakeVisible(&chorusLabel);
    chorusLabel.attachToComponent(&chorusMix, false);
    chorusLabel.setJustificationType(juce::Justification::centred);
    chorusLabel.setFont(juce::Font(12.0f, juce::Font::plain));

    chorusOnOff.setButtonText("On/Off");
    addAndMakeVisible(&chorusOnOff);
    chorusOnOff.setClickingTogglesState(true);

    chorusVal = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.valueTree, "CHORUSMIX", chorusMix);
    chorusBool = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processor.valueTree, "CHORUSONOFF", chorusOnOff);
}
Chorus::~Chorus()
{
    chorusVal = nullptr;
    chorusBool = nullptr;
}
void Chorus::paint(juce::Graphics& g)
{

}

void Chorus::resized()
{
    chorusMix.setBounds(20, 30, sliderWidth, 100);
    chorusOnOff.setBounds(60, 30, buttonWidth, 50);
}