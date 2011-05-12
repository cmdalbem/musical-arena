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
	// Skill(name, targetType, soundEffect, cost, element, effects[], neffects, effectFunction, keys[], nkeys, nSlots, skillDescription );
	
	/////////////////////////////
	// FIRE
	/////////////////////////////
	{
		buttonType k[] = {B1,B1};
		effectStruct e[] = {{T_DAMAGE, 50}, {T_DEFENSE_DOWN, 1}};
		Skill s("Fireball", ATTACK, S_FIRE2, 50, FIRE, e, 2, EFFECT_FIREBALL, k, 2, 1,
			"Launch a fireball in the enemy. Cheap and fast damage, will also decrease the defenses of the enemy a little each time, which can be a disaster later on!");
		skills[FIREBALL] = s;
	}
	{
		buttonType k[] = {B5,B4,B3,B2,B1,B5,B4,B3,B2,B1,B5,B4,B3,B2,B1};
		effectStruct e[] = {{T_DAMAGE, 120},{T_DEFENSE_DOWN, 2},{T_BURN, 15}};
		Skill s("Fire Rain", ATTACK, S_FIRESTORM, 150, FIRE, e, 3, EFFECT_FIRE_RAIN, k, 15, 3,
		"The skies are on fire! Causes damage, lowers defense and put your enemy on fire. Also, all that fire makes the fretting harder to see...");
		skills[FIRE_RAIN] = s;
	}
	{
		buttonType k[] = {B4,B5,B3,B4,B2,B3,B1,B2,B4,B5,B3,B4,B2,B3,B1,B2};
		effectStruct e[] = {{T_BURN, 20},{T_BREAK_DEFENSE, 20}};
		Skill s("Starbirth", ATTACK, S_BIG, 200, FIRE, e, 2, EFFECT_SUN, k, 16,
		5, "Uses the mightiest powers of Fire to give birth to a Star right in the front your enemy, obstructing his vision and leaving him with no defenses at all!");
		skills[SUNBIRTH] = s;
	}
	{
		buttonType k[] = {B1,B2,B1,B3,B1,B4,B1,B5};
		effectStruct e[] = {{T_DAMAGE, 300},{T_DEFENSE_DOWN, 3}};
		Skill s("Explosion", ATTACK, S_EXPLOSION, 250, FIRE, e, 2, EFFECT_EXPLOSION, k, 8,
		8, "A big explosion comes from the ground! Causes lots of damage, lowers the defense and knocks the enemy into the ground, causing him to lose stamina");
		skills[EXPLOSION] = s;
	}	
	
	
	/////////////////////////////
	// THUNDER
	/////////////////////////////
	{
		buttonType k[] = {B2,B2,B4,B4};
		effectStruct e[] = {{T_DAMAGE, 50},{T_ELETRIFY, 2},{T_SPEED_UP, 2}};
		Skill s("Lightning Bolt", ATTACK, S_LIGHTNING, 50, THUNDER, e, 3, EFFECT_BOLT, k, 4,
		2, "Cheap and fast damage which eletrifies the opponents instrument, causing notes to be harder to hit.");
		skills[LIGHTNING_BOLT] = s;
	}
	{
		buttonType k[] = {B1,B5,B1,B5,B1,B5,B1,B5,B1,B5,B1,B5,B1,B5};
		effectStruct e[] = {{T_ELETRIFY, 6}};
		Skill s("Eletrified Ground", ATTACK, S_BOLT2, 130, THUNDER, e, 1, EFFECT_ELETRIC_GROUND, k, 14,
		4, "Watch out, the ground is eletrified! When an enemies instrument is eletrified, fretting becomes a confusing thing to do.");
		skills[ELETRIFIED_GROUND] = s;
	}
	{
		buttonType k[] = {B1,B3,B4,B5,B1,B3,B4,B5,B1,B3,B4,B5};
		effectStruct e[] = {{T_DAMAGE, 170},{T_ELETRIFY, 8},{T_SPEED_UP, 8}};
		Skill s("Thunder Storm", ATTACK, S_STORM, 270, THUNDER, e, 3, EFFECT_THUNDERSTORM, k, 12,
		8, "A powerfull thunder storm which causes a lot of damage and yet eletrifies the opponent.");
		skills[THUNDERSTORM] = s;
	}
	{
		buttonType k[] = {B3,B3,B4,B4,B5,B5};
		effectStruct e[] = {{T_CHAOTIC_SPEED, 8}};
		Skill s("Ball Lightning", ATTACK, S_ELECTRIC1, 110, THUNDER, e, 1, EFFECT_BALL_LIGHTNING, k, 6,
		7, "An ancient tecnique, taught by the Zen masters who could also control Thunder. It shummons an electric being from another dimension, which can eletrify the body and the mind of the opponent.");
		skills[BALL_LIGHTNING] = s;
	}
	
	
	/////////////////////////////
	// SPIRITUAL
	/////////////////////////////	
	{
		buttonType k[] = {B1,B2,B5,B4,B1,B2,B5,B4};
		effectStruct e[] = {{T_HEAL, 100},{T_ANTIDOTE, -1},{T_DEFENSE_UP, 2}};
		Skill s("Inspiration", DEFENSE, S_MAGIC1, 70, SPIRITUAL, e, 3, EFFECT_GLOW_AREA, k, 8,
		2, "Cures all illness of the body and soul.");
		skills[INSPIRATION] = s;
	}
	{
		buttonType k[] = {B1,B2,B3,B4,B5,B5,B4,B3,B2,B1};
		effectStruct e[] = {{T_MIRROR, 8},{T_DEFENSE_UP, 2}};
		Skill s("Mirror", DEFENSE, S_DIGITAL, 65, SPIRITUAL, e, 2, EFFECT_SHIELD, k, 10,
		4, "Still, be like a mirror. Respond like an echo.");
		skills[MIRROR] = s;
	}
	{
		buttonType k[] = {B4,B5,B3,B2,B1};
		effectStruct e[] = {{T_CLEAR_STONES, -1},{T_DEFENSE_UP, 2}};
		Skill s("Divine Silence", DEFENSE, S_FIRERUMBLE, 160, SPIRITUAL, e, 2, EFFECT_SILENCE, k, 5,
		6, "...");
		skills[DIVINE_SILENCE] = s;
	}
	{
		buttonType k[] = {B4,B5,B3,B4,B5,B3};
		effectStruct e[] = {{T_BLESS, 5},{T_DEFENSE_UP, 3}};
		Skill s("Aurora", DEFENSE, S_MAGIC1, 270, SPIRITUAL, e, 2, EFFECT_AURORA, k, 6,
		7, "A beautifull bless. Give you Bless status and raises up the defense.");
		skills[AURORA] = s;
	}
	
	
	/////////////////////////////
	// DARK
	/////////////////////////////	
	{
		buttonType k[] = {B4,B4,B5,B5,B3,B3,B2,B2,B1,B1};
		effectStruct e[] = {{T_CURSE, 10}};
		Skill s("Soul Corruption", ATTACK, S_HORROR1, 90, DARK, e, 1, EFFECT_BLACKHOLE, k, 10,
		3, "Corrupts the soul of your enemy with the creepiest thoughts, making him take damage even with what should save him, which is playing the music!");
		skills[SOUL_CORRUPTION] = s;
	}
	{
		buttonType k[] = {B1,B3,B2,B4, B5,B4,B3};
		effectStruct e[] = {{T_FEEDBACK, -1}};
		Skill s("Feedback", ATTACK, S_MAGIC2, 170, DARK, e, 1, EFFECT_FEEDBACK, k, 7,
		5, "From the world of Starcraft, this will make your enemy pretty irritated when he was so much time saving his stamina...");
		skills[FEEDBACK] = s;
	}
	{
		buttonType k[] = {B5,B4,B3,B3,B4,B5,B5,B4,B3,B2,B1};
		effectStruct e[] = {{T_VAMPIRIC, 100}};
		Skill s("Vampiric", ATTACK, S_HORROR2, 90, DARK, e, 1, EFFECT_VAMPIRIC, k, 11,
		4, "You will suck the life out of me!");
		skills[VAMPIRIC] = s;
	}	
	{
		buttonType k[] = {B1,B2,B3,B4,B5, B2,B3,B4,B5, B3,B4,B5, B4,B5, B5,B4,B3,B2,B1};
		effectStruct e[] = {{T_STAMINA_UP, 100}};
		Skill s("Black Lotus", DEFENSE, S_MAGIC1, 0, DARK, e, 1, EFFECT_LOTUS, k, 19,
		4, "The favorite of Darth Vader's bride.");
		skills[BLACK_LOTUS] = s;
	}
	
	/////////////////////////////
	// WATER
	/////////////////////////////
	{
		buttonType k[] = {B3,B4,B5};
		effectStruct e[] = {{T_DAMAGE, 30},{T_STAMINA_DOWN, 40},{T_DROWN, 3}};
		Skill s("Water Beam", ATTACK, S_STREAM2, 50, WATER, e, 3, EFFECT_WATER_BEAM, k, 3,
		2, "TORTOOOOOOOIIIIIIIIISEEEEEEEEEEEEEEEEEEEEEEEEEE");
		skills[WATER_BEAM] = s;
	}
	{
		buttonType k[] = {B1,B2,B3,B4,B5,B1,B2,B3,B4,B5,B1,B2,B3,B4,B5};
		effectStruct e[] = {{T_DROWN, 10}};
		Skill s("Flood", ATTACK, S_STREAM3, 150, WATER, e, 1, EFFECT_FLOOD, k, 15,
		4, "Just like Lara Croft would behave underwater.");
		skills[FLOOD] = s;
	}	
	{
		buttonType k[] = {B3,B4,B5,B1,B2,B3,B4,B5,B1,B2};
		effectStruct e[] = {{T_POISONOUS, 8},{T_STAMINA_DOWN, 30}};
		Skill s("Swamp", ATTACK, S_UNDERWATER1, 120, WATER, e, 2, EFFECT_SWAMP, k, 10,
		5, "Drawn your opponent on a poisonous swamp.");
		skills[SWAMP] = s;
	}
	{
		buttonType k[] = {B1,B2,B3,B4,B5,B5,B1,B2,B3,B4,B5,B5};
		effectStruct e[] = {{T_FREEZE, 8}};
		Skill s("Siberian", ATTACK, S_ICE1, 120, WATER, e, 1, EFFECT_FREEZE, k, 12,
		7, "Thaught by Grand Master Camus of Aquarius, this will freeze your enemies fingers until he cannot play anymore.");
		skills[SIBERIAN] = s;
	}	
	
}

void SkillBank::print()
{
	cout << "\n\n\n";
	cout << "-----------------------------------" << endl;
	cout << "Bank of Skills:" << endl;
	
	for(int i=0; i<SKILLS_TOTAL; i++)		
		skills[i].print();
	
	cout<<"*************************************************" << endl;	
}
