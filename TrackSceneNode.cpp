#include "TrackSceneNode.h"

#include "utils.h"

#include <iostream>
using namespace std;

using namespace irr;
using namespace irr::video;
using namespace irr::core;

TrackSceneNode::TrackSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, int sizex, int sizey, int posx, int posy, int posz)
		: scene::ISceneNode(parent, mgr, id)
{
	Material.Wireframe = false;
	Material.Lighting = false;
	
	setPosition( vector3df(posx,posy,posz) );

	Vertices[0] = video::S3DVertex(-sizex/2-2,0,0, 0,0,-1,
					video::SColor(55,255,255,255), 0, 0);
	Vertices[1] = video::S3DVertex(sizex/2+2,0,0, 0,0,-1,
					video::SColor(55,255,255,255), 1, 0);
	Vertices[2] = video::S3DVertex(-sizex/2-2,-sizey,0, 0,0,-1,
					video::SColor(255,255,255,2555), 0, 1);
	Vertices[3] = video::S3DVertex(sizex/2+2,-sizey,0, 0,0,-1,
					video::SColor(255,255,255,255), 1, 1);
					//video::SColor(255,84,36,55), 0, 0);
					
	Box.reset(Vertices[0].Pos);
	for (s32 i=1; i<4; ++i)
			Box.addInternalPoint(Vertices[i].Pos);
			
	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	Material.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
	Material.setTexture(0, driver->getTexture("img/neck.png")); 
	Material.setFlag(video::EMF_TEXTURE_WRAP, video::ETC_REPEAT);
	//Material.setFlag(video::EMF_ZWRITE_ENABLE, false);
	Material.getTextureMatrix(0).setTextureScale(1,NECK_TEXTURE_PROPORTION);
}

void TrackSceneNode::OnRegisterSceneNode()
{
	if( IsVisible )
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}
void TrackSceneNode::render()
{
	u16 indices[] = {      0,1,3, 0,3,2      };
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	driver->setMaterial(Material);
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	driver->drawVertexPrimitiveList(&Vertices[0], 4, &indices[0], 2, video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_16BIT);
	
	// draw a line for every fret
	SMaterial m;
	m.Lighting = 0;
	m.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
	driver->setMaterial(m);
	driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());  //global positioning
	for(int i=0; i<NFRETS; i++)
		driver->draw3DLine( vector3df(getStoneXPos(i),getPosition().Y,getPosition().Z),
							vector3df(getStoneXPos(i),getPosition().Y-TRACK_SIZE_Y,getPosition().Z),
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
