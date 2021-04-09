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
    juce::Slider chorusDepth;

    juce::Label chorusMixLabel { {}, "Dry/Wet" };
    juce::Label chorusDepthLabel{ {}, "Depth" };


    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> chorusValMix;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> chorusValDepth;

    int const sliderWidth = 40.0f;
};