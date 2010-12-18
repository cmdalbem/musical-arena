#pragma once

#include "utils.h"

using namespace std;



struct musicLibEntry
{
	std::string name;
	difficultyType difficulty;
};

const musicLibEntry musicLib[] = {  {"api", HARD},
									{"roundabout",HARD},
									{"incubus_stellar",HARD},
									{"cochise",HARD},
									{"iron_man",MEDIUM},
									{"tom_sawyer",MEDIUM},
								};

class SoundBank
{
	public:
		SoundBank( bool mute=false );
		~SoundBank();
	
		musicLibEntry 	selectedSong;
		bool			mute;
		
		void 			selectMusic( int n );
		void 			playSelectedMusic();
		void			playEffect( soundEffectType which );
		void			playMissEffect();
		
		
	private:
		FMOD::System 	*system;
		FMOD::Channel 	*ch;
		
		// music
		FMOD::Sound 	*song, *guitar;
		
		// sound effects
		FMOD::Sound		*effects[S_TOTAL], *missEffects[6];

};
