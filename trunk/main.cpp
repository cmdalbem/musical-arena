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
IrrlichtDevice 			*device=0;
IVideoDriver 			*driver=0;
ISceneManager 			*smgr=0;
IGUIEnvironment			*env=0;
ICameraSceneNode 		*camera=0;
EventReceiver 			*receiver;
ITexture				*mainMenuBg;

ITexture				*glowTex;

// MusA globals
Decoder 				decoder;
Screen					*screen;
music			 		*theMusic[NDIFFICULTIES], selMusic;
difficultyType			difficulty;
Player 					*player[2];
SkillBank				skillBank;
SoundBank				*soundBank;
SAppContext 			musaGui;

bool 					isMusicPlaying =false;
double 					musicTime;

// Other global
sem_t semaphore;
sem_t receiverSemaphore;

void 			handleSpells();
void 			handleHittingStates();
static void 	*updater(void *argument);
bool 			loadNotes( std::string path );
void 			loadSong( std::string path, int which );
void 			startGame( int difficulty, bool useAI);
void 			initGame();
void 			initIrrlicht();
static void 	*debugger (void *argument);

void handleSpells()
{
	for (int i = 0; i < 2; i++)
	{
		if (((player[i]->fretting->castedSpell) != NULL))
		{
			Skill *casted = player[i]->fretting->castedSpell;

			if (player[i]->stamina > casted->cost)
			{			
				player[i]->changeStamina (-casted->cost);
			
                // sound effect
				soundBank->playEffect(casted->soundEffect); 
				
				bool isEnemyMirrored = false;
				for (unsigned int j = 0; j < player[!i]->status.size(); j++)
					if (player[!i]->status[j].status == ST_MIRROR)
						isEnemyMirrored = true;
						
                if( isEnemyMirrored ) {
                    player[i]->fretting->castedSpell = NULL;
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
						player[i]->changeArmor( casted->effects[j].param );
						break;
					case T_INVENCIBLE:
						player[i]->addStatus( statusType(ST_INVENCIBLE, casted->effects[j].param) );
						break;
					case T_MAGIC_BARRIER:
						player[i]->addStatus( statusType(ST_MAGIC_BARRIER, casted->effects[j].param) );
						break;
					case T_POISONOUS:
						player[!i]->addStatus( statusType(ST_POISON, casted->effects[j].param) );
						break;
					case T_DAMAGE:
						player[!i]->takeDamage( casted->effects[j].param);
						break;
					case T_DEFENSE_DOWN:
						player[!i]->changeArmor( - casted->effects[j].param );
						break;
					case T_HEAL:
						player[i]->recoverHP(casted->effects[j].param);
						break;
					case T_ANTIDOTE:
						player[i]->setStatusNormal();
						break;
					case T_STAMINA_DOWN:
						player[!i]->changeStamina( -casted->effects[j].param );
						break;
					case T_BURN:
						player[!i]->addStatus( statusType(ST_FIRE, casted->effects[j].param) );
						break;
					case T_FEEDBACK:
						player[!i]->takeDamage (player[!i]->stamina/2);
						player[!i]->stamina = 0;
						break;
					case T_TOLERANCE_DOWN:
						player[!i]->fretting->tolerance -= casted->effects[j].param;
						break;
					case T_ELETRIFY:
						player[!i]->addStatus( statusType(ST_ELETRIFIED, casted->effects[j].param) );
						break;
					case T_DROWN:
						player[!i]->addStatus( statusType(ST_DROWNED, casted->effects[j].param) );
						break;
					case T_MIRROR:
						player[i]->addStatus( statusType(ST_MIRROR, casted->effects[j].param) );
						break;
					case T_SPEED_UP:
						player[!i]->track->setSpeed( player[!i]->track->getSpeed() + casted->effects[j].param );
						break;		
					case T_CHAOTIC_SPEED:
						player[!i]->addStatus( statusType(ST_CHAOTIC_SPEED, casted->effects[j].param) );
						break;		
					case T_BREAK_DEFENSE:
						player[!i]->addStatus( statusType(ST_BROKEN_DEFENSE, casted->effects[j].param) );
						break;
					case T_CLEAR_STONES:
						player[i]->track->destroyAllStones();
						break;
					case T_FREEZE:
						player[!i]->addStatus( statusType(ST_FROZEN, casted->effects[j].param) );
						break;
					case T_VAMPIRIC:
						player[i]->recoverHP( player[!i]->takeDamage(casted->effects[j].param) );
						break;
					case T_BLESS:
						player[i]->addStatus( statusType(ST_BLESSED, casted->effects[j].param) );
						break;
					case T_STAMINA_UP:
						player[i]->changeStamina( casted->effects[j].param );
						break;
					case T_CURSE:
						player[!i]->addStatus( statusType(ST_CURSED, casted->effects[j].param) );
						break;
					
					case T_EFFECTS_TOTAL: break;
					}
				}
			}
		}
		player[i]->fretting->castedSpell = NULL;
	}
}

