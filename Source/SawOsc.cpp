/*
  ==============================================================================

    SawOsc.cpp
    Created: 9 Jan 2021 8:49:39pm
    Author:  Ruari

  ==============================================================================
*/

#include "SawOsc.h"
SawOsc::SawOsc(RSynth1AudioProcessor& p) :
    processor(p)
{
    sawOscSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    sawOscSlider.setRange(0.1f, 5000.0f);
    sawOscSlider.setValue(0.1f);
    sawOscSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(&sawOscSlider);
    addAndMakeVisible(&sawOscLabel);
    sawOscLabel.attachToComponent(&sawOscSlider, false);
    sawOscLabel.setJustificationType(juce::Justification::centred);
    sawOscLabel.setFont(juce::Font(12.0f, juce::Font::plain));

    sawOscVal = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.valueTree, "SAWOSC", sawOscSlider);
}
SawOsc::~SawOsc()
{
    sawOscVal = nullptr;
}
void SawOsc::paint(juce::Graphics& g)
{

}

void SawOsc::resized()
{
    sawOscSlider.setBounds(20, 30, sliderWidth, 100);
}