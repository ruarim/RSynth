/*
  ==============================================================================

    Gain.cpp
    Created: 9 Jan 2021 7:20:20pm
    Author:  Ruari

  ==============================================================================
*/

#include "Amplifier.h"
Amplifier::Amplifier(RSynth1AudioProcessor& p) :
    processor(p)
{
    amplifierSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    //amplifierSlider.setRange(0.1f, 1.5f);
    //amplifierSlider.setValue(0.1f);
    amplifierSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(&amplifierSlider);
    addAndMakeVisible(&amplifierLabel);
    amplifierLabel.attachToComponent(&amplifierSlider, false);
    amplifierLabel.setJustificationType(juce::Justification::centred);
    amplifierLabel.setFont(juce::Font(12.0f, juce::Font::plain));

    amplifierVal = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.valueTree, "AMP", amplifierSlider);
}
Amplifier::~Amplifier()
{
    amplifierVal = nullptr;
}
void Amplifier::paint(juce::Graphics& g)
{
        
}

void Amplifier::resized()
{
    amplifierSlider.setBounds(20, 30, sliderWidth, 100);
}