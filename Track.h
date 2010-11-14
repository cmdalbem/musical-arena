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


static const irr::video::SColor fretColors[] = { SColor(255,0,255,0),
												SColor(255,255,0,0),
												SColor(255,255,255,0),
												SColor(255,0,0,255),
												SColor(255,255,128,0) };

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
		
		double 			getStoneXPos( int track );
	
	private:
		
		void			insertStone( musicEvent event );
		void			drawTrack();
		void			drawStones();
		void			drawQuarters();

};
