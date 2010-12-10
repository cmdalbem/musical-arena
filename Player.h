// Player.h
//
#pragma once

#include "Fretting.h"
#include "Track.h"
#include "Instrument.h"
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
		bool			usingSkill;

		int					HP;
		int					maxHP;
		int					stamina;
		int					maxStamina;
		int					armor;
				
		int					XP;
		int					level;
		int					gold;
		statusType			status;
		Instrument		*instrument;
		vector<Skill>		skills;
		
		void 			setInstrumentAttributes();
		void			update();
		void			addSkill( Skill s );
		void			takeDamage( double damage );
		void 			updateStatus();
		
	private:
		timeval			lastTimeUpdatedStatus;
		int				counterPoison;
		int				timeInStatus;
		bool			magicBarrier;
		bool			mirror;
		
		void 			initialize();
		void 			staminaRecover();
		void 			staminaDecrease(int howMuch);
};

