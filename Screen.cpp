#include "Screen.h"
#include "utils.h"

#include "CBoltSceneNode.h"

Screen::Screen( IrrlichtDevice *device, video::IVideoDriver *driver )
{
	fpsText = device->getGUIEnvironment()->addStaticText(L"", core::rect<int>(0, 0, 100, 10));
	good = device->getGUIEnvironment()->addImage( driver->getTexture("img/good.png"), core::position2d<s32>(200,500), true );
	bad = device->getGUIEnvironment()->addImage( driver->getTexture("img/bad.png"), core::position2d<s32>(200,500), true );
	neutral = device->getGUIEnvironment()->addImage( driver->getTexture("img/neutral.png"), core::position2d<s32>(200,500), true );
	good->setVisible(false);
	bad->setVisible(false);
	//device->getGUIEnvironment()->addButton(core::rect<int>(50,240,110,240 + 32), 0, 0, L"Increase speed", L"Increases speed");
	
	//irr::scene::CBoltSceneNode* beam = new irr::scene::CBoltSceneNode(device->getSceneManager()->getRootSceneNode(), device->getSceneManager(), -1,"img/laser3.bmp"); 
	//beam->setLine(irr::core::vector3df(-20,0,0), irr::core::vector3df(60,-50,5), 50, 10, 10,3, false,10.0f, irr::video::SColor(255,0,0,255)); 
}

Screen::~Screen()
{

}

void Screen::update( Fretting *fretting1, Fretting *fretting2 )
{
	/*if( time_diff(lastEventTime)>SHOW_DELAY ) {
		good->setVisible(false);
		good->setVisible(false);
	}*/
	
	switch( fretting1->frettingState )
	{
		case 1:
			showGood();
			break;
		case -1:
			showBad();
			break;
		case 0:
			showNeutral();
			break;
	}
}

void Screen::showGood()
{
	gettimeofday(&lastEventTime, NULL);
	
	good->setVisible(true);
	bad->setVisible(false);
	neutral->setVisible(false);
}

void Screen::showBad()
{
	gettimeofday(&lastEventTime, NULL);
	
	bad->setVisible(true);	
	good->setVisible(false);
	neutral->setVisible(false);
}

void Screen::showNeutral()
{
	gettimeofday(&lastEventTime, NULL);
	
	bad->setVisible(false);	
	good->setVisible(false);
	neutral->setVisible(true);
}	
