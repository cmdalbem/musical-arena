#pragma once

#include <vector>
using namespace std;
#include "utils.h"


class Skill
{
	public:
		Skill();
		Skill( std::string name, double cooldown, double cost, elementType element, effectStruct effects[], int neffects, visualEffectFunction effectFunction, buttonType keys[], int nkeys );
		~Skill();
		
		vector<buttonType>		keysSequence;
		vector<effectStruct>	effects;
		std::string				name;
		double					cooldown;
		double 					cost;
		
		elementType			element;
		visualEffectFunction effectFunction;
		
	private:
		

};
