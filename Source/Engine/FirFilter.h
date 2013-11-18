#pragma once
#include  "SynthEngine.h"

class UpsampleFIRFilter
{
	float h1,h3,h5;
	float R1,R2,R3,R4,R5;
	public:
	UpsampleFIRFilter()
	{
		R1=R2=R3=R4=R5=0;
		h1 = (208/692.0f);
		h3 = (-22/692.0f);
		h5 = (9/692.0f);
	}
	float processUpsamplingFiltreing(float x)
	{
		float h1x = h1 * x;
		float h3x = h3 * x;
		float h5x = h5 * x;
		float R6 = R5 +h5x;
		R5 = R4 +h3x;
		R4 = R3 +h1x;
		R3= R2 + h1x;
		R2 = R1 + h3x;
		R1 = h5x;
		return R6;
	}
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UpsampleFIRFilter);
};