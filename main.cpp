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
#include "SoundBank.h"
#include "Instrument.h"
#include "gui.h"

core::array<SJoystickInfo> joystickInfo;



// Irrlicht globals
IrrlichtDevice 				*device=0;
video::IVideoDriver 		*driver=0;
scene::ISceneManager 		*smgr=0;
IGUIEnvironment				*env=0;
scene::ICameraSceneNode 	*camera=0;
EventReceiver 				*receiver;

ITexture					*glowTex;

// MusA globals
Decoder 					decoder;
Screen						*screen;
music			 			*theMusic[NDIFFICULTIES], selMusic;
difficultyType				diffy;
Player 						player[2];
SkillBank					skillBank;
SoundBank					*soundBank;
SAppContext 				musaGui;

bool 						isMusicPlaying =false;
double 						musicTime;

// Other global
sem_t semaphore;
sem_t receiverSemaphore;

void castSpell ()
{
	for (int i = 0; i < 2; i++)
	{
		if (((player[i].fretting->castedSpell) != NULL))
		{
			Skill *casted = player[i].fretting->castedSpell;

			if (player[i].stamina > casted->cost)
			{			
				player[i].changeStamina (-casted->cost);
			
                // sound effect
				soundBank->playEffect(casted->soundEffect); 
				
				bool isEnemyMirrored = false;
				for (unsigned int j = 0; j < player[!i].status.size(); j++)
					if (player[!i].status[j].status == ST_MIRROR)
						isEnemyMirrored = true;
						
                if( isEnemyMirrored ) {
                    player[i].fretting->castedSpell = NULL;
                    i = !i;
                }

				// visual effects
				switch(casted->targetType)
				{
					case ATTACK:
						screen->effectFactory->queueEffect(0, casted->effectFunction, !i);
						break;
					case DEFENSE:
						screen->effectFactory->queueEffect(0, casted->effectFunction, i);
						break;
				}

				for (unsigned int j = 0; j < casted->effects.size(); j++)
				{
					switch (casted->effects[j].type)
					{
					case T_DEFENSE_UP:
						player[i].changeArmor( casted->effects[j].param );
						break;
					case T_INVENCIBLE:
						player[i].addStatus( statusType(ST_INVENCIBLE, casted->effects[j].param) );
						break;
					case T_MAGIC_BARRIER:
						player[i].addStatus( statusType(ST_MAGIC_BARRIER, casted->effects[j].param) );
						break;
					case T_POISONOUS:
						player[!i].addStatus( statusType(ST_POISON, casted->effects[j].param) );
						break;
					case T_DAMAGE:
						player[!i].takeDamage( casted->effects[j].param);
						break;
					case T_DEFENSE_DOWN:
						player[!i].changeArmor( - casted->effects[j].param );
						break;
					case T_HEAL:
						player[i].recoverHP(casted->effects[j].param);
						break;
					case T_ANTIDOTE:
						player[i].setStatusNormal();
						break;
					case T_STAMINA_DOWN:
						player[!i].changeStamina( -casted->effects[j].param );
						break;
					case T_BURN:
						player[!i].addStatus( statusType(ST_FIRE, casted->effects[j].param) );
						break;
					case T_FEEDBACK:
						player[!i].takeDamage (player[!i].stamina/2);
						player[!i].stamina = 0;
						break;
					case T_TOLERANCE_DOWN:
						player[!i].fretting->tolerance -= casted->effects[j].param;
						break;
					case T_ELETRIFY:
						player[!i].addStatus( statusType(ST_ELETRIFIED, casted->effects[j].param) );
						break;
					case T_DROWN:
						player[!i].addStatus( statusType(ST_DROWNED, casted->effects[j].param) );
						break;
					case T_MIRROR:
						player[i].addStatus( statusType(ST_MIRROR, casted->effects[j].param) );
						break;
					case T_SPEED_UP:
						player[!i].track->setSpeed( player[!i].track->getSpeed() + casted->effects[j].param );
						break;		
					case T_CHAOTIC_SPEED:
						player[!i].addStatus( statusType(ST_CHAOTIC_SPEED, casted->effects[j].param) );
						break;		
					case T_BREAK_DEFENSE:
						player[!i].addStatus( statusType(ST_BROKEN_DEFENSE, casted->effects[j].param) );
						break;
					case T_CLEAR_STONES:
						player[i].track->destroyAllStones();
						break;
					case T_FREEZE:
						player[!i].addStatus( statusType(ST_FROZEN, casted->effects[j].param) );
						break;
					case T_VAMPIRIC:
						player[i].recoverHP( player[!i].takeDamage(casted->effects[j].param) );
						break;
					case T_BLESS:
						player[i].addStatus( statusType(ST_BLESSED, casted->effects[j].param) );
						break;
					case T_STAMINA_UP:
						player[i].changeStamina( casted->effects[j].param );
						break;
					case T_CURSE:
						player[!i].addStatus( statusType(ST_CURSED, casted->effects[j].param) );
						break;
					
					case T_EFFECTS_TOTAL: break;
					}
				}
			}
		}
		player[i].fretting->castedSpell = NULL;
	}
}

