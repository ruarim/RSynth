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
    env1.setAttack(*attack);
    env1.setDecay(*decay);
    env1.setSustain(*sustain);
    env1.setRelease(*release);
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    env1.trigger = 1;
    env1.attackphase = 1;
    env1.decayphase = 0;
    level = velocity;
    frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    env1.trigger = 0;
    env1.attackphase = 0;
    env1.decayphase = 1;

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

void SynthVoice::setGain(std::atomic<float>* gainInput)
{
    gain = *gainInput;
}

void SynthVoice::setLFO(std::atomic<float>* lfoRateInput, std::atomic<float>* lfoLevelInput)
{
    //lfo type sqr saw sin?? 
    lfoRate = *lfoRateInput;
    lfoDepth = *lfoLevelInput;
}

double SynthVoice::getLFO()
{
    double lfoOut = lfo.sinewave(lfoRate) * lfoDepth;
    return lfoOut;
}

void SynthVoice::setOscGains(std::atomic<float>* sqr, std::atomic<float>* saw, std::atomic<float>* sub)
{
    sqrOscGain = *sqr;
    sawOscGain = *saw;
    subOscGain = *sub;
}
    
double SynthVoice::getSawOsc()
{
    return sawOsc.saw(currentFrequency) * sawOscGain;
}

double SynthVoice::getSqrOsc()
{
    return sqrOsc.square(currentFrequency / 2) * sqrOscGain;
}

double SynthVoice::getSubOsc()
{
    return subOsc.square(currentFrequency / 4) * subOscGain;
}

double SynthVoice::combineOsc()
{
    return getSqrOsc() + getSawOsc() + getSubOsc();
}

void SynthVoice::renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
    for (int sample = 0; sample < numSamples; ++sample)
    {
        currentFrequency = frequency + (frequency * getLFO());
        double oscSound = combineOsc();
        double currentVolume = env1.adsr(1., env1.trigger) * gain;

        double filteredSound = 0.0f;
        if (fChoice == 0.0f)
            filteredSound = filter1.lores(oscSound, cutoff, resonance);
        else
            filteredSound = filter1.hires(oscSound, cutoff, resonance);

        double outputSound = filteredSound; //apply gain

        for (int channel = 0; channel < outputBuffer.getNumChannels(); channel++)
        {
            outputBuffer.addSample(channel, startSample + sample, outputSound * currentVolume);
        }
    }
}
