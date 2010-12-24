#include "gui.h"

extern IVideoDriver *driver;
extern IGUIEnvironment *env;
extern IrrlichtDevice *device;
extern SAppContext musaGui;
extern ITexture	*mainMenuBg;

void makeMainMenu()
{
	#define WINSIZEX SCREENX/6.
	#define WINSIZEY SCREENY/2.7
	
	// background image
	musaGui.mainBg = env->addImage( rect<s32>(position2di(0, 0),dimension2di(SCREENX,SCREENY)) );
	musaGui.mainBg->setScaleImage(true);
	musaGui.mainBg->setImage( mainMenuBg );	
	
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
	musaGui.diffBox = env->addComboBox(rect<s32>(position2di(WINSIZEX/2-100/2, 195-20/2),dimension2di(100,20)), window);
	musaGui.diffBox->setEnabled(false);
	// AI checkbox
	//musaGui.aiCheck = env->addCheckBox(true, rect<s32>(position2di(WINSIZEX/2-20/2-40, 220-20/2),dimension2di(20,20)), window);
	//env->addStaticText(L"Play against AI?", rect<s32>(position2di(WINSIZEX/2+25-100/2, 220-20/2),dimension2di(100,20)), false, true, window);
	
	musaGui.control[0] = env->addComboBox(rect<s32>(position2di(WINSIZEX/2-75, 220-20/2),dimension2di(75,20)), window);
	musaGui.control[1] = env->addComboBox(rect<s32>(position2di(WINSIZEX/2, 220-20/2),dimension2di(75,20)), window);
	for(int i=0; i<NPLAYERS; i++) {
		musaGui.control[i]->addItem(L"Keyboard", C_KEYBOARD);
		musaGui.control[i]->addItem(L"Joystick", C_JOYSTICK);
		musaGui.control[i]->addItem(L"AI", C_AI);
	}	
	
	
	// start button
	musaGui.startButton = env->addButton(rect<s32>(position2di(WINSIZEX/2-100/2, 250-32/2),dimension2di(100,32)), window, GUI_ID_START_BUTTON, L"Start", L"Starts the game");
	musaGui.startButton->setEnabled(false);
}
