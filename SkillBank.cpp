#include "SkillBank.h"

SkillBank::SkillBank()
{
	makeSkills();
}

SkillBank::~SkillBank()
{
 
}

void SkillBank::makeSkills()
{
	// Skill( string name, double cooldown, double cost, skillType type, effectType effect, visualEffectFunction effectFunction, buttonType keys[], int nkeys );
	
	{
		buttonType k[] = {B1,B1};
		effectStruct e[] = {{T_DAMAGE, 50, -1}, {T_DEFENSE_DOWN, 10, -1}};
		Skill s("Fireball", ATTACK, S_FIRE, 10, 30, FIRE, e, 2, CREATE_FIREBALL, k, 2,
			"Launch a fireball in the enemy. Cheap and fast damage, will also decrease the defenses of the enemy a little each time, which can be a disaster later on!");
		skills[FIREBALL] = s;
	}
	
	{
		buttonType k[] = {B1,B2,B5,B4,B1,B2,B5,B4};
		effectStruct e[] = {{T_HEAL, 50, -1},{T_ANTIDOTE, -1}};
		Skill s("Cure", DEFENSE, S_MAGIC1, 15, 70, WATER, e, 2, CREATE_GLOW_AREA, k, 8, "Cures illness and HP");
		skills[CURE] = s;
	}
	
	{
		buttonType k[] = {B1,B2,B1,B3,B1,B4,B1,B5,B5,B5};
		effectStruct e[] = {{T_DAMAGE, 300, -1},{T_DEFENSE_DOWN, 15, 6},{T_STAMINA_DOWN, 25, -1}};
		Skill s("Explosion", ATTACK, S_EXPLOSION, 30, 190, EARTH, e, 3, CREATE_EXPLOSION, k, 10,
		"A big explosion right in the enemies face! Causes damage, lowers the defense and knocks the enemy into the ground, causing him to lose stamina");
		skills[EXPLOSION] = s;
	}

	{
		buttonType k[] = {B2,B2};
		effectStruct e[] = {{T_DAMAGE, 50},{T_SHOCK, -1, -1}};
		Skill s("Lightning Bolt", ATTACK, S_LIGHTNING, 15, 70, THUNDER, e, 2, CREATE_BOLT, k, 2,
		"Cheap and fast damage which eletrifies the opponents instrument, causing notes to be harder to hit");
		skills[LIGHTNING_BOLT] = s;
	}

	{
		buttonType k[] = {B5,B4,B3,B2,B1,B5,B4,B3,B2,B1,B5,B4,B3,B2,B1};
		effectStruct e[] = {{T_DAMAGE, 170, -1},{T_DEFENSE_DOWN, 40, 5},{T_BURN, 7, -1}};
		Skill s("Fire Rain", ATTACK, S_EXPLOSION, 50, 240, FIRE, e, 3, CREATE_FIRE_RAIN, k, 15,
		"The skies are on fire! Causes damage, lowers defense and put your enemy on fire. Also, all that fire makes the fretting harder to see...");
		skills[FIRE_RAIN] = s;
	}
	
	{
		buttonType k[] = {B1,B3,B2,B4};
		effectStruct e[] = {{T_FEEDBACK, -1, -1}};
		Skill s("Feedback", ATTACK, S_MAGIC2, 60, 170, WATER, e, 1, CREATE_FEEDBACK, k, 4,
		"From the world of Starcraft, this will make your enemy pretty irritated when he was so much time saving his stamina...");
		skills[FEEDBACK] = s;
	}
	
	{
		buttonType k[] = {B1,B5,B1,B5,B1,B5,B1,B5,B1,B5,B1,B5,B1,B5};
		effectStruct e[] = {{T_ELETRIFY, 6, 0.02}};
		Skill s("Eletrified Ground", ATTACK, S_BOLT2, 25, 100, THUNDER, e, 1, CREATE_ELETRIC_GROUND, k, 14,
		"OMG, the ground is eletrified! When the enemies instrument is eletrified, fretting is a painfull thing to do...");
		skills[ELETRIFIED_GROUND] = s;
	}
	
	{
		buttonType k[] = {B1,B2,B3,B4,B5,B1,B2,B3,B4,B5,B1,B2,B3,B4,B5};
		effectStruct e[] = {{T_DROWN, 15, -1}};
		Skill s("Flood", ATTACK, S_WATERFALL, 70, 270, WATER, e, 1, CREATE_FLOOD_EFFECT, k, 15,
		"OMG, water raising from the ground! I can't see a thing! And I can't even breath! I think I'm being drownebgblbgbgbdb......");
		skills[FLOOD] = s;
	}
	
	{
		buttonType k[] = {B3,B4,B5,B3,B4,B5};
		effectStruct e[] = {{T_DAMAGE, 50, -1},{T_STAMINA_DOWN, 30, -1}, {T_SPEED_UP, 0, -1}};
		Skill s("Water Beam", ATTACK, S_WATER2, 15, 70, WATER, e, 3, CREATE_WATER_BEAM, k, 6,
		"TORTOOOOOOOIIIIIIIIISEEEEEEEEEEEEEEEEEEEEEEEEEE");
		skills[WATER_BEAM] = s;
	}
	
	{
		buttonType k[] = {B1,B3,B4,B5,B1,B3,B4,B5,B1,B3,B4,B5};
		effectStruct e[] = {{T_DAMAGE, 200, -1},{T_ELETRIFY, 4, 0.05}};
		Skill s("Thunder Storm", ATTACK, S_STORM, 15, 70, WATER, e, 2, CREATE_THUNDERSTORM, k, 12,
		"");
		skills[THUNDERSTORM] = s;
	}
	
	{
		buttonType k[] = {B1,B2,B3,B4,B5,B5,B4,B3,B2,B1};
		effectStruct e[] = {{T_MIRROR, 3.5, -1}};
		Skill s("Mirror", DEFENSE, S_DIGITAL, 30, 120, WATER, e, 1, SHOW_SHIELD, k, 10,
		"");
		skills[MIRROR] = s;
	}	
	
	{
		buttonType k[] = {B3,B4,B5,B1,B2,B3,B4,B5,B1,B2};
		effectStruct e[] = {{T_POISONOUS, 8, -1}};
		Skill s("Swamp", ATTACK, S_UNDERWATER2, 30, 120, WATER, e, 1, CREATE_SWAMP_EFFECT, k, 10,
		"");
		skills[SWAMP] = s;
	}	
	
}
