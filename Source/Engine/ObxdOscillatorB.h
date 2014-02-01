#pragma once

#include "ObxdVoice.h"
#include "SynthEngine.h"
#include "AudioUtils.h"
#include "BlepData.h"
#include "DelayLine.h"
#include "SawOsc.h"
#include "PulseOsc.h"
#include "TriangleOsc.h"

class ObxdOscillatorB
{
private:
	float SampleRate;
	float d1,d2,d3,d4;
	float pitch1;
	float pitch2;
	bool pw1t,pw1tw,pw2t,pw2tw;
	float sampleRateInv;


	float x1,x2;
	float x1p,x2p;


	float pw1w,pw2w;
	//blep const
	const int n;
	const int hsam;
	const float *Blep;
	float *buffer1,*buffer2;
	int bP1,bP2;
	float *dbuffer1,*dbuffer2;
	int dP1,dP2;
	//delay line implements fixed sample delay
	DelayLine *del1,*del2;
	DelayLine *flt1,*flt2; 
	DelayLine *xmodd,*pastXmod;
	DelayLineBoolean *syncd;
	DelayLine *syncFracd;
	DelayLine *cvd;
	Random wn;
	SawOsc o1s,o2s;
	PulseOsc o1p,o2p;
	TriangleOsc o1t,o2t;

	bool active;
public:

	float tune;//+-1 
	int oct;

	float notePlaying;


	float osc1Factor;
	float osc2Factor;
	float totalDetune;

	float osc2Det;
	float pulseWidth;
	float pw1,pw2;

	bool osc1w,osc2w;

	bool quantizeCw;

	float o1mx,o2mx;
	float nmx;
	float pto1,pto2;


	//osc pitches
	float osc1Saw,osc2Saw,
		osc1Pul,osc2Pul;

	float osc1p,osc2p;
	bool hardSync;
	float xmod;

	float br;


