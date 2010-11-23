#include "Screen.h"
#include "utils.h"

#include "CBoltSceneNode.h"
#include "CBloodEffect.h"

using irr::core::vector3df;
using irr::video::SColor;
using irr::video::SMaterial;

Screen::Screen( IrrlichtDevice *_device )
{
	this->device = _device;
	this->driver = device->getVideoDriver();
	
	fpsText = device->getGUIEnvironment()->addStaticText(L"", core::rect<int>(0, 0, 100, 10));
	
	good[0] = device->getGUIEnvironment()->addImage( driver->getTexture("img/good.png"), core::position2d<s32>(200,500), true );
	bad[0] = device->getGUIEnvironment()->addImage( driver->getTexture("img/bad.png"), core::position2d<s32>(200,500), true );
	neutral[0] = device->getGUIEnvironment()->addImage( driver->getTexture("img/neutral.png"), core::position2d<s32>(200,500), true );
	
	good[1] = device->getGUIEnvironment()->addImage( driver->getTexture("img/good.png"), core::position2d<s32>(600,500), true );
	bad[1] = device->getGUIEnvironment()->addImage( driver->getTexture("img/bad.png"), core::position2d<s32>(600,500), true );
	neutral[1] = device->getGUIEnvironment()->addImage( driver->getTexture("img/neutral.png"), core::position2d<s32>(600,500), true );
	
	
	// external scenenodes playground
	
	//new CBloodEffect(device->getSceneManager(), EGL_MEDIUM, vector3df(0, 0, 0), vector3df(0, 0.0f, -1.0f));
	
	//irr::scene::CBoltSceneNode* beam = new irr::scene::CBoltSceneNode(device->getSceneManager()->getRootSceneNode(), device->getSceneManager(), -1,"img/laser3.bmp"); 
	//beam->setLine(irr::core::vector3df(-20,0,0), irr::core::vector3df(60,-50,5), 50, 10, 10,3, false,10.0f, irr::video::SColor(255,0,0,255)); 
}

Screen::~Screen()
{

}

void Screen::drawKeys()
{

	for(int i=0; i<NPLAYERS; i++)  {
		SMaterial m;
		SColor color;
		
		m.Thickness = 5;
		m.Lighting = 0;
		m.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
		driver->setMaterial(m);
		driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4()); //global positioning
		for(int k=0; k<NUMBER_OF_FRETS; k++) {
			int zvar = 0;
			
			switch(players[i]->fretting->keyState[k])
			{
				case 0:
					color = fretColors[k];
					color.setAlpha(128);
					break;
				case 1: 
					color = fretColors[k];
					color.setAlpha(255);
					zvar=3;
					break;
				case -1:
					color = SColor(255,0,0,0);
					color.setAlpha(255);
					break;
			}
			
			driver->draw3DLine( vector3df(players[i]->track->getStoneXPos(k) -1, -players[i]->track->sizey, players[i]->track->posz - zvar),
								vector3df(players[i]->track->getStoneXPos(k) +1, -players[i]->track->sizey, players[i]->track->posz - zvar),
								color );			
		}
	}
}	

void Screen::update()
{
	for(int i=0; i<NPLAYERS; i++) 
	{
		switch( players[i]->fretting->frettingState )
		{
			case 1: showGood(i); break;
			case -1: showBad(i); break;
			case 0: showNeutral(i); break;
		}
	}
	
	drawKeys();
}

void Screen::showGood( int i )
{
	gettimeofday(&lastEventTime, NULL);
	
	good[i]->setVisible(true);
	bad[i]->setVisible(false);
	neutral[i]->setVisible(false);
}

void Screen::showBad( int i )
{
	gettimeofday(&lastEventTime, NULL);
	
	bad[i]->setVisible(true);	
	good[i]->setVisible(false);
	neutral[i]->setVisible(false);
}

void Screen::showNeutral( int i )
{
	gettimeofday(&lastEventTime, NULL);
	
	bad[i]->setVisible(false);	
	good[i]->setVisible(false);
	neutral[i]->setVisible(true);
}	
