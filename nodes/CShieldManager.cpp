#include "CShieldManager.h"

#include <iostream>
using namespace std;

CShieldManager::CShieldManager(scene::ISceneManager* psmgr, irr::ITimer *ptimer){
    //Assign all parameters
    smgr = psmgr;
    driver=psmgr->getVideoDriver();
    timer = ptimer;

    ShaderType=-1;
    shaderMaterial = video::EMT_SOLID;

    //Witout OpenGL these shaders won't work
    if(!driver->getDriverType()==video::EDT_OPENGL)
        return;

    if(driver->queryFeature(video::EVDF_ARB_GLSL)){
        ///GLSL Shader Code
        //Vertex Shader
        const c8 *vertShader = "varying vec3 vertpos;           \n\
                                                                \n\
                        void main(void)                         \n\
                        {                                       \n\
                            vertpos = gl_Vertex.xyz;            \n\
                                                                \n\
                            gl_TexCoord[0] = gl_MultiTexCoord0; \n\
                            gl_Position = ftransform();         \n\
                        }                                       \n\
                        ";

        //Fragment Shader
        const c8 *fragShader = "uniform sampler2D ColorMap;                                                                     \n\
                        uniform sampler2D GradientTexture;                                                                      \n\
                        uniform float radius;                                                                                   \n\
                        uniform vec4 color;                                                                                     \n\
                        uniform vec3 current_position;                                                                          \n\
                                                                                                                                \n\
                        varying vec3 vertpos;                                                                                   \n\
                                                                                                                                \n\
                        void main(void)                                                                                         \n\
                        {                                                                                                       \n\
                            vec3 dif =  vertpos - current_position;                                                             \n\
                            float dist = dif.x*dif.x + dif.y*dif.y + dif.z*dif.z;                                               \n\
                                                                                                                                \n\
                            float intensity=1/dist*radius*0.01;                                                                 \n\
                            if(intensity>0.9)                                                                                   \n\
                                intensity=0.9;	                                                                                \n\
                            if(intensity<0)                                                                                     \n\
                                intensity=0;	                                                                                \n\
                                                                                                                                \n\
                            vec4 diffuse = texture2D(ColorMap, vec2(gl_TexCoord[0]));                                           \n\
                            vec4 gradient = vec4(texture2D(GradientTexture,vec2(intensity,1)));                                 \n\
                                                                                                                                \n\
                            vec4 final_color = gradient*diffuse*intensity*color;                                                \n\
                                                                                                                                \n\
                            gl_FragColor=final_color;                                                                           \n\
                        }                                                                                                       \n\
                        ";

        ShaderType = 1;
        //Create the shader material
        shaderMaterial = driver->getGPUProgrammingServices()->addHighLevelShaderMaterial(
                vertShader, "main", video::EVST_VS_1_1,
                fragShader, "main", video::EPST_PS_1_1,
                this, video::EMT_ONETEXTURE_BLEND);
    }else{
        if(driver->queryFeature(video::EVDF_ARB_VERTEX_PROGRAM_1) && driver->queryFeature(video::EVDF_ARB_FRAGMENT_PROGRAM_1)){
            //ARB Shader for compatibility from Viz_fuerte
            const c8 *vertShader = "!!ARBvp1.0\n\
                                    \n\
                                    PARAM c[5] = { program.local[0],\n\
                                              state.matrix.mvp };\n\
                                    \n\
                                    MOV result.texcoord[0], vertex.texcoord[0];\n\
                                    MOV result.texcoord[1].xyz, vertex.position;\n\
                                    DP4 result.position.w, vertex.position, c[4];\n\
                                    DP4 result.position.z, vertex.position, c[3];\n\
                                    DP4 result.position.y, vertex.position, c[2];\n\
                                    DP4 result.position.x, vertex.position, c[1];\n\
                                    END\n\
                                    ";

            const c8 *fragShader = "!!ARBfp1.0\n\
                                    OPTION ARB_precision_hint_nicest;\n\
                                    \n\
                                    # texture  ColorMap -> index 0\n\
                                    # texture  GradientTexture -> index 1\n\
                                    \n\
                                    # float    radius -> 0\n\
                                    # float[4] color -> 1\n\
                                    # float[3] current_position -> 2\n\
                                    \n\
                                    \n\
                                    PARAM c[4] = { program.local[0..2],\n\
                                            { 0.0099999998, 0.89999998, 0, 1 } };\n\
                                    TEMP R0;\n\
                                    TEMP R1;\n\
                                    TEMP R2;\n\
                                    ADD R0.xyz, fragment.texcoord[1], -c[2];\n\
                                    MUL R0.y, R0, R0;\n\
                                    MAD R0.x, R0, R0, R0.y;\n\
                                    MAD R0.x, R0.z, R0.z, R0;\n\
                                    RSQ R0.x, R0.x;\n\
                                    RCP R0.x, R0.x;\n\
                                    MUL R0.x, R0, R0;\n\
                                    RCP R0.x, R0.x;\n\
                                    MUL R0.x, R0, c[0];\n\
                                    MUL R0.x, R0, c[3];\n\
                                    ADD R0.y, R0.x, -c[3];\n\
                                    CMP R0.x, -R0.y, c[3].y, R0;\n\
                                    CMP R2.x, R0, c[3].z, R0;\n\
                                    MOV R2.y, c[3].w;\n\
                                    TEX R1, fragment.texcoord[0], texture[0], 2D;\n\
                                    TEX R0, R2, texture[1], 2D;\n\
                                    MUL R0, R0, R1;\n\
                                    MUL R0, R0, R2.x;\n\
                                    MUL result.color, R0, c[1];\n\
                                    END\n\
                                    ";
            ShaderType=0;
            shaderMaterial = driver->getGPUProgrammingServices()->addShaderMaterial(
                    vertShader,fragShader,this, video::EMT_ONETEXTURE_BLEND);
        }
    }
}