void handleHittingStates()
{
	for(int i=0; i<NPLAYERS; i++) 
	{
		switch( player[i]->fretting->frettingState )
		{
			case 1:
				break;
			case -1:
				//soundBank->playMissEffect();
				//player[i]->fretting->frettingState = 0;
				break;
			case 0:
			case -2:
				break;
		}
	}
}	

static void *updater(void *argument) 
{
	double	start, timeMusicEnded;
	bool	havetoEndMusic = false,
			endMusicFirstTime = true;
	double	endMusicOffset;
	ITimer	*timer;
	
	timer = device->getTimer();
	
	// get the time before starting the music (so we can know how much time passed in each note)
	start = timer->getTime()/1000.;
	  
	while(1) {
		usleep(1);
		musicTime = timer->getTime()/1000. - start;
		
		// handle end of the game
		if (havetoEndMusic || player[0]->HP == 0 || player[1]->HP == 0) {
			havetoEndMusic = true;
			if (endMusicFirstTime) {
				screen->showKO();
				endMusicFirstTime = false;
				timeMusicEnded = timer->getTime()/1000.;
			}
			
			endMusicOffset = timer->getTime()/1000. - timeMusicEnded;
			if (endMusicOffset > DELAY_AFTER_KO) {
				initGame();
				return 0;
			}
		}
 
		// stones spawning
		for(int i=0; i<NPLAYERS; i++) {
			while( (unsigned int)player[i]->track->musicPos < selMusic.size() &&
				   (musicTime + player[i]->track->spawnDelay) > selMusic[player[i]->track->musicPos].time ) {
				
				player[i]->track->processEvent(selMusic[player[i]->track->musicPos]);
			}
		}
		
		// hitting state sounds
		handleHittingStates();
		
		sem_wait(&semaphore);
			// AI spell casting
			if (player[1]->useAI == true && !player[1]->isFrozen)
			{
				double chance = rand() % 30000;
				if (chance == 0)
				{
					int random = rand() % (player[1]->instrument->skills.size());
					player[1]->fretting->castedSpell = &(player[1]->instrument->skills[random]);	
				}
			}
			
			handleSpells();
			
			while (receiver->getEventsSize() != 0) {
				player[0]->updateEvents();
				player[1]->updateEvents();
				receiver->removeEvent();
			}
			
			player[0]->update();
			player[1]->update();
		sem_post(&semaphore);
		
	}
	
	return NULL;
}

