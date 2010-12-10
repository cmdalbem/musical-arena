// Instrument.h
//
// Each instrument has its own skills. It's the most important characteristic of an instrument.
// Besides, each instument has some effects on the player, like increase the staminaRecoverSpeed,
// prevent some damage, cause an diferent status, etc...
//

#ifndef _INSTRUMENT_H_
#define _INSTRUMENT_H_

#include "types.h"
#include "Skill.h"
#include <vector>

using namespace std;

class Instrument
{
	public:
		Instrument();
		Instrument( vector< Skill > newSkills, statusType newStatus);
		void init();
		
		int			sumHP;
		int			sumStamina;
		int			armor;

		Skill* getSkill (int index);
		statusType getStatus();
		
		void setStatus (statusType newStatus);
		void addSkill (Skill newSkill);
//		void removeSkill (...)  // pensar melhor
		
		
	private:
		vector< Skill > 	skills;
		statusType			status;
		// We can put some other variables indicating several interesting effects on the Instrument
};

#endif _INSTRUMENT_H_
