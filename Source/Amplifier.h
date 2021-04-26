/*
  ==============================================================================

    Gain.h
    Created: 9 Jan 2021 7:20:20pm
    Author:  Ruari

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class Amplifier : public juce::Component
{
public:
    Amplifier(RSynth1AudioProcessor& p);
    ~Amplifier();

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    RSynth1AudioProcessor& processor;

    juce::Slider amplifierSlider;

    juce::Label amplifierLabel{ {}, "Gain" };

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> amplifierVal;

    int const sliderWidth = 30.0f;
};