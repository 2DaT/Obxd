#pragma once
#include "ObxdVoice.h"
#include <math.h>
class Filter
{
private:
	float s1,s2,s3,s4;
	float R;
	float R24;
	float rcor;
	float rcor24;
	//24 db multimode
	float mmt;
	int mmch;
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
		SampleRate = 44000;
		sampleRateInv = 1 / SampleRate;
		rcor =560.0 / 44000;
		rcor24 = 970 / 44000;
		R=1;
		R24=0;
	}
	void setMultimode(float m)
	{
		mm = m;
		mmch = (int)(mm * 3);
		mmt = mm*3-mmch;
	}
	inline void setSampleRate(float sr)
	{
		SampleRate = sr;
		sampleRateInv = 1/SampleRate;
		//rcor = 560 /sr ;
		float rcrate =sqrt((44000/SampleRate));
		rcor = (480.0 / 44000)*rcrate;
		rcor24 = (970.0 / 44000)*rcrate;
	}
	inline void setResonance(float res)
	{
		R = 1-res;
		R24 =( 3.5 * res);
	}
	inline float NR(float sample, float g)
	{ 
		float y = ((sample- R * s1*2 - g*s1  - s2)/(1+ R*g*2 + g*g)) + dc;
		return y;
	}
	inline float NR24(float sample,float g,float lpc)
	{
		float correct = 1 / (1+g);
		float S = (lpc*lpc*lpc*s1 + lpc*lpc * s2 + lpc*s3 +s4)*correct;
		float G = lpc*lpc*lpc*lpc;
		float y = (sample - R24 * S) / (1 + R24*G);
		//volume compensation
		return y + 1e-8;
	}
	inline float Apply4Pole(float sample,float g)
	{
			float g1 = (float)tan(g *sampleRateInv * juce::float_Pi);
			g = g1;
			
			float lpc = g / (1 + g);

			float y0 = NR24(sample,g,lpc);

			double v = (y0 - s1) * g / (1 + g);
			double res = v + s1;
			s1 = res + v;
			s1 =atan(s1*rcor24)/rcor24;
			float y1= res;
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
			float mc;
			//mc = (1-mm)*y4 + (mm)*y1;
			switch(mmch)
			{
			case 0:
				mc = ((1 - mmt) * y4 + (mmt) * y3);
				break;
			case 1:
				mc = ((1 - mmt) * y3 + (mmt) * y2);
				break;
			case 2:
				mc = ((1 - mmt) * y2 + (mmt) * y1);
				break;
			case 3:
				mc = y1;
				break;
			}
			return mc * (1 + R24 * 0.45);
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