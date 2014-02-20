/*
==============================================================================

This file was auto-generated!

It contains the basic startup code for a Juce application.

==============================================================================
*/
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Engine\Params.h"
#include <xmmintrin.h>
//==============================================================================
#define S(T) (juce::String(T))
ObxdAudioProcessor::ObxdAudioProcessor() : bindings(),programs()
{
	midiControlledParamSet = false;
	lastMovedController = 0;
	lastUsedParameter = 0;
	synth = new SynthEngine();
	synth->setSampleRate(44100);
	initAllParams();
}

ObxdAudioProcessor::~ObxdAudioProcessor()
{
	delete synth;
}
//==============================================================================
void ObxdAudioProcessor::initAllParams()
{
	for(int i = 0 ; i < PARAM_COUNT;i++)
		setParameter(i,programs.currentProgramPtr->values[i]);
}
//==============================================================================
const String ObxdAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

int ObxdAudioProcessor::getNumParameters()
{
	return PARAM_COUNT;
}

float ObxdAudioProcessor::getParameter (int index)
{
	return programs.currentProgramPtr->values[index];
}

void ObxdAudioProcessor::setParameter (int index, float newValue)
{
	if(!midiControlledParamSet || index==MIDILEARN || index==UNLEARN)
	lastUsedParameter = index;
	programs.currentProgramPtr->values[index] = newValue;
	switch(index)
	{
	case VAMPENV:
		synth->procAmpVelocityAmount(newValue);
		break;
	case VFLTENV:
		synth->procFltVelocityAmount(newValue);
		break;
	case ASPLAYEDALLOCATION:
		synth->procAsPlayedAlloc(newValue);
		break;
	case BENDLFORATE:
		synth->procModWheelFrequency(newValue);
		break;
	case FOURPOLE:
		synth->processFourPole(newValue);
		break;
	case LEGATOMODE:
		synth->processLegatoMode(newValue);
		break;
	case ENVPITCH:
		synth->processEnvelopeToPitch(newValue);
		break;
	case OSCQuantize:
		synth->processPitchQuantization(newValue);
		break;
	case VOICE_COUNT:
		synth->setVoiceCount(newValue);
		break;
	case BANDPASS:
		synth->processBandpassSw(newValue);
		break;
	case FILTER_WARM:
		synth->processOversampling(newValue);
		break;
	case BENDOSC2:
		synth->procPitchWheelOsc2Only(newValue);
		break;
	case BENDRANGE:
		synth->procPitchWheelAmount(newValue);
		break;
	case NOISEMIX:
		synth->processNoiseMix(newValue);
		break;
	case OCTAVE:
		synth->processOctave(newValue);
		break;
	case TUNE:
		synth->processTune(newValue);
		break;
	case BRIGHTNESS:
		synth->processBrightness(newValue);
		break;
	case MULTIMODE:
		synth->processMultimode(newValue);
		break;
	case LFOFREQ:
		synth->processLfoFrequency(newValue);
		break;
	case LFO1AMT:
		synth->processLfoAmt1(newValue);
		break;
	case LFO2AMT:
		synth->processLfoAmt2(newValue);
		break;
	case LFOSINWAVE:
		synth->processLfoSine(newValue);
		break;
	case LFOSQUAREWAVE:
		synth->processLfoSquare(newValue);
		break;
	case LFOSHWAVE:
		synth->processLfoSH(newValue);
		break;
	case LFOFILTER:
		synth->processLfoFilter(newValue);
		break;
	case LFOOSC1:
		synth->processLfoOsc1(newValue);
		break;
	case LFOOSC2:
		synth->processLfoOsc2(newValue);
		break;
	case LFOPW1:
		synth->processLfoPw1(newValue);
		break;
	case LFOPW2:
		synth->processLfoPw2(newValue);
		break;
	case PORTADER:
		synth->processPortamentoDetune(newValue);
		break;
	case FILTERDER:
		synth->processFilterDetune(newValue);
		break;
	case ENVDER:
		synth->processEnvelopeDetune(newValue);
		break;
	case XMOD:
		synth->processOsc2Xmod(newValue);
		break;
	case OSC2HS:
		synth->processOsc2HardSync(newValue);
		break;
	case OSC2P:
		synth->processOsc2Pitch(newValue);
		break;
	case OSC1P:
		synth->processOsc1Pitch(newValue);
		break;
	case PORTAMENTO:
		synth->processPortamento(newValue);
		break;
	case UNISON:
		synth->processUnison(newValue);
		break;
	case FLT_KF:
		synth->processFilterKeyFollow(newValue);
		break;
	case OSC1MIX:
		synth->processOsc1Mix(newValue);
		break;
	case OSC2MIX:
		synth->processOsc2Mix(newValue);
		break;
	case PW:
		synth->processPulseWidth(newValue);
		break;
	case OSC1Saw:
		synth->processOsc1Saw(newValue);
		break;
	case OSC2Saw:
		synth->processOsc2Saw(newValue);
		break;
	case OSC1Pul:
		synth->processOsc1Pulse(newValue);
		break;
	case OSC2Pul:
		synth->processOsc2Pulse(newValue);
		break;
	case VOLUME:
		synth->processVolume(newValue);
		break;
	case UDET:
		synth->processDetune(newValue);
		break;
	case OSC2_DET:
		synth->processOsc2Det(newValue);
		break;
	case CUTOFF:
		synth->processCutoff(newValue);
		break;
	case RESONANCE:
		synth->processResonance(newValue);
		break;
	case ENVELOPE_AMT:
		synth->processFilterEnvelopeAmt(newValue);
		break;
	case LATK:
		synth->processLoudnessEnvelopeAttack(newValue);
		break;
	case LDEC:
		synth->processLoudnessEnvelopeDecay(newValue);
		break;
	case LSUS:
		synth->processLoudnessEnvelopeSustain(newValue);
		break;
	case LREL:
		synth->processLoudnessEnvelopeRelease(newValue);
		break;
	case FATK:
		synth->processFilterEnvelopeAttack(newValue);
		break;
	case FDEC:
		synth->processFilterEnvelopeDecay(newValue);
		break;
	case FSUS:
		synth->processFilterEnvelopeSustain(newValue);
		break;
	case FREL:
		synth->processFilterEnvelopeRelease(newValue);
		break;
	case PAN1:
		synth->processPan(newValue,1);
		break;
	case PAN2:
		synth->processPan(newValue,2);
		break;
	case PAN3:
		synth->processPan(newValue,3);
		break;
	case PAN4:
		synth->processPan(newValue,4);
		break;
	case PAN5:
		synth->processPan(newValue,5);
		break;
	case PAN6:
		synth->processPan(newValue,6);
		break;
	case PAN7:
		synth->processPan(newValue,7);
		break;
	case PAN8:
		synth->processPan(newValue,8);
		break;
	}
	sendChangeMessage();
}
const String ObxdAudioProcessor::getParameterName (int index)
{
	switch(index)
	{
	case UNLEARN:
		return S("MidiUnlearn");
	case MIDILEARN:
		return S("MidiLearn");
	case VAMPENV:
		return S("VAmpFactor");
	case VFLTENV:
		return S("VFltFactor");
	case ASPLAYEDALLOCATION:
		return S("AsPlayedAllocation");
	case BENDLFORATE:
		return S("VibratoRate");
	case FOURPOLE:
		return S("FourPole");
	case LEGATOMODE:
		return S("LegatoMode");
	case ENVPITCH:
		return S("EnvelopeToPitch");
	case OSCQuantize:
		return S("PitchQuant");
	case VOICE_COUNT:
		return S("VoiceCount");
	case BANDPASS:
		return S("BandpassBlend");
	case FILTER_WARM:
		return S("Filter_Warm");
	case BENDRANGE:
		return S("BendRange");
	case BENDOSC2:
		return S("BendOsc2Only");
	case OCTAVE:
		return S("Octave");
	case TUNE:
		return S("Tune");
	case BRIGHTNESS:
		return S("Brightness");
	case NOISEMIX:
		return S("NoiseMix");
	case OSC1MIX:
		return S("Osc1Mix");
	case OSC2MIX:
		return S("Osc2Mix");
	case MULTIMODE:
		return S("Multimode");
	case LFOSHWAVE:
		return S("LfoSampleHoldWave");
	case LFOSINWAVE:
		return S("LfoSineWave");
	case LFOSQUAREWAVE:
		return S("LfoSquareWave");
	case LFO1AMT:
		return S("LfoAmount1");
	case LFO2AMT:
		return S("LfoAmount2");
	case LFOFILTER:
		return S("LfoFilter");
	case LFOOSC1:
		return S("LfoOsc1");
	case LFOOSC2:
		return S("LfoOsc2");
	case LFOFREQ:
		return S("LfoFrequency");
	case LFOPW1:
		return S("LfoPw1");
	case LFOPW2:
		return S("LfoPw2");
	case PORTADER:
		return S("PortamentoDetune");
	case FILTERDER:
		return S("FilterDetune");
	case ENVDER:
		return S("EnvelopeDetune");
	case PAN1:
		return S("Pan1");
	case PAN2:
		return S("Pan2");
	case PAN3:
		return S("Pan3");
	case PAN4:
		return S("Pan4");
	case PAN5:
		return S("Pan5");
	case PAN6:
		return S("Pan6");
	case PAN7:
		return S("Pan7");
	case PAN8:
		return S("Pan8");
	case XMOD:
		return S("Xmod");
	case OSC2HS:
		return S("Osc2HardSync");
	case OSC1P:
		return S("Osc1Pitch");
	case OSC2P:
		return S("Osc2Pitch");
	case PORTAMENTO:
		return S("Portamento");
	case UNISON:
		return S("Unison");
	case FLT_KF:
		return S("FilterKeyFollow");
	case PW:
		return S("PulseWidth");
	case OSC2Saw:
		return S("Osc2Saw");
	case OSC1Saw:
		return S("Osc1Saw");
	case OSC1Pul:
		return S("Osc1Pulse");
	case OSC2Pul:
		return S("Osc2Pulse");
	case VOLUME:
		return S("Volume");
	case UDET:
		return S("VoiceDetune");
	case OSC2_DET:
		return S("Oscillator2detune");
	case CUTOFF:
		return S("Cutoff");
	case RESONANCE:
		return S("Resonance");
	case ENVELOPE_AMT:
		return S("FilterEnvAmount");
	case LATK:
		return S("Attack");
	case LDEC:
		return S("Decay");
	case LSUS:
		return S("Sustain");
	case LREL:
		return S("Release");
	case FATK:
		return S("FilterAttack");
	case FDEC:
		return S("FilterDecay");
	case FSUS:
		return S("FilterSustain");
	case FREL:
		return S("FilterRelease");
	}
	return String::empty;
}

