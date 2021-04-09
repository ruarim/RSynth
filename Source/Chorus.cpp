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
    addAndMakeVisible(&chorusMixLabel);
    chorusMixLabel.attachToComponent(&chorusMix, false);
    chorusMixLabel.setJustificationType(juce::Justification::centred);
    chorusMixLabel.setFont(juce::Font(12.0f, juce::Font::plain));

    chorusDepth.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    chorusDepth.setValue(0.1f);
    chorusDepth.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(&chorusDepth);
    addAndMakeVisible(&chorusDepthLabel);
    chorusDepthLabel.attachToComponent(&chorusDepth, false);
    chorusDepthLabel.setJustificationType(juce::Justification::centred);
    chorusDepthLabel.setFont(juce::Font(12.0f, juce::Font::plain));

    chorusValMix = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.valueTree, "CHORUSMIX", chorusMix);
    chorusValDepth = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.valueTree, "CHORUSDEPTH", chorusDepth);
}
Chorus::~Chorus()
{
    chorusValMix = nullptr;
    chorusValDepth = nullptr;
}
void Chorus::paint(juce::Graphics& g)
{

}

void Chorus::resized()
{
    chorusMix.setBounds(20, 30, sliderWidth, 100);
    chorusDepth.setBounds(60, 30, sliderWidth, 100);
}