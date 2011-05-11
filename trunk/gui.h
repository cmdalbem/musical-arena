#pragma once

#include <irrlicht.h>
#include <iostream>

#include "utils.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


// GUI Controls
enum
{
        GUI_ID_QUIT_BUTTON = 101,
        GUI_ID_START_BUTTON,
        GUI_ID_LOAD_NOTES,
        GUI_ID_LOAD_MUSIC1,
        GUI_ID_LOAD_MUSIC2,
        GUI_ID_LOAD_NOTES_DIALOG,
        GUI_ID_LOAD_MUSIC1_DIALOG,
        GUI_ID_LOAD_MUSIC2_DIALOG,
        GUI_INITIAL_MENU
};

enum guiState
{
	GUI_MAIN_MENU,
	GUI_PLAYING
};

enum controlType
{
	C_KEYBOARD,
	C_JOYSTICK,
	C_AI
};

enum instrumentType
{
	I_FIRE,
	I_THUNDER,
	I_SPIRITUAL,
	I_DARK,
	I_WATER,
	I_CUSTOM
};

struct SAppContext
{
	guiState		state;
	IrrlichtDevice	*device;
	s32				counter;
	IGUIListBox		*listbox;
	IGUIButton		*startButton;
	IGUIComboBox	*diffBox, *control[NPLAYERS], *instrument[NPLAYERS];
	
	path			musicPath;
	
	IGUIImage 		*mainBg;
	IGUIWindow 		*mainWindow;
	
	void (*startGame)(int, controlType[NPLAYERS], instrumentType[NPLAYERS]);
	void (*loadSong)(std::string, int);
	bool (*loadNotes)(std::string);
};


void makeMainMenu();
