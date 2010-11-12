#pragma once

#include "irrlicht.h"
using namespace irr;
#include "time.h"


#define SHOW_DELAY 1 //seconds


class Screen
{
	public:
		Screen( IrrlichtDevice *device, video::IVideoDriver *driver );
		~Screen();
	
		gui::IGUIImage 				*good, *bad;
		gui::IGUIStaticText 		*fpsText;

		void						update();
		
		void						showGood();
		void						showBad();
	
	private:
		struct timeval				lastEventTime;
		IrrlichtDevice				*device;
		video::IVideoDriver			*driver;
		

};
