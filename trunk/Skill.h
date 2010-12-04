#pragma once

#include <vector>
using namespace std;
#include "utils.h"


class Skill
{
	public:
		Skill();
		Skill( std::string name, double cooldown, double cost, skillType type, effectType effects[], int neffects, visualEffectFunction effectFunction, buttonType keys[], int nkeys );
		~Skill();
		
		vector<buttonType>	keysSequence;
		vector<effectType>	effects;
		std::string			name;
		double				cooldown;
		double 				cost;
		
		skillType			type;
		visualEffectFunction effectFunction;
	
		std::string			getType();
		
	private:
		

};
