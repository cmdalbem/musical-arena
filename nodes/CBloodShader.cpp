#include "CBloodShader.h"

#include <iostream>
using namespace std;

using namespace irr;
using namespace video;
using namespace core;
using namespace scene;
using namespace io;

CBloodShader::CBloodShader() : driver(0), timer(0), matBlood(-1)
{
}

void CBloodShader::OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
{
  matrix4 mWorldViewProjection  = driver->getTransform(ETS_PROJECTION);
          mWorldViewProjection *= driver->getTransform(ETS_VIEW);
          mWorldViewProjection *= driver->getTransform(ETS_WORLD);
  services->setVertexShaderConstant("mWorldViewProj", mWorldViewProjection.pointer(), 16);
  
  int bloodAdress=0;
	services->setPixelShaderConstant("blood", (float*)&bloodAdress, 1); 

  f32 time = (f32)timer->getTime();
  services->setPixelShaderConstant("time", &time, 1);
}

const irr::s32 CBloodShader::createMaterial(irr::IrrlichtDevice *pDevice)
{
  if (!pDevice || matBlood != -1) return matBlood;

  driver = pDevice->getVideoDriver();
  timer  = pDevice->getTimer();

  IGPUProgrammingServices *gpu = driver->getGPUProgrammingServices();

  if (gpu)
  {
    matBlood = gpu->addHighLevelShaderMaterialFromFiles
    (
      "nodes/shaders/blood_vs.glsl", "main", EVST_VS_2_0,
      "nodes/shaders/blood_ps.glsl", "main",  EPST_PS_2_0,
      this, EMT_TRANSPARENT_ALPHA_CHANNEL
    );
  }

  return matBlood;
}
