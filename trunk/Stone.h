// Stone.h
//

#pragma once

#include "irrlicht.h"

#include "Decoder.h"

using namespace irr::core;
using namespace irr::video;
using namespace irr::scene;
using namespace irr;

class Stone
{
	public:
		Stone( IrrlichtDevice *device, musicEvent event, SColor stoneColor, ITexture *glowTex, double speed, float x, float y, float z );

		void initialize( IrrlichtDevice *device, musicEvent event, SColor stoneColor, ITexture *glowTex, double _speed, float x, float y, float z );

		~Stone();

		musicEvent				event;
		double					destroyTime;
		double		 			creationTime;
		
		bool					pressed;
		bool					countedChord;
		
		ISceneNode 				*node;
		ISceneNode 				*glow;
		
		vector3df				trailEndPos;
		vector3df				displace;

		vector3df				getPosition();
		double 					howLongActive( double musicTime );
		void 					update( double musicTime );	
		void					draw( IVideoDriver* driver );
		
	private:
		irr::core::vector3df 	initPos;
		double					speed;
		
		void					drawTrail( IVideoDriver* driver );
		
};
