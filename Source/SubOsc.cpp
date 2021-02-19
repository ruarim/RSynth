/*
  ==============================================================================

    SubOsc.cpp
    Created: 9 Jan 2021 8:49:47pm
    Author:  Ruari

  ==============================================================================
*/

#include "SubOsc.h"
SubOsc::SubOsc(RSynth1AudioProcessor& p) :
    processor(p)
{
    subOscSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    subOscSlider.setRange(0.1f, 5000.0f);
    subOscSlider.setValue(0.1f);
    subOscSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(&subOscSlider);
    addAndMakeVisible(&subOscLabel);
    subOscLabel.attachToComponent(&subOscSlider, false);
    subOscLabel.setJustificationType(juce::Justification::centred);
    subOscLabel.setFont(juce::Font(12.0f, juce::Font::plain));

    subOscVal = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.valueTree, "SUBOSC", subOscSlider);
}
SubOsc::~SubOsc()
{
    subOscVal = nullptr;
}
void SubOsc::paint(juce::Graphics& g)
{

}

void SubOsc::resized()
{
    subOscSlider.setBounds(20, 30, sliderWidth, 100);
}