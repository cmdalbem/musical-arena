#include "Skill.h"

Skill::Skill()
{
	name = "Undefined";
}

Skill::Skill( string _name, double _cooldown, double _cost, elementType _element, effectStruct _effects[], int neffects, visualEffectFunction _effectFunction, buttonType _keys[], int nkeys )
{
	name = _name;
	cooldown = _cooldown; 
	cost = _cost;
	element = _element;
	
	effectFunction = _effectFunction;
	for(int i=0; i<nkeys; i++)
		keysSequence.push_back(_keys[i]);
	for(int i=0; i<neffects; i++)
		effects.push_back(_effects[i]);
}

Skill::~Skill()
{
 
}
