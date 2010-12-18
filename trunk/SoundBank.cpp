#include "SoundBank.h"

SoundBank::SoundBank( bool _mute )
{
	FMOD_RESULT result;
	result = FMOD::System_Create(&system);		//  the main system object.
	ERRCHECK(result)
	result = system->init(100, FMOD_INIT_NORMAL, 0);	// Initialize FMOD.
	ERRCHECK(result)

	song = NULL;
	guitar = NULL;
	mute = _mute;
	
	char filename[128];
	for(int i=0; i<S_TOTAL; i++) {
		sprintf(filename,"sound/%s.wav",wavFiles[i].c_str());
		result = system->createSound(filename, FMOD_DEFAULT, 0, &effects[i]);
		ERRCHECK(result);
	}
	
	for(int i=0; i<6; i++) {
		sprintf(filename,"sound/%s.ogg",missFiles[i].c_str());
		result = system->createSound(filename, FMOD_DEFAULT, 0, &missEffects[i]);
		ERRCHECK(result);
	}
}

SoundBank::~SoundBank()
{

}

void SoundBank::selectMusic( int n )
{
	FMOD_RESULT result;
	char path[100];
	FILE *check;
	
	selectedSong = musicLib[n];

	// open song file
	sprintf(path,"music/%s/song.ogg",selectedSong.name.c_str());
	check = fopen(path,"r");
	if( check )
	{ 
		fclose(check);
		result = system->createSound(path, FMOD_DEFAULT, 0, &song);
		ERRCHECK(result);
	}
	
	// open guitar track file
	sprintf(path,"music/%s/guitar.ogg",selectedSong.name.c_str());
	check = fopen(path,"r");
	if( check )
	{ 
		fclose(check);
		result = system->createSound(path, FMOD_DEFAULT, 0, &guitar);
		ERRCHECK(result);
	}
	
	cout << "abri tudo" << endl;
}

void SoundBank::playSelectedMusic()
{ 
	if(!mute) {
		if(song)
			system->playSound(FMOD_CHANNEL_FREE, song, false, &ch);
		if(guitar)
			system->playSound(FMOD_CHANNEL_FREE, guitar, false, &ch);
	}
}

void SoundBank::playEffect( soundEffectType which )
{
	if(!mute)
		system->playSound(FMOD_CHANNEL_FREE, effects[which], false, &ch);
}

void SoundBank::playMissEffect()
{
	if(!mute)
		system->playSound(FMOD_CHANNEL_FREE, missEffects[ rand()%6 ], false, &ch);
}
