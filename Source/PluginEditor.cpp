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
    : AudioProcessorEditor (&p), audioProcessor (p), envGUI(p), filterGUI(p), gainGUI(p), sawGUI(p), subGUI(p), sqrGUI(p), cGUI(p), aGUI(p), lfoGUI(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (440, 300);

    addAndMakeVisible(&envGUI);
    addAndMakeVisible(&filterGUI);
    addAndMakeVisible(&gainGUI);
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
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::black);
    g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void RSynth1AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    juce::Rectangle<int> area = getLocalBounds();

    //const int componentWidth = 200;
    //const int componentHeight = 200;

    envGUI.setBounds(150, 150, 400, 200);
    filterGUI.setBounds(50, 0, 200, 200);
    gainGUI.setBounds(0, 0, 50, 150);
    sawGUI.setBounds(0, 150, 50, 150);
    subGUI.setBounds(50, 150, 50, 150);
    sqrGUI.setBounds(100, 150, 50, 150);
    cGUI.setBounds(225, 0, 150, 150);
    aGUI.setBounds(310, 0, 150, 150);
    lfoGUI.setBounds(310, 150, 100, 200);
}
