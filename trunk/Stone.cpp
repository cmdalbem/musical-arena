// stone.cpp
//

#include "Stone.h"
#include "limits.h"

Stone::Stone(musicEvent event, irr::scene::ISceneManager* sceneManager, float x, float y, float z)
{
	this->event = event;
	this->destroy_time = INT_MAX;
	
	// creates a new scene node
	node = sceneManager->addSphereSceneNode();
	node->setPosition( irr::core::vector3df(x,y,z) );
	node->setScale( irr::core::vector3df(0.1,0.1,0.1) );
} 

Stone::~Stone()
{
	node->drop();
	node = NULL;
	cout << "destroyed." << endl;
}

void Stone::draw()
{
	
	
}

void Stone::update()
{
	irr::core::vector3df pos = node->getPosition();
	//pos.Y -= 300*frameDeltaTime;
	pos.Y -= 0.00001;
	node->setPosition(pos);	
}
