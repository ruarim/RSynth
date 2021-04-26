/*
  ==============================================================================

    SynthVoice.cpp
    Created: 4 Mar 2021 2:29:15pm
    Author:  Ruari

  ==============================================================================
*/

#include "SynthVoice.h"

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast <SynthSound*>(sound) != nullptr;
}

void SynthVoice::setADSRParams(std::atomic<float>* attack, std::atomic<float>* decay, std::atomic<float>* sustain, std::atomic<float>* release)
{
    env.setAttack(*attack);
    env.setDecay(*decay);
    env.setSustain(*sustain);
    env.setRelease(*release);
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    if (canPlaySound(sound))
    {
        frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        env.trigger = 1; //trigger env 
        env.attackphase = 1; //start attack
        //env.decayphase = 0;
    }
    else
    {
        jassertfalse; // debug error if the sound object passed is invalid
    }
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    env.trigger = 0;
    env.attackphase = 0; //if attack length not reached before note off end attack ramp
    env.decayphase = 1; 
    env.release = 1;    

    allowTailOff = true;
}

void SynthVoice::setFilterChoice(std::atomic<float>* fChoiceInput)
{
    fChoice = *fChoiceInput;
}

void SynthVoice::setFilter(std::atomic<float>* cutoffValue, std::atomic<float>* resonanceValue)
{
    cutoff = *cutoffValue;
    resonance = *resonanceValue;
}

void SynthVoice::setLevel(std::atomic<float>* volInput)
{
    gain = *volInput;
}

void SynthVoice::setLFO(std::atomic<float>* lfoRateInput, std::atomic<float>* lfoLevelInput)
{
    lfoRate = *lfoRateInput;
    lfoDepth = *lfoLevelInput;
}

double SynthVoice::getLFO()
{
    return lfo.sinewave(lfoRate) * lfoDepth;
}

void SynthVoice::setOscLevels(std::atomic<float>* sqr, std::atomic<float>* saw, std::atomic<float>* sub)
{
    sqrOscLevel = *sqr;
    sawOscLevel = *saw;
    subOscLevel = *sub;
}
    
double SynthVoice::getSawOsc()
{
    return sawOsc.saw(currentFrequency);
}

double SynthVoice::getSqrOsc()
{   
    return sqrOsc.square(currentFrequency / 2);
}

double SynthVoice::getSubOsc()
{
    return subOsc.square(currentFrequency / 4);
}

double SynthVoice::combineOsc()
{
    return (getSqrOsc() * sqrOscLevel) + (getSawOsc() * sawOscLevel) + (getSubOsc() * subOscLevel);
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    for (int sample = 0; sample < numSamples; ++sample)
    {
        currentFrequency = frequency + (frequency * getLFO()); //modulate frequency with LFO

        double oscSound = combineOsc(); //get osc sources

        double processedSample = 0.0f;

        //apply filter
        if (fChoice == 0.0f)  //low pass filter signal
            processedSample = filter.lores(oscSound, cutoff, resonance);
        else                  //high pass filter signal
            processedSample = filter.hires(oscSound, cutoff, resonance);
        
        double ampLevel = env.adsr(gain, env.trigger); //apply env to amp 

        processedSample = (2.0f / juce::float_Pi) * atan(processedSample * ampLevel); //apply waveshaper

        for (int channel = 0; channel < outputBuffer.getNumChannels(); channel++)
        {
            outputBuffer.addSample(channel, startSample + sample, processedSample); //add identical audio samples to both channels
        }
    }
}
