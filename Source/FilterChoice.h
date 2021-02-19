///*
//  ==============================================================================
//
//    FilterChoice.h
//    Created: 10 Jan 2021 2:28:54pm
//    Author:  Ruari
//
//  ==============================================================================
//*/
//
//#pragma once
//#include <JuceHeader.h>
//#include "PluginProcessor.h"
//
//class FilterChoice : public juce::Component
//{
//public:
//    FilterChoice(RSynth1AudioProcessor& p);
//    ~FilterChoice(); 
//
//    void paint(juce::Graphics&) override; //? do i need this? 
//    void resized() override;
//
//private:
//    RSynth1AudioProcessor& processor;
//
//    juce::TextButton filterChoiceButton;
//
//    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> filterBool;
//
//    int const buttonWidth = 50.0f;
//
//};