// main.cpp
//

#include <vector>
#include <iostream>
using namespace std;

#include "irrlicht.h"
using namespace irr;
using irr::core::vector3df;

#include "Decoder.h"
#include "Stone.h"
#include "utils.h"
#include "eventReceiver.h"
#include "Fretting.h"
#include "Track.h"


Decoder decoder; 

vector<musicEvent> theMusic;

Track* mainTrack;

// indicates the end of the music. must be implemented to be turned "true" when ogg file ends its playing.
bool endOfMusic=false;

Fretting fretting1;
EKEY_CODE eventos[5] = { irr::KEY_KEY_A, irr::KEY_KEY_S, irr::KEY_KEY_J, irr::KEY_KEY_K, irr::KEY_KEY_L };

double	musicTime = 0;
int		musicIndex=0;

char array[20];

int mutex=1;

std::string defaultFile = "music/example.mid",
			songFile = "music/example.ogg",
			guitarFile = "";
			
note theScreen[SCREEN_Y][NUMBER_OF_FRETS];

// Irrlicht-related globals
eventReceiver receiver;
IrrlichtDevice *device;
video::IVideoDriver* driver;
scene::ISceneManager* smgr;
scene::ICameraSceneNode *camera;


static void *updater(void *argument) 
{
	struct timeval start;
	
	// get the time before starting the music (so we can know how much time passed in each note)
	gettimeofday(&start, NULL);
	
	while( !endOfMusic ) {
		
		musicTime = time_diff(start);// + 73;

		while( (unsigned int)musicIndex < theMusic.size() &&
			   (musicTime + mainTrack->spawnDelay) > theMusic[musicIndex].time ) {
			
			mainTrack->processEvent(theMusic[musicIndex]);
			
			++musicIndex; // we won't be deleting things anymore
		}
		
		while(mutex==0);
		mainTrack->update(musicTime);
		mutex=0;
	}
	
	return NULL;
}

void* fretting (void *arg)
// Poll the keyboard testing if the player has pressed the right notes.
{	
	while(0)//!endOfMusic)
	{
		/*
		for (int color = 0; color < 5; color++)
			fretting1.verify_event(color, mainTrack.stones, musicTime, tolerance);*/

		/*
		for (int color = 0; color < 5; color++)
			fretting2.verify_event(color);*/
	}
	
	return NULL;
}

void musa_init()
{
	fretting1.setEvents(eventos);
	fretting1.setReceiver(&receiver);
	
	mainTrack = new Track(smgr,driver,15);
	
	theMusic = decoder.decodeMidi(defaultFile);
	cout << "MIDI parsed. This is your music:" << endl;
	// Shows the events (of the midi file read) on the command line
	for(unsigned int i = 0; i < theMusic.size(); i++) {
		cout << theMusic[i].time << " - " << theMusic[i].button << ": ";
		switch(theMusic[i].type)
		{
			case ON: cout << "ON"; break;
			case OFF: cout << "OFF"; break;
			default: break;
		}
		cout<<endl;
	}
		
	/*
	//initializes the matrix (for drawer)
	for(int lin=0; lin<SCREEN_Y; lin++)
		for(int col=0; col<5; col++)
			theScreen[lin][col] = NOTHING;*/
}

void initializeIrrlicht()
{
// Graphical engine initializing
	device = createDevice( video::EDT_OPENGL, core::dimension2d<u32>(800,600), 32,
							false, //fullscreen?
							false, //used stencil buffer?
							false, //use vsync?
							&receiver //event receiver
						);
	device->setWindowCaption(L"Musical Arena (MusA): The Adventures of Lucy in the Enchanted Realm of Diamond Sky");

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	
	
	scene::ILightSceneNode *light = smgr->addLightSceneNode(0, vector3df(0,-80,-30), video::SColorf(1.0f, 1.0f, 1.0f), 20.0f);
	//light->setLightType(video::ELT_DIRECTIONAL);
	//light->setRotation(vector3df(-90,0,0));
	
    // like the real game camera
    camera = smgr->addCameraSceneNode (
				0,					  // Camera parent
				vector3df(0, -90, -40), // Look from
				vector3df(0, -30, 20), // Look to
				1);						  // Camera ID

}

