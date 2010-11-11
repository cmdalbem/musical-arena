#pragma once

#include <vector>
using namespace std;

#include "Stone.h"
#include "utils.h"

using irr::scene::ISceneManager;
using irr::video::IVideoDriver;

class Track
{
	public:
		Track( ISceneManager* sceneManager, IVideoDriver* driver, double speed );
		~Track();
	
		vector<Stone*> 	stones[NUMBER_OF_FRETS];
				
		ISceneManager* 	sceneManager;
		IVideoDriver*	driver;
		int				sizex, sizey;
		int				posx, posy;
		double 			spawnDelay;
		double 			speed;
		
		void			update( double musicTime );
		void			draw();		
		void 	 	 	processEvent( musicEvent event );
	
	private:
		void			insertStone( musicEvent event );
		void			drawDelimiters();
		void			drawTrails();
		void			drawStones();

};
