#include "Skill.h"
#include "types.h"

class SkillBank
{
	public:
		SkillBank();
		~SkillBank();

		Skill 		skills[SKILLS_TOTAL];
		
		void		print();
		
	private:
		void		makeSkills();

};

