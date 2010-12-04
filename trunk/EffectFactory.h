#pragma once

#include "irrlicht.h"
using namespace irr;
#include "utils.h"

#include "Player.h"

#include "CBoltSceneNode.h"
#include "CBloodEffect.h"
#include "CBloodShader.h"
#include "CShieldManager.h"
#include "CDeleteParticleAffector.h"

using namespace irr::core;
using namespace irr::gui;
using namespace irr::scene;
using namespace irr::video;


class EffectFactory
{
	public:
		EffectFactory( IrrlichtDevice *device, IVideoDriver *driver, ISceneManager *smgr, vector<Player*> players );
		~EffectFactory();
		
		CShieldManager 				*shieldmanager;
	
		void						queueEffect( int msecondsAhead, visualEffectFunction functionToCall, int targetPlayer );
		void						handleEffectsQueue();
		
		void						splitBlood( int targetPlayer );
		
	
	private:
		vector<Player*>				players;
		set<effectEvent>			effectsQueue;
		
		IrrlichtDevice				*device;
		IVideoDriver				*driver;
		ISceneManager				*smgr;
		
		CBloodEffect				*blood;
		IMeshSceneNode 				*shields[NPLAYERS];
		ITexture 					*fireballTex, *glowTex, *laserTex, *bloodTex[5], *waterTex[5];
		array<ITexture*> 			feedbackTex, explosion;
		
		
		void 						createShield( int targetPlayer );
		void 						createFeedback( int targetPlayer );
		void						createExplosion( vector3df pos );
		void						createFireRain( int targetPlayer );
		void						createFireball( int targetPlayer, bool randomizeTarget );
		void						createFireball( array<vector3df> path );
		void						createAreaEffect(int player, ITexture *tex, int timeMs=700 );
		void						createBolt( int targetPlayer );
		void						createEletrifiedGround( int targetPlayer );
		void						createWaterBeam( int targetPlayer );
		void						createDrunkEffect ( int targetPlayer, int times );
		void						createParticlesExplosion( vector3df pos, ITexture *tex );

};
