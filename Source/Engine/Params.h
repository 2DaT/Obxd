#pragma once
#include "ObxdVoice.h"
#include "ParamsEnum.h"
class ObxdParams
{
public:
	float* values;
	ObxdParams()
	{
		values = new float[PARAM_COUNT];
		for(int i = 0 ; i < PARAM_COUNT;++i)
		{
			values[i] = 0.0f;
		}
		values[VOICE_COUNT] = 1.0f;
		values[BRIGHTNESS]=1.0f;
		values[OCTAVE]=0.5;
		values[TUNE] = 0.5f;
		values[OSC2_DET]=0.4;
		values[LSUS]=1.0f;
		values[CUTOFF]=0.5f;
		values[VOLUME]=0.8f;
		values[OSC1MIX]=1;
		values[OSC2MIX]=1;
		values[FILTER_DRIVE]= 0.2;
		values[PAN1]=0.5;
		values[PAN2]=0.5;
		values[PAN3]=0.5;
		values[PAN4]=0.5;
		values[PAN5]=0.5;
		values[PAN6]=0.5;
		values[PAN7]=0.5;
		values[PAN8]=0.5;
	}
};