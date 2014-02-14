#pragma once
#include "SynthEngine.h"
class ApInterpolator
{
	private:
	const float Nu;
	float zd;
	float li;
public :
	ApInterpolator() : Nu(( 1 - 0.5) /(1 + 0.5))
		{
			zd = 0;
			li=0;
	};
	inline float getInterp(float in)
	{
		float out = Nu * ( in - zd) + li;
		zd = out;
		li = in;
		return out;
	}
};