#pragma once
#include "SynthEngine.h"
class Lfo
{
private:
	float phase;
	float s, sq, sh;
	float s1;
    Random rg;
	float SampleRate;
	float SampleRateInv;
public:
	float Frequency;
	int waveForm;
	Lfo()
	{
		s1=0;
		Frequency=1;
		phase=0;
		s=sq=sh=0;
		rg=Random();
	}
	inline float getVal()
	{
		 float Res = 0;
            if((waveForm &1) !=0 )
                Res+=s;
            if((waveForm&2)!=0)
                Res+=sq;
            if((waveForm&4)!=0)
                Res+=sh;
			return tptlpupw(s1, Res,200,SampleRateInv);
	}
	void setSamlpeRate(float sr)
	{
		SampleRate=sr;
		SampleRateInv = 1 / SampleRate;
	}
	inline void update()
	{
		phase+=((Frequency * SampleRateInv * float_Pi*2));
		sq = (phase>0?1:-1);
		s = sin(phase);
		if(phase > float_Pi)
		{
			phase-=2*float_Pi;
			sh = rg.nextFloat()*2-1;
		}

	}
};