#pragma once
#include "SynthEngine.h"
class Lfo
{
private:
	float phase;
	float s, sq, sh;
    Random rg;
	float SampleRate;
public:
	float Frequency;
	int waveForm;
	Lfo()
	{
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
            return Res;
	}
	void setSamlpeRate(float sr)
	{
		SampleRate=sr;
	}
#define fastsin(x) ((( x + ( x * (-x * x / 6)))))
	inline void update()
	{
		phase+=((Frequency / SampleRate * float_Pi));
		sq = (phase>0?1:-1);
		s = sin(phase);
		if(phase > float_Pi)
		{
			phase-=2*float_Pi;
			sh = rg.nextFloat()*2-1;
		}

	}
};