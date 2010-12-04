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
	
	
	// Launch a fireball in the enemy. Cheap and fast damage, will also decrease the defenses of the enemy a little each time, which can be a disaster later on!
	buttonType k1[] = {B1,B1};
	effectType e1[] = {T_DAMAGE,T_DEFENSE_DOWN};
	Skill s1("Fireball", 10, 10, ATTACK, e1, 2, CREATE_FIREBALL, k1, 2);
	skills[FIREBALL] = s1;
	
	
	// Cures illness and HP
	buttonType k2[] = {B2,B3};
	effectType e2[] = {T_HEAL,T_ANTIDOTE};
	Skill s2("Cure", 25, 70, DEFENSE, e2, 2, CREATE_GLOW_AREA, k2, 2);
	skills[CURE] = s2;
	
	
	// A big explosion right in the enemies face! Causes damage, lowers the defense and knocks the enemy into the ground, causing him to lose stamina
	buttonType k3[] = {B2,B2,B3,B4,B5};
	effectType e3[] = {T_DAMAGE,T_DEFENSE_DOWN,T_STAMINA_DOWN};
	Skill s3("Explosion", 50, 170, ATTACK, e3, 3, CREATE_EXPLOSION, k3, 5);
	skills[EXPLOSION] = s2;


	// Cheap and fast damage which eletrifies the opponents instrument, causing notes to be harder to hit
	buttonType k4[] = {B2,B2};
	effectType e4[] = {T_DAMAGE,T_SHOCK,T_TOLERANCE_DOWN};
	Skill s4("Lightning Bolt", 30, 10, ATTACK, e4, 3, CREATE_BOLT, k4, 5);
	skills[LIGHTNING_BOLT] = s4;


	// The skies are on fire! Causes damage, lowers defense and put your enemy on fire. Also, all that fire makes the fretting harder to see...
	buttonType k5[] = {B1,B2,B3,B4,B5,B1,B2,B3,B4,B5};
	effectType e5[] = {T_DAMAGE,T_DEFENSE_DOWN,T_BURN};
	Skill s5("Fire Rain", 1, 1, ATTACK, e5, 3, CREATE_FIRE_RAIN, k5, 10);
	skills[FIRE_RAIN] = s5;
	

	// From the world of Starcraft, this will make your enemy pretty irritated when he was so much time saving his stamina...
	buttonType k6[] = {B1,B3,B2,B4};
	effectType e6[] = {T_FEEDBACK};
	Skill s6("Feedback", 1, 1, ATTACK, e6, 1, CREATE_FEEDBACK, k6, 4);
	skills[FEEDBACK] = s6;
	
	
	// OMG, the ground is eletrified! When the enemies instrument is eletrified, fretting is a painfull thing to do...
	buttonType k7[] = {B1,B1,B1,B2,B2};
	effectType e7[] = {T_ELETRIFY};
	Skill s7("Eletrified Ground", 1, 1, ATTACK, e7, 1, CREATE_ELETRIC_GROUND, k7, 4);
	skills[ELETRIFIED_GROUND] = s7;
}
