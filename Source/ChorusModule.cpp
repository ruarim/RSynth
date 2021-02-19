/*
  ==============================================================================

    ChorusModule.cpp
    Created: 16 Jan 2021 7:07:44pm
    Author:  Ruari

  ==============================================================================
*/

#include "ChorusModule.h"
ChorusModule::ChorusModule(){}

ChorusModule::~ChorusModule() {}

double ChorusModule::processChorus(float inputSample, double chorusMix) { //could take main lfo as input for chorus when implemented
    double delayedSignal = delay.dl(inputSample, 1 + 175 * (1 + lfo.sinebuf4(lfoRate)), 0);
    double returnVal = inputSample + (delayedSignal * chorusMix);
    return inputSample + (delayedSignal * chorusMix);
}