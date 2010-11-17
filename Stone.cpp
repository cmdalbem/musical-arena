// stone.cpp
//

#include "Stone.h"
#include "utils.h"
#include "limits.h"

using irr::scene::ISceneManager;
using irr::scene::ISceneNode;
using irr::video::SColor;
using irr::video::IVideoDriver;
using irr::video::SColor;

Stone::Stone( IVideoDriver *driver, ISceneManager* sceneManager, musicEvent event, SColor stoneColor, double _speed, 
		float x, float y, float z)
{
	initialize( driver, sceneManager, event, stoneColor, _speed, x, y, z );
}

Stone::Stone( IVideoDriver *driver, ISceneManager* sceneManager, musicEvent event, SColor stoneColor, double _speed, ISceneNode* sceneNode,
		float x, float y, float z)
{
	initialize( driver, sceneManager, event, stoneColor, _speed, x, y, z );

	sceneNode->addChild( node );
}

void Stone::initialize( IVideoDriver *driver, ISceneManager* sceneManager, musicEvent event, SColor stoneColor, double _speed, float x, float y, float z )
{
	this->event = event;
	this->destroyTime = INT_MAX;
	this->initPos = irr::core::vector3df(x,y,z);
	this->trailEndPos = initPos;
	
	this->pressed = false;
	this->countedChord = false;
 
	speed = _speed;

	node = sceneManager->addSphereSceneNode(STONE_RADIUS);
	node->setPosition( initPos );
	
	node->getMaterial(0).EmissiveColor = stoneColor;
}

Stone::~Stone()
{
	node->remove();
}

double Stone::howLongActive( double musicTime )
{
	return musicTime - event.time;
}

void Stone::update( double musicTime )
{
	// utilizes MRU equation to determine position of the stone
	irr::core::vector3df pos = node->getPosition();
	pos.Y = initPos.Y - howLongActive(musicTime)*speed;
	node->setPosition(pos);
	
	trailEndPos.Y = pos.Y + (destroyTime - event.time)*speed;
	if( trailEndPos.Y > initPos.Y )
		trailEndPos.Y = initPos.Y;
}

void Stone::draw( irr::video::IVideoDriver* driver )
{
	#define TOLERANCE (0.1)
	// draw Trails
	irr::video::SMaterial m = node->getMaterial(0);
	m.Thickness = 2;
	m.Lighting = 0;
	driver->setMaterial(m);
	driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4()); //global positioning
	driver->draw3DLine( node->getPosition(), this->trailEndPos, node->getMaterial(0).EmissiveColor );
	
	//tolerance box
	m.Thickness = 1;
	driver->draw3DBox(  irr::core::aabbox3df( node->getPosition().X-2, node->getPosition().Y-TOLERANCE*speed, node->getPosition().Z,
											  node->getPosition().X+2, node->getPosition().Y+TOLERANCE*speed, node->getPosition().Z),
					   node->getMaterial(0).EmissiveColor );
}
