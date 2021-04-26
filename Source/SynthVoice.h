/*
  ==============================================================================

    SynthVoice.h
    Created: 6 Jan 2021 8:51:23pm
    Author:  Ruari

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SynthSound.h"
#include "maximilian.h"


class SynthVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound(juce::SynthesiserSound* sound) override;
   
    void setADSRParams(std::atomic<float>* attack, std::atomic<float>* decay, std::atomic<float>* sustain, std::atomic<float>* release); 

    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition);
    
    void stopNote(float velocity, bool allowTailOff) override;
    
    void setFilterChoice(std::atomic<float>* fChoiceInput);
    
    void setFilter(std::atomic<float>* cutoffValue, std::atomic<float>* resonanceValue);
    
    void setLevel(std::atomic<float>* gainInput);
    
    void setLFO(std::atomic<float>* lfoRateInput, std::atomic<float>* lfoLevelInput);
    
    double getLFO();
    
    void setOscLevels(std::atomic<float>* sqr, std::atomic<float>* saw, std::atomic<float>* sub);
    
    double getSawOsc();
    
    double getSqrOsc();

    double getSubOsc();

    double combineOsc();

    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples);

    //unused overrides
    void pitchWheelMoved(int) override {}
    void controllerMoved(int, int) override {}

private:
    //adsr
    maxiEnv env;

    //oscilators
    maxiOsc sawOsc;
    maxiOsc sqrOsc;
    maxiOsc subOsc;

    //mixer settings
    double sawOscLevel;
    double sqrOscLevel;
    double subOscLevel;

    //voice frequency
    double frequency;
    double currentFrequency;

    //amp level  
    double gain;

    //filter
    maxiFilter filter;
    double cutoff;
    double resonance;
    double fChoice;
    
    //low frequency osc
    maxiOsc lfo;
    double lfoRate = 0.0;
    double lfoDepth = 0.0;
};