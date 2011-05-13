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
IrrlichtDevice 			*device=NULL;
IVideoDriver 			*driver=NULL;
ISceneManager 			*smgr=NULL;
IGUIEnvironment			*env=NULL;
ICameraSceneNode 		*camera=NULL;
EventReceiver 			*receiver;
ITexture				*mainMenuBg;
ITimer					*timer;

ITexture				*glowTex;

// MusA globals
Decoder 				decoder;
music			 		*theMusic[NDIFFICULTIES], selMusic;
difficultyType			difficulty;
SkillBank				skillBank;
SoundBank				*soundBank=NULL;
Screen					*screen=NULL;
Player 					*player[2]={NULL};
SAppContext 			musaGui;
vector<Instrument>		instruments;

bool 					isGameStopped=true, isGamePaused=false;
double 					musicTime, gamePausedTime=0;

// Other global
sem_t semaphore;
sem_t receiverSemaphore;


void 			handleCastedSpells();
void 			handleHittingStates();
static void 	*updater(void *argument);
bool			testGameOver();
bool 			loadNotes( std::string path );
void 			loadSong( std::string path, int which );
void 			startGame( int difficulty, bool useAI);
void			showHelp();
void			pauseGame();
void 			initGame();
void 			initIrrlicht();
//static void 	*debugger (void *argument);


#define START_DELAY 3

