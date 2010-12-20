#pragma once

#include "utils.h"
#include "irrlicht.h"
#include <fmod.hpp>

using namespace std;
using namespace irr;
 


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
		SoundBank( IrrlichtDevice *device );
		~SoundBank();
	
		void		 	openSongFile( const char path[] );
		void 			openGuitarFile( const char path[] );
		void 			playSelectedMusic();
		void			playEffect( soundEffectType which );
		void			playMissEffect();
		void			playTheme();
		void			stop();
		
	private:
		IrrlichtDevice	*device;
		FMOD::System 	*system;
		FMOD::Channel 	*ch;
		
		// music
		FMOD::Sound 	*song, *guitar, *theme;
		
		// sound effects
		FMOD::Sound		*effects[S_TOTAL], *missEffects[6];
		
};
