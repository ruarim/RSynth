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
#include "ChorusModule.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound(juce::SynthesiserSound* sound) override;
   
    //prepare to play to add juce statevarfilter

    void setADSRParams(std::atomic<float>* attack, std::atomic<float>* decay, std::atomic<float>* sustain, std::atomic<float>* release); 

    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition);
    
    void stopNote(float velocity, bool allowTailOff) override;
    
    void setFilterChoice(std::atomic<float>* fChoiceInput);
    
    void setFilter(std::atomic<float>* cutoffValue, std::atomic<float>* resonanceValue);
    
    void setGain(std::atomic<float>* gainInput);
    
    void setLFO(std::atomic<float>* lfoRateInput, std::atomic<float>* lfoLevelInput);
    
    double getLFO();
    
    void setOscGains(std::atomic<float>* sqr, std::atomic<float>* saw, std::atomic<float>* sub);
    
    double getSawOsc();
    
    double getSqrOsc();

    double getSubOsc();

    double combineOsc();

    void renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples);

    //unused overrides
    void pitchWheelMoved(int) override {}
    void controllerMoved(int, int) override {}

private:
    //adsr
    maxiEnv env1;

    //osc
    //pulse width?? 
    maxiOsc sawOsc;
    maxiOsc sqrOsc;
    maxiOsc subOsc;
    double sawOscGain;
    double sqrOscGain;
    double subOscGain;

    double frequency;
    double currentFrequency;
    //double processedFrequency;

    //gain 
    double gain;
    double level;

    //filter
    maxiFilter filter1;
    double cutoff;
    double resonance;
    double fChoice;
    
    //low frequency osc
    maxiOsc lfo;
    double lfoRate = 0.0;
    double lfoDepth = 0.0;
};