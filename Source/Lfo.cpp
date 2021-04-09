/*
  ==============================================================================

    Lfo.cpp
    Created: 15 Feb 2021 9:08:03am
    Author:  Ruari

  ==============================================================================
*/
#include <JuceHeader.h>
#include "Lfo.h"

Lfo::Lfo(RSynth1AudioProcessor& p) :
    processor(p)
{
    setSize(200, 100);

    lfoRateSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    lfoRateSlider.setRange(0.1f, 5.0f);
    lfoRateSlider.setValue(0.1f);
    lfoRateSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(&lfoRateSlider);
    addAndMakeVisible(&rateLabel);
    rateLabel.attachToComponent(&lfoRateSlider, false);
    //attackLabel.setText("A", juce::dontSendNotification);
    rateLabel.setJustificationType(juce::Justification::centred);
    rateLabel.setFont(juce::Font(12.0f, juce::Font::plain));

    lfoDepthSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    lfoDepthSlider.setRange(0.1f, 5.0f);
    lfoDepthSlider.setValue(0.1f);
    lfoDepthSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(&lfoDepthSlider);
    addAndMakeVisible(&levelLabel);
    levelLabel.attachToComponent(&lfoDepthSlider, false);
    //attackLabel.setText("A", juce::dontSendNotification);
    levelLabel.setJustificationType(juce::Justification::centred);
    levelLabel.setFont(juce::Font(12.0f, juce::Font::plain));

    lfoRateSlider.setSkewFactorFromMidPoint(1.0f);

    lfoRateVal = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.valueTree, "LFORATE", lfoRateSlider);
    lfoDepthVal = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.valueTree, "LFODEPTH", lfoDepthSlider);
}
Lfo::~Lfo()
{
    lfoRateVal = nullptr;
    lfoDepthVal = nullptr;
}

void Lfo::paint(juce::Graphics& g)
{

}

void Lfo::resized()
{
    lfoRateSlider.setBounds(20, 30, sliderWidth, 100);
    lfoDepthSlider.setBounds(60, 30, sliderWidth, 100);

}