CShieldManager::~CShieldManager(){
    //Erase everything
    for(u32 i=0; i<array_Shields.size();i++){
        while(!array_Shields[i].array_Impacts.empty()){
            array_Shields[i].array_Impacts.erase(0);
        }
    }
    while(!array_Shields.empty()){
        array_Shields.erase(0);
    }
}

bool CShieldManager::addShield(scene::ISceneNode *node){
    //Check if shield already exists
    for(u32 i=0; i<array_Shields.size();i++){
	    if(array_Shields[i].shieldNode == node){
	        //Shield already exists
	        return false;
	    }
    }
    //Create a new shield
    SShield tmpShield;
    tmpShield.shieldNode = node;
    array_Shields.push_back(tmpShield);
    return true;
}

bool CShieldManager::addLocalImpact(const scene::ISceneNode *node, const core::vector3df position,const f32 radius, const u32 duration, const video::SColorf color)
{
    //Find the shield
    for(u32 i=0; i<array_Shields.size();i++){
	    if(array_Shields[i].shieldNode == node){
	        //Create a new impact
            SImpact tmpImpact;
            tmpImpact.pos = position;
            tmpImpact.startTime = timer->getTime();
            tmpImpact.endTime = timer->getTime()+duration;
            tmpImpact.radius = radius;
            tmpImpact.color = color;

            array_Shields[i].array_Impacts.push_back(tmpImpact);
            return true;
	    }
    }
    //Shield was not found
    return false;
}

bool CShieldManager::addGlobalImpact(const scene::ISceneNode *node, core::vector3df &position,const f32 radius, const u32 duration, const video::SColorf color)
{
    //Find the shield
    for(u32 i=0; i<array_Shields.size();i++){
	    if(array_Shields[i].shieldNode == node){
	        //Transform the position to local space
            core::matrix4 trans = node->getAbsoluteTransformation();
            trans.makeInverse();
            trans.transformVect(position);

	        //Create a new impact
            SImpact tmpImpact;
            tmpImpact.pos = position;
            tmpImpact.startTime = timer->getTime();
            tmpImpact.endTime = timer->getTime()+duration;
            tmpImpact.radius = radius;
            tmpImpact.color = color;

            array_Shields[i].array_Impacts.push_back(tmpImpact);
            return true;
	    }
    }
    //Shield was not found
    return false;
}

core::vector3df CShieldManager::addImpact(const scene::ISceneNode *node, const core::line3df ray, const f32 radius, const u32 duration, const video::SColorf color){
    //Check if node pointer is given, otherwise the collision point routine will crash
    if(!node)
        return ray.end;

    //Check for intersection
    core::vector3df colPoint;
    core::triangle3df colTris;
    const scene::ISceneNode* outNode;
    if(smgr->getSceneCollisionManager()->getCollisionPoint(ray,
            node->getTriangleSelector(), colPoint,
            colTris, outNode))
    {
        //Intersection found
        //Transform position to local space
        core::vector3df tmpPoint = core::vector3df(colPoint);
        core::matrix4 trans = node->getAbsoluteTransformation();
        trans.makeInverse();
        trans.transformVect(colPoint);
        //Add impact
        addLocalImpact(node,colPoint,radius,duration,color);
        return tmpPoint;
    }
    //No intersection
    return ray.end;
}

