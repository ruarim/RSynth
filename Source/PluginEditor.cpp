/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Envelope.h"

//==============================================================================
RSynth1AudioProcessorEditor::RSynth1AudioProcessorEditor (RSynth1AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), envGUI(p), filterGUI(p), ampGUI(p), sawGUI(p), subGUI(p), sqrGUI(p), cGUI(p), aGUI(p), lfoGUI(p)
{
    setSize (450, 300);

    addAndMakeVisible(&envGUI);
    addAndMakeVisible(&filterGUI);
    addAndMakeVisible(&ampGUI);
    addAndMakeVisible(&sawGUI);
    addAndMakeVisible(&subGUI);
    addAndMakeVisible(&sqrGUI);
    addAndMakeVisible(&cGUI);
    addAndMakeVisible(&aGUI);
    addAndMakeVisible(&lfoGUI);
}

RSynth1AudioProcessorEditor::~RSynth1AudioProcessorEditor()
{
}

//==============================================================================
void RSynth1AudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::black);
    g.setFont (15.0f);
}

void RSynth1AudioProcessorEditor::resized()
{
    juce::Rectangle<int> area = getLocalBounds();

    envGUI.setBounds(145, 160, 400, 200);
    filterGUI.setBounds(50, 0, 200, 200);
    ampGUI.setBounds(0, 0, 50, 150);
    sawGUI.setBounds(0, 160, 50, 150);
    subGUI.setBounds(50, 160, 50, 150);
    sqrGUI.setBounds(100, 160, 50, 150);
    cGUI.setBounds(225, 0, 150, 150);
    aGUI.setBounds(320, 0, 150, 150);
    lfoGUI.setBounds(320, 160, 100, 200);
}
