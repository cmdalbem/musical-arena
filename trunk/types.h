#pragma once


#include <string>
#include <map>
#include <vector>
using namespace std;

typedef unsigned char u_char;

enum visualEffectFunction
{
	EFFECT_FIREBALL, EFFECT_FIREBALL_SKY, EFFECT_FIRE_RAIN,
	EFFECT_GLOW_AREA, EFFECT_FEEDBACK, EFFECT_SHIELD,
	EFFECT_BOLT, EFFECT_ELETRIC_GROUND, EFFECT_WATER_BEAM,
	EFFECT_DRUNK_EFFECT, EFFECT_DRUNK_EFFECT_SINGLE, EFFECT_EXPLOSION,
	EFFECT_THUNDERSTORM_BOLT, EFFECT_THUNDERSTORM, EFFECT_FLOOD,
	EFFECT_SHIELD_SINGLE, EFFECT_SWAMP, EFFECT_BALL_LIGHTNING,
	EFFECT_IMPLANT_BOMB, EFFECT_SUN, EFFECT_DARK_STORM_BOLT, EFFECT_DARK_STORM,
	EFFECT_FREEZE
};

typedef pair<pair<unsigned int,visualEffectFunction>,int> effectEvent;

enum soundEffectType
{
	S_BOLT1, S_BOLT2, S_LIGHTNING, S_DIGITAL, S_EXPLOSION, S_FIRE1, S_FIRE2,
	S_MAGIC1, S_MAGIC2, S_STORM, S_UNDERWATER1, S_UNDERWATER2, S_STREAM1, 
	S_STREAM2, S_STREAM3, S_WATERFALL, S_FIRERUMBLE, S_HORROR1, S_HORROR2, 
	S_EARTHQUAKE, S_ELECTRIC1, S_ELECTRIC2, S_ICE1, S_ICE2, S_BIG,
	
	S_TOTAL
};
const std::string wavFiles[] = { "bolt1", "bolt2", "lightning", "digital1", 
"explosion1", "fire1", "fire2", "magic1", "magic2", "storm", "underwater", "underwater2", 
"stream1", "stream2","stream3", "waterfall","firerumble","horror1","horror2",
"earthquake", "electric1","electric2","ice1","ice2","big" };
const std::string missFiles[] = { "miss1", "miss2", "miss3", "miss4", "miss5", "miss6"};

enum elementType
{
	FIRE, THUNDER, WATER, ECO, SPIRITUAL, DARK
};

enum skillId
{
	FIREBALL, CURE, EXPLOSION, LIGHTNING_BOLT, FIRE_RAIN, FEEDBACK,
	ELETRIFIED_GROUND, FLOOD, WATER_BEAM, THUNDERSTORM, MIRROR, SWAMP, 
	WORM_HOLE, BALL_LIGHTNING, IMPLANT_BOMB, SUNBIRTH, DIVINE_SILENCE,
	FROZEN_FINGERS,
	
	SKILLS_TOTAL
};

enum skillTargetType
{
	ATTACK, DEFENSE, GLOBAL
};

// Some temporary status the player may have
enum statusType
{
	ST_NORMAL,		// normal status
	ST_POISON,		// takes damage per time
	ST_FIRE,		// damage and defense down per time
	ST_INVENCIBLE,	// infinite armor
	ST_MAGIC_BARRIER, // can't be targeted by spells
	ST_MIRROR,		// will reflect enemy's casted spell
	ST_ELETRIFIED,	// player get shocks and damage per time
	ST_DROWNED,		// stamina will start to decrease, and when it's all gone player will start getting damage per time
					// /\ inspired on Tomb Raider, when Lara Croft is swimming u.u
	ST_DEFENSE_DOWN, // lose armor temporarily
	ST_CHAOTIC_SPEED,	// track speed will change randomically it's speed while the player is in this state
	ST_BROKEN_DEFENSE,  // zero armor
	ST_FROZEN,			// player cannot fret, neither solo
	
	ST_TOTAL	
};

// The effects the skills causes on the player. There's are good and bad effects.
enum effectType
{
	//* SKILLS DE DEFESA*
	T_MAGIC_BARRIER,		// invulnerable of enemy's magics
	T_INVENCIBLE,			// invulnerable to any damage and magic	
	T_HEAL,					// increase caster HP
	T_ANTIDOTE,				// heals from current effects
	T_DEFENSE_UP,			// increase caster defense
	T_CLEAR_STONES,			// clear all stones in the screen
	
	//* SKILLS DE ATAQUE*
	T_DAMAGE,				// decrease enemy's HP
	T_DEFENSE_DOWN,			// decrease enemy's defense
	T_STAMINA_DOWN,			// decrease enemy's stamina
	T_FEEDBACK,				// decrease enemy's stamina and decrease this amount of his HP
	T_MIRROR,				// puts caster on Mirror Status
	T_POISONOUS,			// puts enemy on Poison status
	T_BREAK_DEFENSE,		// puts enemy on Broken Defense Status
	T_BURN,					// puts enemy on Burn Status
	T_DROWN,				// puts enemy on Drowned Status
	T_ELETRIFY,				// puts enemy on Eletrified Status
	T_INCREASE_DIFFICULTY,	// increases enemy's difficulty
	T_DOUBLE_NOTES,			// doubles the notes appearing on enemy's track
	T_TOLERANCE_DOWN,		// decreases enemy's tolerance
	T_REMOVE_CHORD,			// removes a chord from the enemy, making him misses all those notes until it comes back again.
	T_SPEED_UP,				// increases enemy's track speed
	T_CHAOTIC_SPEED,		// put's enemy on Chaotic Speed Status
	T_FREEZE,				// put's enemy on Frozen Status
	
	
	T_EFFECTS_TOTAL
};

typedef struct
{
	effectType	type;
	double		param1;
	double		param2;
} effectStruct;

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

