// main.cpp
//

#include <vector>
#include <iostream>
using namespace std;

#include <semaphore.h>
#include "irrlicht.h"
using namespace irr;
using irr::core::vector3df;

#include "Decoder.h"
#include "Stone.h"
#include "utils.h"
#include "EventReceiver.h"
#include "Fretting.h"
#include "Track.h"
#include "Player.h"
#include "Screen.h"
#include "Skill.h"


// Irrlicht-related globals
IrrlichtDevice 				*device;
video::IVideoDriver 		*driver;
scene::ISceneManager 		*smgr;
scene::ICameraSceneNode 	*camera;
EventReceiver 				receiver;


Decoder decoder; 
Screen *screen;

vector<musicEvent> theMusic;

// indicates the end of the music. must be implemented to be turned "true" when ogg file ends its playing.
bool endOfMusic=false;

double musicTime = 0;

Player player1, player2;

sem_t semaphore;

std::string defaultFile = "music/example.mid",
			songFile = "music/example.ogg",
			guitarFile = "";
			
//note theScreen[SCREEN_Y][NUMBER_OF_FRETS];


static void *updater(void *argument) 
{
	struct timeval start; 
	
	// get the time before starting the music (so we can know how much time passed in each note)
	gettimeofday(&start, NULL);
	  
	 while( !endOfMusic ) {
	
		musicTime = time_diff(start);// + 20;
 
		// spawning on track1
		while( (unsigned int)player1.track->musicPos < theMusic.size() &&
			   (musicTime + player1.track->spawnDelay) > theMusic[player1.track->musicPos].time ) {
			
			player1.track->processEvent(theMusic[player1.track->musicPos]);
		}
		
		// spawning on track2
		while( (unsigned int)player2.track->musicPos < theMusic.size() &&
			   (musicTime + player2.track->spawnDelay) > theMusic[player2.track->musicPos].time ) {
			
			player2.track->processEvent(theMusic[player2.track->musicPos]);
		}
		 
		sem_wait(&semaphore);
		player1.track->update(musicTime);
		player2.track->update(musicTime);
		sem_post(&semaphore);
	}
	
	return NULL;
}


void musa_init()
{
	static vector<Skill> skills;
	Skill s1, s2, s3;
	s1.keysSequence.push_back(B1);
	s1.keysSequence.push_back(B4);
	s1.name = "Meduse";
	s2.keysSequence.push_back(B1);
	s2.keysSequence.push_back(B2);
	s2.keysSequence.push_back(B4);
	s2.name = "Stick with it";
	s3.keysSequence.push_back(B1);
	s3.keysSequence.push_back(B2);
	s3.keysSequence.push_back(B3);
	s3.name = "People = Shit";
	skills.push_back(s1);
	skills.push_back(s2);
	skills.push_back(s3);
	
	player1.fretting = new Fretting(&skills);	
	player1.track = new Track(smgr,driver,15, -20);
	player2.fretting = new Fretting();
	player2.track = new Track(smgr,driver,2, 20);
	
	player1.fretting->tolerance = 1;
	player2.fretting->tolerance = 1;
	player1.fretting->musicTime = &musicTime;
	player2.fretting->musicTime = &musicTime;
	
//	receiver = new eventReceiver();
	receiver.player1 = &player1;
	receiver.player2 = &player2;
	
	EKEY_CODE eventos[NUMBER_OF_FRETS] = { irr::KEY_F1, irr::KEY_F2, irr::KEY_F3, irr::KEY_F4, irr::KEY_F5 };
	
	player1.fretting->setEvents(eventos);
	
	screen = new Screen(device,driver);
	
	theMusic = decoder.decodeMidi(defaultFile, EXPERT);
	//decoder.printMusic(theMusic);
		
	/*
	//initializes the matrix (for drawer)
	for(int lin=0; lin<SCREEN_Y; lin++)
		for(int col=0; col<5; col++)
			theScreen[lin][col] = NOTHING;*/
			
	// start getting signals, baby
	receiver.enabled = true;
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
	
	/*scene::ILightSceneNode *light = */smgr->addLightSceneNode(0, vector3df(0,-80,-30), video::SColorf(1.0f, 1.0f, 1.0f), 20.0f);
	//light->setLightType(video::ELT_DIRECTIONAL);
	//light->setRotation(vector3df(-90,0,0));
	
	//smgr->addSkyDomeSceneNode( driver->getTexture("img/rockwall.jpg") );
	
    // like the real game camera
    camera = smgr->addCameraSceneNode (
				0,					  // Camera parent
				vector3df(0, -90, -40), // Look from
				vector3df(0, -30, 20), // Look to
				1);						  // Camera ID
	
	/*
	// debug camera
	camera = smgr->addCameraSceneNode (
				0,					  // Camera parent
				vector3df(0, -150, -300), // Look from
				vector3df(0, -150, 1), // Look to
				1);						  // Camera ID				
				*/

}

int main(int argc, char *argv[])
{
	/*
	 * initializing the sound engine
	 */

	FMOD_RESULT result;
	FMOD::System *system;
	
	result = FMOD::System_Create(&system);		// Create the main system object.
	ERRCHECK(result)
	
	result = system->init(100, FMOD_INIT_NORMAL, 0);	// Initialize FMOD.
	ERRCHECK(result)

	// loads the sound file on the memory
	FMOD::Sound *song, *guitar;
	result = system->createSound(songFile.c_str(), FMOD_DEFAULT, 0, &song);
	if(guitarFile.size()>0)
		system->createSound(guitarFile.c_str(), FMOD_DEFAULT, 0, &guitar);
	ERRCHECK(result);
	
	/*
	 * initializing the graphics engine
	 */
	initializeIrrlicht();
	/*
	 * initializing game engine
	 */
	musa_init();
	
	FMOD::Channel *channel;
	// plays the ogg (TREMENDOUSLY REDUCES FPS D=) (seriously?) (actually, not)
	result = system->playSound(FMOD_CHANNEL_FREE, song, false, &channel);
	if(guitarFile.size()>0)
		system->playSound(FMOD_CHANNEL_FREE, guitar, false, &channel);
	ERRCHECK(result);

	/*
	 * initializing threads
	 */
	sem_init(&semaphore, 0, 1);
	
	pthread_t thread[2];
	int arg = 1;
	pthread_create(&thread[0], NULL, updater, (void *) arg);
	//pthread_create(&thread[1], NULL, drawer, (void *) arg);
	
	/* 
	 * Irrlicht Main Loop
	 */	
	while(device->run()) {
		
		driver->beginScene(true, true, video::SColor(255,113,113,133));

		// mutex must be 2
		sem_wait(&semaphore);
		smgr->drawAll(); // draw the stones sceneNodes
		player1.track->draw();
		player2.track->draw();
		sem_post(&semaphore);
		
		screen->update();
		device->getGUIEnvironment()->drawAll(); // draw the gui environment

		driver->endScene();

		// FPS
		int fps = driver->getFPS();
		static int lastFPS = 0;
		if (lastFPS != fps) {
			core::stringw tmp(L"fps: ");
			tmp += fps;
			screen->fpsText->setText(tmp.c_str());
			lastFPS = fps;
		}
	}
	
	/*
	 * End the game gracefully =D
	 */
	// wait for threads to complete
	pthread_join(thread[0], NULL);
	
	return 0;
}

////////////////////////////////////////////////////////////////////////
// Debuging function. Saved because may be useful later.
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
