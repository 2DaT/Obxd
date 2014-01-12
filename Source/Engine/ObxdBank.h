#pragma once
#include "ObxdVoice.h"
#include "ParamsEnum.h"
const int PROGRAMCOUNT = 128;
class ObxdBank
{
public:
	ObxdParams programs[PROGRAMCOUNT];
	ObxdParams* currentProgramPtr;
	int currentProgram;
	ObxdBank()
	{
		currentProgram=0;
		currentProgramPtr = programs+currentProgram;
	}
};