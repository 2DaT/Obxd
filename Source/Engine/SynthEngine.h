#pragma once

#include "..\PluginProcessor.h"
#include "ObxdVoice.h"
#include "Motherboard.h"
#include "Params.h"

class SynthEngine
{
private:
	Motherboard * synth;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthEngine)
public:
	SynthEngine()
	{
		synth = new Motherboard();
	}
	~SynthEngine()
	{
		delete synth;
	}
	void setSampleRate(float sr)
	{
		synth->setSampleRate(sr);
	}
	void processSample(float *left,float *right)
	{
		synth->processSample(left,right);
	}
	void procNoteOn(int noteNo)
	{
		synth->setNoteOn(noteNo);
	}
	void procNoteOff(int noteNo)
	{
		synth->setNoteOff(noteNo);
	}
	void procPitchWheel(float val)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->pitchWheel = val;
		}
	}
	void setVoiceCount(float param)
	{
		synth->setVoiceCount(roundToInt((param*7) +1));
	}
	void procPitchWheelAmount(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->pitchWheelAmt = param>0.5?12:2;
		}
	}
	void procPitchWheelOsc2Only(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->pitchWheelOsc2Only = param>0.5;
		}
	}
	void processPan(float param,int idx)
	{
		synth->pannings[idx-1] = param;
	}
	void processTune(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->osc.tune = param*2-1;
		}
	}
	void processLegatoMode(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->legatoMode = roundToInt(param*3 + 1) -1;
		}
	}
	void processOctave(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->osc.oct = ((int)(param*4) - 2)*12;
		}
	}
	void processFilterKeyFollow(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->fltKF = param>0.5;
		}
	}
	void processUnison(float param)
	{
		synth->uni = param>0.5f;
	}
	void processPortamento(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->porta =logsc(1-param,0.14,250,150);
		}
	}
	void processVolume(float param)
	{
		synth->Volume = linsc(param,0,0.15);
	}
	void processLfoFrequency(float param)
	{
		synth->mlfo.Frequency = logsc(param,0,50,120);
	}
	void processLfoSine(float param)
	{
		if(param>0.5)
		{
			synth->mlfo.waveForm |=1;
		}
		else
		{
			synth->mlfo.waveForm&=~1;
		}
	}
	void processLfoSquare(float param)
	{
		if(param>0.5)
		{
			synth->mlfo.waveForm |=2;
		}
		else
		{
			synth->mlfo.waveForm&=~2;
		}
	}
	void processLfoSH(float param)
	{
		if(param>0.5)
		{
			synth->mlfo.waveForm |=4;
		}
		else
		{
			synth->mlfo.waveForm&=~4;
		}
	}
	void processLfoAmt1(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->lfoa1 = logsc(logsc(param,0,1,13),0,60,40);
		}
	}
	void processLfoOsc1(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->lfoo1 = param>0.5;
		}
	}
	void processLfoOsc2(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->lfoo2 = param>0.5;
		}
	}
	void processLfoFilter(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->lfof = param>0.5;
		}
	}
	void processLfoPw1(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->lfopw1 = param>0.5;
		}
	}
	void processLfoPw2(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->lfopw2 = param>0.5;
		}
	}
	void processLfoAmt2(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->lfoa2 = linsc(param,0,0.7);
		}
	}
	void processDetune(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->osc.totalDetune = logsc(param,0.003,0.90);
		}
	}
	void processPulseWidth(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->osc.pulseWidth = linsc(param,0.0,0.95);
		}
	}
	void processOsc2Xmod(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->osc.xmod= param*24;
		}
	}
	void processEnvelopeToPitch(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->envpitchmod= param*36;
		}
	}
	void processOsc2HardSync(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->osc.hardSync = param>0.5;
		}
	}
	void processOsc1Pitch(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->osc.osc1p = (param * 48);
		}
	}
	void processOsc2Pitch(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->osc.osc2p = (param * 48);
		}
	}
	void processPitchQuantization(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->osc.quantizeCw = param>0.5;
		}
	}
	void processOsc1Mix(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->osc.o1mx = param;
		}
	}
	void processOsc2Mix(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->osc.o2mx = param;
		}
	}
	void processNoiseMix(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->osc.nmx = logsc(param,0,1,35);
		}
	}
	void processBrightness(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->setBrightness(  linsc(param,10000,26000));
		}
	}
	void processOsc2Det(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->osc.osc2Det = logsc(param,0.001,0.55);
		}
	}

	void processOsc1Saw(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->osc.osc1Saw = param>0.5;
		}
	}
	void processOsc1Pulse(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->osc.osc1Pul = param>0.5;
		}
	}
	void processOsc2Saw(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->osc.osc2Saw= param>0.5;
		}
	}
	void processOsc2Pulse(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->osc.osc2Pul= param>0.5;
		}
	}

	void processCutoff(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			//synth->voices[i]->cutoff = logsc(param,60,19000,30);
			synth->voices[i]->cutoff = linsc(param,0,120);
		}
	}
	void processBandpassSw(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			//synth->voices[i]->cutoff = logsc(param,60,19000,30);
			synth->voices[i]->flt.bandPassSw = param>0.5;
		}
	}
	void processResonance(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->flt.setResonance(0.993-logsc(1-param,0,0.993,40));
		}
	}
	void processMultimode(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			//synth->voices[i]->flt ;
			synth->voices[i]->flt.mm = linsc(param,0,1);
		}
	}
	void processOversampling(float param)
	{
		synth->SetOversample(param>0.5);
	}
	void processFilterDrive(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->filterDrive = linsc(param,0.01,0.7);
		}
	}
	void processFilterEnvelopeAmt(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->fenvamt = linsc(param,0,140);
		}
	}
	void processLoudnessEnvelopeAttack(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->env.setAttack(logsc(param,4,60000,900));
		}
	}
	void processLoudnessEnvelopeDecay(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->env.setDecay(logsc(param,4,60000,900));
		}
	}
	void processLoudnessEnvelopeRelease(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->env.setRelease(logsc(param,4,60000,900));
		}
	}
	void processLoudnessEnvelopeSustain(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->env.setSustain(param);
		}
	}
	void processFilterEnvelopeAttack(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->fenv.setAttack(logsc(param,1,60000,900));
		}
	}
	void processFilterEnvelopeDecay(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->fenv.setDecay(logsc(param,1,60000,900));
		}
	}
	void processFilterEnvelopeRelease(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->fenv.setRelease(logsc(param,1,60000,900));
		}
	}
	void processFilterEnvelopeSustain(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->fenv.setSustain(param);
		}
	}
	void processEnvelopeDetune(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->setEnvDer(linsc(param,0.0,1));
		}
	}
	void processFilterDetune(float param)
	{
for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->FltDetAmt = linsc(param,0.0,18);
		}
	}
	void processPortamentoDetune(float param)
	{
		for(int i = 0 ; i < synth->MAX_VOICES;i++)
		{
			synth->voices[i]->PortaDetuneAmt = linsc(param,0.0,0.75);
		}
	}

		 
};