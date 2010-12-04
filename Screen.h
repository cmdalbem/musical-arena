#pragma once

#include "irrlicht.h"
using namespace irr;
#include "time.h"
#include <set>
using namespace std;

#include "Fretting.h"
#include "Player.h"

#include "CBoltSceneNode.h"
#include "CBloodEffect.h"
#include "CBloodShader.h"
#include "VxHealthSceneNode.h"
#include "CShieldManager.h"
#include "CDeleteParticleAffector.h"

#define SHOW_DELAY 1 //seconds

using namespace irr::core;
using namespace irr::gui;
using namespace irr::scene;
using namespace irr::video;


enum effectFunction
{
	CREATE_FIREBALL, CREATE_FIREBALL_SKY, CREATE_FIRE_RAIN, CREATE_FIRE_AREA, CREATE_MAGIC_AREA, SPELL_EFFECT, SHOW_SHIELD, CREATE_ELECTRIC, CREATE_WATER_BEAM
};

typedef pair<pair<unsigned int,effectFunction>,int> effectEvent;

class Screen
{
	public:
		Screen( IrrlichtDevice *device, Player* player1, Player* player2 );
		~Screen();
		
		Player*						players[NPLAYERS];
		CShieldManager 				*shieldmanager;

		void						update();
		
		void						setFps( int fps );
		void						queueEffect( int msecondsAhead, effectFunction functionToCall, int targetPlayer );
	
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
		ITexture 					*fireballTex, *glowTex, *laserTex, *smokeTex1, *smokeTex2, *bloodTex[5], *waterTex[5];
		array<video::ITexture*> 	spellEffectTex;
		
		set<effectEvent>			effectsQueue;
		
		// Special Effects implementations
		void 						createShield( int targetPlayer );
		void 						createSpellEffect( int targetPlayer );
		void						createFireRain( int targetPlayer );
		void						createFireball( int targetPlayer, bool randomizeTarget );
		void						createFireball( array<vector3df> path );
		void						createAreaEffect(int player, ITexture *tex );
		void						createElectricEffect( int targetPlayer );
		void						createWaterBeam( int targetPlayer );
		
		// HUD drawing
		void						drawKeys();
		void 						drawHittingState();
		void 						drawHP();
		
		// Inner utility functions
		void						handleEffectsQueue();
		void						initializeEffects();
		void						showGood( int targetPlayer );
		void						showBad( int targetPlayer );
		void						showNeutral( int targetPlayer );
		

};