void handleHittingStates()
{
	for(int i=0; i<NPLAYERS; i++) 
	{
		switch( player[i].fretting->frettingState )
		{
			case 1:
				break;
			case -1:
				//soundBank->playMissEffect();
				//player[i].fretting->frettingState = 0;
				break;
			case 0:
			case -2:
				break;
		}
	}
}	

static void *updater(void *argument) 
{
	struct	timeval start, endMusic;
	bool	havetoEndMusic = false,
			endMusicFirstTime = true;
	double	endMusicOffset;
	
	// get the time before starting the music (so we can know how much time passed in each note)
	gettimeofday(&start, NULL);
	  
	 while(1) {
		usleep(1);
		musicTime = time_diff(start);
		
		if (player[0].HP == 0 || player[1].HP == 1 || havetoEndMusic)
		{
			havetoEndMusic = true;
			if (endMusicFirstTime)
			{
				endMusicFirstTime = false;
				gettimeofday(&endMusic, NULL);
			}
			
			endMusicOffset = time_diff(endMusic);
			if (endMusicOffset > 3.)
			{
				cout << "THANKS FOR PLAYING MUSA - TAOLITERODS" << endl;
				device->closeDevice();
			}
		}
 
		// spawning on track1
		while( (unsigned int)player[0].track->musicPos < selMusic.size() &&
			   (musicTime + player[0].track->spawnDelay) > selMusic[player[0].track->musicPos].time ) {
			
			player[0].track->processEvent(selMusic[player[0].track->musicPos]);
		}
		
		// spawning on track2
		while( (unsigned int)player[1].track->musicPos < selMusic.size() &&
			   (musicTime + player[1].track->spawnDelay) > selMusic[player[1].track->musicPos].time) {
			
			player[1].track->processEvent(selMusic[player[1].track->musicPos]);
		}
		
		handleHittingStates();
		sem_wait(&semaphore);
			if (player[1].useAI == true && !player[1].isFrozen)
			{
				double chance = rand() % 30000;
				if (chance == 0)
				{
					int random = rand() % (player[1].instrument->skills.size());
					player[1].fretting->castedSpell = &(player[1].instrument->skills[random]);	
				}
			}
			
			castSpell();
			
			while (receiver->getEventsSize() != 0)
			{
				player[0].updateEvents();
				player[1].updateEvents();
				receiver->removeEvent();
			}
			player[0].update();
			player[1].update();
			//if ((player[0].gotAnEvent == 0) && (player[1].gotAnEvent == 0))
				//receiver.removeEvent();
		sem_post(&semaphore);
		
	}
	
	return NULL;
}

bool loadNotes( std::string path )
{
	musaGui.box->clear();
	
	int ndiffys=0;
	for(int i=0; i<NDIFFICULTIES; i++) {
		theMusic[i] = decoder.decodeMidi(path, (difficultyType)i);
		if(theMusic[i]) {
			++ndiffys;
			switch(i) {
				case EXPERT:
					musaGui.box->addItem(L"Expert", EXPERT);
					break;
				case HARD:
					musaGui.box->addItem(L"Hard", HARD);
					break;
				case MEDIUM:
					musaGui.box->addItem(L"Medium", MEDIUM);
					break;
				case EASY:
					musaGui.box->addItem(L"Easy", EASY);
					break;
			}
		}
	}
	
	if(ndiffys==0)
		return false;
	else
		return true;
}

void loadSong( std::string path, int which )
{
	switch(which)
	{
		case 0:
			soundBank->openSongFile(path.c_str());
			break;
		case 1:
			soundBank->openGuitarFile(path.c_str());
			break;
	}
}

void startGame( int difficulty, bool useAI)
{	
	selMusic = *theMusic[difficulty];
	screen->musicTotalTime = selMusic.back().time;
	decoder.printMusic(selMusic);
	
	soundBank->stop();
	
	player[1].useAI = useAI;
	
	/*
	 * initializing threads
	 */
	pthread_t thread[3];
	int arg = 1;
	pthread_create(&thread[0], NULL, updater, (void *) arg);
	//pthread_create(&thread[1], NULL, debugger, (void *) arg);
	//pthread_create(&thread[2], NULL, drawer, (void *) arg);
	
	soundBank->playSelectedMusic();
	
	screen->screenFader->fadeIn(1000);
}

