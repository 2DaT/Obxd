#pragma once
#include "ObxdOscillatorB.h"
#include "ObxdOscillator.h"
#include "AdsrEnvelope.h"
#include "Filter.h"
#include "Decimator.h"
#include "FirFilter.h"
#include "APInterpolator.h"

class ObxdVoice
{
private:
	float SampleRate;
	float sampleRateInv;
	float Volume;
	float port;

	UpsampleFIRFilter* upff;
	float d1;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ObxdVoice)
public:

	AdsrEnvelope env;
	AdsrEnvelope fenv;
	ObxdOscillatorB osc;
	Filter flt;

	Random ng;

	float cutoff;
	float fenvamt;

	float EnvDetune;
	float FenvDetune;

	float FltDetune;
	float FltDetAmt;

	float PortaDetune;
	float PortaDetuneAmt;


	int midiIndx;

	bool Active;
	bool fltKF;

	float porta;
	float prtst;

	float cutoffwas,envelopewas;

	float lfoIn;
	float pitchWheel;
	float pitchWheelAmt;
	bool pitchWheelOsc2Only;

	float lfoa1,lfoa2;
	bool lfoo1,lfoo2,lfof;
	bool lfopw1,lfopw2;

	bool Oversample;

	float filterDrive;


	bool stabguard;

	DelayLine *lenvd,*fenvd;
	
	ApInterpolator ap;
	float oscpsw;

	ObxdVoice() 
		: ap(),
		ng()
	{
		oscpsw = 0;
		cutoffwas = envelopewas=0;
		Oversample= false;
		d1=0;
		upff = new UpsampleFIRFilter();
		filterDrive = 0.1;
		pitchWheel=pitchWheelAmt=0;
		lfoIn=0;
		PortaDetuneAmt=0;
		FltDetAmt=0;
		//EnvDetune=1;
		stabguard=  true;
		porta =0;
		prtst=0;
		fltKF= false;
		cutoff=0;
		fenvamt = 0;
		Active = false;
		midiIndx = 30;
		EnvDetune = Random::getSystemRandom().nextFloat()-0.5;
		FenvDetune = Random::getSystemRandom().nextFloat()-0.5;
		FltDetune = Random::getSystemRandom().nextFloat()-0.5;
		PortaDetune =Random::getSystemRandom().nextFloat()-0.5;
		lenvd=new DelayLine(Samples);
		fenvd=new DelayLine(Samples);
	}
	~ObxdVoice()
	{
		delete upff;
		delete lenvd;
		delete fenvd;
	}
	void ProcessSample(float* ptr)
	{
		//portamento on osc input voltage
		//implements rc circuit
		float ptNote  =tptlpupw(prtst, midiIndx-81, porta * (1+PortaDetune*PortaDetuneAmt),sampleRateInv);
		osc.notePlaying = ptNote;

		osc.pw1 = lfopw1?lfoIn*lfoa2:0;
		osc.pw2 = lfopw2?lfoIn*lfoa2:0;

		osc.pto1 =   (!pitchWheelOsc2Only? (pitchWheel*pitchWheelAmt):0 ) + ( lfoo1?lfoIn*lfoa1:0);
		osc.pto2 =  (pitchWheel *pitchWheelAmt) + (lfoo2?lfoIn*lfoa1:0);
		//both envelopes needs a delay equal to osc internal delay
		fenvd->feedDelay(fenv.processSample());
		float cutoffcalc = jmin(getPitch((lfof?lfoIn*lfoa1:0)+cutoff+FltDetune*FltDetAmt+ fenvamt*fenvd->getDelayedSample() -45 + (fltKF ?ptNote+30:0)), (flt.SampleRate*0.5f-120.0f));
		lenvd->feedDelay(env.processSample());
		//if(lfopw1)
		//{
		//filtinput = 2*upff->processUpsamplingFiltreing(filtinput);
		//filtinput2 = 2*upff->processUpsamplingFiltreing(filtinput2);
		//	*(ptr) = tptlp(d1,x1,24000,SampleRate*2);
		//	*(ptr+1)= tptlp(d1,x1,24000,SampleRate*2);
		//}



		//variable sort magic
		float env = lenvd->getDelayedSample();

				float x2 = 0;
				float oscps = osc.ProcessSample();
				if(Oversample)
		{
			x2=  oscpsw;
			x2 = flt.Apply(x2*2,(cutoffcalc+cutoffwas)*0.5);
			x2 = x2 - tptlpupw(d1,x2,20,flt.sampleRateInv);
			//x2 /= (filterDrive);
			x2 *= (env+envelopewas)*0.5;
			*(ptr+1) = x2;
		}
		float x1;
		if(!Oversample)
		{
		x1 = oscps;
		x1 = flt.Apply(x1,(cutoffcalc)); 

		}
		else
		{
			x1 = flt.Apply(2*ap.getInterp(oscps),(cutoffcalc)); 
		}
		x1 = x1 - tptlpupw(d1 , x1 , 20 , flt.sampleRateInv);
		x1 *= (env);
		*(ptr)=x1;


	//*(ptr+1)=x1;
	//
	oscpsw = oscps;
	cutoffwas = cutoffcalc;
	envelopewas = env;
}
void setEnvDer(float d)
{
	env.setUniqueDeriviance(1 + EnvDetune*d);
	fenv.setUniqueDeriviance(1 + FenvDetune*d);
}
void setSampleRate(float sr)
{
	flt.setSampleRate((Oversample)?2*sr:sr);
	osc.setSampleRate(sr);
	env.setSampleRate(sr);
	fenv.setSampleRate(sr);
	SampleRate = sr;
	sampleRateInv = 1 / sr;
}
void ResetEnvelope()
{
	env.ResetEnvelopeState();
	fenv.ResetEnvelopeState();
}
void ToogleOversample()
{
	flt.setSampleRate((Oversample)?SampleRate:2*SampleRate);
	Oversample = !Oversample;
}
void NoteOn(int mididx)
{
	//osc.midiIndex = mididx-81;
	midiIndx = mididx;
	env.triggerAttack();
	fenv.triggerAttack();
	Active = true;
}
void NoteOff()
{
	env.triggerRelease();
	fenv.triggerRelease();
	Active = false;
}
};