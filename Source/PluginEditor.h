/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Envelope.h"
#include "Filter.h"
#include "Amplifier.h"
#include "SawOsc.h"
#include "SubOsc.h"
#include "SquareOsc.h"
#include "Chorus.h"
#include "Arpeggiator.h"
#include "Lfo.h"
//==============================================================================
/**
*/
class RSynth1AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    RSynth1AudioProcessorEditor (RSynth1AudioProcessor&);
    ~RSynth1AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    RSynth1AudioProcessor& audioProcessor;

    Envelope envGUI;
    Filter filterGUI;
    Amplifier ampGUI;
    SawOsc sawGUI;
    SubOsc subGUI;
    SquareOsc sqrGUI;
    Chorus cGUI;
    Arpeggiator aGUI;
    Lfo lfoGUI;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RSynth1AudioProcessorEditor)
};
