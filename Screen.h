#pragma once

#include "irrlicht.h"
using namespace irr;
#include "time.h"
#include <set>
using namespace std;

#include "Fretting.h"
#include "Player.h"
#include "EffectFactory.h"

#include "VxHealthSceneNode.h"

#define SHOW_DELAY 1 //seconds

using namespace irr::core;
using namespace irr::gui;
using namespace irr::scene;
using namespace irr::video;


class Screen
{
	public:
		Screen( IrrlichtDevice *device, double *musicTime, Player* player1, Player* player2 );
		~Screen();
		
		ITexture					*bloodSplit[NPLAYERS];
		vector<Player*> 			player;
		double						musicTotalTime;
		EffectFactory				*effectFactory;


		void						update();		
		void						setFps( int fps );
	
	private:
		IrrlichtDevice				*device;
		IVideoDriver				*driver;
		ISceneManager				*smgr;
		double						*musicTime;
		
		IGUIImage 					*good[NPLAYERS], *bad[NPLAYERS], *neutral[NPLAYERS];
		ISceneNode					*glow[NPLAYERS][NFRETS];
		IGUIStaticText 				*fpsText, *hpTxt[NPLAYERS], *timeText;
		VxHealthSceneNode			*healthBar[NPLAYERS], *staminaBar[NPLAYERS][NSTAMINALEVELS];
		ITexture 					*glowTex, *fireballTex;
		ISceneNode 					*sky;
		
		// HUD drawing
		void 						drawSoloModeState();
		void						drawKeys();
		void 						drawHittingState();
		void 						drawBars();
		void						drawSplitBlood();
		
		// Inner utility functions
		void						initializeScreenElements();
		void						showGood( int targetPlayer );
		void						showBad( int targetPlayer );
		void						showNeutral( int targetPlayer );
		

};
