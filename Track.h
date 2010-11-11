// Track.h
//

#pragma once

#include <vector>
using namespace std;

#include "Stone.h"
#include "utils.h"

using irr::scene::ISceneManager;
using irr::video::IVideoDriver;
using namespace irr;

class TrackSceneNode : public scene::ISceneNode
{
		core::aabbox3d<f32> Box;
        video::S3DVertex Vertices[4];
        video::SMaterial Material;
        
		public:

			TrackSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, int sizex, int sizey, int posx, int posy, int posz)
					: scene::ISceneNode(parent, mgr, id)
			{
					Material.Wireframe = false;
					Material.Lighting = false;

					Vertices[0] = video::S3DVertex(-sizex/2+posx-2,posy,posz, 1,1,0,
									video::SColor(255,0,255,255), 0, 1);
					Vertices[1] = video::S3DVertex(sizex/2+posx+2,posy,posz, 1,0,0,
									video::SColor(255,255,0,255), 1, 1);
					Vertices[3] = video::S3DVertex(-sizex/2+posx-2,-sizey+posy,posz, 0,1,1,
									video::SColor(255,255,255,0), 1, 0);
					Vertices[2] = video::S3DVertex(sizex/2+posx+2,-sizey+posy,posz, 0,0,1,
									video::SColor(255,0,255,0), 0, 0);
									
					Box.reset(Vertices[0].Pos);
					for (s32 i=1; i<4; ++i)
							Box.addInternalPoint(Vertices[i].Pos);
			}

			virtual void OnRegisterSceneNode() {
					if (IsVisible)
							SceneManager->registerNodeForRendering(this);

					ISceneNode::OnRegisterSceneNode();
			}
			
			virtual void render() {
					u16 indices[] = {       0,2,3, 2,1,3, 1,0,3, 2,0,1      };
					video::IVideoDriver* driver = SceneManager->getVideoDriver();

					driver->setMaterial(Material);
					driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
					driver->drawVertexPrimitiveList(&Vertices[0], 4, &indices[0], 4, video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_16BIT);
			}

			virtual const core::aabbox3d<f32>& getBoundingBox() const {
					return Box;
			}

			virtual u32 getMaterialCount() const {
					return 1;
			}

			virtual video::SMaterial& getMaterial(u32 i) {
					return Material;
			}       
};



class Track
{
	public:
		Track( ISceneManager* sceneManager, IVideoDriver* driver, double speed, int posx=0, int posy=0, int posz=0 );
		~Track();
	
		vector<Stone*> 	stones[NUMBER_OF_FRETS];
		scene::ISceneNode *node;
				
		ISceneManager* 	sceneManager;
		IVideoDriver*	driver;
		int				sizex, sizey;
		int				posx, posy, posz;
		double 			spawnDelay;
		double 			speed;
		int				musicPos;
		
		void			update( double musicTime );
		void			draw();		
		void 	 	 	processEvent( musicEvent event );
	
	private:
		void			insertStone( musicEvent event );
		void			drawTrack();
		void			drawTrails();
		void			drawStones();

};
