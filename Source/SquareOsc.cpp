/*
  ==============================================================================

    SquareOsc.cpp
    Created: 9 Jan 2021 8:50:10pm
    Author:  Ruari

  ==============================================================================
*/

#include "SquareOsc.h"
SquareOsc::SquareOsc(RSynth1AudioProcessor& p) :
    processor(p)
{
    squareOscSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    squareOscSlider.setRange(0.1f, 5000.0f);
    squareOscSlider.setValue(0.1f);
    squareOscSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(&squareOscSlider);
    addAndMakeVisible(&squareOscLabel);
    squareOscLabel.attachToComponent(&squareOscSlider, false);
    squareOscLabel.setJustificationType(juce::Justification::centred);
    squareOscLabel.setFont(juce::Font(12.0f, juce::Font::plain));

    squareOscVal = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.valueTree, "SQROSC", squareOscSlider);
}
SquareOsc::~SquareOsc()
{
    squareOscVal = nullptr;
}
void SquareOsc::paint(juce::Graphics& g)
{

}

void SquareOsc::resized()
{
    squareOscSlider.setBounds(20, 30, sliderWidth, 100);
}