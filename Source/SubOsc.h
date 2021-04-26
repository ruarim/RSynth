/*
  ==============================================================================

    SubOsc.h
    Created: 9 Jan 2021 8:49:47pm
    Author:  Ruari

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class SubOsc : public juce::Component
{
public:
    SubOsc(RSynth1AudioProcessor& p);
    ~SubOsc();

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    RSynth1AudioProcessor& processor;

    juce::Slider subOscSlider;

    juce::Label subOscLabel{ {}, "Sub" };

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> subOscVal;

    int const sliderWidth = 40.0f;
};