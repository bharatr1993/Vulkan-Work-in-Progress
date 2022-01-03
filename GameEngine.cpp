#ifndef GAMEENGINE_H
#define GAMEENGINE_H
#include "GameEngine.h"

#endif // !GAMEENGINE_H

//To do:
/*
Space for firing
Jump is parabolic
Jump is slow/parabolic: Kindo of fixed but might want to exaggerate parabolism
Jump duration// wrt button press
*/

void GameEngine::Sounds_BCKG(std::string adname)
{
	SoundDevice* mysounddevice = SoundDevice::get();

	uint32_t /*ALuint*/ sound1 = SoundBuffer::get()->addSoundEffect(adname.c_str());

	SoundSourcebckg mySpeaker;

	mySpeaker.Play(sound1);
}

int GameEngine::FireCountforPos(float xpos, bool up)
{
	if (up)
	{
		if (xpos < -1.8 || xpos > 1.8)
			return 1;
		else if (xpos > -1.8 && xpos < -0.6)
			return 3;
		else if (xpos > 0.6 && xpos < 2.1)
			return 4;
		else if(xpos > -0.6 && xpos < 0.6)
			return 1;
	}
	else if (!up)
	{
		if (xpos < -1.8 || xpos > 1.8)
			return 2;
		else if (xpos > -1.8 && xpos < -1.3)
			return 5;
		else if (xpos > 1.3 & xpos < 1.8)
			return 6;
		else if (xpos > -1.8 && xpos < 1.8)
			return 1;
	}

	return -1;
}


float GameEngine::UpDowny(bool up, float xpos)
{
	float ypos = 0;
	if (up)
	{
		if (xpos < -1.8 || xpos > 1.8)
			ypos = 0.6;//UpStairs Height:0.7, Downstairs height:1.65
		else if (xpos < -0.6)
			ypos = ((1.275 - 0.6) / (2.1 - 0.6)) * xpos + 1.45;//UpStairs Gradient towards left
		else if (xpos > 0.6 & xpos < 2.1)
			ypos = -((1.275 - 0.6) / (2.1 - 0.6)) * xpos + 1.45;//UpStairs Gradient towards right
		else
			ypos = 1.175;
	}
	else
	{
		if (xpos < -1.8 || xpos > 1.8)
			ypos = 1.45;
		else if (xpos < -1.3)
			ypos = -((1.275 - 0.6) / (2.1 - 0.6)) * xpos + 0.6;
		else if (xpos > 1.3 && xpos < 1.8)
			ypos = ((1.275 - 0.6) / (2.1 - 0.6)) * xpos + 0.6;
		else
			ypos = 1.175;
	}

	return ypos;
}

float GameEngine::WUpDowny(bool up, float xpos)
{
	float ypos = 0;
	if (up)
	{
		if (xpos < -10.5) //Top stairs
			ypos = (75. / 300) * 12.;
		else if (xpos < -3.)
			ypos = (((75. / 300) * 11.5) / (10. - 3.5)) * xpos + (170. / 300) * 13.;
		else if (xpos > -3. && xpos < 7.)
			ypos = (145. / 300) * 13.;
		else if (xpos > 7. && xpos < 10.)
			ypos = (((75. / 300) * 11.5) / (10. - 3.5)) * xpos + (70. / 300) * 13.;
		else
			ypos = (((75. / 300) * 11.5) / (10. - 3.5)) * 10. + (70. / 300) * 13.;
	}
	else
	{
		if (xpos < -10.) //Top stairs
			ypos = (75. / 300) * 30;
		else if (xpos < -7.)
			ypos = -(((75. / 300) * 11.5) / (10. - 3.5)) * xpos + (75. / 300) * 13;
		else if (xpos > -7. && xpos < 2.)
			ypos = (145. / 300) * 13;
		else if (xpos > 2. && xpos < 10.)
			ypos = -(((75. / 300) * 11.5) / (10. - 3.5)) * xpos + (200. / 300) * 11.;
		else
			ypos = -(((75. / 300) * 11.5) / (10. - 3.5)) * 10. + (200. / 300) * 11.;
	}

	return ypos;
}

