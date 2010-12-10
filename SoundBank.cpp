#include "SoundBank.h"

SoundBank::SoundBank()
{
	FMOD_RESULT result;
	result = FMOD::System_Create(&system);		// Create the main system object.
	ERRCHECK(result)
	result = system->init(100, FMOD_INIT_NORMAL, 0);	// Initialize FMOD.
	ERRCHECK(result)

	song = NULL;
	guitar = NULL;
	
	char filename[128];
	for(int i=0; i<S_TOTAL; i++) {
		sprintf(filename,"sound/%s.wav",wavFiles[i].c_str());
		system->createSound(filename, FMOD_DEFAULT, 0, &effects[i]);
	}
}

SoundBank::~SoundBank()
{

}

void SoundBank::selectMusic( int n )
{
	FMOD_RESULT result;
	
	selectedSong = musicLib[n];

	// load files
	result = system->createSound(selectedSong.song.c_str(), FMOD_DEFAULT, 0, &song);
	if(selectedSong.guitar.size()>0)
		system->createSound(selectedSong.guitar.c_str(), FMOD_DEFAULT, 0, &guitar);
	ERRCHECK(result);
}

void SoundBank::playSelectedMusic()
{
	FMOD_RESULT result;
	
	// play files
	result = system->playSound(FMOD_CHANNEL_FREE, song, false, &ch);
	if(guitar)
		system->playSound(FMOD_CHANNEL_FREE, guitar, false, &ch);
	ERRCHECK(result);
}

void SoundBank::playEffect( soundEffectType which )
{
	system->playSound(FMOD_CHANNEL_FREE, effects[which], false, &ch);
}