void CShieldManager::drawAll(){

	for(u32 i=0; i<array_Shields.size();i++)
	{
	    array_Shields[i].shieldNode->updateAbsolutePosition();

        //Check for dead impacts and delete them
        for(u32 j=0; j<array_Shields[i].array_Impacts.size();){
            if(timer->getTime()>array_Shields[i].array_Impacts[j].endTime){ //If impact is dead
                array_Shields[i].array_Impacts.erase(j);
            }else{
                ++j;
            }
	    }
        //I learned this meshbuffer trick from Viz_Fuerte's "Simple but useful projects"
        //Draw shield only when node is visible
        if(!smgr->isCulled(array_Shields[i].shieldNode))
        {
            //Reset the transformation
            driver->setTransform(video::ETS_WORLD,array_Shields[i].shieldNode->getAbsoluteTransformation());

            //Draw all impacts
            for(u32 j=0; j<array_Shields[i].array_Impacts.size();j++){
                tmp_position = array_Shields[i].array_Impacts[j].pos;

                //Calculate the impact's radius dependant on time
                u32 dif1 = array_Shields[i].array_Impacts[j].endTime-array_Shields[i].array_Impacts[j].startTime;
                u32 dif2 = timer->getTime()-array_Shields[i].array_Impacts[j].startTime;
                //Range from start (0.0) to end (1.0)
                f32 dif = (dif2/(dif1*1.0));


                ///Different effects for the shield:
                ///You can calculate the impacts radius with a function that uses "dif" as an argument. dif is betweeen 0.0 (life start) and 1.0 (life end)

                ///\n These are two preset effects:
                //1. Absorbing effect:
                /*tmp_radius = array_Shields[i].array_Impacts[j].radius*(0.25 - pow((dif-0.5),2));
                tmp_color = array_Shields[i].array_Impacts[j].color;*/

                //2. Fade-out effect:
                /*tmp_radius = array_Shields[i].array_Impacts[j].radius*dif;
                tmp_color = array_Shields[i].array_Impacts[j].color;
                tmp_color.a = 1-dif;*/

                //Using effect 2
                tmp_radius = array_Shields[i].array_Impacts[j].radius*dif;
                tmp_color = array_Shields[i].array_Impacts[j].color;
                tmp_color.a = 1-dif;

                //Draw the impact on all materials of the shield node
				for(u32 k=0; k<array_Shields[i].shieldNode->getMaterialCount(); ++k)
				{
					video::SMaterial material = array_Shields[i].shieldNode->getMaterial(k);
					material.MaterialType = (video::E_MATERIAL_TYPE)shaderMaterial;
					material.MaterialTypeParam = pack_texureBlendFunc(video::EBF_SRC_ALPHA,video::EBF_ONE);
					//Prevent transparency issues
					material.setFlag(video::EMF_ZWRITE_ENABLE,false);

					driver->setMaterial(material);
					switch(array_Shields[i].shieldNode->getType()){
                    case scene::ESNT_ANIMATED_MESH:
                        driver->drawMeshBuffer(((scene::IAnimatedMeshSceneNode*)array_Shields[i].shieldNode)->getMesh()->getMeshBuffer(k));
                        break;
                    default:
                        driver->drawMeshBuffer(((scene::IMeshSceneNode*)array_Shields[i].shieldNode)->getMesh()->getMeshBuffer(k));
                        break;
					};
				}
			}
        }
	}
}

//This is called for each impact render
void CShieldManager::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
    switch(ShaderType){
    case 0:
        // Color
        services->setPixelShaderConstant((float*)(&tmp_color),1, 4);

        // Radius
        services->setPixelShaderConstant((float*)(&tmp_radius),0, 1);

        // Current position
        services->setPixelShaderConstant((float*)(&tmp_position),2, 3);

        break;
    case 1:
        // Color
        services->setPixelShaderConstant("color", (float*)(&tmp_color ), 4);

        // Radius
        services->setPixelShaderConstant("radius", (float*)(&tmp_radius ), 1);

        // Current position
        services->setPixelShaderConstant("current_position", (float*)(&tmp_position ), 3);

        //Set the texture layers
        int var0=0;
        services->setPixelShaderConstant("ColorMap", (float*)(&var0), 1);
        int var1=1;
        services->setPixelShaderConstant("GradientTexture", (float*)(&var1), 1);
        break;
    };
}

