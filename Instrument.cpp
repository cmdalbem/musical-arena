// Instrument.cpp
//

#include "Instrument.h"

Instrument::Instrument()
{
	init();
}

Instrument::Instrument( statusType newStatus)
{
	status = newStatus;
	
	init();
}

void Instrument::init ()
{
	sumHP = DEFAULT_HP;
	sumStamina = DEFAULT_STAMINA;
	armor = DEFAULT_ARMOR;
	tolerance = DEFAULT_TOLERANCE;
}

statusType Instrument::getStatus()
{	return status;	}

void Instrument::setStatus(statusType newStatus)
{	status = newStatus;	}

void Instrument::addSkill(Skill newSkill)
{
	// checks if there is not an equivalent skill on the skills vector
	// ...
	
	// adds the skill
	// if there is not an equivalent skill
	skills.push_back(newSkill);
}
