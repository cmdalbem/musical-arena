// Player.h
//

#include "Fretting.h"
#include "Track.h"


class Player
{
	public:
		Player();
		~Player();
		
		void inicialize();

		// Instrument	instrument;
		Fretting	*fretting;
		Track		*track;
		
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
