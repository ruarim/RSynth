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
    bool canPlaySound(juce::SynthesiserSound* sound) override
    {
        return dynamic_cast <SynthSound*>(sound) != nullptr;
    }
    void setADSRParams(std::atomic<float>* attack, std::atomic<float>* decay, std::atomic<float>* sustain, std::atomic<float>* release)
    {
        env1.setAttack(*attack);
        env1.setDecay(*decay);
        env1.setSustain(*sustain);
        env1.setRelease(*release);
    }
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
    {
        env1.trigger = 1;
        env1.attackphase = 1;
        env1.decayphase = 0;
        level = velocity;
        frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    }
    void stopNote(float velocity, bool allowTailOff) override
    {
        env1.trigger = 0;
        env1.attackphase = 0;
        env1.decayphase = 1;

        allowTailOff = true;
    }
    void setFilterChoice(std::atomic<float>* fChoiceInput)
    {
        fChoice = *fChoiceInput;
    }
    void setFilter(std::atomic<float>* cutoffValue, std::atomic<float>* resonanceValue)
    {

        cutoff = *cutoffValue;
        resonance = *resonanceValue;
    }
    void setGain(std::atomic<float>* gainInput)
    {
        gain = *gainInput;
    }
    void setChorus(std::atomic<float>* chorusMixInput, std::atomic<float>* chorusOnOffInput)
    {
        chorusLevel = *chorusMixInput;
        chorusOnOff = *chorusOnOffInput;
    }
    void setLFO(std::atomic<float>* lfoRateInput, std::atomic<float>* lfoLevelInput)
    {
        //lfo type sqr saw sin?? 
        lfoRate = *lfoRateInput;
        lfoLevel = *lfoLevelInput;
    }
    double getLFO()
    {
        double lfoOut = lfo.sinewave(lfoRate) * lfoLevel;
        return lfoOut;
    }
    void setOscGains(std::atomic<float>* sqr, std::atomic<float>* saw, std::atomic<float>* sub)
    {
        sqrOscGain = *sqr;
        sawOscGain = *saw;
        subOscGain = *sub;
    }
    double getSawOsc()
    {
        return sawOsc.saw(currentFrequency) * sawOscGain;
    }
    double getSqrOsc()
    {
        return sqrOsc.square(currentFrequency/2) * sqrOscGain;
    }
    double getSubOsc()
    {
        return subOsc.square(currentFrequency/4) * subOscGain;
    }
    double combineOsc()
    {
        return getSqrOsc() + getSawOsc() + getSubOsc();
    }
    void renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
    {
        for (int sample = 0; sample < numSamples; ++sample)
        {
            currentFrequency = frequency;
            double oscSound = combineOsc();
            double currentVolume = env1.adsr(1., env1.trigger) * gain;

            double filteredSound = 0.0f;
            if(fChoice == 0.0f)
                filteredSound = filter1.lores(oscSound, cutoff, resonance);
            else
                filteredSound = filter1.hires(oscSound, cutoff, resonance);
             
            double processedSound = 0.0f + getLFO();
            if (chorusOnOff == 1.0f)
                processedSound = filteredSound  + (chorus.chorus(filteredSound, 4500, 0.0, 0.01, 1.0) * chorusLevel);
            else
                processedSound = filteredSound;


            double outputSound = processedSound; //apply gain

            for(int channel = 0; channel < outputBuffer.getNumChannels(); channel++)
            {
                outputBuffer.addSample(channel, startSample+sample, outputSound * currentVolume);
            }
        }
    }
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

    double chorusLevel;
    double chorusOnOff;
    maxiChorus chorus;
    
    //low frequency osc
    maxiOsc lfo;
    double lfoRate = 0.0;
    double lfoLevel = 0.0;
};