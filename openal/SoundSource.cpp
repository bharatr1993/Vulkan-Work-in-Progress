#include "SoundSource.h"
#include <iostream>

#pragma comment(lib,"OpenAL32.lib")

SoundSourcebckg::SoundSourcebckg()
{
	alGenSources(1, &p_Source);
	alSourcef(p_Source, AL_PITCH, p_Pitch);
	alSourcef(p_Source, AL_GAIN, p_Gain);
	alSource3f(p_Source, AL_POSITION, p_Position[0], p_Position[1], p_Position[2]);
	alSource3f(p_Source, AL_VELOCITY, p_Velocity[0], p_Velocity[1], p_Velocity[2]);
	alSourcei(p_Source, AL_LOOPING, p_LoopSoundbckg);
	alSourcei(p_Source, AL_BUFFER, p_Buffer);
}

SoundSourcebckg::~SoundSourcebckg()
{
	alDeleteSources(1, &p_Source);
}

void SoundSourcebckg::Play(const ALuint buffer_to_play)
{
	if (buffer_to_play != p_Buffer)
	{
		p_Buffer = buffer_to_play;
		alSourcei(p_Source, AL_BUFFER, (ALint)p_Buffer);
	}

	alSourcePlay(p_Source);


	static ALint state = AL_PLAYING;
	std::cout << "playing sound\n";
	while (state == AL_PLAYING && alGetError() == AL_NO_ERROR)
	{
		//std::cout << "currently playing sound\n";
		alGetSourcei(p_Source, AL_SOURCE_STATE, &state);

		if (stopsound)
		{
			state = AL_STOPPED;
		}
	}
	//std::cout << "done playing sound\n";

}