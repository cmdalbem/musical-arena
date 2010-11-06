// stone.cpp
//

#include "Stone.h"
#include "limits.h"

Stone::Stone(irr::scene::ISceneManager* sceneManager, musicEvent event, struct timeval _creationTime, float x, float y, float z)
{
	this->event = event;
	this->destroyTime = INT_MAX;
	this->creationTime = _creationTime;
	this->initPos = irr::core::vector3df(x,y,z);

	node = sceneManager->addSphereSceneNode();
	node->setPosition( initPos );
	node->setScale( irr::core::vector3df(0.1,0.1,0.1) );
} 

Stone::~Stone()
{
	node->remove();
	cout << "destroyed." << endl;
}

double Stone::howLongActive()
{	return time_diff(creationTime);	}

void Stone::update( double acc /*default value=DEFAULT_ACC*/ )
{
	// utilizes MRU equation to determine position of the stone
	irr::core::vector3df pos = node->getPosition();
	pos.Y = initPos.Y - time_diff(creationTime)*acc;
	node->setPosition(pos);	
}
