// Track.cpp
//

#include "Track.h"

using irr::video::SColor;
using irr::video::SMaterial;
using irr::core::vector3df;
using irr::core::aabbox3d;
using irr::core::rect;
using irr::core::position2d;
using irr::core::dimension2d;

Track::Track( ISceneManager* _sceneManager, IVideoDriver* _driver, double _speed, double _posx, double _posy, double _posz )
{
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

Track::~Track()
{

}

void Track::update( double musicTime )
{
	for (int i = 0; i < NUMBER_OF_FRETS; i++) {
	
		// calculates stones positions
		for(unsigned int j=0; j < stones[i].size(); j++) 
			stones[i][j]->update(musicTime + spawnDelay);
	
		// deleting stones
		while ( (stones[i].size() > 0) && (musicTime > stones[i][0]->destroyTime)) {
				
				delete stones[i][0]; //call class destructor
				stones[i].erase(stones[i].begin()); //remove reference (should call stone's destructor, but sometimes it doesn't work)
		}
	
		// hiding stones
		for(unsigned int k = 0; k < stones[i].size(); k++)
			if( musicTime > stones[i][k]->event.time )
				stones[i][k]->node->setVisible(false);
	}
	
}

void Track::insertStone( musicEvent event )
{
	Stone *newStone = new Stone(driver, this->sceneManager, event, speed, event.button*((sizex+1)/5) - sizex/2 + this->posx, this->posy, 0); //x,y,z
	
	stones[event.button].push_back(newStone);	
}

void Track::drawTrack()
{
	///*
	// Starting and Ending track lines
	SMaterial m;
	m.Thickness = 1;
	driver->setMaterial(m);
	driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());  //global positioning
	driver->draw3DBox( aabbox3d<float>(-sizex/2+posx-2, -sizey+posy, 0,
										sizex/2+posx+2, posy, 0),
						SColor(255,255,255,255) );
						//*/
						
	//node->render();
	
}	

void Track::drawStones()
{
	for (int i = 0; i < NUMBER_OF_FRETS; i++)
		for(unsigned int k = 0; k < stones[i].size(); k++)
			stones[i][k]->draw(driver);
}

void Track::draw()
{
	drawStones();
	drawTrack();
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
