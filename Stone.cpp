// stone.cpp
//

#include "Stone.h"
#include "limits.h"

using irr::scene::ISceneManager;
using irr::scene::ISceneNode;
using irr::video::SColor;

Stone::Stone( ISceneManager* sceneManager, musicEvent event, double _speed,
		float x, float y, float z)
{
	initialize( sceneManager, event, _speed, x, y, z );
}

Stone::Stone( ISceneManager* sceneManager, musicEvent event, double _speed, ISceneNode* sceneNode,
		float x, float y, float z)
{
	initialize( sceneManager, event, _speed, x, y, z );

	sceneNode->addChild( node );
}

void Stone::initialize( ISceneManager* sceneManager, musicEvent event, double _speed, float x, float y, float z )
{
	this->event = event;
	this->destroyTime = INT_MAX;
	this->initPos = irr::core::vector3df(x,y,z);
	this->trailEndPos = initPos;

	speed = _speed;

	node = sceneManager->addSphereSceneNode();
	node->setPosition( initPos );
	node->setScale( irr::core::vector3df(0.2,0.2,0.2) );
	
	switch(event.button) {
		case B1:
			node->getMaterial(0).EmissiveColor = SColor(255,0,255,0);
			break;
		case B2:
			node->getMaterial(0).EmissiveColor = SColor(255,255,0,0);
			break;
		case B3:
			node->getMaterial(0).EmissiveColor = SColor(255,255,255,0);
			break;
		case B4:
			node->getMaterial(0).EmissiveColor = SColor(255,0,0,255);
			break;
		case B5:
			node->getMaterial(0).EmissiveColor = SColor(255,255,128,0);
			break;
		default:
			break;
	}
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

void Stone::drawTrail( irr::video::IVideoDriver* driver )
{
		irr::video::SMaterial m = node->getMaterial(0);
		m.Thickness = 2;
		driver->setMaterial(m);
		driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4()); //global positioning
		//driver->setMaterial( stonesOnScreen[i][k]->node->getMaterial(0) );
		driver->draw3DLine( node->getPosition(), this->trailEndPos, node->getMaterial(0).EmissiveColor );
}
