#include "Skill.h"


enum skillId
{
	FIREBALL,
	
	SKILLS_TOTAL
};

class SkillBank
{
	public:
		SkillBank();
		~SkillBank();

		Skill 		skills[SKILLS_TOTAL];
		
		
	private:
		void		makeSkills();

};

