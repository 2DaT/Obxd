#pragma once
#include "SynthEngine.h"
const float PSSC = 0.0030;
class ParamSmoother
{
	private:
		float steepValue;
		float integralValue;
		float srCor;
	public :
	ParamSmoother()
	{
		steepValue=integralValue=0;
		srCor=1;
	};
	float smoothStep()
	{
		integralValue = integralValue + ( steepValue - integralValue)*PSSC*srCor + dc;
		return integralValue;
	}
	void setSteep(float value)
	{
		steepValue = value;
	}
	void setSampleRate(float sr)
	{
		srCor = sr / 44000;
	}

};