const String ObxdAudioProcessor::getParameterText (int index)
{
	return String(programs.currentProgramPtr->values[index],2);
}

const String ObxdAudioProcessor::getInputChannelName (int channelIndex) const
{
	return String (channelIndex + 1);
}

const String ObxdAudioProcessor::getOutputChannelName (int channelIndex) const
{
	return String (channelIndex + 1);
}

bool ObxdAudioProcessor::isInputChannelStereoPair (int index) const
{
	return true;
}

bool ObxdAudioProcessor::isOutputChannelStereoPair (int index) const
{
	return true;
}

bool ObxdAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool ObxdAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool ObxdAudioProcessor::silenceInProducesSilenceOut() const
{
	return false;
}

double ObxdAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}
int ObxdAudioProcessor::getNumPrograms()
{
	return PROGRAMCOUNT;
}

int ObxdAudioProcessor::getCurrentProgram()
{
	return programs.currentProgram;
}

void ObxdAudioProcessor::setCurrentProgram (int index)
{
	programs.currentProgram = index;
	programs.currentProgramPtr = programs.programs + programs.currentProgram;
	for(int i = 0 ; i < PARAM_COUNT;i++)
		setParameter(i,programs.currentProgramPtr->values[i]);
}

const String ObxdAudioProcessor::getProgramName (int index)
{
	return programs.programs[index].name;
}

