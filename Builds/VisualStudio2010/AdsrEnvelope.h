#pragma once
#include "ObxdVoice.h"
class AdsrEnvelope
{
private:
	    float Value;
        float attack, decay, sustain, release;
        float atklvl;
        float coef;
        int state;//1 - attack 2- decay 3 - sustain 4 - release 5-silence
		float SampleRate;
public:
	void setSampleRate(float sr)
	{
		SampleRate = sr;
	}
	void setAttack(float atk)
	{
			   coef = (float)((log(0.001) - log(1.0)) / (SampleRate * (atk) / 1000));
	}
	void setDecay(float dec)
	{
		coef = (float)((log(jmin(sustain + 0.0001,0.99)) - log(1.0)) / (SampleRate * (dec) / 1000));
	}
	void setSustain(float sust)
	{
		sustain = sust;
	}
	void setRelease(float rel)
	{
		 coef = (float)((log(0.00001) - log(Value + 0.0001)) / (SampleRate * (rel) / 1000));
	}
	AdsrEnvelope()
	{
		Value = 0.0;
		atklvl = 0;
		coef = 0;
		state = 5;
	}
	float processSample()
        {
            switch (state)
            {
                case 1:
                    if (Value - atklvl > -0.1)//Abjust later accordingly to ob-x envelopes
                    {
                        Value = jmin(Value, 0.99f);
                        state = 2;
                        coef = (float)((log(jmin(sustain + 0.0001, 0.99)) - log(1.0)) / (SampleRate * (decay) / 1000));
                    }
                        Value = Value - (1-Value)*(coef);
                    break;
                case 2:
                    if (Value - sustain < 10e-7)
                    {
                        state = 3;
                    }
                        Value =Value + Value * coef + dc;
                    break;
                case 3: Value = jmin(sustain, 0.9f);
                    break;
                case 4:
                   // if (Value > 10e-6)
                        Value = Value + Value * coef + dc;
                    break;
                case 5:
                    Value = 0.0f;
                    break;
            }
        }

}