// Instrument.cpp
//

#include "Instrument.h"

Instrument::Instrument()
{
	// nothing to be done, actually, for now D=
}

Instrument::Instrument( vector< Skill > newSkills, statusType newStatus)
{
	for (int i = 0; i < newSkills.size(); i++)
		skills.push_back(&newSkills[i]);
	
	status = newStatus;
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
{	status = newStatus	}

void Instrument::addSkill(Skill newSkill)
{
	// checks if there is not an equivalent skill on the skills vector
	// ...
	
	// adds the skill
	// if there is not an equivalent skill
	skills.push_back(&newSkill);
}