void ObxdAudioProcessor::changeProgramName (int index, const String& newName)
{
	 programs.programs[index].name = newName;
}

//==============================================================================
void ObxdAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
	nextMidi= new MidiMessage(0xF0);
	midiMsg = new MidiMessage(0xF0);
	synth->setSampleRate(sampleRate);

}

void ObxdAudioProcessor::releaseResources()
{

}
inline void ObxdAudioProcessor::processMidiPerSample(MidiBuffer::Iterator* iter,const int samplePos)
{
	while (getNextEvent(iter, samplePos))
	{
		if(midiMsg->isNoteOn())
		{
			synth->procNoteOn(midiMsg->getNoteNumber(),midiMsg->getFloatVelocity());
		}
		if (midiMsg->isNoteOff())
		{
			synth->procNoteOff(midiMsg->getNoteNumber());
		}
		if(midiMsg->isPitchWheel())
		{
			// [0..16383] center = 8192;
			synth->procPitchWheel((midiMsg->getPitchWheelValue()-8192) / 8192.0);
		}
		if(midiMsg->isController() && midiMsg->getControllerNumber()==1)
			synth->procModWheel(midiMsg->getControllerValue() / 127.0);
		if(midiMsg->isController())
		{
			lastMovedController = midiMsg->getControllerNumber();
			if(programs.currentProgramPtr->values[MIDILEARN]  > 0.5)
				bindings.controllers[lastMovedController] = lastUsedParameter;
			if(programs.currentProgramPtr->values[UNLEARN] >0.5)
			{
				midiControlledParamSet = true;
				bindings.controllers[lastMovedController] = 0;
				setParameter(UNLEARN,0);
				lastMovedController = 0;
				lastUsedParameter = 0;
				midiControlledParamSet = false;
			}

			if(bindings.controllers[lastMovedController] > 0)
			{
				midiControlledParamSet = true;
				setParameter(bindings.controllers[lastMovedController],midiMsg->getControllerValue() / 127.0);
				setParameter(MIDILEARN,0);
				lastMovedController = 0;
				lastUsedParameter = 0;

				midiControlledParamSet = false;
			}

		}
		if(midiMsg->isSustainPedalOn())
		{
			synth->sustainOn();
		}
		if(midiMsg->isSustainPedalOff() || midiMsg->isAllNotesOff()||midiMsg->isAllSoundOff())
		{
			synth->sustainOff();
		}
		if(midiMsg->isAllNotesOff())
		{
			synth->allNotesOff();
		}
		if(midiMsg->isAllSoundOff())
		{
			synth->allSoundOff();
		}

	}
}
bool ObxdAudioProcessor::getNextEvent(MidiBuffer::Iterator* iter,const int samplePos)
{
	if (hasMidiMessage && midiEventPos <= samplePos)
	{
		*midiMsg = *nextMidi;
		hasMidiMessage = iter->getNextEvent(*nextMidi, midiEventPos);
		return true;
	} 
	return false;
}
void ObxdAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	//SSE flags set
	_MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
	_MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
	MidiBuffer::Iterator ppp(midiMessages);
	const ScopedLock sl (this->getCallbackLock());
	hasMidiMessage = ppp.getNextEvent(*nextMidi,midiEventPos);
	int samplePos = 0;
	int numSamples = buffer.getNumSamples();
	float* channelData1 = buffer.getSampleData(0);
	float* channelData2 = buffer.getSampleData(1);
	while (samplePos < numSamples)
	{
		processMidiPerSample(&ppp,samplePos);

		synth->processSample(channelData1+samplePos,channelData2+samplePos);

		samplePos++;
	}
}

