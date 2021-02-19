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
                       ), valueTree(*this, nullptr, "Parameters", createParams()) //initialises the audioprocessorvaluetree nullptr as no undo manager
    //instantiate synth params and add to tree here
#endif
{
    //tree = std::make_unique<juce::AudioProcessorValueTreeState>(*this);


    rSynth.clearVoices();
    for (int i = 0; i < 4; i++)
    {
        rSynth.addVoice(new SynthVoice());
    }
    rSynth.clearSounds();
    rSynth.addSound(new SynthSound());
}

RSynth1AudioProcessor::~RSynth1AudioProcessor()
{
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

    //setup arpeggiator
    //if arp on?? 
    notes.clear();
    currentNote = 0;
    lastNote = -1;
    time = 0;
    rate = static_cast<float> (sampleRate);

    juce::ignoreUnused(samplesPerBlock);
    lastSampleRate = sampleRate;
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
    ////setup up arpeggiator 
    if (*valueTree.getRawParameterValue("ARPONOFF") == true)
    {
        auto numSamples = buffer.getNumSamples();
        auto noteDuration = static_cast<int> (std::ceil(rate * 0.25f * (0.1f + (1.0f - (*valueTree.getRawParameterValue("ARPSPEED"))))));
        
        for (const auto metaData : midiMessages)
        {
            const auto msg = metaData.getMessage();

            if (msg.isNoteOn())
                notes.add(msg.getNoteNumber());
            else if (msg.isNoteOff())
                notes.removeValue(msg.getNoteNumber());
        }
        midiMessages.clear();

        if ((time + numSamples) >= noteDuration)
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
        if ((rVoice = dynamic_cast<SynthVoice*>(rSynth.getVoice(i))))
        {
            //audio processor value tree state for
            //osc volume
            //adrs
            //voice->getADSRParams(10.0f, );
            //lfo
            //gain
            //ect...
            rVoice->setADSRParams(valueTree.getRawParameterValue("ATTACK"),
                                  valueTree.getRawParameterValue("DECAY"),
                                  valueTree.getRawParameterValue("SUSTAIN"),
                                  valueTree.getRawParameterValue("RELEASE"));
            rVoice->setFilter(valueTree.getRawParameterValue("CUTOFF"),
                              valueTree.getRawParameterValue("RESO"));
            rVoice->setGain(valueTree.getRawParameterValue("GAIN"));
            rVoice->setOscGains(valueTree.getRawParameterValue("SQROSC"),
                                valueTree.getRawParameterValue("SAWOSC"),
                                valueTree.getRawParameterValue("SUBOSC"));
            rVoice->setFilterChoice(valueTree.getRawParameterValue("FILTERTYPE"));
            rVoice->setChorus(valueTree.getRawParameterValue("CHORUSMIX"),
                                valueTree.getRawParameterValue("CHORUSONOFF"));
            rVoice->setLFO(valueTree.getRawParameterValue("LFORATE"),
                                valueTree.getRawParameterValue("LFOLEVEL"));
            
        }
    }


    buffer.clear();

    //keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);

    rSynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
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
}

void RSynth1AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
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

    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange<float>(0.0f, 5000.0f), 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float>(0.0f, 5000.0f), 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange<float>(0.0f, 5000.0f), 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", juce::NormalisableRange<float>(0.0f, 5000.0f), 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("CUTOFF", "Cutoff", juce::NormalisableRange<float>(20.0f, 5000.0f), 2000.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RESO", "Resonance", juce::NormalisableRange<float>(0.0f, 10.0f), 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("GAIN", "Gain", juce::NormalisableRange<float>(0.0f, 0.75f), 0.25f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SAWOSC", "SawOsc", juce::NormalisableRange<float>(0.0f, 0.33f), 0.15f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUBOSC", "SubOsc", juce::NormalisableRange<float>(0.0f, 0.33f), 0.15f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SQROSC", "SqrOsc", juce::NormalisableRange<float>(0.0f, 0.33f), 0.15f));
    params.push_back(std::make_unique<juce::AudioParameterBool>("FILTERTYPE", "Hi/Lowpass", false));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("CHORUSMIX", "ChorusMix", juce::NormalisableRange<float>(0.0f, 1.00f), 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterBool>("CHORUSONOFF", "On/Off", false));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ARPSPEED", "ArpSpeed", juce::NormalisableRange<float>(0.0f, 1.00f), 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterBool>("ARPONOFF", "On/Off", false));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("LFORATE", "lfoRate", juce::NormalisableRange<float>(0.0f, 20.0f), 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("LFOLEVEL", "lfoLevel", juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f));
    
    return{ params.begin(), params.end() }; //returns vector of parameters
}
