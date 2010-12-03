#pragma once

#include "irrlicht.h"
using namespace irr;
#include "time.h"

#include "Fretting.h"
#include "Player.h"

#include "CBoltSceneNode.h"
#include "CBloodEffect.h"
#include "CBloodShader.h"
#include "VxHealthSceneNode.h"
#include "CShieldManager.h"

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
		CShieldManager 				*shieldmanager;

		void						update();
		
		void						setFps( int fps );
		void						showGood( int i );
		void						showBad( int i );
		void						showNeutral( int i );
		void 						showShield( int player );
		void 						showSpellEffect( int player );
		void						createFireball( int attackedPlayer, bool randomize=false );
	
	private:
		IrrlichtDevice				*device;
		IVideoDriver				*driver;
		ISceneManager				*smgr;
		
		IGUIImage 					*good[NPLAYERS], *bad[NPLAYERS], *neutral[NPLAYERS];
		ISceneNode					*glow[NPLAYERS][NFRETS];
		IGUIStaticText 				*fpsText, *hpTxt[NPLAYERS];
		VxHealthSceneNode			*healthBars[NPLAYERS];
		IMeshSceneNode 				*shields[NPLAYERS];
		IVolumeLightSceneNode 		*spellEffect;
		ITexture 					*fireballTex;
		array<video::ITexture*> 	spellEffectTex;
		
		void						drawKeys();
		void 						drawHittingState();
		void 						drawHP();
		

};
