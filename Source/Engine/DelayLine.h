#pragma once
#include "SynthEngine.h"
class DelayLine
{
private:
	float* dl;
	int iidx;
	int maxc;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayLine)
public:
	DelayLine() 
	{
		dl  = NULL;
		iidx = 0;
		maxc=0;
	}
	DelayLine(int count)
	{
		iidx=0;
		maxc=count;
		dl = new float[maxc];
		for(int i = 0 ; i < maxc;i++)
			dl[i]=0;
	}
	~DelayLine()
	{
		delete dl;
	}
	inline void feedDelay(float sm)
	{
		dl[iidx] = sm;
		iidx++;
		if(iidx >= (maxc))
			iidx-=(maxc);
	}
	inline float getDelayedSample()
	{
		int idx;
		idx = iidx-(maxc);
		if(idx <0)
			idx+=maxc;
		return dl[idx];
	}
};
class DelayLineBoolean
{
private:
	bool* dl;
	int iidx;
	int maxc;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayLineBoolean)
public:
	DelayLineBoolean() 
	{
		dl  = NULL;
		iidx = 0;
		maxc=0;
	}
	DelayLineBoolean(int count)
	{
		iidx=0;
		maxc=count;
		dl = new bool[maxc];
		for(int i = 0 ; i < maxc;i++)
			dl[i]=0;
	}
	~DelayLineBoolean()
	{
		delete dl;
	}
	inline void feedDelay(bool sm)
	{
		dl[iidx] = sm;
		iidx++;
		if(iidx >= (maxc))
			iidx-=(maxc);
	}
	inline bool getDelayedSample()
	{
		int idx;
		idx = iidx-(maxc);
		if(idx <0)
			idx+=maxc;
		return dl[idx];
	}
};