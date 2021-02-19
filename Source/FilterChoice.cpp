///*
//  ==============================================================================
//
//    FilterChoice.cpp
//    Created: 10 Jan 2021 2:28:54pm
//    Author:  Ruari
//
//  ==============================================================================
//*/
//
//#include "FilterChoice.h"
//FilterChoice::FilterChoice(RSynth1AudioProcessor& p) :
//    processor(p)
//{
//    filterChoiceButton.setButtonText("Hi/low");
//    addAndMakeVisible(&filterChoiceButton);
//    filterChoiceButton.setClickingTogglesState(true);
//    filterBool = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processor.valueTree, "FILTERTYPE", filterChoiceButton);
//}
//FilterChoice::~FilterChoice()
//{
//    filterBool = nullptr;
//}
//void FilterChoice::paint(juce::Graphics& g)
//{
//
//}
//
//void FilterChoice::resized()
//{
//    filterChoiceButton.setBounds(20, 30, buttonWidth, 50);
//}