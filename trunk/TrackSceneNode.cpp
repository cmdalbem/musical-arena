#include "TrackSceneNode.h"

#include "utils.h"

#include <iostream>
using namespace std;

using namespace irr;
using namespace irr::video;
using namespace irr::core;

TrackSceneNode::TrackSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, int posx)
		: scene::ISceneNode(parent, mgr, id)
{
	Material.Wireframe = false;
	//Material.Lighting = false;
	
	setPosition( vector3df(posx,0,TRACK_POS_Z) );

	Vertices[0] = S3DVertex(vector3df(-TRACK_SIZE_X/2,0,0),
							vector3df(0,0,-1),
							SColor(255,255,255,255),
							vector2df(0, 0));
	Vertices[1] = S3DVertex(vector3df(0,0,0),
							vector3df(0,0,-1),
							SColor(255,255,255,255),
							vector2df(0.5, 0));
	Vertices[2] = S3DVertex(vector3df(TRACK_SIZE_X/2,0,0),
							vector3df(0,0,-1),
							SColor(255,255,255,255),
							vector2df(1, 0));
	
	Vertices[3] = S3DVertex(vector3df(-TRACK_SIZE_X/2,-TRACK_SIZE_Y/2,0),
							vector3df(0,0,-1),
							SColor(255,255,255,255),
							vector2df(0, 0.5));
	Vertices[4] = S3DVertex(vector3df(0,-TRACK_SIZE_Y/2,0),
							vector3df(0,0,-1),
							SColor(255,255,255,255),
							vector2df(0.5, 0.5));
	Vertices[5] = S3DVertex(vector3df(TRACK_SIZE_X/2,-TRACK_SIZE_Y/2,0),
							vector3df(0,0,-1),
							SColor(255,255,255,255),
							vector2df(1, 0.5));
							
	Vertices[6] = S3DVertex(vector3df(-TRACK_SIZE_X/2,-TRACK_SIZE_Y,0),
							vector3df(0,0,-1),
							SColor(255,255,255,255),
							vector2df(0, 1));
	Vertices[7] = S3DVertex(vector3df(0,-TRACK_SIZE_Y,0),
							vector3df(0,0,-1),
							SColor(255,255,255,255),
							vector2df(0.5, 1));
	Vertices[8] = S3DVertex(vector3df(TRACK_SIZE_X/2,-TRACK_SIZE_Y,0),
							vector3df(0,0,-1),
							SColor(255,255,255,255),
							vector2df(1, 1));
					
	Box.reset(Vertices[0].Pos);
	for (s32 i=1; i<9; ++i)
			Box.addInternalPoint(Vertices[i].Pos);
			
	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	Material.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
	Material.setTexture(0, driver->getTexture("img/neck_alpha.png")); 
	Material.setFlag(video::EMF_TEXTURE_WRAP, video::ETC_REPEAT);
	Material.getTextureMatrix(0).setTextureScale(1,NECK_TEXTURE_PROPORTION);
	Material.Shininess = 30;
}

void TrackSceneNode::OnRegisterSceneNode()
{
	if( IsVisible )
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}
void TrackSceneNode::render()
{
	u16 indices[] = {      0,1,3, 1,4,3, 1,2,4, 2,5,4, 3,4,6, 4,7,6, 4,5,7, 5,8,7,    };
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	driver->setMaterial(Material);
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	driver->drawVertexPrimitiveList(&Vertices[0], 9, &indices[0], 8, video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_16BIT);
	
	// draw a line for every fret
	SMaterial m;
	m.Lighting = 0;
	m.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;
	driver->setMaterial(m);
	driver->setTransform(ETS_WORLD, matrix4());  //global positioning
	for(int i=0; i<NFRETS; i++)
		driver->draw3DLine( vector3df(getStoneXPos(i), 0, TRACK_POS_Z),
							vector3df(getStoneXPos(i), -TRACK_SIZE_Y, TRACK_POS_Z),
							SColor(170,0,0,0) );	
}

double TrackSceneNode::getStoneXPos( int track )
{
	return (track+0.5)*(TRACK_SIZE_X/NFRETS) - TRACK_SIZE_X/2 + getPosition().X;
}

const core::aabbox3d<f32>& TrackSceneNode::getBoundingBox() const
{
	return Box;
}

u32 TrackSceneNode::getMaterialCount() const
{
	return 1;
}

video::SMaterial& TrackSceneNode::getMaterial(u32 i)
{
	return Material;
}
