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
		
		Fretting*	getFretting();
		void		verify_buttons();
		
		void inicialize();
		
		// Instrument	instrument;
		Fretting	*fretting;
		Track		*track;
		
		int			HP;
		int			maxHP;
		int			stamina;
		int			maxStamina;
		
		int			XP;
		int			level;
		// vector<Skill>	skills; // each skill has its own cooldown
		
		int			gold;
};
