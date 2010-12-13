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
#include "LavaShader.h"
#include "CSceneNodeAnimatorScale.h"
#include "CLensFlareSceneNode.h"

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
		
		void						splitBlood( int targetPlayer, E_GORE_LEVEL gore );
		void						soloEffect(int player, ITexture *tex, int timeMs=700 );
		void						areaEffect(int player, ITexture *tex, int timeMs=700 );		
		
		
		// works in progress
		void 						effectVampireAttack( int player, int timeMs );
		void						effectBlackHole( int targetPlayer, int timeMs );
	private:
		vector<Player*>				players;
		set<effectEvent>			effectsQueue;
		
		IrrlichtDevice				*device;
		IVideoDriver				*driver;
		ISceneManager				*smgr;
		
		CBloodEffect				*blood;
		IMeshSceneNode 				*shields[NPLAYERS];
		ITexture 					*blackTex, *lensFlareTex, *fireballTex, *glowTex, *electricTex, *laserTex1, *laserTex2, *laserTex3, *bloodTex[5], *waterTex[5];
		array<ITexture*> 			feedbackTex, explosion;
		
		
		void 						effectShield( int targetPlayer );
		void 						effectFeedback( int targetPlayer );
		void						effectExplosion( vector3df pos );
		void						effectFireRain( int targetPlayer );
		void						effectFireball( int targetPlayer, bool randomizeTarget );
		void						effectFireball( array<vector3df> path );
		void						effectLaserBeam( int target );
		void						effectBolt( int targetPlayer );
		void						effectThunderstormBolt( int targetPlayer );
		void						effectEletrifiedGround( int targetPlayer, int timeMs );
		void						effectWaterBeam( int targetPlayer );
		void						effectDrunkEffect ( int targetPlayer, int times );
		void						effectParticlesExplosion( vector3df pos, ITexture *tex );
		void 						effectFloodEffect( int targetPlayer );
		void 						effectSwampEffect( int targetPlayer, int timeMs );
		void						effectBallLightningEffect( int targetPlayer, int timeMs );
		void						effectImplantBomb( int targetPlayer );
		void 						effectSun( int target, int timeMs );
		void						effectDarkStormBolt( int target);

};
