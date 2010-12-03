#include "Skill.h"
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
	Skill s("Fireball", 1, 1, ATTACK, DAMAGE);//, s2, s3;
	s.keysSequence.push_back(B1);
	s.keysSequence.push_back(B2);
	s.keysSequence.push_back(B3);
	s.keysSequence.push_back(B4);
	s.keysSequence.push_back(B5);
	
	skills[FIREBALL] = s;
	
	/*s2.keysSequence.push_back(B1);
	s2.keysSequence.push_back(B2);
	s2.keysSequence.push_back(B4);
	s2.name = "Stick with it";
	s3.keysSequence.push_back(B1);
	s3.keysSequence.push_back(B2);
	s3.keysSequence.push_back(B3);
	s3.name = "People = Shit";
	skills.push_back(s1);
	skills.push_back(s2);
	skills.push_back(s3);*/
}
