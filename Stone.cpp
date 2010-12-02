// stone.cpp
//

#include "Stone.h"
#include "utils.h"
#include "limits.h"

using namespace irr::scene;
using namespace irr::core;
using namespace irr::video;

Stone::Stone( IrrlichtDevice *device, musicEvent event, SColor stoneColor, ITexture *glowTex, double _speed, 
		float x, float y, float z)
{
	initialize( device, event, stoneColor, glowTex, _speed, x, y, z );
}

Stone::Stone( IrrlichtDevice *device, musicEvent event, SColor stoneColor, ITexture *glowTex, double _speed, ISceneNode* sceneNode,
		float x, float y, float z)
{
	initialize( device, event, stoneColor, glowTex, _speed, x, y, z );

	sceneNode->addChild( node );
}

void Stone::initialize( IrrlichtDevice *device, musicEvent event, SColor stoneColor, ITexture *glowTex, double _speed, float x, float y, float z )
{
	ISceneManager *sceneManager = device->getSceneManager();
	
	this->event = event;
	this->destroyTime = INT_MAX;
	this->initPos = vector3df(x,y,z);
	this->trailEndPos = initPos;
	
	this->pressed = false;
	this->countedChord = false;
 
	speed = _speed;

	node = sceneManager->addSphereSceneNode(STONE_RADIUS);
	node->setPosition( initPos );
	node->getMaterial(0).EmissiveColor = stoneColor;
	  
	glow = sceneManager->addBillboardSceneNode(node, dimension2d<float>(10, 10));
	glow->setMaterialFlag(EMF_LIGHTING, false);
	glow->setMaterialType(EMT_TRANSPARENT_ADD_COLOR); 
	((IBillboardSceneNode*)glow)->setColor(stoneColor);
	glow->setMaterialTexture(0, glowTex);
	//glow->addAnimator( sceneManager->createFlyCircleAnimator(glow->getPosition(), 1.5f,0.01,vector3df(0,0,1)) ); //gay stuff

}

Stone::~Stone()
{
	//node->remove();
}

double Stone::howLongActive( double musicTime )
{
	return musicTime - event.time;
}

void Stone::update( double musicTime )
{
	// utilizes MRU equation to determine position of the stone
	vector3df pos = node->getPosition();
	pos.Y = initPos.Y - howLongActive(musicTime)*speed;
	node->setPosition(pos);
	
	trailEndPos.Y = pos.Y + (destroyTime - event.time)*speed;
	// culling of the ending of the line
	if( trailEndPos.Y > initPos.Y )
		trailEndPos.Y = initPos.Y;
}

void Stone::drawTrail( IVideoDriver* driver )
{
	// draw Trails
	SMaterial m = node->getMaterial(0);
	m.Thickness = 2;
	m.Lighting = 0; 
	driver->setMaterial(m);
	driver->setTransform(ETS_WORLD, matrix4()); //global positioning
	
	// culling of the beggining of the line
	vector3df startPos =  node->getPosition();
	if( startPos.Y < -TRACK_SIZE_Y )
		startPos.Y = -TRACK_SIZE_Y;
	
	driver->draw3DLine( startPos, this->trailEndPos, node->getMaterial(0).EmissiveColor );	
}

void Stone::draw( IVideoDriver* driver )
{
	drawTrail(driver);
	
	// draw Tolerance Boxes
	
	/*#define TOLERANCE (0.1)
	//tolerance box
	m.Thickness = 1;
	driver->draw3DBox(  irr::core::aabbox3df( node->getPosition().X-2, node->getPosition().Y-TOLERANCE*speed, node->getPosition().Z,
											  node->getPosition().X+2, node->getPosition().Y+TOLERANCE*speed, node->getPosition().Z),
					   node->getMaterial(0).EmissiveColor );*/
}
