#pragma once

#include "irrlicht.h"
using namespace irr;
#include "time.h"

#include "Fretting.h"


#define SHOW_DELAY 1 //seconds


class Screen
{
	public:
		Screen( IrrlichtDevice *device, video::IVideoDriver *driver );
		~Screen();
	
		gui::IGUIImage 				*good, *bad, *neutral;
		gui::IGUIStaticText 		*fpsText;

		void						update( Fretting *fretting1, Fretting *fretting2 );
		
		void						showGood();
		void						showBad();
		void						showNeutral();
	
	private:
		struct timeval				lastEventTime;
		IrrlichtDevice				*device;
		video::IVideoDriver			*driver;
		

};
