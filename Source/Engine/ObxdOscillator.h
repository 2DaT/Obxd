#pragma once

#include "ObxdVoice.h"
#include "SynthEngine.h"
#include "AudioUtils.h"

class ObxdOscillator
{
private:
	float angle1;
	float angle2;
	float SampleRate;
	float d1,d2,d3,d4;
	float pitch1;
	float pitch2;
	float Udet;
public:
	float notePlaying;
	float osc2Det;
	float pulseWidth;
	bool osc1w,osc2w;
	bool osc1Only;
	ObxdOscillator()
	{
		osc2Det = 0;
		notePlaying = 30;
		angle1 = angle2=0;
		d1=d2=d3=d4=0;
		pulseWidth = 0;
		osc1w=osc2w=false;
		osc1Only=false;
	}
	void setSampleRate(float sr)
	{
		SampleRate = sr;
	}
	float ProcessSample()
	{
		float rez = 0;
		pitch1 = getPitch(notePlaying);

		if(osc1w)
		{
			rez +=tptlp(d1,blit(angle1,pitch1) - blit(angle1-(jmin(pulseWidth+ 1, 2.0f) * float_Pi),pitch1),pitch1/8,SampleRate);
		}

		else
		{
			rez +=tptlp(d1,blit(angle1,pitch1),pitch1/8,SampleRate);
		}
		pitch2 = getPitch(notePlaying + osc2Det);
		if(!osc1Only)
		{
		if(osc2w)
		{
			rez +=tptlp(d2,blit(angle2,pitch2) - blit(angle2-(jmin(pulseWidth + 1, 2.0f) * float_Pi),pitch2),pitch2/8,SampleRate);
		}
		else
		{
			rez +=tptlp(d2,blit(angle2,pitch2),pitch2/8,SampleRate);
		}
		}
		angle1 += 2*float_Pi*pitch1/SampleRate;
		angle2 += 2*float_Pi*pitch2/SampleRate;

		//nlp(d3,blit(angle1,pitch1),pitch1/8,SampleRate);
		//nlp(d4,blit(angle2,pitch2),pitch2/8,SampleRate);
		if(angle1 > float_Pi)
			angle1-=float_Pi*2;
		if(angle2 > float_Pi)
			angle2-=float_Pi*2;
		//return d1*40;
		return (rez*4);
	}
	float blit(float phase,float pitch)
	{
		float op = 20000 / pitch;
		float m = floor(op) + 0.5f;
		float sum = ((sin(m * phase) / (sin(phase * 0.5))))+dc;
		//double ts = Shared.OscillatorLimit / pitch;
		//double freq = Math.Floor(ts);
		//double coef = 1 - (Shared.OscillatorLimit - (freq * pitch)) / pitch;
		// sum += Math.Cos((phase * freq)) / -ts * coef *2;
		// sum *= (op);
		if (phase == 0)
			sum = 0.0;
		sum -= 1.0;
		return sum;

	}
};