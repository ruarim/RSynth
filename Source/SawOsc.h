/*
  ==============================================================================

    SawOsc.h
    Created: 9 Jan 2021 8:49:39pm
    Author:  Ruari

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class SawOsc : public juce::Component
{
public:
    SawOsc(RSynth1AudioProcessor& p);
    ~SawOsc();

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    RSynth1AudioProcessor& processor;

    juce::Slider sawOscSlider;

    juce::Label sawOscLabel{ {}, "SawOsc" };

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sawOscVal;

    int const sliderWidth = 40.0f;
};