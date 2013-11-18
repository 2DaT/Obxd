#pragma once
#include "ObxdVoice.h"
#include <math.h>
class Filter
{
private:
	float cutoffWas;
	float s1,s2;
	float R;
public:
	float SampleRate;
	bool newtonRaphson;
	float mm;
	Filter()
	{
		newtonRaphson = false;
		mm=0;
		s1=s2=0;
		cutoffWas=0;
		SampleRate = 44000;
		R=1;
	}
	void setSampleRate(float sr)
	{
		SampleRate = sr;
	}
	void setResonance(float res)
	{
		R = 1-res;
	}
	inline float NR(float sample, float g1,float g2)
	{
		//float y = ((sample- R * s1*2 - g*s1 - s2)/(1+ R*g*2 + g*g));
		float y = ((sample- R * s1*2 - g2*s1  - s2)/(1+ R*g1*2 + g1*g2));
		if(newtonRaphson)
		{
		for(int i = 0 ; i< 4;i++)
		{
			y = y - nrfdb(y,g1,g2,sample) / -nrfdbder(y,g1,g2);
		}
		}
		return y;
	}
	inline float nrfdb(float y,float g1,float g2,float x)
	{
		float fp = inl (y*g1 + s1);
		return y + 2*R*(fp) + nl(fp) + inl( g2 * fp + s2)- x;
	}
	inline float nrfdbder(float y,float g1,float g2)
	{
		float fp = inl(y*g1 + s1);
		float fpd = inld(y*g1 + s1) * g1;
		return 1 + 2*R*fpd + nld(fp) *fpd + inld( g2 * fp + s2)*fpd*g2;
	}
	float inl (float y)
	{
		//return y;
		return  atan(y/4)*4;
	}
	float inld(float y)
	{
		//return 1;
		y = y/7;
		return 1.0 / (1 + y *y);
	}
	float nl(float y)
	{
		//return 0;
		return pow(y/13,3);
		//return 0;
	}
	float nld(float y)
	{
		return 0;
		return 3*pow(y/13,2)/13;
		//return 0;
	}
	float Apply(float sample,float g)
        {
			float g1 = (float)tan(g /SampleRate * juce::float_Pi);
			float g2 = g1;
            //float v = ((sample- R * s1*2 - g2*s1 - s2)/(1+ R*g1*2 + g1*g2));
			float v = NR(sample,g1,g2);
            //first integrator
            float y1 = v*g1 + s1;
			//y1 = inl(y1/3)*3;
			//y1 = jlimit(-2.0f,2.0f,y1)*0.98;
			y1 = jlimit(-1.0f,1.0f,y1) * 0.999;
			// = y1+nl(y1);
            s1 = v*g1 + y1; 

			float y2 = y1*g2 + s2;
			//y2=inl(y2);
			s2 = y2 + y1*g2;
            double mc;
                mc =mm==-1? y1: (1-mm)*y2 + (mm)*v;
				return mc;
          //  return inl(mc*10)/10;
        }
};