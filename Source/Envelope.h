/*
  ==============================================================================

    Envelope.h
    Created: 6 Jan 2021 8:52:51pm
    Author:  Ruari

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class Envelope : public juce::Component
{
public: 
    Envelope(RSynth1AudioProcessor& p);
    ~Envelope();

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    RSynth1AudioProcessor& processor;

    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;

    juce::Label attackLabel{ {}, "A" };
    juce::Label decayLabel{ {}, "D" };
    juce::Label sustainLabel{ {}, "S" };
    juce::Label releaseLabel{ {}, "R" };

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackVal;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayVal;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainVal;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseVal;
   
    int const sliderWidth = 40.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Envelope)
};