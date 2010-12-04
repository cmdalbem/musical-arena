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
	
		
		void 						createShield( int targetPlayer );
		void 						createSpellEffect( int targetPlayer );
		void						createFireRain( int targetPlayer );
		void						createFireball( int targetPlayer, bool randomizeTarget );
		void						createFireball( array<vector3df> path );
		void						createAreaEffect(int player, ITexture *tex );
		void						createElectricEffect( int targetPlayer );
		void						createWaterBeam( int targetPlayer );
		void						splitBlood( int targetPlayer );
	
	private:
		vector<Player*>				players;
		
		IrrlichtDevice				*device;
		IVideoDriver				*driver;
		ISceneManager				*smgr;
		
		CBloodEffect				*blood;
		IMeshSceneNode 				*shields[NPLAYERS];
		IVolumeLightSceneNode 		*spellEffect;
		ITexture 					*fireballTex, *glowTex, *laserTex, *bloodTex[5], *waterTex[5];
		array<video::ITexture*> 	spellEffectTex;
		

};
