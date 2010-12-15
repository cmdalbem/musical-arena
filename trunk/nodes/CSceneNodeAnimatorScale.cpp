#include "CSceneNodeAnimatorScale.h"

namespace irr
{
namespace scene
{

CSceneNodeAnimatorScale::CSceneNodeAnimatorScale(const core::vector3df& startScale, 
									  const core::vector3df& endScale, u32 timeForScaling,
									  bool loop, u32 now)
  : StartScale(startScale),
  EndScale(endScale),
  ScaleDiff(0, 0, 0),
  TimeFactor(0, 0, 0),
  StartTime(now),
  TimeForScaling(timeForScaling),
  Loop(loop)
{
	#ifdef _DEBUG
	setDebugName("CSceneNodeAnimatorScale");
	#endif

	recalculateImmediateValues();
}


void CSceneNodeAnimatorScale::recalculateImmediateValues()
{
	ScaleDiff = EndScale - StartScale;

	TimeFactor = ScaleDiff / static_cast<f32>(TimeForScaling);
}


CSceneNodeAnimatorScale::~CSceneNodeAnimatorScale(void)
{
}


void CSceneNodeAnimatorScale::animateNode(ISceneNode* node, u32 timeMs)
{
	if (!node)
		return;

	if( timeMs<StartTime )
		return;

	u32 t = (timeMs-StartTime);

	core::vector3df scale = StartScale;

	if (!Loop && t >= TimeForScaling)
		scale = EndScale;
	else
		scale += static_cast<f32>(fmod(static_cast<f32>(t), static_cast<f32>(TimeForScaling) ) ) * TimeFactor;

	node->setScale(scale);
}

void CSceneNodeAnimatorScale::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* /*options*/)
{
	out->addVector3d("StartScale", StartScale);
	out->addVector3d("EndScale", EndScale);
	out->addInt("TimeForScaling", TimeForScaling);
	out->addBool("Loop", Loop);
}


void CSceneNodeAnimatorScale::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* /*options*/)
{
	StartScale = in->getAttributeAsVector3d("StartScale");
	EndScale = in->getAttributeAsVector3d("EndScale");
	TimeForScaling = in->getAttributeAsInt("TimeForScaling");
	Loop = in->getAttributeAsBool("Loop");

	recalculateImmediateValues();
}

ISceneNodeAnimator* CSceneNodeAnimatorScale::createClone(ISceneNode* node, ISceneManager* newManager)
{
	return new CSceneNodeAnimatorScale(StartScale, EndScale, TimeForScaling, Loop, StartTime);
}

}	//end namespace scene
}	//end namespace irr

