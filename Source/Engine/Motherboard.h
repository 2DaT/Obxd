#pragma once
#include "VoiceQueue.h"
#include "SynthEngine.h"
#include "Lfo.h"

class Motherboard
{
private:
	VoiceQueue vq;
	int totalvc;
	bool wasUni;
	Decimator9 left,right;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Motherboard)
public:
	Lfo mlfo;
	float Volume;
	float* pannings;
	ObxdVoice** voices;
	const static int MAX_VOICES=8;
	bool uni;
	bool Oversample;
	Motherboard()
	{
		Oversample=false;
		left = Decimator9();
		right =  Decimator9();
		mlfo= Lfo();
		uni = false;
		wasUni = false;
		Volume=0;
		voices = new ObxdVoice* [MAX_VOICES];
		pannings = new float[MAX_VOICES];
		totalvc = MAX_VOICES;
		vq = VoiceQueue(MAX_VOICES,voices);
		for(int i = 0 ; i < MAX_VOICES;++i)
		{
			pannings[i]= 0.5;
			voices[i]=new ObxdVoice();
		}
	}
	~Motherboard()
	{
		delete pannings;
		for(int i = 0 ; i < MAX_VOICES;++i)
		{
			delete voices[i];
		}
		delete voices;
	}
	void setVoiceCount(int count)
	{
		for(int i = count ; i < MAX_VOICES;i++)
		{
			voices[i]->NoteOff();
			voices[i]->ResetEnvelope();
		}
		vq.ReInit(count);
		totalvc = count;
	}
	void unisonOn()
	{

	}
	void setSampleRate(float sr)
	{
		mlfo.setSamlpeRate(sr);
		for(int i = 0 ; i < MAX_VOICES;++i)
		{
			voices[i]->setSampleRate(sr);
		}
	}
	void setNoteOn(int noteNo)
	{
		bool processed=false;
		if (wasUni != uni)
			unisonOn();
		if (uni)
		{
			for (int i = 0; i < totalvc; i++)
			{
				voices[i]->NoteOn(noteNo);
			}
			processed = true;
		}
		for (int i = 0; i < totalvc && !processed; i++)
		{
			ObxdVoice* p = vq.GetNext();
			if (!p->Active)
			{
				p->NoteOn(noteNo);
				processed = true;
			}
		}
		if(!processed)
			vq.GetNext()->NoteOn(noteNo);
		wasUni = uni;
		//  }
		// WasUni = Params.Unison;
		//if (!processed)
		//vq.GetNext().Trigger(noteNo);
	}

	void setNoteOff(int noteNo)
	{
		for (int i = 0; i < totalvc; i++)
		{
			ObxdVoice* n = vq.GetNext();
			if (n->midiIndx==noteNo && n->Active)
				n->NoteOff();
		}
	}
	void SetOversample(bool over)
	{
		if(over!=Oversample)
		{
			for(int i = 0 ; i < MAX_VOICES;i++)
			{
				vq.GetNext()->ToogleOversample();
	
			}
		}
		Oversample = over;
	}
	void processSample(float* sm1,float* sm2)
	{
		mlfo.update();
		float vl=0,vr=0;
		float vlo = 0 , vro = 0 ;
		float lfovalue = mlfo.getVal();
		for(int i = 0 ; i < totalvc;i++)
		{
			float mem[2] = {0,0};
			voices[i]->lfoIn=lfovalue;
			(voices[i]->ProcessSample(mem));
			float x1 = mem[0];
			vl+=x1*(1-pannings[i]);
			vr+=x1*(pannings[i]);
			if(Oversample)
			{
				float x2 = mem[1];
				vlo+=x2*(1-pannings[i]);
				vro+=x2*(pannings[i]);
			}
		}
		if(Oversample)
		{

			//Variables are swapped!
			vl = 0.5*left.Calc(vlo,vl);
			vr = 0.5*right.Calc(vro,vr);
		}
		*sm1 = vl*Volume;
		*sm2 = vr*Volume;
	}
};