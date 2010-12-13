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
	/* A Skill Constructor requires:
	 * 
	 * string 					name
	 * skillTargetType 			targetType
	 * soundEffectType 			soundEffect
	 * double 					cooldown
	 * double 					cost
	 * elementType 				element
	 * effectStruct 			effects[]
	 * int 						neffects 		// !!DONT FORGET THIS!!
	 * visualEffectFunction 	effectFunction
	 * buttonType 				keys[]
	 * int 						nkeys			// !!DONT FORGET THIS!!
	 * string 					skillDescription
	 */
	
	
	/////////////////////////////
	// FIRE
	/////////////////////////////
	{
		buttonType k[] = {B1,B1};
		effectStruct e[] = {{T_DAMAGE, 50, -1}, {T_DEFENSE_DOWN, 1, -1}};
		Skill s("Fireball", ATTACK, S_FIRE2, 10, 30, FIRE, e, 2, EFFECT_FIREBALL, k, 2,
			"Launch a fireball in the enemy. Cheap and fast damage, will also decrease the defenses of the enemy a little each time, which can be a disaster later on!");
		skills[FIREBALL] = s;
	}
	{
		buttonType k[] = {B5,B4,B3,B2,B1,B5,B4,B3,B2,B1,B5,B4,B3,B2,B1};
		effectStruct e[] = {{T_DAMAGE, 170, -1},{T_DEFENSE_DOWN, 2, 5},{T_BURN, 15, -1}};
		Skill s("Fire Rain", ATTACK, S_EXPLOSION, 50, 200, FIRE, e, 3, EFFECT_FIRE_RAIN, k, 15,
		"The skies are on fire! Causes damage, lowers defense and put your enemy on fire. Also, all that fire makes the fretting harder to see...");
		skills[FIRE_RAIN] = s;
	}
	{
		buttonType k[] = {B2,B4,B5,B2,B5,B4,B2,B4,B5,B2,B5,B4};
		effectStruct e[] = {};
		Skill s("Implant Bomb", ATTACK, S_HORROR1, 0, 60, FIRE, e, 0, EFFECT_FIREBALL, k, 12,
		"Implants a bomb on an opponent note, which will be activated if he plays it, causing a lot of damage.");
		skills[IMPLANT_BOMB] = s;
	}
	{
		buttonType k[] = {B4,B5,B3,B4,B2,B3,B1,B2,B4,B5,B3,B4,B2,B3,B1,B2};
		effectStruct e[] = {{T_BURN, 20, -1},{T_BREAK_DEFENSE, 20, -1}};
		Skill s("Sunbirth", ATTACK, S_FIRERUMBLE, 0, 200, FIRE, e, 2, EFFECT_SUN, k, 16,
		"");
		skills[SUNBIRTH] = s;
	}	
	
	
	/////////////////////////////
	// THUNDER
	/////////////////////////////
	{
		buttonType k[] = {B2,B2,B4,B4};
		effectStruct e[] = {{T_DAMAGE, 50, -1},{T_ELETRIFY, 2, -1},{T_SPEED_UP, 2, -1}};
		Skill s("Lightning Bolt", ATTACK, S_LIGHTNING, 15, 40, THUNDER, e, 3, EFFECT_BOLT, k, 4,
		"Cheap and fast damage which eletrifies the opponents instrument, causing notes to be harder to hit.");
		skills[LIGHTNING_BOLT] = s;
	}
	{
		buttonType k[] = {B1,B5,B1,B5,B1,B5,B1,B5,B1,B5,B1,B5,B1,B5};
		effectStruct e[] = {{T_ELETRIFY, 6, 0.02}};
		Skill s("Eletrified Ground", ATTACK, S_BOLT2, 25, 100, THUNDER, e, 1, EFFECT_ELETRIC_GROUND, k, 14,
		"Watch out, the ground is eletrified! When an enemies instrument is eletrified, fretting becomes a confusing thing to do.");
		skills[ELETRIFIED_GROUND] = s;
	}
	{
		buttonType k[] = {B1,B3,B4,B5,B1,B3,B4,B5,B1,B3,B4,B5};
		effectStruct e[] = {{T_DAMAGE, 200, -1},{T_ELETRIFY, 4, 0.05}};
		Skill s("Thunder Storm", ATTACK, S_STORM, 15, 70, THUNDER, e, 2, EFFECT_THUNDERSTORM, k, 12,
		"A powerfull thunder storm which causes a lot of damage and yet eletrifies the opponent.");
		skills[THUNDERSTORM] = s;
	}
	{
		buttonType k[] = {B3,B3,B4,B4,B5,B5};
		effectStruct e[] = {{T_CHAOTIC_SPEED, 8, -1}/*,{T_ELETRIFY, 8, 0.05}*/};
		Skill s("Ball Lightning", ATTACK, S_ELECTRIC1, 15, 150, THUNDER, e, 1, EFFECT_BALL_LIGHTNING, k, 6,
		"And ancient tecnique, taught by the Zen masters who could also control Thunder. It shummons an electric begin from another dimension, which can eletrify the body and the mind of the opponent.");
		skills[BALL_LIGHTNING] = s;
	}	
	
	
	/////////////////////////////
	// SPIRITUAL
	/////////////////////////////	
	{
		buttonType k[] = {B1,B2,B5,B4,B1,B2,B5,B4};
		effectStruct e[] = {{T_HEAL, 50, -1},{T_ANTIDOTE, -1}};
		Skill s("Cure", DEFENSE, S_MAGIC1, 15, 70, SPIRITUAL, e, 2, EFFECT_GLOW_AREA, k, 8, "Cures all illness of the body and soul.");
		skills[CURE] = s;
	}
	{
		buttonType k[] = {B1,B3,B2,B4};
		effectStruct e[] = {{T_FEEDBACK, -1, -1}};
		Skill s("Feedback", ATTACK, S_MAGIC2, 60, 170, SPIRITUAL, e, 1, EFFECT_FEEDBACK, k, 4,
		"From the world of Starcraft, this will make your enemy pretty irritated when he was so much time saving his stamina...");
		skills[FEEDBACK] = s;
	}	
	{
		buttonType k[] = {B1,B2,B3,B4,B5,B5,B4,B3,B2,B1};
		effectStruct e[] = {{T_MIRROR, 3.5, -1}};
		Skill s("Mirror", DEFENSE, S_DIGITAL, 30, 120, SPIRITUAL, e, 1, EFFECT_SHIELD, k, 10,
		"... Still, be like a mirror. Respond like an echo.");
		skills[MIRROR] = s;
	}
	{
		buttonType k[] = {B4,B4,B5,B5,B3,B3,B2,B2,B1,B1,B1,B2,B3,B4,B5};
		effectStruct e[] = {{T_TIME_WARP, 5, -1}};
		Skill s("Time Warp", ATTACK, S_DIGITAL, 30, 90, SPIRITUAL, e, 1, EFFECT_GLOW_AREA, k, 15,
		"Enemy will get pretty confused when he has to play like in the 80's!");
		skills[TIME_WARP] = s;
	}
	
	
	/////////////////////////////
	// ECO
	/////////////////////////////	
	{
		buttonType k[] = {B1,B2,B1,B3,B1,B4,B1,B5,B5,B5};
		effectStruct e[] = {{T_DAMAGE, 300, -1},{T_DEFENSE_DOWN, 3, 6},{T_STAMINA_DOWN, 25, -1}};
		Skill s("Earth Explosion", ATTACK, S_EXPLOSION, 30, 190, ECO, e, 3, EFFECT_EXPLOSION, k, 10,
		"A big explosion comes from the ground! Causes lots of damage, lowers the defense and knocks the enemy into the ground, causing him to lose stamina");
		skills[EXPLOSION] = s;
	}
	// create some forest-related things here!
	
	
	/////////////////////////////
	// WATER
	/////////////////////////////
	{
		buttonType k[] = {B3,B4,B5,B3,B4,B5};
		effectStruct e[] = {{T_DAMAGE, 50, -1},{T_STAMINA_DOWN, 50, -1}};
		Skill s("Water Beam", ATTACK, S_STREAM2, 15, 30, WATER, e, 2, EFFECT_WATER_BEAM, k, 6,
		"TORTOOOOOOOIIIIIIIIISEEEEEEEEEEEEEEEEEEEEEEEEEE");
		skills[WATER_BEAM] = s;
	}
	{
		buttonType k[] = {B1,B2,B3,B4,B5,B1,B2,B3,B4,B5,B1,B2,B3,B4,B5};
		effectStruct e[] = {{T_DROWN, 10, -1}};
		Skill s("Flood", ATTACK, S_STREAM3, 70, 150, WATER, e, 1, EFFECT_FLOOD, k, 15,
		"Just like Lara Croft would behave underwater.");
		skills[FLOOD] = s;
	}	
	{
		buttonType k[] = {B3,B4,B5,B1,B2,B3,B4,B5,B1,B2};
		effectStruct e[] = {{T_POISONOUS, 8, -1},{T_STAMINA_DOWN, 30, -1}};
		Skill s("Swamp", ATTACK, S_UNDERWATER1, 30, 120, WATER, e, 2, EFFECT_SWAMP, k, 10,
		"Drawn your opponent on a poisonous swamp.");
		skills[SWAMP] = s;
	}
	
}
