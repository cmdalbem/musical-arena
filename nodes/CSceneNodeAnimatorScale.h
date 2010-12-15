#pragma once
#include "ISceneNode.h"


namespace irr
{
namespace scene
{

class CSceneNodeAnimatorScale :
	public ISceneNodeAnimator
{
	public:

		/// constructor
		CSceneNodeAnimatorScale(const core::vector3df& startScale, 
									  const core::vector3df& endScale, u32 timeForScaling,
									  bool loop, u32 now);

		/// destructor
		virtual ~CSceneNodeAnimatorScale();

		/// animates a scene node
		virtual void animateNode(ISceneNode* node, u32 timeMs);

		/// Writes attributes of the scene node animator.
		virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0);

		/// Reads attributes of the scene node animator.
		virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);

		/// Returns type of the scene node animator
		virtual ESCENE_NODE_ANIMATOR_TYPE getType()	{ return ESNAT_UNKNOWN; }
		
		//! Creates a clone of this animator.
		/** Please note that you will have to drop
		(IReferenceCounted::drop()) the returned pointer after calling this.*/
		virtual ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager=0);		

	private:

		void recalculateImmediateValues();

		core::vector3df StartScale;
		core::vector3df EndScale;
		core::vector3df ScaleDiff;
		core::vector3df TimeFactor;
		u32 StartTime;
		u32 TimeForScaling;
		bool Loop;
};

}	//end namespace scene
}	//end namespace irr
