// Instrument.h
//
// Each instrument has its own skills. It's the most important characteristic of an instrument.
// Besides, each instument has some effects on the player, like increase the staminaRecoverSpeed,
// prevent some damage, cause an diferent status, etc...
//
#pragma once

#include "types.h"
#include "Skill.h"
#include <vector>
#include <string>

using namespace std;

class Instrument
{
	public:
		Instrument();
		Instrument( std::string name );
		void init();
		
		std::string		name;
		int				sumHP;
		int				sumStamina;
		int				armor;
		double			tolerance;
		vector<Skill>	skills;

		statusType 	getStatus();
		
		void 		setStatus (statusType newStatus);
		void 		addSkill (Skill newSkill);
//		void 		removeSkill (...)  // pensar melhor
		void		printSkills();
		
	private:
		statusType			status;
		// We can put some other variables indicating several interesting effects on the Instrument
};