void GameEngine::ObjSpawn(int click,float x,float y, double fElapsedTime, std::string KeyPress)
{
	static bool clickonce = true, clickonceright=true, init=true, up=false, jump=false, fall=false, fire=false, fireinit=false;
	static double tcounter = 0;
	static double yexact;

	static int dir = 0;

	if (init)
	{
		hero.x = 0;
		hero.y = (75. / 300) * 5.5;//Center Point Start
		init = false;
	}

	if (KeyPress == "A" && !jump)
	{
		if (hero.x > -2.7)
			hero.x -= 0.002 * fElapsedTime;
		directionofhero = 1;
		dir = 1;
	}
	else if (KeyPress == "D" && !jump)
	{
		if (hero.x < 2.7)
			hero.x += 0.002 * fElapsedTime;
		directionofhero = 0;
		dir = 0;
	}
	else if (KeyPress == "W")
	{
		jump = true;
		yexact = GameEngine::UpDowny(up, hero.x);
	}
	else if (KeyPress == "S" && !jump && up)
	{
		fall = true;
		yexact = GameEngine::UpDowny(false, hero.x);
	}
	else if (KeyPress == "sp" && !jump)
	{
		fire = true;
		//yexact = GameEngine::UpDowny(false, hero.x);
	}
	else if (KeyPress == "")
	{
		fire = false;
	}
	//---------------------Hero Controls
	if (!jump && !fall)
	{
		static double tc = 0, tf = 0;
		tc += fElapsedTime;

		if (tf < 0.5)
			tf += fElapsedTime / 1000.;

		if (KeyPress == "")
		{
			if (tc > 0.1)
			{
				dir = 2;
				tc = 0;
			}
		}

		hero.y = GameEngine::UpDowny(up,hero.x);

		if (fire)
		{
			if (tf >= 0.5)
			{
				tf = 0;
				fireinit = true;
			}
		}

		if (fireinit)
		{
			if (directionofhero == 0)
			{
				bobj.poscount = GameEngine::FireCountforPos(hero.x, up);
				bobj.x = (hero.x / 7) + 3.5;
				bobj.y = (hero.y / 7) - 3.5;

				if (bobj.poscount == 1 || bobj.poscount == 2)
					bobj.angle = 0;
				else if (bobj.poscount == 3)
				{
					bobj.angle = atan(((1.275 - 0.6) / (2.1 - 0.6)) + 1.45) - PI / 2.0;
					//bobj.x = (hero.x / 2.88) * (7.2) + 1.;
					//bobj.y = (hero.y / 2.88) * (7.3) - 2.2 * (hero.x / 2.88);
				}
				else if (bobj.poscount == 4)
				{
					bobj.angle = atan(((1.275 - 0.6) / (2.1 - 0.6)) + 1.45) - PI / 5.;
					//bobj.x = (hero.x / 2.88) * (7.2) - 1.5;
					//bobj.y = (hero.y / 2.88) * (7.3) + 2. * (hero.x / 2.88);
				}
				else if (bobj.poscount == 5)
				{
					bobj.angle = PI/4.;
					//bobj.x = (hero.x / 2.88) * (7.2)-4.;
					//bobj.y = (hero.y / 2.88) * (7.3)-4.;
				}
				bulletpos.push_back(bobj);
				printf("Pushed Back\n");
			}
			fireinit = false;
		}
	}

	if (jump)
	{
		static double powcounter = 0.4;
		if (KeyPress != "" && !jumpinit)
		{
			
			if(powcounter < 1.)
				powcounter += fElapsedTime/1000;
		}
		else
		{
			jumpinit = true;
			tcounter += fElapsedTime / 1000.;
			double td = 0, te = 0, acceleration = 1;
			static double vinit, time, nframes;
			vinit = 0.035 * powcounter;
			if ((hero.y - 0.01) < yexact || nframes > 0.)
			{
				nframes = ((vinit * tcounter) - (0.5 * gravity * tcounter * tcounter));
				hero.y -= nframes * fElapsedTime;
				if (dir == 0 && hero.x < 2.7)
					hero.x += 0.25 * fElapsedTime / 100.;
				else if (dir == 1 && hero.x > -2.7)
					hero.x -= 0.25 * fElapsedTime / 100.;
				if(powcounter>0.6)
					yexact = UpDowny(true, hero.x);
				else
					yexact = UpDowny(false, hero.x);
			}
			else
			{
				jump = false;
				jumpinit = false;
				if (powcounter > 0.6)
				{
					yexact = UpDowny(true, hero.x);
					up = true;
				}
				else
				{
					yexact = UpDowny(false, hero.x);
					up = false;
				}
				tcounter = 0;
				powcounter = 0.4;
			}
		}
	}

	if (fall)
	{
		if (hero.y < yexact)
		{
			tcounter += fElapsedTime/1000.;
			double td = 0, te = 0, acceleration = 1;
			static double time, nframes;
			nframes= -(0.5 * gravity * tcounter * tcounter);

			hero.y -= nframes * fElapsedTime;
			if (dir == 0 && hero.x < 2.7)
				hero.x += 0.25 * fElapsedTime/100.;
			else if (dir == 1 && hero.x > -2.7)
				hero.x -= 0.25 * fElapsedTime/100.;
			yexact = UpDowny(false, hero.x);
		}
		else
		{
			fall = false;
			hero.y = UpDowny(false, hero.x);
			up = false;
			tcounter = 0;
		}
	}

	//-----------------------------------

	if (!clickonce & click == 0)
		clickonce = true;

	if (!clickonceright & click == 0)
		clickonceright = true;

	if (click == 4 && clickonce && objcounttl < 100 && objcounttr < 100 && objcountbl < 100 && objcountbr < 100)
	{
		clickonce = false;

		int tmp = rand() % 4;

		//int tmp=0;

		if (tmp == 0)
		{

			object.x = -17.;
			object.y = (75. / 300) * 14.5;
			object.angle = 0;
			objvectopleft.push_back(object);
			objcounttl++;
		}
		else if (tmp == 1)
		{
			object.x = -17.;
			object.y = (75. / 300) * 34;
			object.angle = 0;
			objvecbottomleft.push_back(object);
			objcountbl++;
		}
		else if (tmp == 2)
		{
			object.x = 17.;
			object.y = (75. / 300) * 14.5;
			object.angle = 0;
			objvectopright.push_back(object);
			objcounttr++;
		}
		else
		{
			object.x = 17.;
			object.y = (75. / 300) * 34;
			object.angle = 0;
			objvecbottomright.push_back(object);
			objcountbr++;
		}
		printf("%f", object.x);
	}

	int itrctr = 0;

	
	for (auto& itr : objvectopleft)
	{
		itr.x += wormspeed * fElapsedTime;
		//printf("%f", object.x);
		//object.x = 15;	//15: Right Screen Coordinate 3.47: Bottom image for fit to top stairs, image offset : 0.46, 75: Top stairs, 180:Bottom stairs
		
		itr.y = WUpDowny(true, itr.x);


		if (itr.x > 17.)
		{
			objcounttl--;
			objvectopleft.erase(objvectopleft.begin()+itrctr);
			itrctr--;
			printf("Popped");
		}
		itrctr++;
	}

	itrctr = 0;
	
	for (auto& itr : objvecbottomleft)
	{
		itr.x += wormspeed * fElapsedTime;
		//printf("%f", object.x);
		//object.x = 15;	//15: Right Screen Coordinate 3.47: Bottom image for fit to top stairs, image offset : 0.46, 75: Top stairs, 180:Bottom stairs
		
		itr.y = WUpDowny(false, itr.x);

		if (itr.x > 17.)
		{
			objcountbl--;
			objvecbottomleft.erase(objvecbottomleft.begin() + itrctr);
			itrctr--;
			printf("Popped");
		}
		itrctr++;
	}

	itrctr = 0;

	for (auto& itr : objvectopright)
	{
		itr.x = -itr.x;
		itr.x += wormspeed * fElapsedTime;
		//printf("%f", object.x);
		//object.x = 15;	//15: Right Screen Coordinate 3.47: Bottom image for fit to top stairs, image offset : 0.46, 75: Top stairs, 180:Bottom stairs
		itr.y = WUpDowny(true, itr.x);

		itr.x = -itr.x;

		if (itr.x < -17.)
		{
			objcounttr--;
			objvectopright.erase(objvectopright.begin() + itrctr);
			itrctr--;
			printf("Popped");
		}
		itrctr++;
	}

	itrctr = 0;

	for (auto& itr : objvecbottomright)
	{
		itr.x = -itr.x;
		itr.x += wormspeed * fElapsedTime;
		//printf("%f", object.x);
		//object.x = 15;	//15: Right Screen Coordinate 3.47: Bottom image for fit to top stairs, image offset : 0.46, 75: Top stairs, 180:Bottom stairs
		itr.y = WUpDowny(false, itr.x);
		itr.x = -itr.x;

		if (itr.x < -17.)
		{
			objcountbr--;
			objvecbottomright.erase(objvecbottomright.begin() + itrctr);
			itrctr--;
			printf("Popped");
		}
		itrctr++;
	}

	itrctr = 0;

	for (auto& bitr : bulletpos)
	{
	
		bitr.x += 0.8 * fElapsedTime / 100.;
		if (bitr.x > 8.)
		{
			bulletpos.erase(bulletpos.begin() + itrctr);
			printf("Popped Bullet");
		}

		itrctr++;
	}

	if (click == 1 && clickonceright)
	{
		clickonceright = false;
		//objcount--;

		//objvec.pop_back();
		printf("%f\n",object.x);
	}
}