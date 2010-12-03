#include "Skill.h"

Skill::Skill()
{
	name = "Undefined";
}

Skill::Skill( string _name, double _cooldown, double _cost, skillType _type, effectType _effect)
{
	name = _name;
	cooldown = _cooldown;
	cost = _cost;
	type = _type;
	effect = _effect;
}

Skill::~Skill()
{
 
}

string	Skill::getType()
{
	switch(type)
	{
		case ATTACK:
				return "Attack";
				break;
		case DEFENSE:
				return "Defense";
				break;
	}
	
	return "Unknown";
}
