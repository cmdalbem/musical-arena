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
		bool			isFrozen;
		
		int				multiplier;
		int				streak;
				
		int				XP;
		int				level;
		int				gold;
		vector<statusType>		status;
		//double			timeInStatus;
		bool			activateAI;

		int 			getArmor();
		void 			changeStamina(int howMuch);
		void			changeArmor( double howMuch );
		void 			initializeAtributes();
		void			update();
		void			updateEvents();
		void			recoverHP ( double howMuch );
		double			takeDamage( double damage  );
		void 			updateStatus();
		void			setStatusNormal();
		
		void 			addStatus(statusType newStatus);
		void 			removeStatus(statusType oldStatus, int indexInVector);
		void 			setStatusAttNormal();
		
	private:
		int				armor;
		
				
		timeval			lastTimeUpdatedStatus;
		int				lastSpeed;
		int				poisonCounter;
		int				chaoticCounter;
		bool			isChaotic;
		bool			hasMagicBarrier;
		bool			hasMirror;
		
		void 			initialize();
		void 			staminaRecover();
};

