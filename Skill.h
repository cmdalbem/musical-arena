#pragma once

#include <vector>
using namespace std;
#include "utils.h"


enum skillType
{
	ATTACK, DEFENSE //etc...
};

class Skill
{
	public:
		Skill();
		~Skill();
		
		vector<buttonType>	keysSequence;
		string				name;
		skillType			type;
		double				cooldown;
	
		string				getType();
		
	private:
		

};
