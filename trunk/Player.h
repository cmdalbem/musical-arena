// Player.h
//
#pragma once

#include "Fretting.h"
#include "Track.h"
#include <ctime>

class Player
{
	public:
		Player();
		~Player();
		
		//Instrument		instrument;
		Fretting		*fretting;
		Track			*track;
		int				gotAnEvent;
		
		int				usingSkill;
		timeval			staminaLastTimeDecreased;
		timeval			staminaLastTimeRecovered;
		double			staminaDecreaseSpeed;
		double			staminaRecoverSpeed;
		
		int				HP;
		int				maxHP;
		int				stamina;
		int				maxStamina;
		int				XP;
		int				level;
		int				gold;
		statusType		status;
		vector<Skill>	skills;
		
		void			update();
		void			addSkill( Skill s );
		void			takeDamage( double damage );
		void 			updateStamina();
	private:
		void 			initialize();
		void 			staminaRecover();
		void 			staminaDecrease();
};
