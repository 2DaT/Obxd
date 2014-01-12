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
	float velocityValue;

	UpsampleFIRFilter* upff;
	float d1,d2;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ObxdVoice)
public:

	AdsrEnvelope env;
	AdsrEnvelope fenv;
	ObxdOscillatorB osc;
	Filter flt;

	Random ng;

	float vamp,vflt;

	float cutoff;
	float fenvamt;

	float EnvDetune;
	float FenvDetune;

	float FltDetune;
	float FltDetAmt;

	float PortaDetune;
	float PortaDetuneAmt;

	float brightCoef;

	int midiIndx;

	bool Active;
	bool fltKF;

	float porta;
	float prtst;

	float cutoffwas,envelopewas;

	float lfoIn;
	float lfoVibratoIn;

	float pitchWheel;
	float pitchWheelAmt;
	bool pitchWheelOsc2Only;

	float lfoa1,lfoa2;
	bool lfoo1,lfoo2,lfof;
	bool lfopw1,lfopw2;

	bool Oversample;

	float filterDrive;

	float envpitchmod;

	bool fourpole;


	bool stabguard;

	DelayLine *lenvd,*fenvd;

	ApInterpolator ap;
	float oscpsw;
	int legatoMode;
	float briHold;

	ObxdVoice() 
		: ap(),
		ng()
	{
		vamp=vflt=0;
		velocityValue=0;
		lfoVibratoIn=0;
		fourpole = false;
		legatoMode = 0;
		brightCoef =briHold= 1;
		envpitchmod = 0;
		oscpsw = 0;
		cutoffwas = envelopewas=0;
		Oversample= false;
		d1=d2=0;
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
		lenvd=new DelayLine(Samples*2);
		fenvd=new DelayLine(Samples*2);
	}
	~ObxdVoice()
	{
		delete upff;
		delete lenvd;
		delete fenvd;
	}
	inline void ProcessSample(float* ptr)
	{
		//portamento on osc input voltage
		//implements rc circuit
		float ptNote  =tptlpupw(prtst, midiIndx-81, porta * (1+PortaDetune*PortaDetuneAmt),sampleRateInv);
		osc.notePlaying = ptNote;
		//both envelopes needs a delay equal to osc internal delay
		float envm = fenv.processSample() * (1 - (1-velocityValue)*vflt);
		fenvd->feedDelay(envm);
		float cutoffcalc = jmin(getPitch((lfof?lfoIn*lfoa1:0)+cutoff+FltDetune*FltDetAmt+ fenvamt*fenvd->getDelayedSample() -45 + (fltKF ?ptNote+40:0)), (flt.SampleRate*0.5f-120.0f));
		lenvd->feedDelay(env.processSample() * (1 - (1-velocityValue)*vamp));

		osc.pw1 = lfopw1?lfoIn*lfoa2:0;
		osc.pw2 = lfopw2?lfoIn*lfoa2:0;
		osc.pto1 =   (!pitchWheelOsc2Only? (pitchWheel*pitchWheelAmt):0 ) + ( lfoo1?lfoIn*lfoa1:0) + lfoVibratoIn;
		osc.pto2 =  (pitchWheel *pitchWheelAmt) + (lfoo2?lfoIn*lfoa1:0) + (envpitchmod * envm) + lfoVibratoIn;
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
			if(fourpole)
			x2 = flt.Apply4Pole(x2,(cutoffcalc+cutoffwas)*0.5);
			else
				x2 = flt.Apply(x2,(cutoffcalc+cutoffwas)*0.5);
			x2 = tptpc(d2,x2,brightCoef);
			//x2 /= (filterDrive);
			x2 *= (env+envelopewas)*0.5;
			*(ptr+1) = x2;
		}
		float x1;
		if(!Oversample)
		{
			x1 = oscps;
			if(fourpole)
			x1 = flt.Apply4Pole(x1,(cutoffcalc)); 
			else
				x1 = flt.Apply(x1,(cutoffcalc)); 

		}
		else
		{
			if(fourpole)
			x1 = flt.Apply4Pole(ap.getInterp(oscps),(cutoffcalc)); 
			else
				x1 = flt.Apply(ap.getInterp(oscps),(cutoffcalc)); 
		}
		x1 = tptpc(d2,x1,brightCoef);
		x1 *= (env);
		*(ptr)=x1;

		//*(ptr+1)=x1;
		//
		oscpsw = oscps;
		cutoffwas = cutoffcalc;
		envelopewas = env;
	}
	void setBrightness(float val)
	{
		briHold = val;
		brightCoef = tan(jmin(val,flt.SampleRate*0.5f-10)* (juce::float_Pi)*flt.sampleRateInv);

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
		brightCoef = tan(jmin(briHold,flt.SampleRate*0.5f-10)* (juce::float_Pi) * flt.sampleRateInv);
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
		brightCoef = tan(jmin(briHold,flt.SampleRate*0.5f-10)* (juce::float_Pi)* flt.sampleRateInv);
	}
	void NoteOn(int mididx,float velocity)
	{
		if(velocity!=-0.5)
		velocityValue = velocity;
		//osc.midiIndex = mididx-81;
		midiIndx = mididx;
		if((!Active)||(legatoMode&1))
			env.triggerAttack();
		if((!Active)||(legatoMode&2))
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