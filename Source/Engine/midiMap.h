#pragma once
#include "SynthEngine.h"
class MidiMap
{
public:
	int controllers[255];
	MidiMap()
	{
		for(int i = 0 ; i < 255;i++)
			controllers[i] = 0;
	}
};