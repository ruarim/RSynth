/*
  ==============================================================================

    SquareOsc.h
    Created: 9 Jan 2021 8:50:10pm
    Author:  Ruari

  ==============================================================================
*/

#pragma once
#include "PluginProcessor.h"

class SquareOsc : public juce::Component
{
public:
    SquareOsc(RSynth1AudioProcessor& p);
    ~SquareOsc();

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    RSynth1AudioProcessor& processor;

    juce::Slider squareOscSlider;

    juce::Label squareOscLabel{ {}, "SquareOsc" };

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> squareOscVal;

    int const sliderWidth = 40.0f;
};