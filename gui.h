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


// GUI TYPES ///////////////////////////////////////////////////////////

enum {
	// GUI Controls
	GUI_ID_QUIT_BUTTON = 101,
	GUI_ID_START_BUTTON,
	GUI_ID_LOAD_NOTES,
	GUI_ID_LOAD_MUSIC1,
	GUI_ID_LOAD_MUSIC2,
	GUI_ID_LOAD_NOTES_DIALOG,
	GUI_ID_LOAD_MUSIC1_DIALOG,
	GUI_ID_LOAD_MUSIC2_DIALOG,
	
	GUI_ID_P1SKILLS,
	GUI_ID_P2SKILLS,
	GUI_ID_EXIT_TO_MAIN
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
	I_RANDOM,
	I_FIRE,
	I_THUNDER,
	I_SPIRITUAL,
	I_DARK,
	I_WATER,
	//I_CUSTOM,
	
	I_TOTAL
};

// CONTEXT STRUCTURES //////////////////////////////////////////////////

struct SAppContext
{
	guiState		state;
	IrrlichtDevice	*device;
	s32				counter;
	IGUIListBox		*listbox;
	IGUIButton		*startButton;
	IGUIComboBox	*diffBox, *control[NPLAYERS], *instrument[NPLAYERS];
	
	bool			loadedNotes, loadedSong1, loadedSong2;
	
	path			musicPath;
	
	IGUIImage 		*mainBg;
	IGUIWindow 		*mainWindow;
	IGUIWindow		*gameMenu;
	
	void (*startGame)(int, controlType[NPLAYERS], instrumentType[NPLAYERS]);
	void (*loadSong)(std::string, int);
	bool (*loadNotes)(std::string);
	void (*showHelp)(int);
	void (*pauseGame)();
	void (*initGame)();
};

// PROTOTYPES //////////////////////////////////////////////////////////

void	initGui();
void	initGameMenu();

void	showMainMenu();
void	showGameMenu();
void	hideGameMenu();
