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


enum effectFunction
{
	CREATE_FIREBALL, CREATE_FIREBALL_SKY, CREATE_FIRE_RAIN, CREATE_FIRE_AREA, CREATE_GLOW_AREA, SPELL_EFFECT, SHOW_SHIELD, CREATE_ELECTRIC, CREATE_WATER_BEAM
};

typedef pair<pair<unsigned int,effectFunction>,int> effectEvent;

class Screen
{
	public:
		Screen( IrrlichtDevice *device, Player* player1, Player* player2 );
		~Screen();
		
		ITexture					*bloodSplit[NPLAYERS];
		ISceneNode					*abox;
		vector<Player*> 			players;

		void						update();		
		void						setFps( int fps );
		void						queueEffect( int msecondsAhead, effectFunction functionToCall, int targetPlayer );
	
	private:
		IrrlichtDevice				*device;
		IVideoDriver				*driver;
		ISceneManager				*smgr;
		EffectFactory				*effectFactory;
		
		set<effectEvent>			effectsQueue;
		
		IGUIImage 					*good[NPLAYERS], *bad[NPLAYERS], *neutral[NPLAYERS];
		ISceneNode					*glow[NPLAYERS][NFRETS];
		IGUIStaticText 				*fpsText, *hpTxt[NPLAYERS];
		VxHealthSceneNode			*healthBars[NPLAYERS];
		ITexture 					*glowTex;
		
		// HUD drawing
		void						drawKeys();
		void 						drawHittingState();
		void 						drawHP();
		
		// Inner utility functions
		void						handleEffectsQueue();
		void						initializeScreenElements();
		void						showGood( int targetPlayer );
		void						showBad( int targetPlayer );
		void						showNeutral( int targetPlayer );
		

};
