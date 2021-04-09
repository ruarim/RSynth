/*
  ==============================================================================

    Filter.cpp
    Created: 9 Jan 2021 7:19:57pm
    Author:  Ruari

  ==============================================================================
*/

#include "Filter.h"
#include <JuceHeader.h>

Filter::Filter(RSynth1AudioProcessor& p) :
    processor(p)
{
    setSize(200, 100);

    cutoffSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    cutoffSlider.setRange(20.0f, 20000.0f);
    cutoffSlider.setValue(20.0f);
    cutoffSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(&cutoffSlider);
    addAndMakeVisible(&cutoffLabel);
    cutoffLabel.attachToComponent(&cutoffSlider, false);
    cutoffLabel.setJustificationType(juce::Justification::centred);
    cutoffLabel.setFont(juce::Font(12.0f, juce::Font::plain));

    resoSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    resoSlider.setValue(0.1f);
    resoSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(&resoSlider);
    addAndMakeVisible(&resoLabel);
    resoLabel.attachToComponent(&resoSlider, false);
    resoLabel.setJustificationType(juce::Justification::centred);
    resoLabel.setFont(juce::Font(12.0f, juce::Font::plain));

    filterChoiceButton.setButtonText("Hi/low");
    addAndMakeVisible(&filterChoiceButton);
    filterChoiceButton.setClickingTogglesState(true);

    cutoffSlider.setSkewFactorFromMidPoint(800.0f);

    cutoffVal = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.valueTree, "CUTOFF", cutoffSlider);
    resoVal = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.valueTree, "RESO", resoSlider);
    filterBool = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processor.valueTree, "FILTERTYPE", filterChoiceButton);
}
Filter::~Filter()
{
    cutoffVal = nullptr;
    resoVal = nullptr;
    filterBool = nullptr;   
}
void Filter::paint(juce::Graphics& g)
{

}

void Filter::resized()
{
    cutoffSlider.setBounds(20, 30, sliderWidth, 100);
    resoSlider.setBounds(60, 30, sliderWidth, 100);
    filterChoiceButton.setBounds(120, 30, buttonWidth, 50);
}