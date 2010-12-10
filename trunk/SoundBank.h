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
							       {"music/roundabout/notes.mid", "music/roundabout/song.ogg", "", HARD}
							  };

const std::string wavFiles[] = { "bolt1", "bolt2", "lightning", "digital1", "explosion1", "fire1", "magic1", "magic2", "storm", "underwater", "underwater2", "water1", "water2", "waterfall" };
const std::string oggFiles[] = { "miss1", "miss2", "miss3", "miss4", "miss5", "miss6", "start" };

class SoundBank
{
	public:
		SoundBank();
		~SoundBank();
	
		musicLibEntry 	selectedSong;
		
		void 			selectMusic( int n );
		void 			playSelectedMusic();
		void			playEffect( soundEffectType which );
		
		
	private:
		FMOD::System 	*system;
		FMOD::Channel 	*ch;
		
		// music
		FMOD::Sound 	*song, *guitar;
		
		// sound effects
		FMOD::Sound		*effects[S_TOTAL];

};
