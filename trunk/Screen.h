#pragma once

#include "irrlicht.h"
using namespace irr;
#include "time.h"

#include "Fretting.h"
#include "Player.h"

#define SHOW_DELAY 1 //seconds

using irr::core::vector3df;

class Screen
{
	public:
		Screen( IrrlichtDevice *device );
		~Screen();
	
		gui::IGUIImage 				*good[NPLAYERS], *bad[NPLAYERS], *neutral[NPLAYERS];
		gui::IGUIStaticText 		*fpsText;
		
		Player*						players[NPLAYERS];

		void						update();
		
		void						showGood( int i );
		void						showBad( int i );
		void						showNeutral( int i );
	
	private:
		struct timeval				lastEventTime;
		IrrlichtDevice				*device;
		video::IVideoDriver			*driver;
		
		void						drawKeys();
		

};
