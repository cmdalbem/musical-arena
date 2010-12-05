/*******************************************************************
 * Copyright (C) 2006 Brett R. Jones issued to public domain
 *********************************************************************/
#include "irrlicht.h"
#include "VxHealthSceneNode.h"
#include "ISceneManager.h"
#include "IVideoDriver.h"

#include <iostream>
using namespace std;


namespace irr
{
		namespace scene
		{

				//! constructor
				VxHealthSceneNode::VxHealthSceneNode( ISceneNode* parent,
								ISceneManager* mgr,
								s32 id,
								s32 direction, bool vertical,
								s32 width,
								s32 height,
								const core::vector3df& position,
								video::SColor BarColorIn,
								video::SColor BkgColorIn,
								video::SColor BorderColorIn )
						: ISceneNode(parent, mgr, id, position),
						BarColor(BarColorIn),
						BorderColor(BorderColorIn),
						BkgColor(BkgColorIn),
						m_gDim( width, height ),
						m_bBorder(1),
						isVisible(1),
						m_s32Percent(100),
						direction(direction),
						isVertical(vertical)
				{
#ifdef _DEBUG
						setDebugName("VxHealthSceneNode");
#endif

						setAutomaticCulling(EAC_OFF);
						//////////////////////////////////////////////////////////
						//Set Bounding box
						f32 halfWidth = (f32)m_gDim.Width/2;
						f32 halfHeight = (f32)m_gDim.Height/2;
						Box.MinEdge.set(-halfWidth,-halfHeight,-1.0);
						Box.MaxEdge.set(halfWidth,halfHeight,1.0);
				}

				//! destructor
				VxHealthSceneNode::~VxHealthSceneNode()
				{
				}

				void VxHealthSceneNode::OnRegisterSceneNode()
				{
					if (IsVisible)
						SceneManager->registerNodeForRendering(this, ESNRP_SHADOW);
					
					ISceneNode::OnRegisterSceneNode();
				} 

				//! renders the node.
				void VxHealthSceneNode::render()
				{
						if(!isVisible)
						{
								return;
						}

						/////////////////////////////////////////////////////////////////////
						video::IVideoDriver* driver = SceneManager->getVideoDriver();
						ICameraSceneNode* camera = SceneManager->getActiveCamera();

						if (!camera || !driver)
								return;

						if (DebugDataVisible)
						{
								driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
								video::SMaterial m;
								m.Lighting = false;
								driver->setMaterial(m);
								driver->draw3DBox(Box, video::SColor(0,208,195,152));
						}
						//////////////////////////////////////////////////////////////////////

						// calculate health bar rectangle
						core::rect<s32> AbsoluteRect( core::position2d<s32>(getAbsolutePosition().X,getAbsolutePosition().Y), m_gDim );
						// center the bar
						f32 halfWidth = (f32)m_gDim.Width/2;
						AbsoluteRect.UpperLeftCorner.X -= halfWidth;
						AbsoluteRect.LowerRightCorner.X -= halfWidth;

						core::rect<s32> BarRect = AbsoluteRect;

						// draw boarder if needed
						if( m_bBorder )
						{
								driver->draw2DLine( BarRect.UpperLeftCorner,
													core::vector2di(BarRect.LowerRightCorner.X,BarRect.UpperLeftCorner.Y),
													BorderColor);
								driver->draw2DLine( core::vector2di(BarRect.UpperLeftCorner.X,BarRect.LowerRightCorner.Y),
													BarRect.LowerRightCorner,
													BorderColor);
								
								driver->draw2DLine( BarRect.UpperLeftCorner,
													core::vector2di(BarRect.UpperLeftCorner.X,BarRect.LowerRightCorner.Y),
													BorderColor);
								driver->draw2DLine( core::vector2di(BarRect.LowerRightCorner.X,BarRect.UpperLeftCorner.Y),
													BarRect.LowerRightCorner,
													BorderColor);
								
								// shrink by one for bar
								BarRect.UpperLeftCorner.X += 1;
								BarRect.UpperLeftCorner.Y += 1;
								BarRect.LowerRightCorner.X -= 1;
						}
						// calculate progress bar
						if(isVertical)
							MaxNum = (BarRect.UpperLeftCorner.Y - BarRect.LowerRightCorner.Y) - 1;
						else
							MaxNum = (BarRect.LowerRightCorner.X - BarRect.UpperLeftCorner.X) - 1;
						s32 PercentNum = (s32)((m_s32Percent * MaxNum) / 100.);
						
						// draw progress part
						core::rect<s32> LoadRect = BarRect;

						if(isVertical) 
							LoadRect.UpperLeftCorner.Y -= PercentNum;
						else if(direction==1)	
							LoadRect.UpperLeftCorner.X += PercentNum;
						else
							LoadRect.LowerRightCorner.X -= PercentNum;
						driver->draw2DRectangle( BkgColor, LoadRect );
				}


				//! returns the axis aligned bounding box of this node
				const core::aabbox3d<f32>& VxHealthSceneNode::getBoundingBox() const
				{
						return Box;
				}

				//! returns amount of materials used by this scene node.
				s32 VxHealthSceneNode::getMaterialCount()
				{
						return 0;
				}

				//! sets the progress percentage ( or health )
				void VxHealthSceneNode::setProgress( s32 s32Percent )
				{
						m_s32Percent = s32Percent;
						if( m_s32Percent < 0 )
						{
								m_s32Percent = 0;
						}
						if( m_s32Percent > 100 )
						{
								m_s32Percent = 100;
						}

				}

				//! sets the color of the progress
				void VxHealthSceneNode::setProgressColor(video::SColor color)
				{
						BarColor = color;
				}

				//! sets the color of the progress bar background
				void VxHealthSceneNode::setBackgroundColor(video::SColor color)
				{
						BkgColor = color;
				}

				//! sets the color of the progress bar border
				void VxHealthSceneNode::setBorderColor(video::SColor color)
				{
						BorderColor = color;
				}


		} // end namespace scene
} // end namespace irr

//------------------------------------------------------------------------
