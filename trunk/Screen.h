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
		IGUIInOutFader				*screenFader;


		void						update();		
		void						setFps( int fps );
		void						showKO();
		void						hideKO();
	
	private:
		IrrlichtDevice				*device;
		IVideoDriver				*driver;
		ISceneManager				*smgr;
		double						*musicTime;
		
		ILightSceneNode 			*sceneLight;
		ISceneNode					*glow[NPLAYERS][NFRETS];
		IGUIStaticText 				*fpsText, *hpText[NPLAYERS], *timeText;
		VxHealthSceneNode			*healthBar[NPLAYERS], *armorBar[NPLAYERS], *staminaBar[NPLAYERS];
		IGUIImage					*statusIcon[NPLAYERS][5], *koImage, *mult[NPLAYERS];
		ITexture 					*koTex, *glowTex, *poisonTex, *fireTex, *magicBarrierTex, *eletrifiedTex, *drownedTex, *frozenTex, *mirrorTex, *blessedTex, *cursedTex;
		ITexture					*mult2xTex, *mult3xTex, *mult4xTex;
		ISceneNode 					*sky;
			
		// HUD drawing
		void 						drawSoloModeState();
		void						drawKeys();
		void 						drawOSD();
		void						drawSplitBlood();
		void 						drawStatus();
		void						drawMultiplier();
		
		// Utility functions
		void						initializeScreenElements();
		void						loadTextures();
		

};