//==============================================================================
bool ObxdAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ObxdAudioProcessor::createEditor()
{
	 return new ObxdAudioProcessorEditor (this);
	//return NULL;
}

//==============================================================================
void ObxdAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	//XmlElement* const xmlState = getXmlFromBinary(;
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
	XmlElement xmlState = XmlElement("Datsounds");
	xmlState.setAttribute(S("currentProgram"),programs.currentProgram);
	XmlElement* xprogs=  new XmlElement("programs");
	for(int i = 0 ; i < PROGRAMCOUNT;i++)
		 {
			 XmlElement* xpr = new XmlElement("program");
			 xpr->setAttribute(S("programName"),programs.programs[i].name);
			 for(int k = 0 ; k < PARAM_COUNT;k++)
			 {
				 xpr->setAttribute(String(k), programs.programs[i].values[k]);
			 }
			 xprogs->addChildElement(xpr);
		 }
		xmlState.addChildElement(xprogs);
		for(int i = 0 ; i < 255;i++)
		{
			xmlState.setAttribute(String(i),bindings.controllers[i]);
		}
		 copyXmlToBinary(xmlState,destData);
}

void ObxdAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
	XmlElement* const xmlState = getXmlFromBinary(data,sizeInBytes);
	XmlElement* xprogs = xmlState->getFirstChildElement();
	if(xprogs->hasTagName(S("programs")));
	{
		int i = 0 ;
		forEachXmlChildElement (*xprogs, e)
            {
				programs.programs[i].setDefaultValues();
				for(int k = 0 ; k < PARAM_COUNT;k++)
				{
					programs.programs[i].values[k] = e->getDoubleAttribute(String(k),programs.programs[i].values[k]);
				}
				programs.programs[i].name= e->getStringAttribute(S("programName"),S("Default"));
                i++;
            }
	}
	for(int i = 0 ; i < 255;i++)
		{
			bindings.controllers[i] = xmlState->getIntAttribute(String(i),0);
		}
	setCurrentProgram(xmlState->getIntAttribute(S("currentProgram"),0));
	delete xmlState;
}
void  ObxdAudioProcessor::setCurrentProgramStateInformation(const void* data,int sizeInBytes)
{
	XmlElement* const e = getXmlFromBinary(data,sizeInBytes);
	programs.currentProgramPtr->setDefaultValues();
	for(int k = 0 ; k < PARAM_COUNT;k++)
				{
					programs.currentProgramPtr->values[k] = e->getDoubleAttribute(String(k),programs.currentProgramPtr->values[k]);
				}
	programs.currentProgramPtr->name =  e->getStringAttribute(S("programName"),S("Default"));
	setCurrentProgram(programs.currentProgram);
}
void ObxdAudioProcessor::getCurrentProgramStateInformation(MemoryBlock& destData)
{
	XmlElement xmlState = XmlElement("Datsounds");
	for(int k = 0 ; k < PARAM_COUNT;k++)
				{
				 xmlState.setAttribute(String(k), programs.currentProgramPtr->values[k]);
	}
	xmlState.setAttribute(S("programName"),programs.currentProgramPtr->name);
		 copyXmlToBinary(xmlState,destData);
}
//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new ObxdAudioProcessor();
}
