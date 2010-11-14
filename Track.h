// Track.h
//

#pragma once

#include <vector>
using namespace std;

#include "Stone.h"
#include "TrackSceneNode.h"
#include "utils.h"

using irr::scene::ISceneManager;
using irr::video::IVideoDriver;
using namespace irr;


class Track
{
	public:
		Track( music* theMusic, double *musicTime, ISceneManager* sceneManager, IVideoDriver* driver, double speed, double posx=0, double posy=0, double posz=2 );
		~Track();
	
		vector<Stone*> 	stones[NUMBER_OF_FRETS];
		scene::ISceneNode *node;
				
		ISceneManager	*sceneManager;
		IVideoDriver	*driver;
		int				sizex, sizey;
		int				posx, posy, posz;
		double 			spawnDelay;
		double 			speed;
		int				musicPos;
		double 			*musicTime;
		music			*theMusic;	
			
		void			update();
		void			draw();		
		void 	 	 	processEvent( musicEvent event );
	
	private:
		
		double 			getStoneXPos( int track );
		void			insertStone( musicEvent event );
		void			drawTrack();
		void			drawStones();
		void			drawQuarters();

};
