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

Track::Track( music* _theMusic, double *_musicTime, IrrlichtDevice *_device, double _speed, double _posx, double _posy, double _posz )
{
	theMusic = _theMusic;
	musicTime = _musicTime;
	
	speed = _speed;
	
	device = _device;
	sceneManager = device->getSceneManager();
	driver = device->getVideoDriver();
	
	sizex = TRACK_SIZE_X;
	sizey = TRACK_SIZE_Y;
	posx = _posx;
	posy = _posy;
	posz = _posz;
	
	musicPos = 0;
	
	spawnDelay = sizey/speed;
	
	glowTex = driver->getTexture("img/glow.png");
	
	node = new TrackSceneNode(sceneManager->getRootSceneNode(), sceneManager, 0, sizex, sizey, posx, posy, posz);
}

Track::~Track() {}


void Track::update()
{	
	nonPressedChord		= false;
	notesOnChord		= 0;
	chordCreationTime	= INT_MAX;

	// warns the player that there's a non-pressed chord to deal with
	for (int i = 0; i < NFRETS; i++)
		if (stones[i].size() > 0)
		{
			if( ((*musicTime > stones[i][0]->event.time + tolerance) || *musicTime > stones[i][0]->destroyTime) &&
				(stones[i][0]->pressed == false) &&
				(stones[i][0]->countedChord == false) )
			{
				nonPressedChord = true;
				chordCreationTime = stones[i][0]->event.time;
				//cout << "nota perdida: " << stones[i][0]->event.button << "\t chordCreationTime: " << chordCreationTime << endl;
			}	
		}

	// chord detection (done because, even some notes of the chord were pressed, we have to make them cause damage if
	// the entire chords wasn't)
	if (nonPressedChord)
		for (unsigned int i = 0; i < NFRETS; i++)
			if ((stones[i].size() > 0) &&
				(stones[i][0]->event.time == chordCreationTime ) )
			{
				stones[i][0]->countedChord = true;
				notesOnChord++;
				//cout << "nota: " << stones[i][0]->event.button << "\tcountedChord: " << stones[i][0]->countedChord
				//     << "\tpressed: " << stones[i][0]->pressed << endl;
			}


	// update stones 
	for (int i = 0; i < NFRETS; i++) {
	
		// calculates stones positions
		for(unsigned int j=0; j < stones[i].size(); j++) 
			stones[i][j]->update(*musicTime + spawnDelay);
	
		// deleting stones
		while ( (stones[i].size() > 0) && (*musicTime > stones[i][0]->destroyTime)) {
			destroyStone(i,0);
		}
	
		// hiding stones and non-pressed-notes treatment
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
	return (track+0.5)*(sizex/NFRETS) - sizex/2 + this->posx;
}

vector3df Track::getCentroid()
{
	return vector3df(posx,-TRACK_SIZE_Y/2,posz);
}

vector3df Track::getRandomPos()
{
	vector3df p = getCentroid();
	return vector3df(p.X + rand()%TRACK_SIZE_X - TRACK_SIZE_X/2, p.Y + rand()%TRACK_SIZE_Y - TRACK_SIZE_Y/2, p.Z);
}

void Track::insertStone( musicEvent event )
{
	Stone *newStone = new Stone(device, event, fretColors[event.button], glowTex, speed, getStoneXPos(event.button), this->posy, posz-0.1); //x,y,z
	
	stones[event.button].push_back(newStone);	
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

void Track::drawStones()
{
	for (int i = 0; i < NFRETS; i++)
		for(unsigned int k = 0; k < stones[i].size(); k++)
			stones[i][k]->draw(driver);
}

void Track::destroyStone( int fret, int stone )
{
	delete stones[fret][stone]; //call class destructor
	stones[fret].erase(stones[fret].begin()+stone); //remove reference (should call stone's destructor, but sometimes it doesn't work)
}

void Track::drawStoneTrails()
{
	drawStones();
	//drawQuarters();
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
