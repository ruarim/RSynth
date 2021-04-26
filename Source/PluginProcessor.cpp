/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RSynth1AudioProcessor::RSynth1AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), valueTree(*this, nullptr, juce::Identifier("RSynthParameters"), createParams()) //initialises the audioprocessorvaluetree nullptr as no undo manager
                                                                                            //create synth params and add to tree here
#endif
{

    rSynth.clearVoices();
    for (int i = 0; i < rVoices; i++)
    {
        rSynth.addVoice(new SynthVoice());
    }

    rSynth.clearSounds();
    rSynth.addSound(new SynthSound());

    //valueTree.state = juce::ValueTree("ATTACK"); // for each to implement xml state export
}

RSynth1AudioProcessor::~RSynth1AudioProcessor()
{
    rSynth.clearVoices();
    rSynth.clearSounds();
}

//==============================================================================
const juce::String RSynth1AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool RSynth1AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool RSynth1AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool RSynth1AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double RSynth1AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int RSynth1AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int RSynth1AudioProcessor::getCurrentProgram()
{
    return 0;
}

void RSynth1AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String RSynth1AudioProcessor::getProgramName (int index)
{
    return {};
}

void RSynth1AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void RSynth1AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
     
    //create dsp spec for chorus
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getMainBusNumOutputChannels();

    chorus.prepare(spec);
    chorus.reset();
    
    //setup chorus
    updateChorus();

    //setup arpeggiator
    //if arp on?? 
    notes.clear();
    currentNote = 0;
    lastNote = -1;
    time = 0;
    rate = static_cast<float> (sampleRate);

    juce::ignoreUnused(samplesPerBlock);
    rSynth.setCurrentPlaybackSampleRate(sampleRate);
    
    //midiCollector.reset(sampleRate);
}

void RSynth1AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool RSynth1AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void RSynth1AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDernormals; 

    //setup up arpeggiator 
    if (*valueTree.getRawParameterValue("ARPONOFF") == true)
    {
        auto numSamples = buffer.getNumSamples();
        auto noteDuration = static_cast<int> (std::ceil(rate * 0.25f * (0.1f + (1.0f - (*valueTree.getRawParameterValue("ARPSPEED"))))));
        
        //get pressed keys
        for (const auto metaData : midiMessages) //loop through keys pressed
        {
            const auto msg = metaData.getMessage();

            if (msg.isNoteOn())  //add note sorted set if key down
                notes.add(msg.getNoteNumber());
            else if (msg.isNoteOff()) //remove note if key is lifted
                notes.removeValue(msg.getNoteNumber());
        }
        midiMessages.clear();

        //create arp sequence from held keys
        if ((time + numSamples) >= noteDuration) //
        {
            auto offset = juce::jmax(0, juce::jmin((int)(noteDuration - time), numSamples - 1)); 

            if (lastNote > 0)
            {
                midiMessages.addEvent(juce::MidiMessage::noteOff(1, lastNote), offset); 
                lastNote = -1;
            }
            if (notes.size() > 0)
            {
                currentNote = (currentNote + 1) % notes.size();
                lastNote = notes[currentNote];
                midiMessages.addEvent(juce::MidiMessage::noteOn(1, lastNote, (juce::uint8)127), offset);
            }
        }
        time = (time + numSamples) % noteDuration;
    }

    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    

    for (int i = 0; i < rSynth.getNumVoices(); i++)
    {
        if (SynthVoice* rVoice = dynamic_cast<SynthVoice*>(rSynth.getVoice(i))) //get pointer to voice
        {
            rVoice->setADSRParams(valueTree.getRawParameterValue("ATTACK"), //apply paramerter to voice
                                  valueTree.getRawParameterValue("DECAY"),
                                  valueTree.getRawParameterValue("SUSTAIN"),
                                  valueTree.getRawParameterValue("RELEASE"));

            rVoice->setFilter(valueTree.getRawParameterValue("CUTOFF"),
                              valueTree.getRawParameterValue("RESO"));

            rVoice->setLevel(valueTree.getRawParameterValue("AMP"));

            rVoice->setOscLevels(valueTree.getRawParameterValue("SQROSC"),
                                valueTree.getRawParameterValue("SAWOSC"),
                                valueTree.getRawParameterValue("SUBOSC"));

            rVoice->setFilterChoice(valueTree.getRawParameterValue("FILTERTYPE"));

            rVoice->setLFO(valueTree.getRawParameterValue("LFORATE"),
                                valueTree.getRawParameterValue("LFODEPTH"));
        }
    }

    rSynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    //proccess signal with chorus
    juce::dsp::AudioBlock<float> block(buffer);

    updateChorus();
    chorus.process(juce::dsp::ProcessContextReplacing<float>(block));

    midiMessages.clear();
}

//==============================================================================
bool RSynth1AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* RSynth1AudioProcessor::createEditor()
{
    return new RSynth1AudioProcessorEditor (*this);
}

//==============================================================================
void RSynth1AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    /*juce::MemoryOutputStream stream(destData, false);
    valueTree.state.writeToStream(stream);*/
}

void RSynth1AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    /*juce::ValueTree tree = juce::ValueTree::readFromData(data, sizeInBytes);

    if (tree.isValid())
    {
        valueTree.state = tree;
    }*/
}

void RSynth1AudioProcessor::updateChorus()
{   
    //get values from gui
    //create chorus sound
    //set depth 
    chorus.setDepth(*valueTree.getRawParameterValue("CHORUSDEPTH"));

    //set dry wet level
    chorus.setMix(*valueTree.getRawParameterValue("CHORUSMIX"));

}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RSynth1AudioProcessor();
}
juce::AudioProcessorValueTreeState::ParameterLayout RSynth1AudioProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange<float>(1.0f, 5000.0f), 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float>(1.0f, 2000.0f), 500.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange<float>(0.0f, 1.0f), 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", juce::NormalisableRange<float>(1.0f, 5000.0f), 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("CUTOFF", "Cutoff", juce::NormalisableRange<float>(20.0f, 5000.0f), 2000.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RESO", "Resonance", juce::NormalisableRange<float>(0.0f, 10.0f), 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("AMP", "Amp", juce::NormalisableRange<float>(0.0f, 5.0f), 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SAWOSC", "SawOsc", juce::NormalisableRange<float>(0.0f, 0.33f), 0.15f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUBOSC", "SubOsc", juce::NormalisableRange<float>(0.0f, 0.33f), 0.15f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SQROSC", "SqrOsc", juce::NormalisableRange<float>(0.0f, 0.33f), 0.15f));
    params.push_back(std::make_unique<juce::AudioParameterBool>("FILTERTYPE", "Hi/Lowpass", false));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("CHORUSMIX", "ChorusMix", juce::NormalisableRange<float>(0.0f, 1.00f), 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("CHORUSDEPTH", "ChorusDepth", juce::NormalisableRange<float>(0.0f, 1.00f), 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ARPSPEED", "ArpSpeed", juce::NormalisableRange<float>(0.0f, 1.00f), 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterBool>("ARPONOFF", "On/Off", false));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("LFORATE", "lfoRate", juce::NormalisableRange<float>(0.0f, 5.0f), 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("LFODEPTH", "lfodepth", juce::NormalisableRange<float>(0.0f, 1.0f), 0.0f));
    
    return{ params.begin(), params.end() }; //returns parameters list
}