void initMusa()
{		
	player[0].track = new Track(&selMusic,&musicTime,device,23, -18);
	player[1].track = new Track(&selMusic,&musicTime,device,23, 18);

	// TEMPORARY
	Instrument* violin = new Instrument();
	Instrument* drums = new Instrument();
	
	for(int i=0; i<SKILLS_TOTAL; i++) {
		drums->addSkill( skillBank.skills[i] );
		violin->addSkill( skillBank.skills[i] );
	}
		
	player[0].setInstrument(violin);
	player[1].setInstrument(drums);
	
	player[0].fretting->musicTime = &musicTime;
	player[1].fretting->musicTime = &musicTime;
	
	player[0].fretting->receiver = receiver;
	player[1].fretting->receiver = receiver;
	

	if(device->activateJoysticks(joystickInfo)) {
		cout << "Joystick support is enabled and " << joystickInfo.size() << " joystick(s) are present." << endl;
		for(u32 joystick = 0; joystick < joystickInfo.size(); ++joystick) {
			cout << "Joystick " << joystick << ":" << endl;
			cout << "\tName: '" << joystickInfo[joystick].Name.c_str() << "'" << endl;
			cout << "\tAxes: " << joystickInfo[joystick].Axes << endl;
			cout << "\tButtons: " << joystickInfo[joystick].Buttons << endl;
		}
	}
	else
		cout << "Joystick support is not enabled." << endl;

	
	EKEY_CODE eventsKeyboard1[NFRETS] = { irr::KEY_KEY_A, irr::KEY_KEY_S, irr::KEY_KEY_J, irr::KEY_KEY_K, irr::KEY_KEY_L };
	EKEY_CODE eventsKeyboard2[NFRETS] = { irr::KEY_KEY_Q, irr::KEY_KEY_W, irr::KEY_KEY_U, irr::KEY_KEY_I, irr::KEY_KEY_O };
	//int eventsJoystick1[NFRETS] = {4,6,7,5,2};
	//int eventsJoystick2[NFRETS] = {4,6,7,5,2};

	player[0].fretting->setEvents(eventsKeyboard1, irr::KEY_SPACE );
	//player[0].fretting->setEvents(eventsJoystick1, joystickInfo, 0, 3);	//comment this line to use keyboard for player 1
	player[1].fretting->setEvents(eventsKeyboard2, irr::KEY_KEY_C );
	//player[1].fretting->setEvents(eventsJoystick2, joystickInfo, 0, 3);	//comment this line to use keyboard for player 2
	
	screen = new Screen(device,&musicTime,&player[0],&player[1]);
	sem_init(&semaphore, 0, 1);
	

	//////////
	// GUI!!
	//////////
	
	#define WINSIZEX SCREENX/6.
	#define WINSIZEY SCREENY/2.7
	
	// background image
	musaGui.mainBg = env->addImage( rect<s32>(position2di(0, 0),dimension2di(SCREENX,SCREENY)) );
	musaGui.mainBg->setScaleImage(true);
	musaGui.mainBg->setImage( driver->getTexture("img/bgs/main.jpg") );	
	
	// main window
	IGUIWindow *window = env->addWindow( recti(position2di(640, 100),dimension2di(WINSIZEX,WINSIZEY)), false, 0, 0, GUI_INITIAL_MENU );
	window->setDraggable(false);
	//window->setDrawBackground(false);
	window->setDrawTitlebar(false);
	musaGui.mainWindow = window;
	
	// load file buttons
	env->addButton(rect<s32>(position2di(WINSIZEX/2-100/2, 50-32/2),dimension2di(100,32)), window, GUI_ID_LOAD_NOTES, L"Load MIDI", L"Loads a Fret's on Fire music file");
	env->addButton(rect<s32>(position2di(WINSIZEX/2-100/2, 90-32/2),dimension2di(100,32)), window, GUI_ID_LOAD_MUSIC1, L"Load Song File 1", L"Loads the main song's audio file (usually all lines except guitar)");
	env->addButton(rect<s32>(position2di(WINSIZEX/2-100/2, 130-32/2),dimension2di(100,32)), window, GUI_ID_LOAD_MUSIC2, L"Load Song File 2", L"Loads an optional secondary song's audio file (usually the guitar line)");
	
	// difficulty combobox
	musaGui.box = env->addComboBox(rect<s32>(position2di(WINSIZEX/2-100/2, 195-20/2),dimension2di(100,20)), window);
	musaGui.box->setEnabled(false);
	// AI checkbox
	musaGui.aiCheck = env->addCheckBox(true, rect<s32>(position2di(WINSIZEX/2-20/2-40, 220-20/2),dimension2di(20,20)), window);
	env->addStaticText(L"Play against AI?", rect<s32>(position2di(WINSIZEX/2+25-100/2, 220-20/2),dimension2di(100,20)), false, true, window);
	// start button
	musaGui.startButton = env->addButton(rect<s32>(position2di(WINSIZEX/2-100/2, 250-32/2),dimension2di(100,32)), window, GUI_ID_START_BUTTON, L"Start", L"Starts the game");
	musaGui.startButton->setEnabled(false);
	
	// game functions to be accessed by the GUI
	musaGui.startGame = &startGame;
	musaGui.loadSong = &loadSong;
	musaGui.loadNotes = &loadNotes;
}

