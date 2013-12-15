#pragma once
#include "SynthEngine.h"
#include "BlepData.h"
class TriangleOsc 
{
	DelayLine* del1;
	bool fall;
	float *buffer1,*buffer2;
	const int hsam;
	const int n;

	int bP1,bP2;
public:
	TriangleOsc() : hsam(Samples)
		, n(Samples*2)
	{
		del1 =new DelayLine(hsam);
		fall = false;
		bP1=bP2=0;
		buffer1= new float[n];
		for(int i = 0 ; i < n ; i++)
			buffer1[i]=0;
		buffer2= new float[n];
		for(int i = 0 ; i < n ; i++)
			buffer2[i]=0;
	}
	~TriangleOsc()
	{
		delete buffer1;
		delete buffer2;
		delete del1;
	}
	inline float aliasReduction()
	{
		return -getNextBlep(buffer1,bP1);
		//return 0;
	}
	inline void processMaster(float x,float delta,bool& hardSyncReset,float& hardSyncFrac)
	{
		if(x >= 1.0)
		{
			x-=1.0;
			hardSyncFrac = x/delta;
			hardSyncReset = true;
			mixInBlampCenter(buffer1,bP1,x/delta,-4*Samples*delta);
		}
		if(x >= 0.5 && x - delta < 0.5)
		{
			mixInBlampCenter(buffer1,bP1,(x-0.5)/delta,4*Samples*delta);
		}
		if(x >= 1.0)
		{
			x-=1.0;
			hardSyncFrac = x/delta;
			hardSyncReset = true;
			mixInBlampCenter(buffer1,bP1,x/delta,-4*Samples*delta);
		}
	}
	inline float getValue(float x)
	{
		float mix = x < 0.5 ? 2*x-0.5 : 1.5-2*x;
		del1->feedDelay(mix);
		return del1->getDelayedSample();
	}
	inline float getValueFast(float x)
	{
		float mix = x < 0.5 ? 2*x-0.5 : 1.5-2*x;
		return mix;
	}
	inline void processSlave(float x , float delta,bool hardSyncReset,float hardSyncFrac)
	{
		bool hspass = true;
		if(x >= 1.0)
		{
			x-=1.0;
			if(((!hardSyncReset)||(x/delta > hardSyncFrac)))//de morgan processed equation
			{
				mixInBlampCenter(buffer1,bP1,x/delta,-4*Samples*delta);
			}
			else
			{
				x+=1;
				hspass = false;
			}
		}
		if(x >= 0.5 && x - delta < 0.5 &&hspass)
		{
			float frac = (x - 0.5) / delta;
			if(((!hardSyncReset)||(frac > hardSyncFrac)))//de morgan processed equation
			{
				mixInBlampCenter(buffer1,bP1,frac,4*Samples*delta);
			}
		}
		if(x >= 1.0 && hspass)
		{
			x-=1.0;
			if(((!hardSyncReset)||(x/delta > hardSyncFrac)))//de morgan processed equation
			{
				mixInBlampCenter(buffer1,bP1,x/delta,-4*Samples*delta);
			}
			else
			{
				//if transition do not ocurred 
				x+=1;
				//x2-=fs;
			}
		}
		if(hardSyncReset)
		{
			float fracMaster = (delta * hardSyncFrac);
			float trans = (x-fracMaster);
			float mix = trans < 0.5 ? 2*trans-0.5 : 1.5-2*trans;
			if(trans >0.5)
				mixInBlampCenter(buffer1,bP1,hardSyncFrac,-4*Samples*delta);
			mixInImpulseCenter(buffer1,bP1,hardSyncFrac,mix+0.5);
			//x2 =fracMaster;
			//mix = x;
		}
	}
	inline void mixInBlampCenter(float * buf,int& bpos,float offset, float scale)
	{
		int lpIn =(int)(B_OVERSAMPLING*(offset));
		float frac = offset * B_OVERSAMPLING - lpIn;
		for(int i = 0 ; i <n;i++)
		{
			float mixvalue = 0;
			//mixvalue = Blep[lpIn] * scale;
			mixvalue = (blamp[lpIn]*(1-frac)+blamp[lpIn+1]*(frac));
			//Substract trivial ramp
			if(i >=Samples) 
				mixvalue   -=  ((lpIn + frac) / (B_OVERSAMPLING * Samples)) - 1;
			buf[(bpos+i)&(n-1)]  +=mixvalue*scale;
			lpIn += B_OVERSAMPLING;
		}
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
	inline void mixSincCenter(float * buf,int& bpos,float offset,float scale)
	{
		int lpIn =(int)(B_OVERSAMPLING*(offset));
		float frac = offset * B_OVERSAMPLING - lpIn;
		for(int i = 0 ; i <n;i++)
		{
			float mixvalue = 0;
			//mixvalue = Blep[lpIn] * scale;

			mixvalue = (sinc[lpIn]*(1-frac)+sinc[lpIn+1]*(frac));
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
};