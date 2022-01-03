#pragma once
#include "../openal-inc/include/AL/al.h"

bool stopsound = false;

class SoundSourcebckg
{
public:
	 SoundSourcebckg();
	~SoundSourcebckg();

	void Play(const ALuint buffer_to_play);

private:
	ALuint p_Source;
	float p_Pitch = 1.f;
	float p_Gain = 1.f;
	float p_Position[3] = { 0,0,0 };
	float p_Velocity[3] = { 0,0,0 };
	bool p_LoopSoundbckg = true;
	ALuint p_Buffer = 0;
};

