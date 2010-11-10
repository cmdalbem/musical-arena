// Stone.h
//

#pragma once

#include "irrlicht.h"

#include "Decoder.h"

class Stone
{
	public:
		Stone(irr::scene::ISceneManager* sceneManager, musicEvent event, float x, float y, float z);

		Stone(irr::scene::ISceneManager* sceneManager, irr::scene::ISceneNode* sceneNode, musicEvent event, float x, float y, float z);

		void initialize( irr::scene::ISceneManager* sceneManager, musicEvent event, float x, float y, float z );

		~Stone();

		musicEvent				event;
		double					destroyTime;
		double		 			creationTime;
		
		irr::scene::ISceneNode 	*node;
		irr::core::vector3df	trailEndPos;
				
		double 					howLongActive( double musicTime );
		void 					update( double musicTime, double vel=DEFAULT_VEL );	
		
	private:
		irr::core::vector3df initPos;
};
