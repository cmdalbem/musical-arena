// Player.h
//

#include "Fretting.h"


class Player
{
	public:
		Player(Fretting *fretting = NULL);
		~Player();
		
		void inicialize();

		// Instrument	instrument;
		Fretting	*fretting;
		
		int			HP;
		int			maxHP;
		int			stamina;
		int			maxStamina;
		
		int			XP;
		int			level;
		// vector<Skill>	skills; // each skill has its own cooldown
		
		int			gold;
	private:
};
