#pragma once

#include <vector>
using namespace std;
#include "utils.h"


enum skillType
{
	ATTACK, DEFENSE //etc...
};

enum effectType
{
	//* SKILLS DE DEFESA*
	MAGIC_BARRIER,			// invulnerable of enemy's magics
	INVINCIBLE,				// invulnerable to any damage and magic	
	HEAL,					// increase caster HP
	ANTIDOTE,				// heals from current effects
	DEFENSE_UP,				// increase caster defense
	
	//* SKILLS DE ATAQUE*
	DAMAGE,					// decrease enemy's HP
	DEFENSE_DOWN,			// decrease enemy's defense
	POISON,					// enemy takes damage per time
	STAMINA_DOWN,			// decrease enemy's stamina
	FEEDBACK,				// decrease enemy's stamina and decrease this amount of his HP
	MIRROR,					// casts the last casted speel by the enemy on him	
	TIME_WARP,				// changes actual music position of stones spawning on enemy's track
	
	//* SKILLS SACANINHAS*
	INCREASE_DIFFICULTY,	// increases enemy's difficulty
	DOUBLE_NOTES,			// doubles the notes appearing on enemy's track
	TOLERANCE_DOWN,			// decreases enemy's tolerance
	REMOVE_CHORD,			// removes a chord from the enemy, making him misses all those notes until it comes back again.
	REMOVE_CHORD_COMBO,		// if casted right after REMOVE_CHORD, will knock down all enemy's chords	
	
	//* SKILLS QUE SÓ FUNCIONARIAM COM IA OU COM MULTIPLAYER EM REDE*
	SHRINK,					// reduces enemy's track size
	SLOW_DOWN,				// reduces caster track speed
	SPEED_UP,				// increases enemy's track speed
	
	
	EFFECTS_TOTAL
};

class Skill
{
	public:
		Skill();
		~Skill();
		
		vector<buttonType>	keysSequence;
		string				name;
		double				cooldown;
		
		skillType			type;
		effectType			effect;
	
		string				getType();
		
	private:
		

};
