/*
  ==============================================================================

    Arpeggiator.cpp
    Created: 15 Feb 2021 1:29:49am
    Author:  Ruari

  ==============================================================================
*/

#include "Arpeggiator.h"
Arpeggiator::Arpeggiator(RSynth1AudioProcessor& p) :
    processor(p)
{
    setSize(200, 100);

    arpSpeed.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    arpSpeed.setValue(0.1f);
    arpSpeed.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(&arpSpeed);
    
    arpLabel.attachToComponent(&arpSpeed, false);
    arpLabel.setJustificationType(juce::Justification::centred);
    arpLabel.setFont(juce::Font(12.0f, juce::Font::plain));
    addAndMakeVisible(&arpLabel);

    arpOnOff.setButtonText("On/Off");
    addAndMakeVisible(&arpOnOff);
    arpOnOff.setClickingTogglesState(true);

    arpVal = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.valueTree, "ARPSPEED", arpSpeed);
    arpBool = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processor.valueTree, "ARPONOFF", arpOnOff);
}
Arpeggiator::~Arpeggiator()
{
    arpVal = nullptr;
    arpBool = nullptr;
}
void Arpeggiator::paint(juce::Graphics& g)
{

}

void Arpeggiator::resized()
{
    arpSpeed.setBounds(20, 30, sliderWidth, 100);
    arpOnOff.setBounds(60, 30, buttonWidth, 50);
}
