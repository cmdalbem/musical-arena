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
        GUI_ID_NEW_WINDOW_BUTTON,
        GUI_ID_FILE_OPEN_BUTTON,
};

enum
{
	MENU_INSTRUMENT,
	READY_TO_PLAY,
	PLAYING
};
	

struct SAppContext
{
	int		fase;
        IrrlichtDevice	*device;
        s32		counter;
        IGUIListBox*	listbox;
};
