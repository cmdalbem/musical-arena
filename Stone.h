// Stone.h
//

#pragma once

#include "irrlicht.h"

#include "Decoder.h"

using irr::scene::ISceneManager;
using irr::scene::ISceneNode;
using irr::video::IVideoDriver;
using irr::video::IVideoDriver;
using irr::video::SColor;

class Stone
{
	public:
		Stone( IVideoDriver *driver, ISceneManager* sceneManager, musicEvent event, SColor stoneColor, double speed, float x, float y, float z );
		Stone( IVideoDriver *driver, ISceneManager* sceneManager, musicEvent event, SColor stoneColor, double speed, ISceneNode* sceneNode, float x, float y, float z );

		void initialize( IVideoDriver *driver, ISceneManager* sceneManager, musicEvent event, SColor stoneColor, double _speed, float x, float y, float z );

		~Stone();

		musicEvent				event;
		double					destroyTime;
		double		 			creationTime;
		
		irr::scene::ISceneNode 	*node;
		irr::core::vector3df	trailEndPos;
				
		double 					howLongActive( double musicTime );
		void 					update( double musicTime );	
		void					draw( IVideoDriver* driver );
		
	private:
		irr::core::vector3df 	initPos;
		double					speed;
		
};
