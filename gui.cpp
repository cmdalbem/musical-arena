#include "gui.h"

extern IVideoDriver *driver;
extern IGUIEnvironment *env;
extern IrrlichtDevice *device;
extern SAppContext musaGui;
extern ITexture	*mainMenuBg;


void showMainMenu()
{
	#define WINSIZEX SCREENX/6.
	#define WINSIZEY SCREENY/2.5
	
	// background image
	musaGui.mainBg = env->addImage( rect<s32>(position2di(0, 0),dimension2di(SCREENX,SCREENY)) );
	musaGui.mainBg->setScaleImage(true);
	musaGui.mainBg->setImage( mainMenuBg );	
	
	// main window
	IGUIWindow *window = env->addWindow( recti(position2di(640, 100),dimension2di(WINSIZEX,WINSIZEY)), false );
	window->setDraggable(false);
	//window->setDrawBackground(false);
	window->setDrawTitlebar(false);
	musaGui.mainWindow = window;
	window->getCloseButton()->setVisible(false); 
	
	// load file buttons
	env->addButton(rect<s32>(position2di(WINSIZEX/2-100/2, 40-32/2),dimension2di(100,32)), window, GUI_ID_LOAD_NOTES, L"Load MIDI", L"Loads a Fret's on Fire music file");
	env->addButton(rect<s32>(position2di(WINSIZEX/2-100/2, 80-32/2),dimension2di(100,32)), window, GUI_ID_LOAD_MUSIC1, L"Load Song File", L"Loads the main song's audio file (usually all lines except guitar)");
	env->addButton(rect<s32>(position2di(WINSIZEX/2-100/2, 120-32/2),dimension2di(100,32)), window, GUI_ID_LOAD_MUSIC2, L"Load Guitar File", L"Loads an optional secondary song's audio file (usually the guitar line)");
	
	// difficulty combobox
	musaGui.diffBox = env->addComboBox(rect<s32>(position2di(WINSIZEX/2-100/2, 195-20/2),dimension2di(100,20)), window);
	musaGui.diffBox->setEnabled(false);
	
	musaGui.control[0] = env->addComboBox(rect<s32>(position2di(WINSIZEX/2-75, 220-20/2),dimension2di(75,20)), window);
	musaGui.control[1] = env->addComboBox(rect<s32>(position2di(WINSIZEX/2, 220-20/2),dimension2di(75,20)), window);
	for(int i=0; i<NPLAYERS; i++) {
		musaGui.control[i]->addItem(L"Keyboard", C_KEYBOARD);
		musaGui.control[i]->addItem(L"Joystick", C_JOYSTICK);
		musaGui.control[i]->addItem(L"AI", C_AI);
	}	
	
	musaGui.instrument[0] = env->addComboBox(rect<s32>(position2di(WINSIZEX/2-75, 245-20/2),dimension2di(75,20)), window);
	musaGui.instrument[1] = env->addComboBox(rect<s32>(position2di(WINSIZEX/2, 245-20/2),dimension2di(75,20)), window);
	for(int i=0; i<NPLAYERS; i++) {	
		musaGui.instrument[i]->addItem(L"Random", I_RANDOM);
		musaGui.instrument[i]->addItem(L"Fire Drums", I_FIRE);
		musaGui.instrument[i]->addItem(L"Thunder Guitar", I_THUNDER);
		musaGui.instrument[i]->addItem(L"Zen Keyboards", I_SPIRITUAL);
		musaGui.instrument[i]->addItem(L"Evil Vocals", I_DARK);
		musaGui.instrument[i]->addItem(L"Water Violin", I_WATER);
		//musaGui.instrument[i]->addItem(L"Custom", I_CUSTOM);
	}	
	
	// start button
	musaGui.startButton = env->addButton(rect<s32>(position2di(WINSIZEX/2-100/2, 280-32/2),dimension2di(100,32)), window, GUI_ID_START_BUTTON, L"Start", L"Starts the game");
	musaGui.startButton->setEnabled(false);
	
	musaGui.loadedNotes = false, 
	musaGui.loadedSong1 = false,
	musaGui.loadedSong2 = false;
}

void initGameMenu()
{
	#define MENUX 170
	#define MENUY 180
		
	IGUIWindow *window = env->addWindow( recti(position2di((SCREENX-MENUX)/2., (SCREENY-MENUY)/2.),dimension2di(MENUX,MENUY)),
										 false, L"GAME PAUSED" );
	window->setDraggable(false);
	window->setDrawBackground(true);
	window->setDrawTitlebar(true);
	window->setVisible(false);
	window->getCloseButton()->setVisible(false); 
	musaGui.gameMenu = window;
	
	env->addButton(rect<s32>(position2di(MENUX/2-120/2, 50-32/2),dimension2di(120,32)), window, GUI_ID_P1SKILLS, L"Player 1 Skills");
	env->addButton(rect<s32>(position2di(MENUX/2-120/2, 90-32/2),dimension2di(120,32)), window, GUI_ID_P2SKILLS, L"Player 2 Skills");
	
	env->addButton(rect<s32>(position2di(MENUX/2-120/2, 150-32/2),dimension2di(120,32)), window, GUI_ID_EXIT_TO_MAIN, L"Exit to Main Menu");
}

void showGameMenu()
{
	musaGui.gameMenu->setVisible(true);
}

void hideGameMenu()
{
	musaGui.gameMenu->setVisible(false);
}

void initGui()
{
	initGameMenu();	
}
