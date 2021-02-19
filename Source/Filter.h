/*
  ==============================================================================

    Filter.h
    Created: 9 Jan 2021 7:19:57pm
    Author:  Ruari

  ==============================================================================
*/

#pragma once
#include "PluginProcessor.h"

class Filter : public juce::Component
{
public:
    Filter(RSynth1AudioProcessor& p);
    ~Filter();

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    RSynth1AudioProcessor& processor;

    juce::Slider cutoffSlider;
    juce::Slider resoSlider;
    juce::TextButton filterChoiceButton;

    juce::Label cutoffLabel{ {}, "Cutoff" };
    juce::Label resoLabel{ {}, "Reso" };

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffVal;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> resoVal;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> filterBool;

    int const buttonWidth = 50.0f;
    int const sliderWidth = 40.0f;

};