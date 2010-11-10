// Stone.h
//

#pragma once

#include "irrlicht.h"

#include "Decoder.h"

class Stone
{
	public:
		Stone(irr::scene::ISceneManager* sceneManager, musicEvent event, double speed, float x, float y, float z);
		Stone(irr::scene::ISceneManager* sceneManager, musicEvent event, double speed, irr::scene::ISceneNode* sceneNode, float x, float y, float z);

		void initialize( irr::scene::ISceneManager* sceneManager, musicEvent event, double _speed, float x, float y, float z );

		~Stone();

		musicEvent				event;
		double					destroyTime;
		double		 			creationTime;
		
		irr::scene::ISceneNode 	*node;
		irr::core::vector3df	trailEndPos;
				
		double 					howLongActive( double musicTime );
		void 					update( double musicTime );	
		
	private:
		irr::core::vector3df 	initPos;
		double					speed;
		
};
