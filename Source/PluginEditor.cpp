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
    setSize (800, 500);

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

    envGUI.setBounds(getWidth() / 2, getHeight() / 2 - 50, 400, 200);
    filterGUI.setBounds(getWidth() / 2 - 100, getHeight() / 2 - 200, 200, 200);
    gainGUI.setBounds(getWidth() / 2 - 150, getHeight() / 2 - 200, 50, 150);
    sawGUI.setBounds(getWidth() / 2 - 150, getHeight() / 2 - 50, 50, 150);
    subGUI.setBounds(getWidth() / 2 - 100, getHeight() / 2 - 50, 50, 150);
    sqrGUI.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 50, 50, 150);
    cGUI.setBounds(getWidth() / 2 + 75, getHeight() / 2 - 200, 150, 150);
    aGUI.setBounds(getWidth() / 2 + 175, getHeight() / 2 - 200, 150, 150);
    lfoGUI.setBounds(getWidth() / 2 + 175, getHeight() / 2 - 50, 100, 200);
}
