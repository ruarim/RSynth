/*
  ==============================================================================

    Chorus.h
    Created: 16 Jan 2021 7:07:59pm
    Author:  Ruari

  ==============================================================================
*/

#pragma once
#include "PluginProcessor.h"

class Chorus : public juce::Component
{
public:
    Chorus(RSynth1AudioProcessor& p);
    ~Chorus();

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    RSynth1AudioProcessor& processor;

    juce::Slider chorusMix;
    juce::TextButton chorusOnOff;

    juce::Label chorusLabel { {}, "Dry/Wet" };

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> chorusVal;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> chorusBool;

    int const buttonWidth = 50.0f;
    int const sliderWidth = 40.0f;
};