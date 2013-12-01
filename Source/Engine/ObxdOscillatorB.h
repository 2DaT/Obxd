#pragma once

#include "ObxdVoice.h"
#include "SynthEngine.h"
#include "AudioUtils.h"
#include "BlepData.h"
#include "DelayLine.h"

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
	Random wn;
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
	float osc1p,osc2p;

	bool hardSync;
	float xmod;

	float br;


	ObxdOscillatorB() : 
		n(Samples*2),
		hsam(Samples),
		Blep(blep)
	{
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
		osc1p = osc2p=0;
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
		delete buffer1;
		delete buffer2;
		delete dbuffer1;
		delete dbuffer2;
	}
	void setSampleRate(float sr)
	{
		SampleRate = sr;
		sampleRateInv = 1.0f / SampleRate;
	}
	float ProcessSample()
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
		if(!osc1w)
			pwcalc=0;


		if(!osc1w)
		{
			if(x1 >= 1.0f)
			{
				x1-=1.0f;
				hsfrac = x1/fs;
				mixInImpulseCenter(buffer1,bP1,x1/fs, 1);
				hsr = true;
			}
		}

		osc1mix=x1;
		if(osc1w)
		{
			float summated = fs- (pwcalc - pw1w);
			if(!(pw1t)&& (x1 >= pwcalc))
			{
				pw1t=true;
				float frac  =(x1-pwcalc) / summated;
				mixInImpulseCenter(buffer1,bP1,frac,-1);
			}
			if(x1 >= 1.0f)
			{
				x1-=1.0f;
				hsfrac = x1/fs;
				if(pw1t)
					mixInImpulseCenter(buffer1,bP1,x1/fs, 1);
				pw1t=false;
				hsr = true;
			}



			if(pw1t)
				osc1mix =  1 - (0.5-pwcalc);
			else
				osc1mix = -(0.5-pwcalc);
		}
		pw1w = pwcalc;

		del1->feedDelay(osc1mix);
		hsr&=hardSync;



		pitch2 = getPitch(notePlaying + osc2Det + (quantizeCw?((int)(osc2p)):osc2p) + pto2+ (osc1mix-0.5)*xmod + tune + oct + totalDetune +totalDetune*osc2Factor);

		if(pitch2>21000)
			pitch2=21000;
		fs = pitch2 * (sampleRateInv);
		pwcalc = jlimit<float>(0.1f,1.0f,(pulseWidth + pw2)*0.5f + 0.5f);
		if(!osc2w)
			pwcalc=0;
		float osc2mix=0.0f;

		x2 +=fs;

		if(!osc2w)
		{
			if(x2 >= 1.0f)
			{
				x2 -= 1.0f;
				if(((!hsr)||(x2/fs > hsfrac)))//de morgan processed equation
				{
					mixInImpulseCenter(buffer2,bP2,x2/fs, 1);
				}
				else
				{
					//if transition do not ocurred 
					x2+=1;
					//x2-=fs;
				}
			}
		}
		osc2mix=x2;
		if(osc2w)
		{
			float summated = fs- (pwcalc - pw2w);
			
			if(!(pw2t)&& (x2 >= pwcalc))
			{
				pw2t=true;
				float frac  =(x2-pwcalc) / summated;
				if(((!hsr)||(frac > hsfrac)))//de morgan processed equation
				{
					//transition to 1
					mixInImpulseCenter(buffer2,bP2,frac,-1);
				}
				else
				{
					//if transition do not ocurred 
					pw2t=false;
				}

			}
			if(x2 >= 1.0f)
			{
				x2 -= 1.0f;
				if(((!hsr)||(x2/fs > hsfrac)))//de morgan processed equation
				{
					if(pw2t)
						mixInImpulseCenter(buffer2,bP2,x2/fs, 1);
					pw2t=false;
				}
				else
				{
					x2+=1;
				}
			}

			if(pw2t)
				osc2mix = 1 - (0.5-pwcalc);
			else
				osc2mix = -(0.5-pwcalc);
		}

		pw2w=pwcalc;
		if(hsr)
		{
			float fracMaster = (fs * hsfrac);
			float trans = osc2w?(pw2t?1 :0):(x2-fracMaster);
			mixInImpulseCenter(buffer2,bP2,hsfrac,trans);
			pw2t = false;
			x2 =fracMaster;
			osc2mix = osc2w?(pw2t?1- (0.5-pwcalc):-(0.5-pwcalc)):x2;
		}

		del2->feedDelay(osc2mix);
		flt1->feedDelay(pitch1);
		flt2->feedDelay(pitch2);
		float pt1f = flt1->getDelayedSample();
		float pt2f = flt2->getDelayedSample();
		float filtration1 = (del1->getDelayedSample() -getNextBlep(buffer1,bP1) - 0.5);
		float filtration2 = (del2->getDelayedSample() - getNextBlep(buffer2,bP2) -0.5);

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
#define LERP(A,B,F) ((B-A)*F+A)
	inline void mixInImpulseCenter(float * buf,int& bpos,float offset, float scale) 
	{
		//offset = fmod(offset,1.0f);
		int lpIn =(int)(B_OVERSAMPLING*(offset));
		float frac = offset * B_OVERSAMPLING - lpIn;
		for(int i = 0 ; i <n;i++)
		{
			float mixvalue = 0;
			//mixvalue = Blep[lpIn] * scale;

			mixvalue = (blep[lpIn]*(1-frac)+blep[lpIn+1]*(frac));
			if(i>=Samples)
				mixvalue-=1;
			buf[(bpos+i)&(n-1)]  += mixvalue*scale;
			lpIn += B_OVERSAMPLING;
		}
	}
	float getDelayedSample(float* buf,int& dpos)
	{
		int idx;
		idx = dpos-(hsam);
		if(idx <0)
			idx+=hsam;
		return buf[idx];
	}
	inline void feedDelay(float* buf,int& dpos,float sm)
	{
		buf[dpos] = sm;
		dpos++;
		if(dpos >= (hsam))
			dpos-=(hsam);
	}
	inline float getNextBlep(float* buf,int& bpos) 
	{
		buf[bpos]= 0.0f;
		bpos++;

		// Wrap pos
		if (bpos>=n) 
		{
			bpos -= n;
		}
		return buf[bpos];
	}
	void resetOscInternals()
	{
	}
};