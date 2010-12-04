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
#include "SkillBank.h"
#include "PostProcessBloom.h"
#include "PostProcessRadialBlur.h"
#include "PostProcessInvert.h"

core::array<SJoystickInfo> joystickInfo;


// Irrlicht globals
IrrlichtDevice 				*device=0;
video::IVideoDriver 		*driver=0;
scene::ISceneManager 		*smgr=0;
IGUIEnvironment				*env=0;
scene::ICameraSceneNode 	*camera=0;
scene::ICameraSceneNode		*aboxCam=0;
EventReceiver 				receiver;

// Post process effects
IPostProcessBloom 			*bloom;
IPostProcessRadialBlur 		*blur;
IPostProcessInvert 			*invert;
bool						bloomEffect=false, blurEffect=false, invertEffect=false;

ITexture					*glowTex;

// MusA globals
Decoder 					decoder;
Screen						*screen;
vector<musicEvent> 			theMusic;
Player 						player1, player2;
SkillBank					skillBank;

bool 						endOfMusic =false; // indicates the end of the music. must be implemented to be turned "true" when ogg file ends its playing.
double 						musicTime =0;

// Other globals
sem_t semaphore;
sem_t receiverSemaphore;

struct musicLibEntry
{
	std::string notes, song, guitar;
	difficultyType difficulty;
};

const musicLibEntry musicLib[] = { {"music/api/notes.mid", "music/api/song.ogg", "music/api/guitar.ogg", HARD},
							       {"music/example/notes.mid", "music/example/song.ogg", "", EXPERT},
							       {"music/roundabout/notes.mid", "music/roundabout/song.ogg", "", HARD}
							  };
const musicLibEntry selectedSong = musicLib[0];



static void *updater(void *argument) 
{
	struct timeval start; 
	
	// get the time before starting the music (so we can know how much time passed in each note)
	gettimeofday(&start, NULL);
	  
	 while( !endOfMusic ) {
		usleep(1);
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
		//receiver.clearEvents();
		sem_post(&semaphore);
		
	}
	
	return NULL;
}


void musa_init()
{	
	player1.addSkill( skillBank.skills[FIREBALL] );
	player1.addSkill( skillBank.skills[CURE] );

	player1.fretting = new Fretting( &player1.skills );
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


        if(device->activateJoysticks(joystickInfo))
        {
                cout << "Joystick support is enabled and " << joystickInfo.size() << " joystick(s) are present." << endl;

                for(u32 joystick = 0; joystick < joystickInfo.size(); ++joystick)
                {
                        cout << "Joystick " << joystick << ":" << endl;
                        cout << "\tName: '" << joystickInfo[joystick].Name.c_str() << "'" << endl;
                        cout << "\tAxes: " << joystickInfo[joystick].Axes << endl;
                        cout << "\tButtons: " << joystickInfo[joystick].Buttons << endl;
                }
        }
        else
        {
                cout << "Joystick support is not enabled." << endl;
        }

	
	//cout << "vai passar" << endl;
	//receiver.semaphore = &receiverSemaphore;
	//cout << "passou" << endl;
	//sem_init(&receiverSemaphore, 0, 1);
	
	EKEY_CODE eventos1[NFRETS] = { irr::KEY_KEY_A, irr::KEY_KEY_S, irr::KEY_KEY_J, irr::KEY_KEY_K, irr::KEY_KEY_L };
	EKEY_CODE eventos2[NFRETS] = { irr::KEY_KEY_Q, irr::KEY_KEY_W, irr::KEY_KEY_U, irr::KEY_KEY_I, irr::KEY_KEY_O };
	int events1[NFRETS] = {0,1,2,3,4};

	player1.fretting->setEvents(eventos1);
	//player1.fretting->setEvents(events1, joystickInfo, 0);	//comment this line to use keyboard for player 1
	player2.fretting->setEvents(eventos2);
	
	screen = new Screen(device,&player1,&player2);
	
	theMusic = decoder.decodeMidi(selectedSong.notes, selectedSong.difficulty);
	//decoder.printMusic(theMusic);
					
	// start getting signals, baby
	receiver.enabled = true;	
}

