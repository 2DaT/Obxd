#pragma once
#include "ObxdVoice.h"
#include "ParamsEnum.h"
class ObxdParams
{
public:
	float* values;
	String name;
	ObxdParams()
	{
		name = "Default";
		values = new float[PARAM_COUNT];
		setDefaultValues();
	}
	void setDefaultValues()
	{
		for(int k = 0 ; k < PARAM_COUNT;++k)
		{
			values[k] = 0.0f;
		}
		values[VOICE_COUNT] = 1.0f;
		values[BRIGHTNESS]=1.0f;
		values[OCTAVE]=0.5;
		values[TUNE] = 0.5f;
		values[OSC2_DET]=0.4;
		values[LSUS]=1.0f;
		values[CUTOFF]=0.5f;
		values[VOLUME]=0.5f;
		values[OSC1MIX]=1;
		values[OSC2MIX]=1;
		values[OSC1Saw]=1;
		values[OSC2Saw]=1;
		values[BENDLFORATE]=0.6;

//		values[FILTER_DRIVE]= 0.01;
		values[PAN1]=0.5;
		values[PAN2]=0.5;
		values[PAN3]=0.5;
		values[PAN4]=0.5;
		values[PAN5]=0.5;
		values[PAN6]=0.5;
		values[PAN7]=0.5;
		values[PAN8]=0.5;
	}
	~ObxdParams()
	{
		delete values;
	}
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ObxdParams)
};