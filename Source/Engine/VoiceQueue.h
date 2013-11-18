#pragma once
#include "Engine\ObxdVoice.h"
class VoiceQueue
{
private:

	ObxdVoice** voices;
	int idx,total;
public:
	VoiceQueue()
	{
		voices  = NULL;
		idx = 0;
		total = 0;
	}
	VoiceQueue(int voiceCount,ObxdVoice** voicesReference)
	{
		voices = voicesReference;
		idx = 0;total = voiceCount;
	}
	ObxdVoice* GetNext()
	{
		idx = idx + 1;
		idx %=total;
		return voices[idx];
	}
	void ReInit(int voiceCount)
	{
		total = voiceCount;
		idx = idx%total;
	}
};