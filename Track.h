// Track.h
//

#pragma once

#include <vector>
using namespace std;

#include "Stone.h"
#include "TrackSceneNode.h"
#include "Fretting.h"
#include "utils.h"
#include <limits.h>

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
		Track( music* theMusic, double *musicTime, IrrlichtDevice *device, double speed, double posx=0, double posy=0, double posz=2 );
		~Track();
	
		vector<Stone*> 	stones[NFRETS];
		scene::ISceneNode *node;
				
		int				sizex, sizey;
		int				posx, posy, posz;
		double 			spawnDelay;
		double 			speed;
		int				musicPos;
		double 			*musicTime;
		music			*theMusic;
		Fretting		*fretting;
		
		// used for update()
		double			tolerance;		// to warn when the balls passed their time without being pressed
		double			chordCreationTime;
		bool			nonPressedChord;	// indicates if the player didn't press a chord
		int				notesOnChord;		// how many notes did we have on the chord??
			
		void			update();
		void			draw();		
		void 	 	 	processEvent( musicEvent event );
		
		double 			getStoneXPos( int track );
	
	private:
		IrrlichtDevice 	*device;
		ISceneManager	*sceneManager;
		IVideoDriver	*driver;
		ITexture		*glowTex;
		
		void			insertStone( musicEvent event );
		void			drawFretLines();
		void			drawQuarters();
		void			drawStones();

};
