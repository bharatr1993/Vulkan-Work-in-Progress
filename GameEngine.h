#pragma once
#ifndef STRUCTURES_H
#define STRUCTURES_H
#include "Structures.h"
#include <vector>
#include "openal/SoundDevice.h"
#include "openal/SoundBuffer.h"
#include "openal/SoundSource.h"
#endif // !STRUCTURES_H

#define PI  3.14159265f

bool jumpinit = false;

class GameEngine {

public:
	std::vector<Object> objvectopleft, objvecbottomleft, objvectopright, objvecbottomright;
	std::vector<BulletObj> bulletpos, bulletposflip;
	Object object,hero;
	BulletObj bobj;
	int objcounttl = 1;
	int objcountbl = 1;
	int objcounttr = 1;
	int objcountbr = 1;
	int directionofhero = 0;//Right facing
	double gravity = 0.1;
	double wormspeed = 0.002;

	void ObjSpawn(int click, float x, float y, double fElapsedTime, std::string KeyPress);
	float UpDowny(bool up, float xpos);
	float WUpDowny(bool up, float xpos);//1 for top and 0 bottom
	int FireCountforPos(float xpos, bool up);
	static void Sounds_BCKG(std::string adname);

};