bool loadNotes( std::string path )
{	
	int foundDifficulties=0;
	for(int i=0; i<NDIFFICULTIES; i++) {
		if(theMusic[i])
			theMusic[i]->clear();

		theMusic[i] = decoder.decodeMidi(path, (difficultyType)i);

		if(theMusic[i]) {
			++foundDifficulties;
			
			switch(i) {
				case EXPERT:
					musaGui.diffBox->addItem(L"Expert", EXPERT);
					break;
				case HARD:
					musaGui.diffBox->addItem(L"Hard", HARD);
					break;
				case MEDIUM:
					musaGui.diffBox->addItem(L"Medium", MEDIUM);
					break;
				case EASY:
					musaGui.diffBox->addItem(L"Easy", EASY);
					break;
			}
		}
	}
	
	if(foundDifficulties==0)
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

	EKEY_CODE eventsKeyboard1[NFRETS] = { KEY_KEY_A, KEY_KEY_S, KEY_KEY_J, KEY_KEY_K, KEY_KEY_L };
	EKEY_CODE eventsKeyboard2[NFRETS] = { KEY_KEY_Q, KEY_KEY_W, KEY_KEY_U, KEY_KEY_I, KEY_KEY_O };
	//int eventsJoystick1[NFRETS] = {4,6,7,5,2};
	//int eventsJoystick2[NFRETS] = {4,6,7,5,2};

	player[0]->fretting->setEvents(eventsKeyboard1, KEY_SPACE );
	//player[0]->fretting->setEvents(eventsJoystick1, joystickInfo, 0, 3);	//comment this line to use keyboard for player 1
	player[1]->fretting->setEvents(eventsKeyboard2, KEY_KEY_C );
	//player[1]->fretting->setEvents(eventsJoystick2, joystickInfo, 0, 3);	//comment this line to use keyboard for player 2
	
		
	selMusic = *theMusic[difficulty];
	screen->musicTotalTime = selMusic.back().time;
	//decoder.printMusic(selMusic);
	
	soundBank->stop();
	
	player[1]->useAI = useAI;
	
	/*
	 * initializing threads
	 */
	pthread_t thread[3];
	int arg = 1;
	cout << "launching Updater thread...";
	pthread_create(&thread[0], NULL, updater, (void *) arg);
	cout << "done." << endl;
	//pthread_create(&thread[1], NULL, debugger, (void *) arg);
	//pthread_create(&thread[2], NULL, drawer, (void *) arg);
	
	soundBank->playSelectedMusic();
	
	//screen->screenFader->fadeIn(1000);
}

void initGame()
{		
	player[0]->initialize();
	player[1]->initialize();
	
	// TEMPORARY
	Instrument* violin = new Instrument();
	Instrument* drums = new Instrument();
	
	for(int i=0; i<SKILLS_TOTAL; i++) {
		drums->addSkill( skillBank.skills[i] );
		violin->addSkill( skillBank.skills[i] );
	}
		
	player[0]->setInstrument(violin);
	player[1]->setInstrument(drums);
	
	makeMainMenu();
}

void initMusa()
{
	player[0] = new Player( device->getTimer(), new Track(&selMusic,&musicTime,device,23, -18) );
	player[1] = new Player( device->getTimer(), new Track(&selMusic,&musicTime,device,23, 18) );
	screen = new Screen(device,&musicTime,player[0],player[1]);
	
	player[0]->fretting->musicTime = &musicTime;
	player[1]->fretting->musicTime = &musicTime;
	
	player[0]->fretting->receiver = receiver;
	player[1]->fretting->receiver = receiver;

	sem_init(&semaphore, 0, 1);

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
	
	mainMenuBg = driver->getTexture("img/bgs/main.jpg");
}

static void *debugger (void *argument)
{
	while(1)
	{
		player[0]->fretting->printHitFret();
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
	 * initializing the game
	 */
	initMusa();
	initGame();
		
	#define NBGSFILES 4
	const path bgsBank[] = {"img/bgs/bg4.jpg", "img/bgs/bg20.jpg", "img/bgs/bg3.jpg", "img/bgs/bg5.jpg"};
	ITexture *bgPic = driver->getTexture( bgsBank[rand()%NBGSFILES] );
	/* 
	 * Irrlicht Main Loop
	 */	
	while(device->run()) {		
		driver->beginScene(true, true);
			// draw background
			driver->draw2DImage( bgPic, recti(position2di(0,0), position2di(SCREENX,SCREENY)), recti(position2di(0,0),bgPic->getOriginalSize()) );

			sem_wait(&semaphore);
				driver->setRenderTarget(0, false, true);
				player[0]->track->draw();
				player[1]->track->draw();
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