	ObxdOscillatorB() : 
		n(Samples*2),
		hsam(Samples),
		Blep(blep),
		o1s(),o2s(),
		o1p(),o2p(),
		o1t(),o2t()
	{
		active = false;
		totalDetune = 0;
		wn = Random(Random::getSystemRandom().nextInt64());
		osc1Factor = wn.nextFloat()-0.5;
		osc2Factor = wn.nextFloat()-0.5;
		nmx=0;
		oct=0;
		tune=0;
		br=1;
		x1p=x2p=0;
		pw1w=pw2w=0;
		pw1t=pw2t=pw1tw=pw2tw=false;
		pto1=pto2=0;
		pw1=pw2=0;
		xmod = 0;
		hardSync = false;
		osc1p=osc2p=10;
		osc1Saw=osc2Saw=osc1Pul=osc2Pul=false;
		osc2Det = 0;
		notePlaying = 30;
		d1=d2=d3=d4=0;
		pulseWidth = 0;
		osc1w=osc2w=false;
		o1mx=o2mx=0;
		Blep = blep;
		bP1 = bP2=0;
		x1=x2=0;

		buffer1= new float[n];
		buffer2 = new float[n];
		dbuffer1= new float[n];
		dbuffer2 = new float[n];
		del1 = new DelayLine(hsam);
		del2 = new DelayLine(hsam);
		flt1 = new DelayLine(hsam);
		flt2 = new DelayLine(hsam);
		xmodd = new DelayLine(hsam);
		pastXmod = new DelayLine(hsam);
		syncd = new DelayLineBoolean(hsam);
		syncFracd =  new DelayLine(hsam);
		cvd = new DelayLine(hsam);
		//for(int i = 0 ; i < 
		for(int i = 0 ; i < n;i++)
		{
			dbuffer1[i]=0;
			dbuffer2[i]=0;
			buffer1[i]=0;
			buffer2[i]=0;
		}
		dP1 = dP2=0;
	}
	~ObxdOscillatorB()
	{
		delete flt1;
		delete flt2;
		delete del1;
		delete del2;
		delete xmodd;
		delete pastXmod;
		delete buffer1;
		delete buffer2;
		delete dbuffer1;
		delete dbuffer2;
		delete cvd;
		delete syncd;
		delete syncFracd;
	}
	void setSampleRate(float sr)
	{
		SampleRate = sr;
		sampleRateInv = 1.0f / SampleRate;
	}
	inline float ProcessSample()
	{
		float res = 0;
		pitch1 = getPitch(notePlaying + (quantizeCw?((int)(osc1p)):osc1p)+ pto1 + tune + oct+totalDetune*osc1Factor);
		if(pitch1 > 21000)
			pitch1 = 21000;
		bool hsr = false;
		float hsfrac=0;

		float fs = pitch1*(sampleRateInv);
		x1+=fs;
		hsfrac = 0;
		float osc1mix=0.0f;
		float pwcalc =jlimit<float>(0.1f,1.0f,(pulseWidth + pw1)*0.5f + 0.5f);

		if(osc1Pul)
			o1p.processMaster(x1,fs,hsr,hsfrac,pwcalc,pw1w);
		if(osc1Saw)
			o1s.processMaster(x1,fs,hsr,hsfrac);
		else if(!osc1Pul)
			o1t.processMaster(x1,fs,hsr,hsfrac);

		if(x1 >= 1.0f)
			x1-=1.0f;

		pw1w = pwcalc;

		hsr &= hardSync;
		syncd->feedDelay(hsr);
		syncFracd ->feedDelay(hsfrac);
		hsr = syncd->getDelayedSample();
		hsfrac = syncFracd->getDelayedSample();

		if(osc1Pul)
		{
			osc1mix += o1p.getValue(x1,pwcalc) + o1p.aliasReduction();
		}
		if(osc1Saw)
		{
			osc1mix += o1s.getValue(x1) + o1s.aliasReduction();
		}
		else if(!osc1Pul)
		{
			osc1mix = o1t.getValue(x1) + o1t.aliasReduction();
		}
		cvd->feedDelay( getPitch(notePlaying + osc2Det + (quantizeCw?((int)(osc2p)):osc2p) + pto2+ osc1mix *xmod + tune + oct +totalDetune*osc2Factor));
		pitch2 = cvd->getDelayedSample();


		if(pitch2>21000)
			pitch2=21000;
		fs = pitch2 * (sampleRateInv);

		pwcalc = jlimit<float>(0.1f,1.0f,(pulseWidth + pw2)*0.5f + 0.5f);

		float osc2mix=0.0f;

		x2 +=fs;

		//o2s.processSlave(x2,fs,hsr,hsfrac);
		if(osc2Pul)
			o2p.processSlave(x2,fs,hsr,hsfrac,pwcalc,pw2w);
		if(osc2Saw)
			o2s.processSlave(x2,fs,hsr,hsfrac);
		else if(!osc2Pul)
			o2t.processSlave(x2,fs,hsr,hsfrac);


		if(x2 >= 1.0f)
			x2-=1.0;


		pw2w=pwcalc;

		if(hsr)
		{
			float fracMaster = (fs * hsfrac);
			x2 =fracMaster;
		}
		xmodd ->feedDelay(osc1mix);
		osc1mix = xmodd->getDelayedSample();

		if(osc2Pul)
			osc2mix += o2p.getValue(x2,pwcalc) + o2p.aliasReduction();
		if(osc2Saw)
			osc2mix += o2s.getValue(x2) + o2s.aliasReduction();
		else if(!osc2Pul)
			osc2mix = o2t.getValue(x2) + o2t.aliasReduction();

		float filtration1 = osc1mix;
		float filtration2 = osc2mix;

		//filtration1 =filtration1 - tptlpstatic(d1,filtration1,3,SampleRate);
		//filtration2 = filtration2 - tptlpstatic(d3,filtration2,3,SampleRate);
		//filtration1 = filtration1 - tptlp(d1,filtration1,pt1f/8,SampleRate);

		//filtration2 = filtration2 - tptlp(d3,filtration2,pt2f/8,SampleRate);

		res+=o1mx*filtration1 + o2mx *filtration2 + (wn.nextFloat()-0.5)*(nmx*1.3);
		//res = res - tptlpupw(d1,res,5,sampleRateInv);
		//res = tptlp(d2,res,br,SampleRate);
		return res*3;
		//return sin(x1 * float_Pi*2 - float_Pi);
	}
	void setActive()
	{
		active = true;
	}
	void setNotActive()
	{
		active = false;
	}
};