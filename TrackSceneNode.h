#include <irrlicht.h>
using namespace irr;


class TrackSceneNode : public scene::ISceneNode
{
		core::aabbox3d<f32> Box;
        video::S3DVertex 	Vertices[9];
        video::SMaterial 	Material;
        
	        		
		public:
			TrackSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, int posx);

			virtual void 						OnRegisterSceneNode();
			virtual void 						render();
			double 								getStoneXPos( int track );
			virtual const core::aabbox3d<f32>& 	getBoundingBox() const;
			virtual u32 						getMaterialCount() const;
			virtual video::SMaterial& 			getMaterial(u32 i);
};
