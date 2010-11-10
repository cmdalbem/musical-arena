// stone.cpp
//

#include "Stone.h"
#include "limits.h"

Stone::Stone(irr::scene::ISceneManager* sceneManager, musicEvent event, float x, float y, float z)
{
	initialize( sceneManager, event, x, y, z );
}

Stone::Stone(irr::scene::ISceneManager* sceneManager, irr::scene::ISceneNode* sceneNode, musicEvent event, float x, float y, float z)
{
	initialize( sceneManager, event, x, y, z );

	sceneNode->addChild( node );
}

void Stone::initialize( irr::scene::ISceneManager* sceneManager, musicEvent event, float x, float y, float z )
{
	this->event = event;
	this->destroyTime = event.time;
	this->initPos = irr::core::vector3df(x,y,z);
	this->trailEndPos = initPos;

	node = sceneManager->addSphereSceneNode();
	node->setPosition( initPos );
	node->setScale( irr::core::vector3df(0.1,0.1,0.1) );
}

Stone::~Stone()
{
	node->remove();
}

double Stone::howLongActive( double musicTime )
{
	return musicTime - event.time;
}

void Stone::update( double musicTime, double vel /*default value=DEFAULT_VEL*/ )
{
	// utilizes MRU equation to determine position of the stone
	irr::core::vector3df pos = node->getPosition();
	pos.Y = initPos.Y - howLongActive(musicTime)*vel;
	node->setPosition(pos);
	
	trailEndPos.Y = pos.Y + (destroyTime - event.time)*vel;
}
