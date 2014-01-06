/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED
#include "../JuceLibraryCode/JuceHeader.h"
#include "Engine\SynthEngine.h"
#include <stack>

//==============================================================================
/**
*/
class ObxdAudioProcessor  : public AudioProcessor,
	 public ChangeBroadcaster
{
public:
    //==============================================================================
    ObxdAudioProcessor();
    ~ObxdAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();

    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

    //==============================================================================
    AudioProcessorEditor* createEditor();
    bool hasEditor() const;
	

	MidiMessage* nextMidi,*midiMsg;
	std::stack<MidiMessage> sustain;
	bool sustainOn;
	
	bool hasMidiMessage;
    int midiEventPos;
	void ObxdAudioProcessor::processMidiPerSample(MidiBuffer::Iterator* iter,const int samplePos);
	bool ObxdAudioProcessor::getNextEvent(MidiBuffer::Iterator* iter,const int samplePos);

    //==============================================================================
	SynthEngine* synth;
	//==============================================
	ObxdParams parameters;
	//==============================================
    const String getName() const;

    int getNumParameters();

	//


	void initAllParams();
	//

    float getParameter (int index);
    void setParameter (int index, float newValue);

    const String getParameterName (int index);
    const String getParameterText (int index);

    const String getInputChannelName (int channelIndex) const;
    const String getOutputChannelName (int channelIndex) const;
    bool isInputChannelStereoPair (int index) const;
    bool isOutputChannelStereoPair (int index) const;

    bool acceptsMidi() const;
    bool producesMidi() const;
    bool silenceInProducesSilenceOut() const;
    double getTailLengthSeconds() const;

    //==============================================================================
    int getNumPrograms();
    int getCurrentProgram();
    void setCurrentProgram (int index);
    const String getProgramName (int index);
    void changeProgramName (int index, const String& newName);

    //==============================================================================
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ObxdAudioProcessor)
};

#endif  // PLUGINPROCESSOR_H_INCLUDED
