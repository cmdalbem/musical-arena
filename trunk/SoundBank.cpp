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
	for(int i=0; i<14; i++) {
		FMOD::Sound * sound;
		sprintf(filename,"sound/%s.wav",wavFiles[i].c_str());

		system->createSound(filename, FMOD_DEFAULT, 0, &sound);
		string name = wavFiles[i];
		if(name.find("bolt"))
			bolt.push_back(sound);
		else if(name.find("lightning"))
			lightning.push_back(sound);
		else if(name.find("digital"))
			digital.push_back(sound);
		else if(name.find("explosion"))
			explosion.push_back(sound);
		else if(name.find("fire"))
			fire.push_back(sound);
		else if(name.find("miss"))
			miss.push_back(sound);
		else if(name.find("water"))
			water.push_back(sound);
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
	result = system->playSound(FMOD_CHANNEL_FREE, song, false, &ch1);
	if(guitar)
		system->playSound(FMOD_CHANNEL_FREE, guitar, false, &ch1);
	ERRCHECK(result);
	
	// play some test effects
	//system->playSound(FMOD_CHANNEL_FREE, lightning, false, &ch2);
}

void SoundBank::playEffect(int efect)
{
	FMOD_RESULT result;
	int index;
	switch (efect)
	{
	case S_MISS:
		index = rand() % miss.size();
		result = system->playSound(FMOD_CHANNEL_FREE, miss[index], false, &ch1);
		break;
	}
	ERRCHECK(result);
}
void SoundBank::playEffect(int efect, int index)
{
	FMOD_RESULT result;
	switch (efect)
	{
	case S_MISS:
		result = system->playSound(FMOD_CHANNEL_FREE, miss[index], false, &ch1);
		break;
	}
	ERRCHECK(result);
}
