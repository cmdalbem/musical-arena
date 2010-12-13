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
		Instrument*		instrument;
		
		Skill*			castedSpell;
		int				gotAnEvent;
		bool			isUsingSkill;
		int				damageTaken;

		int				HP;
		int				maxHP;
		int				stamina;
		int				maxStamina;
		
		int				multiplier;
		int				XP;
		int				level;
		int				gold;
		statusType		status;
		double			timeInStatus;
		bool			activateAI;

		int 			getArmor();
		void 			changeStamina(int howMuch);
		void			decreaseArmor( double howMuch );
		void 			initializeAtributes();
		void			update();
		void			recoverHP ( double howMuch );
		void			takeDamage( double damage  );
		void 			updateStatus();
		void			setStatusNormal();
		
	private:
		int				armor;
		
				
		timeval			lastTimeUpdatedStatus;
		int				lastSpeed;
		int				poisonCounter;
		int				chaoticCounter;
		bool			isChaotic;
//		double			timeInStatus1;
		bool			hasMagicBarrier;
		bool			hasMirror;
		
		void 			initialize();
		void 			staminaRecover();
};

