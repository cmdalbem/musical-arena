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


// Irrlicht globals
IrrlichtDevice 				*device=NULL;
video::IVideoDriver 		*driver=NULL;
scene::ISceneManager 		*smgr=NULL;
scene::ICameraSceneNode 	*camera=NULL;
EventReceiver 				receiver;

ITexture					*glowTex;

// MusA globals
Decoder 					decoder;
Screen						*screen;
vector<musicEvent> 			theMusic;
Player 						player1, player2;

bool 						endOfMusic =false; // indicates the end of the music. must be implemented to be turned "true" when ogg file ends its playing.
double 						musicTime =0;

// Other globals
sem_t semaphore;
sem_t receiverSemaphore;

std::string defaultFile = "music/example.mid",
			songFile = "music/example.ogg",
			guitarFile = "";



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
		player1.update();
		player2.update();
		if ((player1.gotAnEvent == 0) && (player2.gotAnEvent == 0))
			receiver.removeEvent();
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
	player1.track = new Track(&theMusic,&musicTime,device,23, -20);
	player2.fretting = new Fretting();
	player2.track = new Track(&theMusic,&musicTime,device,10, 20);
	
	double tolerance = 0.2;
	
	player1.track->fretting = player1.fretting;
	player2.track->fretting = player2.fretting;
	
	player1.track->tolerance = tolerance;
	player2.track->tolerance = tolerance;
	player1.fretting->tolerance = tolerance;
	player2.fretting->tolerance = tolerance;
	player1.fretting->musicTime = &musicTime;
	player2.fretting->musicTime = &musicTime;
	
	player1.fretting->receiver = &receiver;
	player2.fretting->receiver = &receiver;
	
	//cout << "vai passar" << endl;
	//receiver.semaphore = &receiverSemaphore;
	//cout << "passou" << endl;
	//sem_init(&receiverSemaphore, 0, 1);
	
	EKEY_CODE eventos1[NFRETS] = { irr::KEY_KEY_A, irr::KEY_KEY_S, irr::KEY_KEY_J, irr::KEY_KEY_K, irr::KEY_KEY_L };
	EKEY_CODE eventos2[NFRETS] = { irr::KEY_KEY_Q, irr::KEY_KEY_W, irr::KEY_KEY_U, irr::KEY_KEY_I, irr::KEY_KEY_O };
	player1.fretting->setEvents(eventos1);
	player2.fretting->setEvents(eventos2);
	cout << "vetor de eventos" << endl;
	
	screen = new Screen(device,&player1,&player2);
	
	theMusic = decoder.decodeMidi(defaultFile, EXPERT);
	//decoder.printMusic(theMusic);
					
	// start getting signals, baby
	receiver.enabled = true;
}

void initializeIrrlicht()
{
// Graphical engine initializing
	device = createDevice( video::EDT_OPENGL, core::dimension2d<u32>(SCREENX,SCREENY), 32,
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

static void *debugger (void *argument)
{
	while(1)
	{
		player1.fretting->printHitFret();
		usleep(80000);
	}

	return 0;
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

	// load the sound files
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
	
	
	/*
	 * initializing threads
	 */
	sem_init(&semaphore, 0, 1);
	
	pthread_t thread[3];
	int arg = 1;
	pthread_create(&thread[0], NULL, updater, (void *) arg);
	pthread_create(&thread[1], NULL, debugger, (void *) arg);
	//pthread_create(&thread[2], NULL, drawer, (void *) arg);
	

	/*
	 * start playing the music
	 */
	FMOD::Channel *channel;
	result = system->playSound(FMOD_CHANNEL_FREE, song, false, &channel);
	if(guitarFile.size()>0)
		system->playSound(FMOD_CHANNEL_FREE, guitar, false, &channel);
	ERRCHECK(result);	
	
	
	/* 
	 * Irrlicht Main Loop
	 */	
	while(device->run()) {
		
		driver->beginScene(true, true, video::SColor(255,113,113,133));

		sem_wait(&semaphore);
		smgr->drawAll();
		screen->shieldmanager->drawAll();
		player1.track->draw();
		player2.track->draw();
		sem_post(&semaphore);
		
		screen->update();
		
		device->getGUIEnvironment()->drawAll();

		driver->endScene();

		// FPS
		int fps = driver->getFPS();
		static int lastFPS = 0;
		if (lastFPS != fps) {
			screen->setFps(fps);
			lastFPS = fps;
		}
	}
	
	
	/*
	 * End the game gracefully =D
	 */	
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


	//initializes the matrix
	for(int lin=0; lin<SCREEN_Y; lin++)
		for(int col=0; col<5; col++)
			theScreen[lin][col] = NOTHING;

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
			for(int col=0; col<NFRETS; col++)
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
			for(int col=0; col<NFRETS; col++)
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
