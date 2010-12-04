#pragma once


#include <string>
#include <map>
#include <vector>
using namespace std;

typedef unsigned char u_char;

enum visualEffectFunction
{
	CREATE_FIREBALL, CREATE_FIREBALL_SKY, CREATE_FIRE_RAIN,
	CREATE_GLOW_AREA, CREATE_FEEDBACK, SHOW_SHIELD,
	CREATE_BOLT, CREATE_ELETRIC_GROUND, CREATE_WATER_BEAM,
	CREATE_DRUNK_EFFECT, CREATE_DRUNK_EFFECT_SINGLE, CREATE_EXPLOSION,
};

typedef pair<pair<unsigned int,visualEffectFunction>,int> effectEvent;

enum skillType
{
	ATTACK, DEFENSE
};

enum skillId
{
	FIREBALL, CURE, EXPLOSION, LIGHTNING_BOLT, FIRE_RAIN, FEEDBACK,ELETRIFIED_GROUND,
	
	SKILLS_TOTAL
};

// Some temporary status the player may have
enum statusType
{
	ST_NORMAL,		// normal status
	ST_POISON,		// takes damage per time
	ST_FIRE,		// damage and defense down per time
	ST_BARRIER,		// infinite armor
	ST_MIRROR,		// will reflect enemy's casted spell
	ST_ELETRIFIED	// player get shocks and damge per time
};

// The effects the skills causes on the player. There's are good and bad effects.
enum effectType
{
	//* SKILLS DE DEFESA*
	T_BARRIER,				// invulnerable of enemy's magics
	T_INVINCIBLE,			// invulnerable to any damage and magic	
	T_HEAL,					// increase caster HP
	T_ANTIDOTE,				// heals from current effects
	T_DEFENSE_UP,			// increase caster defense
	
	//* SKILLS DE ATAQUE*
	T_DAMAGE,				// decrease enemy's HP
	T_DEFENSE_DOWN,			// decrease enemy's defense
	T_POISONOUS,			// puts enemy on Poison status
	T_STAMINA_DOWN,			// decrease enemy's stamina
	T_FEEDBACK,				// decrease enemy's stamina and decrease this amount of his HP
	T_MIRROR,				// casts the last casted speel by the enemy on him	
	T_TIME_WARP,			// changes actual music position of stones spawning on enemy's track
	T_BURN,					// puts the enemy on Burn Status
	
	//* SKILLS SACANINHAS*
	T_SHOCK,				// displaces randomly some of the notes
	T_ELETRIFY,				// puts the enemy on Eletrified Status
	T_INCREASE_DIFFICULTY,	// increases enemy's difficulty
	T_DOUBLE_NOTES,			// doubles the notes appearing on enemy's track
	T_TOLERANCE_DOWN,		// decreases enemy's tolerance
	T_REMOVE_CHORD,			// removes a chord from the enemy, making him misses all those notes until it comes back again.
	T_REMOVE_CHORD_COMBO,	// if casted right after REMOVE_CHORD, will knock down all enemy's chords	
	
	//* SKILLS QUE SÓ FUNCIONARIAM COM IA OU COM MULTIPLAYER EM REDE*
	T_SHRINK,				// reduces enemy's track size
	T_SLOW_DOWN,			// reduces caster track speed
	T_SPEED_UP,				// increases enemy's track speed
	
	
	T_EFFECTS_TOTAL
};

enum difficultyType
{
	EXPERT, HARD, MEDIUM, EASY
};

enum note
{
	STRIKE,
	HOLD,
	NOTHING
};

enum eventType		// Types of MIDI events
{
	ON,
	OFF,
	ERROR
};

enum buttonType		// 5 buttons, five frets. Some idea?
{
	B1, B2, B3, B4, B5, NIL
};

typedef struct musicEvent_t
{
	buttonType button;
	eventType type;
	double time; //	time in seconds
	int mspqn; // microseconds per quarter note
} musicEvent;
