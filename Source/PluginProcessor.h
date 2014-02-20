/*
	==============================================================================
	This file is part of Obxd synthesizer.

	Copyright © 2013-2014 Filatov Vadim
	
	Contact author via email :
	justdat_@_e1.ru

	This file may be licensed under the terms of of the
	GNU General Public License Version 2 (the ``GPL'').

	Software distributed under the License is distributed
	on an ``AS IS'' basis, WITHOUT WARRANTY OF ANY KIND, either
	express or implied. See the GPL for the specific language
	governing rights and limitations.

	You should have received a copy of the GPL along with this
	program. If not, go to http://www.gnu.org/licenses/gpl.html
	or write to the Free Software Foundation, Inc.,  
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
	==============================================================================
 */

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED
#include "../JuceLibraryCode/JuceHeader.h"
#include "Engine\SynthEngine.h"
#include <stack>
#include "Engine\midiMap.h"
#include "Engine\ObxdBank.h"

//==============================================================================
/**
*/
class ObxdAudioProcessor  : public AudioProcessor, //public AudioProcessorListener,
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
	
	int lastMovedController,lastUsedParameter;

	MidiMessage* nextMidi,*midiMsg;
	MidiMap bindings;
	bool midiControlledParamSet;
	
	bool hasMidiMessage;
    int midiEventPos;
	void ObxdAudioProcessor::processMidiPerSample(MidiBuffer::Iterator* iter,const int samplePos);
	bool ObxdAudioProcessor::getNextEvent(MidiBuffer::Iterator* iter,const int samplePos);

    //==============================================================================
	SynthEngine* synth;
	//==============================================
	ObxdBank programs;
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
	void setCurrentProgramStateInformation(const void* data,int sizeInBytes);
	void getCurrentProgramStateInformation(MemoryBlock& destData);

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ObxdAudioProcessor)
};

#endif  // PLUGINPROCESSOR_H_INCLUDED
