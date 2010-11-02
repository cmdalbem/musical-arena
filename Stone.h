// Stone.h
//

#pragma once

#include "irrlicht.h"

#include "Decoder.h"

class Stone
{
	public:
		Stone(musicEvent event, irr::scene::ISceneManager* sceneManager, float x, float y, float z);
		~Stone();

		musicEvent	event;
		double		destroy_time;
		irr::scene::ISceneNode *node;
				
		void draw();
		void update();	
};
