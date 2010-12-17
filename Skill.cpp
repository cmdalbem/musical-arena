#include "Skill.h"

Skill::Skill()
{
	name = "Undefined";
}

Skill::Skill( string _name, skillTargetType _targetType, soundEffectType _soundEffect, double _cost, elementType _element, effectStruct _effects[], int neffects, visualEffectFunction _effectFunction, buttonType _keys[], int nkeys, int nSlots, std::string skillDescription )
{
	name = _name;
	targetType = _targetType;
	soundEffect = _soundEffect;
	cost = _cost;
	element = _element;
	
	effectFunction = _effectFunction;
	for(int i=0; i<nkeys; i++)
		keysSequence.push_back(_keys[i]);
	for(int i=0; i<neffects; i++)
		effects.push_back(_effects[i]);
	
	description = skillDescription;
	skillSlots = nSlots;
}

Skill::~Skill()
{
 
}
