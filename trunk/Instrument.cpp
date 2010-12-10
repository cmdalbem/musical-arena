// Instrument.cpp
//

#include "Instrument.h"

Instrument::Instrument()
{
	init();
}

Instrument::Instrument( vector< Skill > newSkills, statusType newStatus)
{
	skills = newSkills;	
	status = newStatus;
	
	init();
}

void Instrument::init ()
{
	sumHP = 100;
	sumStamina = 150;
	armor = 50;
}

Skill* Instrument::getSkill (int index)
{
	Skill* returnValue;
	if (index < 0)
		return NULL;
	
	if (skills.size() < (index - 1))
		return NULL;
	else
		return &(skills[index]);
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

