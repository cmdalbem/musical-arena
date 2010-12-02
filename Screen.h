#pragma once

#include "irrlicht.h"
using namespace irr;
#include "time.h"

#include "Fretting.h"
#include "Player.h"
#include "VxHealthSceneNode.h"

#define SHOW_DELAY 1 //seconds

using namespace irr::core;
using namespace irr::gui;
using namespace irr::scene;
using namespace irr::video;

class Screen
{
	public:
		Screen( IrrlichtDevice *device, Player* player1, Player* player2 );
		~Screen();
		
		Player*						players[NPLAYERS];

		void						update();
		
		void						showGood( int i );
		void						showBad( int i );
		void						showNeutral( int i );
		void						setFps( int fps );
	
	private:
		IrrlichtDevice				*device;
		IVideoDriver				*driver;
		
		IGUIImage 					*good[NPLAYERS], *bad[NPLAYERS], *neutral[NPLAYERS];
		ISceneNode					*glow[NPLAYERS][NFRETS];
		IGUIStaticText 				*fpsText, *hpTxt[NPLAYERS];
		VxHealthSceneNode			*healthBars[NPLAYERS];
		
		void						drawKeys();
		void 						drawHittingState();
		

};
