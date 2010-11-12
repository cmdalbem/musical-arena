#include "Skill.h"

Skill::Skill()
{

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
