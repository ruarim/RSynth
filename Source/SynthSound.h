/*
  ==============================================================================

    SynthSound.h
    Created: 6 Jan 2021 8:51:35pm
    Author:  Ruari

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
public:
    SynthSound() {}

    bool appliesToNote(int midiNoteNumber) override { return true; }
    bool appliesToChannel(int midiChannel) override { return true; }

};