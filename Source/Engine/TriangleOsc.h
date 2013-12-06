#pragma once
#include "SynthEngine.h"
#include "BlepData.h"
class TriangleOsc 
{
	DelayLine *del1;
	bool fall;
	float *buffer1,*buffer2;
	float intSt,arintSt;
	const int hsam;
	const int n;

	float statev,stval;
	float internalSquare;
	float aliasint;
	int bP1,bP2;

	bool pw1tint;
public:
	TriangleOsc() : hsam(Samples)
		, n(Samples*2)
	{
		internalSquare = 0;
		pw1tint = false;
		statev= 0;
		stval = 0;
		intSt = 0;
		arintSt = 0;
		fall = false;
		bP1=bP2=0;
		buffer1= new float[n];
		for(int i = 0 ; i < n ; i++)
			buffer1[i]=0;
		buffer2= new float[n];
		for(int i = 0 ; i < n ; i++)
			buffer2[i]=0;
		del1 = new DelayLine(hsam);
	}
	~TriangleOsc()
	{
		delete buffer1;
		delete del1;
	}
	inline float aliasReduction(float fs)
	{
		float c2 = triangleIntegration(aliasint,-getNextBlep(buffer1,bP1),fs*0.25)*2;
		return c2;
		//return 0;
	}
	inline float triangleIntegration(float& state , float input,float delta)
	{
	//delta = tan(delta * float_Pi);
	double v = (input - state) * delta / (1 + delta);
	double res = v + state;
	state = res + v;
	return res;
	}
	inline void processMaster(float x,float delta,bool& hardSyncReset,float& hardSyncFrac)
	{
		if((pw1tint) && x >= 1.0f)
		{
			x -= 1.0f;
			if(pw1tint)
				mixInImpulseCenter(buffer1,bP1,x/delta, 1);
			pw1tint=false;
		}
		if((!pw1tint)&& (x >= 0.5))
		{
			pw1tint=true;
			float frac  =(x-0.5) / delta;
			mixInImpulseCenter(buffer1,bP1,frac,-1);
		}
		if((pw1tint) && x >= 1.0f)
		{
			x-=1.0f;
			if(pw1tint)
				mixInImpulseCenter(buffer1,bP1,x/delta, 1);
			pw1tint=false;
		}
		float oscmix;
		if(x >= 0.5)
			oscmix = 1 - (0.5-0.5) - 0.5;
		else
			oscmix = -(0.5-0.5) - 0.5;
		internalSquare = oscmix;
	}
	inline float getValue(float x,float fs)
	{
		float mix = triangleIntegration(statev,internalSquare,fs*0.25)*2;
		del1->feedDelay(mix);
		return del1->getDelayedSample();
	}
	inline float getValueFast(float x,float fs)
	{
		float cpv = statev;
		return triangleIntegration(cpv,internalSquare,fs*0.25)*2;
	}
	inline void processSlave(float x , float delta,bool hardSyncReset,float hardSyncFrac)
	{

	}
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
	inline void mixSincCenter(float * buf,int& pbos,float offset,float scale)
	{

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
};