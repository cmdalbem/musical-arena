#include "Screen.h"
#include "utils.h"

#include "CBoltSceneNode.h"

Screen::Screen( IrrlichtDevice *device, video::IVideoDriver *driver )
{
	fpsText = device->getGUIEnvironment()->addStaticText(L"", core::rect<int>(0, 0, 100, 10));
	good = device->getGUIEnvironment()->addImage( driver->getTexture("img/good.png"), core::position2d<s32>(150,20), true );
	bad = device->getGUIEnvironment()->addImage( driver->getTexture("img/bad.png"), core::position2d<s32>(150,20), true );
	good->setVisible(false);
	bad->setVisible(false);
	//device->getGUIEnvironment()->addButton(core::rect<int>(50,240,110,240 + 32), 0, 0, L"Increase speed", L"Increases speed");
	
	//irr::scene::CBoltSceneNode* beam = new irr::scene::CBoltSceneNode(device->getSceneManager()->getRootSceneNode(), device->getSceneManager(), -1);
	//beam->setLine(irr::core::vector3df(0,0,0), irr::core::vector3df(100,100,-10), 50, 10, 10, 3, false, irr::video::SColor(255,255,0,0));
	//beam->drop();
}

Screen::~Screen()
{

}

void Screen::update()
{
	if( time_diff(lastEventTime)>SHOW_DELAY ) {
		good->setVisible(false);
		good->setVisible(false);
	}		
}

void Screen::showGood()
{
	gettimeofday(&lastEventTime, NULL);
	
	good->setVisible(true);
	bad->setVisible(false);
}

void Screen::showBad()
{
	gettimeofday(&lastEventTime, NULL);
	
	bad->setVisible(true);	
	good->setVisible(false);
}
