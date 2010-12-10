#pragma once

#include <vector>
#include <string>
using namespace std;
#include "utils.h"


class Skill
{
	public:
		Skill();
		Skill( std::string name, skillTargetType targetType, soundEffectType soundEffect, double cooldown, double cost, elementType element, effectStruct effects[], int neffects, visualEffectFunction effectFunction, buttonType keys[], int nkeys, std::string skillDescription );
		~Skill();
		
		vector<buttonType>		keysSequence;
		vector<effectStruct>	effects;
		std::string				name;
		double					cooldown;
		double 					cost;
		
		soundEffectType			soundEffect;
		elementType				element;
		skillTargetType			targetType;
		visualEffectFunction 	effectFunction;
		std::string					description;
		
	private:
		

};
