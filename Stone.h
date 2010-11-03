// Stone.h
//

#pragma once

#include "irrlicht.h"

#include "Decoder.h"

class Stone
{
	public:
		Stone(irr::scene::ISceneManager* sceneManager, musicEvent event, struct timeval _creationTime, float x, float y, float z);
		~Stone();

		musicEvent				event;
		double					destroyTime;
		irr::scene::ISceneNode *node;
				
		double 					howLongActive();
		void 					update( double acc=DEFAULT_ACC );	
		
	private:
		struct timeval 		creationTime;
		irr::core::vector3df initPos;
};
