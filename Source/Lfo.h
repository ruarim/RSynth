/*
  ==============================================================================

    Lfo.h
    Created: 15 Feb 2021 9:08:03am
    Author:  Ruari

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class Lfo : public juce::Component
{
public:
    Lfo(RSynth1AudioProcessor& p);
    ~Lfo();

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    RSynth1AudioProcessor& processor;

    juce::Slider lfoRateSlider;
    juce::Slider lfoLevelSlider;

    juce::Label rateLabel{ {}, "Lfo Rate" };
    juce::Label levelLabel{ {}, "Level" };

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfoRateVal;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfoLevelVal;

    int const sliderWidth = 40.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Lfo)
};