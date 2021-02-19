/*
  ==============================================================================

    Gain.cpp
    Created: 9 Jan 2021 7:20:20pm
    Author:  Ruari

  ==============================================================================
*/

#include "Gain.h"
Gain::Gain(RSynth1AudioProcessor& p) :
    processor(p)
{
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    gainSlider.setRange(0.1f, 5000.0f);
    gainSlider.setValue(0.1f);
    gainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(&gainSlider);
    addAndMakeVisible(&gainLabel);
    gainLabel.attachToComponent(&gainSlider, false);
    gainLabel.setJustificationType(juce::Justification::centred);
    gainLabel.setFont(juce::Font(12.0f, juce::Font::plain));

    gainVal = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.valueTree, "GAIN", gainSlider);
}
Gain::~Gain()
{
    gainVal = nullptr;
}
void Gain::paint(juce::Graphics& g)
{

}

void Gain::resized()
{
    gainSlider.setBounds(20, 30, sliderWidth, 100);
}