static void *updater(void *argument) 
{
	double start, gamePausedCompensation=0;
	bool waiting=true;
	
	//game over vars
	double timeMusicEnded, endMusicOffset;
	bool endMusicFirstTime=true;
	
	// get the time before starting the music (so we can know how much time passed at each iteration)
	start = timer->getTime()/1000.;
	
	while(!isGameStopped) {
		usleep(1);
		
		if(isGamePaused) {
			// On Game Paused we compensate the base time with the elapsed paused time
			gamePausedCompensation = timer->getTime()/1000. - musicTime - start - START_DELAY;
			start += gamePausedCompensation;
		}
		else {
		
			musicTime = timer->getTime()/1000. - start - START_DELAY;
			
			if( waiting && musicTime>0 ) {
				waiting = false;
				soundBank->playSelectedMusic();
			}
			
			// Game over?
			if ( !isGameStopped && !waiting && (!soundBank->isPlaying() || player[0]->HP == 0 || player[1]->HP == 0) ) {

				if (endMusicFirstTime) {
					screen->showKO();
					soundBank->playEffect(S_WIN);
					endMusicFirstTime = false;
					timeMusicEnded = timer->getTime()/1000.;
				}
				else {
					endMusicOffset = timer->getTime()/1000. - timeMusicEnded;
					if (endMusicOffset > DELAY_AFTER_KO) {
						screen->hideKO();
						initGame();
						return 0;
					}
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
				for(int i=0; i<NPLAYERS; i++)
					if (player[i]->useAI == true && !player[i]->isFrozen) {
						double chance = rand() % 30000;
						if (chance == 0) {
							int random = rand() % (player[i]->instrument->skills.size());
							player[i]->fretting.castedSpell = &(player[i]->instrument->skills[random]);	
						}
					}
				
				handleCastedSpells();
				
				while (receiver->getEventsSize() != 0) {
					player[0]->updateEvents();
					player[1]->updateEvents();
					receiver->removeEvent();
				}
				
				player[0]->update();
				player[1]->update();
			sem_post(&semaphore);
		}
	}
	
	return NULL;
}


bool loadNotes( std::string path )
{	
	for(int i=0; i<NDIFFICULTIES; i++)
		if(theMusic[i])
			theMusic[i]->clear();
		
	int foundDifficulties=0;
	for(int i=0; i<NDIFFICULTIES; i++) {

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

void handleCastedSpells()
{
	for (int i = 0; i < 2; i++)
	{
		if (((player[i]->fretting.castedSpell) != NULL))
		{
			Skill *casted = player[i]->fretting.castedSpell;

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
                    player[i]->fretting.castedSpell = NULL;
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
						player[!i]->fretting.tolerance -= casted->effects[j].param;
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
		player[i]->fretting.castedSpell = NULL;
	}
}

void handleHittingStates()
{
	for(int i=0; i<NPLAYERS; i++) 
	{
		switch( player[i]->fretting.frettingState )
		{
			case 1:
				break;
			case -1:
				//soundBank->playMissEffect();
				//player[i]->fretting.frettingState = 0;
				break;
			case 0:
			case -2:
				break;
		}
	}
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

void showHelp( int p )
{
	wchar_t title[64], skills[2048];
	char skillstemp[2048];
	char sequence[256];
	
	swprintf(title, 64,L"Skills list of: %s",  player[p]->instrument->name.c_str());

	sprintf(skillstemp, " ");
	for(unsigned int i=0; i<player[p]->instrument->skills.size(); i++) {
		Skill *s = &player[p]->instrument->skills[i];
		
		sprintf(sequence, " ");
		for(unsigned int k=0; k<s->keysSequence.size()-1; k++)
			sprintf(sequence, "%sB%i-", sequence, s->keysSequence[k]+1);
		sprintf(sequence, "%sB%i", sequence, s->keysSequence[s->keysSequence.size()-1]+1);
		
		sprintf(skillstemp, "%s\n\n     %s\n\"%s\"\nCOST: %.0lf\nKEYS COMBINATION: %s", skillstemp,
															s->name.c_str(),
															s->description.c_str(),
															s->cost,
															sequence );
	}
	swprintf(skills, 2048, L"%s", skillstemp );

	env->addMessageBox( title,
						skills,
						false,
						EMBF_OK );
}

void pauseGame()
{
	if(!isGameStopped) {
		
		if(isGamePaused) {
			hideGameMenu();
			soundBank->unpause();
		}
		else {
			showGameMenu();
			soundBank->pause();
		}
			
		isGamePaused = !(isGamePaused);
	}
}

void startGame( int difficulty, controlType controls[NPLAYERS], instrumentType selInstrument[NPLAYERS] )
{		
	player[0]->reset();
	player[1]->reset();
	
	// Set up controls /////////////////////////////////////////////////
		
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

	EKEY_CODE eventsKeyboard[NFRETS] = { KEY_KEY_A, KEY_KEY_S, KEY_KEY_J, KEY_KEY_K, KEY_KEY_L };
	//EKEY_CODE eventsKeyboard2[NFRETS] = { KEY_KEY_Q, KEY_KEY_W, KEY_KEY_U, KEY_KEY_I, KEY_KEY_O };
	int eventsJoystick[NFRETS] = {4,6,7,5,2};

	for(int i=0; i<NPLAYERS; i++)
		switch(controls[i]) {
			case C_KEYBOARD:
				player[i]->fretting.setEvents(eventsKeyboard, KEY_SPACE );
				break;
			case C_JOYSTICK:
				player[i]->fretting.setEvents(eventsJoystick, joystickInfo, 0, 3);
				break;
			case C_AI:
				player[i]->useAI = true;
				break;
		}
		
		
	// Set up instruments //////////////////////////////////////////////
	for(int i=0; i<NPLAYERS; i++)	
		
		if(selInstrument[i]==I_RANDOM)
			player[i]->setInstrument( &instruments[rand()%(I_TOTAL-2)] );
		else
			player[i]->setInstrument( &instruments[selInstrument[i]] );
	
	cout << selInstrument[0] << endl;
	cout << selInstrument[1] << endl;
	
	//player[0]->instrument->printSkills();
	//player[1]->instrument->printSkills();
	
	
	
	// Set up music related variables //////////////////////////////////
	
	selMusic = *theMusic[difficulty];
	screen->musicTotalTime = selMusic.back().time;
	//decoder.printMusic(selMusic);
	
	soundBank->stop(); //stop theme music
	
	isGameStopped = false;	
	
	
	// Initialize updater thread ///////////////////////////////////////
	////////////////////////////////////////////////////////////////////
	pthread_t thread[2];
	int arg = 1;
	//cout << "launching Updater thread...";
	pthread_create(&thread[0], NULL, updater, (void *) arg);
	//cout << "done." << endl;
	//pthread_create(&thread[1], NULL, debugger, (void *) arg);
	
	
	screen->screenFader->fadeIn(1500);
}

void initInstruments()
{
	instruments.push_back( Instrument("Fire Drums") );
	instruments.push_back( Instrument("Thunder Guitar") );
	instruments.push_back( Instrument("Zen Keyboards") );
	instruments.push_back( Instrument("Evil Vocals") );
	instruments.push_back( Instrument("Water Violin") );
	
	for(int i=0; i<SKILLS_TOTAL; i++)
		switch(skillBank.skills[i].element) {
			case FIRE:
				instruments[0].addSkill( skillBank.skills[i] );
				break;
			case THUNDER:
				instruments[1].addSkill( skillBank.skills[i] );
				break;
			case SPIRITUAL:
				instruments[2].addSkill( skillBank.skills[i] );
				break;
			case DARK:
				instruments[3].addSkill( skillBank.skills[i] );
				break;
			case WATER:
				instruments[4].addSkill( skillBank.skills[i] );
				break;
			}
	
	//skillBank.print();
}

void initGame()
{	
	isGameStopped = true;
	soundBank->stop();
	soundBank->clearSongs();
	
	showMainMenu();
	soundBank->playTheme();
}

void initMusa()
{
	player[0] = new Player( device->getTimer(), &musicTime, receiver, new Track(&selMusic,&musicTime,device,23, -18) );
	player[1] = new Player( device->getTimer(), &musicTime, receiver, new Track(&selMusic,&musicTime,device,23, 18) );

	screen = new Screen(device,&musicTime,player[0],player[1]);
	
	sem_init(&semaphore, 0, 1);

	// game functions to be accessed by the GUI
	musaGui.startGame = &startGame;
	musaGui.loadSong = &loadSong;
	musaGui.loadNotes = &loadNotes;
	musaGui.showHelp = &showHelp;
	musaGui.pauseGame = &pauseGame;
	musaGui.initGame = &initGame;
		
	initInstruments();	
	
	initGui();
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
	
	timer = device->getTimer();
	
	// GUI
	IGUISkin* skin = env->getSkin();
	IGUIFont* font = env->getFont("img/fonthaettenschweiler.bmp");
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

/*static void *debugger (void *argument)
{
	while(1)
	{
		player[0]->fretting.printHitFret();
		cout << "vetor.size(): " << receiver->getEventsSize() << endl;
		usleep(80000);
	}

	return 0;
}*/

int main(int argc, char *argv[])
{
	srand(time(0));
	
	/*
	 * initializing the sound engine
	 */
	soundBank = new SoundBank(device);
	
	/*
	 * initializing the graphics engine
	 */
	initIrrlicht();
	
	
	/*
	 * initializing the game
	 */
	initMusa();
	initGame();
	
	ITexture *bgPic = driver->getTexture( bgsBank[rand()%NBGSFILES] );
	
	/* 
	 * Irrlicht Main Loop
	 */	
	while(device->run()) {	
		
		if(!isGamePaused)
			driver->beginScene(true, true);
				// draw background
				driver->draw2DImage( bgPic, recti(position2di(0,0), position2di(SCREENX,SCREENY)), recti(position2di(0,0),bgPic->getOriginalSize()) );

				sem_wait(&semaphore);
					driver->setRenderTarget(0, false, true);
					smgr->drawAll();		
					if(!isGameStopped) {
						player[0]->track->draw();
						player[1]->track->draw();
						screen->update();
					}
				sem_post(&semaphore);
				
				env->drawAll();
			driver->endScene();
	}
	
	return 0;
}