void initIrrlicht()
{
	// Graphical engine initializing
	device = createDevice( video::EDT_OPENGL, core::dimension2d<u32>(SCREENX,SCREENY), 32,
							false, //fullscreen?
							false, //used stencil buffer?
							false //use vsync?
						);
	device->setResizable(false);
	device->setWindowCaption(L"Musical Arena (MusA)");

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	env = device->getGUIEnvironment();
	
	// GUI
	IGUISkin* skin = env->getSkin();
	IGUIFont* font = env->getFont("irrlicht-1.7.2/media/fonthaettenschweiler.bmp");
	skin->setFont(font);
	skin->setFont(env->getBuiltInFont(), EGDF_TOOLTIP);
	/*for (s32 i=0; i<irr::gui::EGDC_COUNT ; ++i) {
		video::SColor col = skin->getColor((EGUI_DEFAULT_COLOR)i);
		col.setAlpha(200);
		skin->setColor((EGUI_DEFAULT_COLOR)i, col);
	}*/
	
	musaGui.device = device;
	musaGui.state = GUI_MAIN_MENU;
	
	receiver = new EventReceiver(&musaGui);	
	device->setEventReceiver(receiver);
	
    
    // like the real game camera
    camera = smgr->addCameraSceneNode (
				0,					  // Camera parent
				vector3df(0, -95, -35), // Look from
				vector3df(0, -30, 15), // Look to
				1);						  // Camera ID
	//camera->setFOV(PI/3);
	
	// a FPS camera for debugging
	//camera = smgr->addCameraSceneNodeFPS(); device->getCursorControl()->setVisible(false);
}

static void *debugger (void *argument)
{
	while(1)
	{
		player[0].fretting->printHitFret();
		cout << "vetor.size(): " << receiver->getEventsSize() << endl;
		usleep(80000);
	}

	return 0;
}

int main(int argc, char *argv[])
{
	srand(time(0));
	
	/*
	 * initializing the sound engine
	 */
	soundBank = new SoundBank(device);
	soundBank->playTheme();
	
	/*
	 * initializing the graphics engine
	 */
	initIrrlicht();
	
	
	/*
	 * initializing game engine
	 */
	initMusa();
		

	ITexture *bgPic = driver->getTexture( bgsBank[2] );
	SColor bgColor = SColor(0,113,113,133);
	/* 
	 * Irrlicht Main Loop
	 */	
	while(device->run()) {		
		driver->beginScene(true, true, bgColor);
			// draw background
			driver->draw2DImage( bgPic, recti(position2di(0,0), position2di(SCREENX,SCREENY)), recti(position2di(0,0),bgPic->getOriginalSize()) );

			sem_wait(&semaphore);
				driver->setRenderTarget(0, false, true, bgColor);
				player[0].track->draw();
				player[1].track->draw();
				smgr->drawAll();		
				screen->update();
			sem_post(&semaphore);
			
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
	// 	at the same time, both are editing selMusic vector.
	// we solve the problem simply creating a copy because there's no
	//  apparent reason need for them to be synchronized.
	vector<musicEvent> selMusicCopied = selMusic;


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
			 << "upcoming event: " << selMusicCopied[0].time << endl;
 
		// propagates the lines of the matrix	
		for(int lin=SCREEN_Y-1; lin>0; lin--)
			for(int col=0; col<NFRETS; col++)
				theScreen[lin][col] = theScreen[lin-1][col];

		while( ((musicTime + STONE_DELAY) > selMusicCopied[0].time) && !endOfMusic) {
			// updates the first line of the matrix with the actual configuration
			switch(selMusicCopied[0].type) {
				case ON:
					theScreen[0][selMusicCopied[0].button] = STRIKE;
					break;
				case OFF:	
					theScreen[0][selMusicCopied[0].button] = NOTHING;		
					break;
				default: break;
			}
			
			// avoiding segmentation faults =D
			if(selMusicCopied.size() > 0)
				selMusicCopied.erase(selMusicCopied.begin());
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
