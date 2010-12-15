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
		void						effectParticlesExplosion( vector3df pos, ITexture *tex );
		
	
		// works in progress
		void						effectBlackHole( int targetPlayer, int timeMs );
		void						effectAnotherDimension( int target, int timeMs );
		void 						effectAurora( int target );
	
	private:
		vector<Player*>				players;
		set<effectEvent>			effectsQueue;
		
		IrrlichtDevice				*device;
		IVideoDriver				*driver;
		ISceneManager				*smgr;
		
		CBloodEffect				*blood;
		IMeshSceneNode 				*shields[NPLAYERS];
		ITexture 					*gridTex, *starTex, *blackTex, *lensFlareTex, *fireballTex, *iceTex, *glowTex, *glowTex2, *glowTex3, *electricTex, *laserTex1, *laserTex2, *laserTex3, *bloodTex[5], *waterTex[5];
		array<ITexture*> 			feedbackTex, snowFlakes, ice1, water1, magic1, fire1;
		
		IAnimatedMesh				*stoneMesh1, *stoneMesh2;
		
		
		void 						effectIce( int targetPlayer, int timeMs );	
		void 						effectShield( int targetPlayer );
		void 						effectFeedback( int targetPlayer );
		void						effectExplosion( vector3df pos );
		void						effectFireRain( int targetPlayer );
		void						effectFireball( int targetPlayer, bool randomizeTarget );
		void						effectFireball( array<vector3df> path );
		void						effectLaserBeam( int target );
		void						effectBolt( int targetPlayer );
		void						effectThunderstormBolt( int targetPlayer );
		void						effectBlackHoleBolt( int target );
		void						effectEletrifiedGround( int targetPlayer, int timeMs );
		void						effectWaterBeam( int targetPlayer );
		void 						effectFloodEffect( int targetPlayer );
		void 						effectSwampEffect( int targetPlayer, int timeMs );
		void						effectBallLightning( int targetPlayer, int timeMs );
		void						effectBallLightningBolt( int target );
		void						effectImplantBomb( int targetPlayer );
		void 						effectSun( int target, int timeMs );
		void						effectDarkStormBolt( int target);
		void 						effectVampiric( int player, int timeMs );

};
