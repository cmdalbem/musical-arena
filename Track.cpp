// Track.cpp
//

#include "Track.h"
#include "utils.h"

using irr::video::SColor;
using irr::video::SMaterial;
using irr::core::vector3df;
using irr::core::aabbox3d;
using irr::core::rect;
using irr::core::position2d;
using irr::core::dimension2d;

Track::Track( music* _theMusic, double *_musicTime, ISceneManager* _sceneManager, IVideoDriver* _driver, double _speed, double _posx, double _posy, double _posz )
{
	theMusic = _theMusic;
	musicTime = _musicTime;
	
	speed = _speed;
	sceneManager = _sceneManager;
	driver = _driver;
	
	sizex = 30;
	sizey = 70;
	posx = _posx;
	posy = _posy;
	posz = _posz;
	
	musicPos = 0;
	
	spawnDelay = sizey/speed;
	
	node = new TrackSceneNode(NULL, _sceneManager, 0, sizex, sizey, posx, posy, posz);
}

Track::~Track() {}


void Track::update()
{
	// update stones 
	for (int i = 0; i < NUMBER_OF_FRETS; i++) {
	
		// calculates stones positions
		for(unsigned int j=0; j < stones[i].size(); j++) 
			stones[i][j]->update(*musicTime + spawnDelay);
	
		// deleting stones
		while ( (stones[i].size() > 0) && (*musicTime > stones[i][0]->destroyTime)) {
				
				delete stones[i][0]; //call class destructor
				stones[i].erase(stones[i].begin()); //remove reference (should call stone's destructor, but sometimes it doesn't work)
		}
	
		// hiding stones
		for(unsigned int k = 0; k < stones[i].size(); k++)
			if( *musicTime > stones[i][k]->event.time )
				stones[i][k]->node->setVisible(false);
	}
	
	// update texture position
	node->getMaterial(0).getTextureMatrix(0).setTextureTranslate( 0, //translate on x
																  0/*track's creation time*/ - ((*musicTime)*speed) / (sizey/NECK_TEXTURE_PROPORTION) );
}

double Track::getStoneXPos( int track )
{
	return (track+0.5)*(sizex/NUMBER_OF_FRETS) - sizex/2 + this->posx;
}

void Track::insertStone( musicEvent event )
{
	Stone *newStone = new Stone(driver, this->sceneManager, event, speed, getStoneXPos(event.button), this->posy, posz-0.1); //x,y,z
	
	stones[event.button].push_back(newStone);	
}

void Track::drawTrack()
{
	node->render();

	// draw a line for fret
	SMaterial m;
	m.Lighting = 0;
	m.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
	driver->setMaterial(m);
	driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());  //global positioning
	for(int i=0; i<NUMBER_OF_FRETS; i++)
		driver->draw3DLine( vector3df(getStoneXPos(i),posy,posz),
							vector3df(getStoneXPos(i),posy-sizey,posz),
							SColor(170,0,0,0) );
}	

void Track::drawStones()
{
	for (int i = 0; i < NUMBER_OF_FRETS; i++)
		for(unsigned int k = 0; k < stones[i].size(); k++)
			stones[i][k]->draw(driver);
}

void Track::drawQuarters()
{
	#define debug(x) cout<<#x<<" = "<<x<<endl

	double spqn; // seconds per quarter note
	if( theMusic->size() < (unsigned int)musicPos-1)
		spqn = theMusic->back().mspqn/1000000.0;
	else
		spqn = theMusic->at(musicPos).mspqn/1000000.0;

	double trackTime = sizey/speed;
	
	double howManyLines = trackTime/spqn;
	
	double offset = fmod(*musicTime,spqn);
	

	// draw one line for each quarter note that fits in the track
	SMaterial m;
	m.Lighting = 0;
	m.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
	driver->setMaterial(m);
	driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());  //global positioning
	for(int i=0; i<(int)howManyLines; i++)
		driver->draw3DLine( vector3df(posx-sizex/2, posy - ( trackTime - (i+1)*spqn + offset )*speed,posz-1),
							vector3df(posx+sizex/2, posy - ( trackTime - (i+1)*spqn + offset )*speed,posz-1),
							SColor(70,255,255,255) ); 
} 

void Track::draw()
{
	drawStones();
	drawTrack();
	drawQuarters();
}

void Track::processEvent( musicEvent event )
{
	int track = event.button; //0~4
	
	switch(event.type) { 
		case ON:					
			insertStone(event);
			break;
		
		case OFF:
			if(stones[track].size() > 0)
				stones[track].back()->destroyTime = event.time;// sets destroy_time
			else
				cout << "received an OFF without an ON O.o" << endl; // lol
			break;
						
		default:
			break;
	}
	
	this->musicPos++;
}
