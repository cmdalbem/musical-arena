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
		Player( ITimer *_timer, double *_musicTime, EventReceiver *_receiver, Track *_track );
		~Player();
		void reset();
				
		Track			*track;
		Instrument		*instrument;
		Fretting		fretting;
		
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
		bool			useAI;
		
		int 			getArmor();
		void 			changeStamina(int howMuch);
		void			changeArmor( double howMuch );
		void			update();
		void			updateEvents();
		void			recoverHP ( double howMuch );
		double			takeDamage( double damage  );
		void 			updateStatus();
		void			setStatusNormal();
		
		void 			addStatus(statusType newStatus);
		void 			removeStatus(statusType oldStatus, int indexInVector);
		void 			setStatusAttNormal();
		void			setInstrument( Instrument *instrument );
		
	private:
		ITimer 			*timer;
		double			lastTimeUpdatedStatus;
		int				lastSpeed;
		int				poisonCounter;
		int				chaoticCounter;
		
		int				armor;
				
		bool			isChaotic;
		bool			hasMagicBarrier;
		bool			hasMirror;
		
		void 			staminaRecover();
};

