#pragma once

#include <irrlicht.h>

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
        GUI_ID_LOAD_MUSIC,
};

enum guiState
{
	GUI_INSTRUMENT_MENU,
	GUI_READY_TO_PLAY,
	GUI_PLAYING
};
	

struct SAppContext
{
	guiState		state;
	IrrlichtDevice	*device;
	s32				counter;
	IGUIListBox		*listbox;
	
	IGUIWindow *window;
};
