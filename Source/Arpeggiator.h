/*
  ==============================================================================

    Arpeggiator.h
    Created: 15 Feb 2021 1:29:49am
    Author:  Ruari

  ==============================================================================
*/

#pragma once
#include "PluginProcessor.h"

class Arpeggiator : public juce::Component
{
public:
    Arpeggiator(RSynth1AudioProcessor& p);
    ~Arpeggiator();

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    RSynth1AudioProcessor& processor;

    juce::Slider arpSpeed;
    juce::TextButton arpOnOff;

    juce::Label arpLabel{ {}, "Speed" };

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> arpVal;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> arpBool;

    int const buttonWidth = 60.0f;
    int const sliderWidth = 40.0f;
};