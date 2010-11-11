// Stone.h
//

#pragma once

#include "irrlicht.h"

#include "Decoder.h"

using irr::scene::ISceneManager;
using irr::scene::ISceneNode;
using irr::video::IVideoDriver;

class Stone
{
	public:
		Stone( ISceneManager* sceneManager, musicEvent event, double speed, float x, float y, float z );
		Stone( ISceneManager* sceneManager, musicEvent event, double speed, ISceneNode* sceneNode, float x, float y, float z );

		void initialize( ISceneManager* sceneManager, musicEvent event, double _speed, float x, float y, float z );

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
