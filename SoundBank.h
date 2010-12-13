#pragma once

#include "utils.h"

using namespace std;


struct musicLibEntry
{
	std::string notes, song, guitar;
	difficultyType difficulty;
};

const musicLibEntry musicLib[] = { {"music/api/notes.mid", "music/api/song.ogg", "music/api/guitar.ogg", HARD},
							       {"music/example/notes.mid", "music/example/song.ogg", "", EXPERT},
							       {"music/roundabout/notes.mid", "music/roundabout/song.ogg", "", HARD},
							       {"music/sectoid/notes.mid", "music/sectoid/song.ogg", "music/sectoid/guitar.ogg", EXPERT},
							  };

class SoundBank
{
	public:
		SoundBank();
		~SoundBank();
	
		musicLibEntry 	selectedSong;
		
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
