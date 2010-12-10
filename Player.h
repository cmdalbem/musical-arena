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
		
		Fretting		*fretting;
		Track			*track;
		int				gotAnEvent;
		bool			usingSkill;
		int				damageTaken;

		int				HP;
		int				maxHP;
		int				stamina;
		int				maxStamina;
		int				armor;
				
		int				XP;
		int				level;
		int				gold;
		statusType		status;
		double			timeInStatus;
		bool			activateAI;

//		statusType		status1;
		
		Instrument*		instrument;
		
		Skill*			castedSpell;
		
		void 			staminaDecrease(int howMuch);
		void 			initializeAtributes();
		void			update();
		void			HPRecover ( double howMuch );
		void			takeDamage( double damage  );
		void 			updateStatus();
		void			setStatusNormal();
		
	private:
		timeval			lastTimeUpdatedStatus;
		int				counterPoison;
//		double			timeInStatus1;
		bool			magicBarrier;
		bool			mirror;
		
		void 			initialize();
		void 			staminaRecover();
};

