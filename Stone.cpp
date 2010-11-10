// stone.cpp
//

#include "Stone.h"
#include "limits.h"

Stone::Stone(irr::scene::ISceneManager* sceneManager, musicEvent event, double _speed,
		float x, float y, float z)
{
	initialize( sceneManager, event, _speed, x, y, z );
}

Stone::Stone(irr::scene::ISceneManager* sceneManager, musicEvent event, double _speed, irr::scene::ISceneNode* sceneNode,
		float x, float y, float z)
{
	initialize( sceneManager, event, _speed, x, y, z );

	sceneNode->addChild( node );
}

void Stone::initialize( irr::scene::ISceneManager* sceneManager, musicEvent event, double _speed, float x, float y, float z )
{
	this->event = event;
	this->destroyTime = INT_MAX;
	this->initPos = irr::core::vector3df(x,y,z);
	this->trailEndPos = initPos;

	speed = _speed;

	node = sceneManager->addSphereSceneNode();
	node->setPosition( initPos );
	node->setScale( irr::core::vector3df(0.2,0.2,0.2) );
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
