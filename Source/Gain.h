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

class Gain : public juce::Component
{
public:
    Gain(RSynth1AudioProcessor& p);
    ~Gain();

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    RSynth1AudioProcessor& processor;

    juce::Slider gainSlider;

    juce::Label gainLabel{ {}, "Gain" };

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainVal;

    int const sliderWidth = 40.0f;
};