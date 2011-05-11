// Instrument.cpp
//

#include "Instrument.h"

using std::string;


Instrument::Instrument()
{
	init();
}

Instrument::Instrument( string _name )
{
	name = _name;
	
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

void Instrument::printSkills()
{	
	cout << "\n\n\n";
	cout << "-----------------------------------" << endl;
	cout << name << "'s Skills:" << endl;
	
	for(unsigned int i=0; i<skills.size(); i++)
		skills[i].print();	
}
