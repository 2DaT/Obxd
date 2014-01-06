#pragma once
#include "ObxdVoice.h"
#include <math.h>
class Filter
{
private:
	float cutoffWas;
	float s1,s2,s3,s4;
	float R;
	float R24;
	float rcor;
	float rezd;
	float gw;
public:
	float SampleRate;
	float sampleRateInv;
	bool newtonRaphson;
	bool bandPassSw;
	float mm;
	Filter()
	{
		bandPassSw = false;
		newtonRaphson = false;
		mm=0;
		s1=s2=s3=s4=0;
		rezd=0;
		gw = 0;
		cutoffWas=0;
		SampleRate = 44000;
		sampleRateInv = 1 / SampleRate;
		rcor =560 / 44000;
		R=1;
		R24=0;
	}
	inline void setSampleRate(float sr)
	{
		SampleRate = sr;
		sampleRateInv = 1/SampleRate;
		//rcor = 560 /sr ;
		rcor = (480.0 / (SampleRate/44000) / 44000);
	}
	inline void setResonance(float res)
	{
		R = 1-res;
		R24 = 3.5 * res;
	}
	inline float NR(float sample, float g)
	{ 
		//float y = ((sample- R * s1*2 - g*s1 - s2)/(1+ R*g*2 + g*g));
		float y = ((sample- R * s1*2 - g*s1  - s2)/(1+ R*g*2 + g*g)) + dc;
		//if(newtonRaphson)
		//{
		//for(int i = 0 ; i< 1;i++)
		//{
//			y -=  nrfdb(y,g1,g2,sample) / nrfdbder(y,g1,g2);
		//}
		//}
		return y;
	}
	inline float NR24(float sample,float g,float lpc)
	{
		float correct = 1 / (1+g);
		float S = (lpc*lpc*lpc*s1 + lpc*lpc * s2 + lpc*s3 +s4)*correct;
		float G = lpc*lpc*lpc*lpc;
		float y = (sample - R24 * S) / (1 + R24*G);

		return y;
	}
	inline float nrfdb(float y,float g1,float g2,float x)
	{
		//float fp = inl (y*g1 + s1);
		//return -( y + 2*R*(y*g1+s) + nl(g1*y + s1) +((g1*y+s1)*g2 + s2)- x);
		return 0;
	}
	inline float nrfdbder(float y,float g1,float g2)
	{
		return-( 1 + 2*R*g1 + nld(g1*y + s1)*g1+ g1*g2);
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
		return pow(y/8,7);
		//return 0;2.3
	}
	float nld(float y)
	{
		//return 0;
		return 7*pow(y/8,6)/8;
		//return 0;
	}
	inline float Apply4Pole(float sample,float g)
	{
			float g1 = (float)tan(g *sampleRateInv * juce::float_Pi);
			g = g1;
			
			float lpc = g / (1 + g);

			float y0 = NR24(sample,g,lpc);

			float y1= tptpc(s1,y0,g);
			float y2 = tptpc(s2,y1,g);
			float y3 = tptpc(s3,y2,g);
			float y4 = tptpc(s4,y3,g);
			//float v = (y0 - s1)*lpc;
            //float y1 = v + s1;
            //s1 = y1 + v;

			//v = (y1 - s2)*lpc;
			//float y2 = v + s2;
			//s2 = y2 + v;

			//v = (y2 - s3)*lpc;
			//float y3 = v + s3;
			//s3 = y3 + v;

			//v = (y3 - s4)*lpc;
			//float y4 = v + s4;
			//s4 = y4 + v;

			return y4;
	}
	inline float Apply(float sample,float g)
        {
			float g1 = (float)tan(g *sampleRateInv * juce::float_Pi);
			g = g1;
            //float v = ((sample- R * s1*2 - g2*s1 - s2)/(1+ R*g1*2 + g1*g2));
			float v = NR(sample,g);
            //first integrator
            float y1 = v*g + s1;
		//	y1 =  tanh(y1*(1.0/ 85))*85;
		//	y1 = jlimit(-5.0f,5.0f,y1);
			//y1 = atan(y1 /75)*75;
            s1 = v*g+y1;
			//s1 = atan(s1 / 50)*50;
			s1 = atan(s1 * rcor) / rcor;
			//s1 = jlimit(-10.0f,10.0f,s1)*0.9999;

			float y2 = y1*g + s2;
			s2 = y2 + y1*g;
            double mc;
			if(!bandPassSw)
            mc = (1-mm)*y2 + (mm)*v;
			else
			{

				mc =2 * ( mm < 0.5 ? 
					((0.5 - mm) * y2 + (mm) * y1):
					((1-mm) * y1 + (mm-0.5) * v)
						);
			}

			return mc;
          //  return inl(mc*10)/10;
        }
};