int main(int argc, char *argv[])
{
	/*
	 * inicializing the sound engine
	 */

	FMOD_RESULT result;
	FMOD::System *system;
	
	result = FMOD::System_Create(&system);		// Create the main system object.
	ERRCHECK(result)
	
	result = system->init(100, FMOD_INIT_NORMAL, 0);	// Initialize FMOD.
	ERRCHECK(result)

	// loads the sound file on the memory
	FMOD::Sound *sound;
	result = system->createSound(songFile.c_str(), FMOD_DEFAULT, 0, &sound);
	ERRCHECK(result);
	
	/*
	 * inicializing the graphics engine
	 */
	initializeIrrlicht();

	/*
	 * inicializing game engine
	 */
	musa_init();

	FMOD::Channel *channel;
	// plays the ogg (TREMENDOUSLY REDUCES FPS D=) (seriously?)
	result = system->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
	ERRCHECK(result);

	pthread_t thread[3];
	int arg = 1;
	//pthread_create(&thread[0], NULL, drawer, (void *) arg);
	pthread_create(&thread[1], NULL, fretting, (void *) arg);
	pthread_create(&thread[2], NULL, updater, (void *) arg);
	
	/*
	 * Irrlicht Main Loop
	 */
	while(device->run()) {
		
		driver->beginScene(true, true, video::SColor(255,113,113,133));

		while(mutex==1);
		smgr->drawAll(); // draw the stones sceneNodes
		mainTrack->draw();
		mutex=1;

		//device->getGUIEnvironment()->drawAll(); // draw the gui environment

		driver->endScene();

		// FPS
		int fps = driver->getFPS();
		static int lastFPS = 0;
		if (lastFPS != fps) {
			core::stringw tmp(L"fps: ");
			tmp += fps;

			device->setWindowCaption(tmp.c_str());
			lastFPS = fps;
		}
	}
	
	/*
	 * End the game gracefully =D
	 */
	// wait for threads to complete
	//pthread_join(thread[0], NULL); //drawer
	pthread_join(thread[1], NULL);
	pthread_join(thread[2], NULL);
	
	return 0;
}

////////////////////////////////////////////////////////////////////////
// Debuging function. Saved because my be useful later.
/*
static void* drawer(void *argument) 
{
	struct	timeval start;
	
	// this is needed because, while we are using drawer() and updater()
	// 	at the same time, both are editing theMusic vector.
	// we solve the problem simply creating a copy because there's no
	//  apparent reason need for them to be synchronized.
	vector<musicEvent> theMusicCopied = theMusic;


	// get the time before starting the music (so we can know how much time passed in each note)
	gettimeofday(&start, NULL);
	
	while( !endOfMusic )
	{
		usleep(80000);
		system("clear");
		
		cout << "music time: "	   << musicTime		 << endl
			 << "upcoming event: " << theMusicCopied[0].time << endl;
 
		// propagates the lines of the matrix	
		for(int lin=SCREEN_Y-1; lin>0; lin--)
			for(int col=0; col<NUMBER_OF_FRETS; col++)
				theScreen[lin][col] = theScreen[lin-1][col];

		while( ((musicTime + STONE_DELAY) > theMusicCopied[0].time) && !endOfMusic) {
			// updates the first line of the matrix with the actual configuration
			switch(theMusicCopied[0].type) {
				case ON:
					theScreen[0][theMusicCopied[0].button] = STRIKE;
					break;
				case OFF:	
					theScreen[0][theMusicCopied[0].button] = NOTHING;		
					break;
				default: break;
			}
			
			// avoiding segmentation faults =D
			if(theMusicCopied.size() > 0)
				theMusicCopied.erase(theMusicCopied.begin());
			else
				endOfMusic = true;
		}
		
		//prints the matrix	
		for(int lin=0; lin<SCREEN_Y; lin++)
		{
			for(int col=0; col<NUMBER_OF_FRETS; col++)
			{
				switch(theScreen[lin][col])
				{
					case NOTHING: cout<<" "; break;
					case STRIKE: cout<<"O"; break;						
					case HOLD: cout<<"|"; break;
				}
				cout<<"  ";
			}
			cout<<endl;
		}
	}
	
	return NULL;	
}*/