void initializePostProcessEffects()
{
	// "dream" effect
	bloom = new IPostProcessBloom(smgr->getRootSceneNode(), smgr, 666);
    PPE_Bloom_Setup setup;
    setup.sampleDist=0.008;
    setup.strength=3;
    setup.multiplier=1;
    bloom->initiate(1024,512,setup,smgr);
    
    // radial blur ("speed" effect)
    blur = new IPostProcessRadialBlur(smgr->getRootSceneNode(), smgr, 666);
    blur->initiate(1024,512,0.8,2,smgr);	
    
    // invert colors
    invert = new IPostProcessInvert(smgr->getRootSceneNode(), smgr, 666);
    invert->initiate(1024,512,smgr);
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
	env = device->getGUIEnvironment();
	
	IGUISkin* skin = env->getSkin();
	IGUIFont* font = env->getFont("img/fonthaettenschweiler.bmp");
	
	skin->setFont(font);

	/*scene::ILightSceneNode *light = */smgr->addLightSceneNode(0, vector3df(0,-80,-30), video::SColorf(1.0f, 1.0f, 1.0f), 20.0f);
	//light->setLightType(video::ELT_DIRECTIONAL);
	//light->setRotation(vector3df(-90,0,0));
	
	/*ISceneNode *sky = smgr->addSkyDomeSceneNode( driver->getTexture("img/stars.tga"));
	sky->setMaterialFlag(video::EMF_TEXTURE_WRAP, video::ETC_REPEAT);
	sky->getMaterial(0).getTextureMatrix(0).setTextureScale(5,5);*/
	
    // lol quake scenario
    /*device->getFileSystem()->addZipFileArchive("map-20kdm2.pk3");
    IAnimatedMesh* mesh = smgr->getMesh("20kdm2.bsp");
    ISceneNode* node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024);
    node->setRotation( vector3df(280,0,0) );
    node->setPosition( vector3df(-1900,-500,400) );*/

    
    // like the real game camera
    camera = smgr->addCameraSceneNode (
				0,					  // Camera parent
				vector3df(0, -90, -40), // Look from
				vector3df(0, -30, 20), // Look to
				1);						  // Camera ID
	
	// a FPS camera for debugging
	//camera = smgr->addCameraSceneNodeFPS(); device->getCursorControl()->setVisible(false);
	
	aboxCam = smgr->addCameraSceneNode(0, core::vector3df(0,0,-120), core::vector3df(0,0,1));
	smgr->setActiveCamera(camera);

	initializePostProcessEffects();
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

void renderPostProcessEffects()
{
	if(bloomEffect)
		bloom->render();
	if(blurEffect)
		blur->render();
	if(invertEffect)
		invert->render();
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
	FMOD::Sound *song, *guitar=0;
	result = system->createSound(selectedSong.song.c_str(), FMOD_DEFAULT, 0, &song);
	if(selectedSong.guitar.size()>0)
		system->createSound(selectedSong.guitar.c_str(), FMOD_DEFAULT, 0, &guitar);
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
	if(guitar)
		system->playSound(FMOD_CHANNEL_FREE, guitar, false, &channel);
	ERRCHECK(result);	
	
	SColor bgColor = SColor(255,113,113,133);
	/* 
	 * Irrlicht Main Loop
	 */	
	while(device->run()) {
		
		driver->beginScene(true, true, bgColor);

		//sem_wait(&semaphore);
		
		/*driver->setRenderTarget(screen->bloodSplit[0], true, true, 0);
			smgr->setActiveCamera(aboxCam);
			screen->abox->setVisible(true);
			screen->abox->render();
			screen->abox->setVisible(false);
			smgr->setActiveCamera(camera);*/
		if(bloomEffect) {
			driver->setRenderTarget(bloom->rt0, true, true, bgColor); 
				smgr->drawAll();
		}
		if(blurEffect) {
			driver->setRenderTarget(blur->rt0, true, true, bgColor);
				smgr->drawAll();
		}
		if(invertEffect) {
			driver->setRenderTarget(invert->rt0, true, true, bgColor);
				smgr->drawAll();
		}
		driver->setRenderTarget(0, true, true, bgColor);
			renderPostProcessEffects();
			smgr->drawAll();
			player1.track->drawStoneTrails();
			player2.track->drawStoneTrails();
		
		//sem_post(&semaphore);
		
		screen->update();
		
		env->drawAll();

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
