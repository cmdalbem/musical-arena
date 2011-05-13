#include "SoundBank.h"

#define MUTE 0


SoundBank::SoundBank( IrrlichtDevice *_device )
{
	device = _device;
	
	FMOD_RESULT result;
	result = FMOD::System_Create(&system);		//  the main system object.
	ERRCHECK(result)
	result = system->init(100, FMOD_INIT_NORMAL, 0);	// Initialize FMOD.
	ERRCHECK(result)


	
	song = NULL;
	guitar = NULL;
	
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
	
	result = system->createSound("sound/theme.mp3",FMOD_LOOP_NORMAL, 0, &theme);
	ERRCHECK(result);
}

SoundBank::~SoundBank()
{

}

void SoundBank::openSongFile( const char path[] )
{
	FMOD_RESULT result = system->createSound(path, FMOD_ACCURATETIME, 0, &song);
	ERRCHECK(result);
}

void SoundBank::openGuitarFile( const char path[] )
{
	FMOD_RESULT result = system->createSound(path, FMOD_ACCURATETIME , 0, &guitar);
	ERRCHECK(result);
}	

void SoundBank::playSelectedMusic()
{ 
	if(!MUTE) {
		if(song)
			system->playSound(FMOD_CHANNEL_FREE, song, false, &ch1);
		if(guitar)
			system->playSound(FMOD_CHANNEL_FREE, guitar, false, &ch2);
	}
}

void SoundBank::playEffect( soundEffectType which )
{
	if(!MUTE)
		system->playSound(FMOD_CHANNEL_FREE, effects[which], false, &ch3);
}

void SoundBank::playMissEffect()
{
	if(!MUTE)
		system->playSound(FMOD_CHANNEL_FREE, missEffects[ rand()%6 ], false, &ch3);
}

void SoundBank::playTheme()
{
	if(!MUTE)
		system->playSound(FMOD_CHANNEL_FREE, theme, false, &ch1);
}

void SoundBank::stop()
{
	ch1->stop();
	ch2->stop();
}

void SoundBank::pause()
{
	ch1->setPaused(true);
	ch2->setPaused(true);
}

void SoundBank::unpause()
{
	ch1->setPaused(false);
	ch2->setPaused(false);
}

bool SoundBank::isPlaying()
{
	ch1->isPlaying(&isMusicPlaying1);
	ch2->isPlaying(&isMusicPlaying2);
	
	return isMusicPlaying1 || isMusicPlaying2;
}

void SoundBank::clearSongs()
{
	guitar = NULL;
	song = NULL;
}
