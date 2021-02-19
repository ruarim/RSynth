/*
  ==============================================================================

    ChorusModule.h
    Created: 16 Jan 2021 7:07:44pm
    Author:  Ruari

  ==============================================================================
*/

#pragma once
#include "maximilian.h"

class ChorusModule
{
public:
    ChorusModule();
    ~ChorusModule();
    double processChorus(float inputSample, double chorusMix);
private:
    maxiOsc lfo;
    float lfoRate = 0.2f;
    maxiFractionalDelay delay;
};