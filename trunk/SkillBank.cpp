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
		// Launch a fireball in the enemy. Cheap and fast damage, will also decrease the defenses of the enemy a little each time, which can be a disaster later on!
		buttonType k[] = {B1,B1};
		effectType e[] = {T_DAMAGE,T_DEFENSE_DOWN};
		Skill s("Fireball", 10, 10, FIRE, e, 2, CREATE_FIREBALL, k, 2);
		skills[FIREBALL] = s;
	}
	
	{
		// Cures illness and HP
		buttonType k[] = {B2,B3};
		effectType e[] = {T_HEAL,T_ANTIDOTE};
		Skill s("Cure", 25, 70, WATER, e, 2, CREATE_GLOW_AREA, k, 2);
		skills[CURE] = s;
	}
	
	{
		// A big explosion right in the enemies face! Causes damage, lowers the defense and knocks the enemy into the ground, causing him to lose stamina
		buttonType k[] = {B2,B2,B3,B4,B5};
		effectType e[] = {T_DAMAGE,T_DEFENSE_DOWN,T_STAMINA_DOWN};
		Skill s("Explosion", 50, 170, EARTH, e, 3, CREATE_EXPLOSION, k, 5);
		skills[EXPLOSION] = s;
	}

	{
		// Cheap and fast damage which eletrifies the opponents instrument, causing notes to be harder to hit
		buttonType k[] = {B2,B2};
		effectType e[] = {T_DAMAGE,T_SHOCK,T_TOLERANCE_DOWN};
		Skill s("Lightning Bolt", 30, 10, THUNDER, e, 3, CREATE_BOLT, k, 2);
		skills[LIGHTNING_BOLT] = s;
	}

	{
		// The skies are on fire! Causes damage, lowers defense and put your enemy on fire. Also, all that fire makes the fretting harder to see...
		buttonType k[] = {B5,B4,B3,B2,B1,B5,B4,B3,B2,B1,B5,B4,B3,B2,B1};
		effectType e[] = {T_DAMAGE,T_DEFENSE_DOWN,T_BURN};
		Skill s("Fire Rain", 1, 1, FIRE, e, 3, CREATE_FIRE_RAIN, k, 15);
		skills[FIRE_RAIN] = s;
	}
	
	{
		// From the world of Starcraft, this will make your enemy pretty irritated when he was so much time saving his stamina...
		buttonType k[] = {B1,B3,B2,B4};
		effectType e[] = {T_FEEDBACK};
		Skill s("Feedback", 1, 1, WATER, e, 1, CREATE_FEEDBACK, k, 4);
		skills[FEEDBACK] = s;
	}
	
	{
		// OMG, the ground is eletrified! When the enemies instrument is eletrified, fretting is a painfull thing to do...
		buttonType k[] = {B1,B1,B2,B2,B1,B1,B2,B2};
		effectType e[] = {T_ELETRIFY};
		Skill s("Eletrified Ground", 1, 1, THUNDER, e, 1, CREATE_ELETRIC_GROUND, k, 8);
		skills[ELETRIFIED_GROUND] = s;
	}
	
	{
		// OMG, water raising from the ground! I can't see a thing! And I can't even breath! I think I'm being drownebgblbgbgbdb......
		buttonType k[] = {B1,B2,B3,B4,B5,B1,B2,B3,B4,B5,B1,B2,B3,B4,B5};
		effectType e[] = {T_DROWN};
		Skill s("Flood", 1, 1, WATER, e, 1, CREATE_FLOOD_EFFECT, k, 15);
		skills[FLOOD] = s;
	}
	
	{
		// TORTOOOOOOOIIIIIIIIISEEEEEEEEEEEEEEEEEEEEEEEEEE
		buttonType k[] = {B3,B4,B5,B3,B4,B5};
		effectType e[] = {T_DAMAGE,T_STAMINA_DOWN};
		Skill s("Water Beam", 1, 1, WATER, e, 2, CREATE_WATER_BEAM, k, 6);
		skills[WATER_BEAM] = s;
	}
}
