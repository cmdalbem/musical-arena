#include "CBoltSceneNode.h"
namespace irr
{
    namespace scene
    {
        CBoltSceneNode::CBoltSceneNode( scene::ISceneNode* parent, scene::ISceneManager *mgr, s32 id) : ISceneNode( parent, mgr, id )
        {
            //ctor
            material.Wireframe = false;
            material.Lighting = false;
            AutomaticCullingState = EAC_FRUSTUM_SPHERE;
        }

        CBoltSceneNode::~CBoltSceneNode()
        {
            //dtor
        }

        void CBoltSceneNode::OnRegisterSceneNode(void)
        {
            if ( IsVisible )
            {
                SceneManager->registerNodeForRendering( this );
            }
        }

        void CBoltSceneNode::OnAnimate(u32 timeMs)
        {

            if ( IsVisible )
            {
                for (u32 i=0;i<boltarray.size();i++)
                {
                    boltarray[i].Update(timeMs);
                }
            }

            ISceneNode::OnAnimate(timeMs);
        }

        void CBoltSceneNode::setLine(core::vector3df start, core::vector3df end, u32 updateTime, u32 height, u32 parts, u32 bolts, bool steddyend, video::SColor color)
        {
           setPosition(start);

            srand(parts);
            boltarray.clear();

            m_start = core::vector3df(0,0,0);
            m_end = end-start;
            m_parts = parts;
            material.DiffuseColor = color;

            core::vector3df dir = end-start;
            dir/=(f32)parts;

            for (u32 i=0;i<bolts;i++)
            {
                //printf("add Bolt\n");
                boltarray.push_back(Bolt(height, updateTime, steddyend));

                for (u32 a=0;a<parts;a++)
                {
                    //printf("add Part[%i] ", a);
                    boltarray[i].points.push_back(m_start+dir*a);
                    if (a == parts-1 || a == 0)
                        boltarray[i].delta.push_back(0.0f);
                    else
                    {
                        u32 h = height*2;
                        f32 d = (rand()%h);
                        d-=(f32)height;
                        //printf("Delta: %f", d);
                        boltarray[i].delta.push_back(d);
                    }
                    //printf("\n");
                }

                u32 vec = rand()%2;
            //printf("vec: %i\n", vec);

            u32 cord = rand()%10+1;

                if (dir.X != 0)
                {
                    if (vec == 0)
                        boltarray[i].normal = core::vector3df(-dir.Y/dir.X*cord,cord,0);
                    else
                        boltarray[i].normal = core::vector3df(-dir.Z/dir.X*cord,0,cord);
                }
                else if (dir.Y != 0)
                {
                    if (vec == 0)
                        boltarray[i].normal = core::vector3df(cord,-dir.X/dir.Y*cord,0);
                    else
                        boltarray[i].normal = core::vector3df(0,-dir.Z/dir.Y*cord,cord);
                }
                else if (dir.Z != 0)
                {
                    if (vec == 0)
                        boltarray[i].normal = core::vector3df(cord,0,-dir.X/dir.Z*cord);
                    else
                        boltarray[i].normal = core::vector3df(0,cord,-dir.Y/dir.Z*cord);
                }
                boltarray[i].normal.normalize();
            }
        }

        void CBoltSceneNode::render(void)
        {
            video::IVideoDriver* driver = SceneManager->getVideoDriver();
            driver->setTransform(irr::video::ETS_WORLD, AbsoluteTransformation);
            driver->setMaterial(material);

            for (u32 i=0;i<boltarray.size();i++)
            {
                boltarray[i].draw(driver, material.DiffuseColor);
            }
        }

        const core::aabbox3d<f32>& CBoltSceneNode::getBoundingBox() const
        {
            return Box;
        }

        u32 CBoltSceneNode::getMaterialCount() const
        {
            return 1;
        }

        video::SMaterial& CBoltSceneNode::getMaterial(u32 i)
        {
            return material;
        }
    }
} 
