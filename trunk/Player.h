// Player.h
//
#pragma once

#include "Fretting.h"
#include "Track.h"

class Player
{
	public:
		Player();
		~Player();
		
		Fretting*		getFretting();
		void			update();
		
		void 			initialize();
		
		//Instrument		instrument;
		Fretting		*fretting;
		Track			*track;
		
		int				HP;
		int				maxHP;
		int				stamina;
		int				maxStamina;
		
		int				XP;
		int				level;
		vector<Skill>	skills;
		
		int			gold;
};
