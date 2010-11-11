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
#include "eventReceiver.h"
#include "Fretting.h"
#include "Track.h"
#include "Player.h"


Decoder decoder; 

vector<musicEvent> theMusic;

// indicates the end of the music. must be implemented to be turned "true" when ogg file ends its playing.
bool endOfMusic=false;

double musicTime = 0;

Player player1, player2;

int mutex=0;
sem_t semaphore;

std::string defaultFile = "music/example.mid",
			songFile = "music/example.ogg",
			guitarFile = "";
			
//note theScreen[SCREEN_Y][NUMBER_OF_FRETS];

// Irrlicht-related globals
IrrlichtDevice 				*device;
video::IVideoDriver 		*driver;
scene::ISceneManager 		*smgr;
scene::ICameraSceneNode 	*camera;
eventReceiver 				receiver;
gui::IGUIImage 				*good, *bad;
gui::IGUIStaticText 		*fpsText;


static void *updater(void *argument) 
{
	struct timeval start;
	
	// get the time before starting the music (so we can know how much time passed in each note)
	gettimeofday(&start, NULL);
	
	while( !endOfMusic ) {
		
		musicTime = time_diff(start);// + 20;

		while( (unsigned int)player1.track->musicPos < theMusic.size() &&
			   (musicTime + player1.track->spawnDelay) > theMusic[player1.track->musicPos].time ) {
			
			player1.track->processEvent(theMusic[player1.track->musicPos]);
		}
		
		while( (unsigned int)player2.track->musicPos < theMusic.size() &&
			   (musicTime + player2.track->spawnDelay) > theMusic[player2.track->musicPos].time ) {
			
			player2.track->processEvent(theMusic[player2.track->musicPos]);
		}
		
		//sem_wait(&semaphore);
		player1.track->update(musicTime);
		player2.track->update(musicTime);
		//sem_post(&semaphore);
	}
	
	return NULL;
}

void* fretting (void *arg)
// Poll the keyboard testing if the player has pressed the right notes.
{	
	double tolerance = 1;
	
	while(!endOfMusic) {
		
		int ret;
		
		//sem_wait(&semaphore);
		for (int track = 0; track < 5; track++)
			if(player1.track->stones[track].size() > 0)
				ret = player1.fretting->verify_event(track, player1.track->stones[track][0], musicTime, tolerance);
		//sem_post(&semaphore);

		if(good && bad)
			switch(ret)
			{
				case 1:
					cout<<"acertei"<<endl;
					good->setVisible(true);
					bad->setVisible(false);
					break;
				case -1:
					cout<<"errei"<<endl;
					bad->setVisible(true);
					good->setVisible(false);
					break;
				case 0:
					//bad->setVisible(false);
					//good->setVisible(false);
					break;
			}

		/*
		for (int track = 0; track < 5; track++)
			fretting2.verify_event(track);*/
	}
	
	return NULL;
}

void musa_init()
{
	player1.fretting = new Fretting();
	player1.track = new Track(smgr,driver,10, -20);
	player2.fretting = new Fretting();
	player2.track = new Track(smgr,driver,5, 20);
	
	EKEY_CODE eventos[5] = { irr::KEY_KEY_A, irr::KEY_KEY_S, irr::KEY_KEY_J, irr::KEY_KEY_K, irr::KEY_KEY_L };
	
	player1.fretting->setEvents(eventos);
	player1.fretting->setReceiver(&receiver);
	
	theMusic = decoder.decodeMidi(defaultFile, EXPERT);
	//decoder.printMusic(theMusic);
		
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
	
	fpsText = device->getGUIEnvironment()->addStaticText(L"", core::rect<int>(0, 0, 100, 10));
	good = device->getGUIEnvironment()->addImage( driver->getTexture("img/good.png"), core::position2d<s32>(100,20), true );
	bad = device->getGUIEnvironment()->addImage( driver->getTexture("img/bad.png"), core::position2d<s32>(150,30), true );
	good->setVisible(false);
	bad->setVisible(false);
	//device->getGUIEnvironment()->addButton(core::rect<int>(50,240,110,240 + 32), 0, 0, L"Increase speed", L"Increases speed");
	
	scene::ILightSceneNode *light = smgr->addLightSceneNode(0, vector3df(0,-80,-30), video::SColorf(1.0f, 1.0f, 1.0f), 20.0f);
	//light->setLightType(video::ELT_DIRECTIONAL);
	//light->setRotation(vector3df(-90,0,0));
	
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

		// mutex must be 2
		sem_wait(&semaphore);
		smgr->drawAll(); // draw the stones sceneNodes
		player1.track->draw();
		player2.track->draw();
		sem_post(&semaphore);

		device->getGUIEnvironment()->drawAll(); // draw the gui environment

		driver->endScene();

		// FPS
		int fps = driver->getFPS();
		static int lastFPS = 0;
		if (lastFPS != fps) {
			core::stringw tmp(L"fps: ");
			tmp += fps;
			fpsText->setText(tmp.c_str());
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
