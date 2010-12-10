#include "Screen.h"
#include "utils.h"

using namespace irr::core;
using namespace irr::video;

Screen::Screen( IrrlichtDevice *_device, double *_musicTime, Player* player1, Player* player2 )
{
	this->device = _device;
	this->driver = device->getVideoDriver();
	this->smgr = device->getSceneManager();
	player.push_back(player1);
	player.push_back(player2);
	musicTime = _musicTime;
	
	effectFactory = new EffectFactory(device,driver,smgr,player);
	
	initializeScreenElements();
	
	//abox = smgr->addCubeSceneNode(5);
	//abox->setPosition(core::vector3df(0,0,-100));
	//abox->addAnimator(smgr->createRotationAnimator(core::vector3df(0.3f, 0.3f,0)));
	
	
	//effectFactory->queueEffect( 300, CREATE_DRUNK_EFFECT, 1 );
	//effectFactory->queueEffect( 0, CREATE_FLOOD_EFFECT, 1);
	//effectFactory->queueEffect( 0, CREATE_WATER_BEAM, 1);
	//effectFactory->queueEffect( 0, CREATE_FLOOD_EFFECT, 1);
	//effectFactory->queueEffect(0, CREATE_BOLT, 1);
	//effectFactory->queueEffect(0, CREATE_THUNDERSTORM, 0);
	//effectFactory->queueEffect(0, CREATE_ELETRIC_GROUND, 0);
	//effectFactory->queueEffect(0, CREATE_FIREBALL, 1);
	//effectFactory->queueEffect( 0, CREATE_FIRE_RAIN, 1 );
	//effectFactory->queueEffect(0, CREATE_BOLT, 1);
	//effectFactory->queueEffect(1000, CREATE_BOLT, 0);
	//effectFactory->queueEffect( 800, CREATE_EXPLOSION, 0 );
	//effectFactory->queueEffect( 0, CREATE_FEEDBACK, 1 );
	//effectFactory->queueEffect( 2000, CREATE_DRUNK_EFFECT_SINGLE, 0 );
	//effectFactory->queueEffect( 0, CREATE_EXPLOSION, 1 );
	//effectFactory->queueEffect( 0, CREATE_GLOW_AREA, 0 );
	//effectFactory->queueEffect( 0, CREATE_ELECTRIC, 1 );
	//for(int i=0;i<10;i++)
		//effectFactory->queueEffect( 1000 + i*100, CREATE_FIREBALL, 0 );
}

Screen::~Screen()
{

}

void Screen::initializeScreenElements()
{
	// Pre-load textures
	ITexture *goodTex = driver->getTexture("img/good.png");
	ITexture *badTex = driver->getTexture("img/bad.png");
	ITexture *neutralTex = driver->getTexture("img/neutral.png");
	this->glowTex = driver->getTexture("img/glow2.bmp");
	
	sky = smgr->addSkyDomeSceneNode( driver->getTexture("img/stars.tga"), 32, 32 );
	sky->setMaterialFlag(video::EMF_TEXTURE_WRAP, video::ETC_REPEAT);
	sky->getMaterial(0).getTextureMatrix(0).setTextureScale(6,6);
	
	fpsText = device->getGUIEnvironment()->addStaticText(L"", core::recti(0, 0, 100, 10));
	timeText = device->getGUIEnvironment()->addStaticText(L"", core::recti( position2di(SCREENX/2-50,5), dimension2di(100,50) ) , false);
	timeText->setTextAlignment( EGUIA_CENTER,EGUIA_CENTER );
	timeText->setOverrideColor( SColor(255,255,255,255) );
	
	// create render target
	for(int i=0;i<NPLAYERS;i++) {
		{
			// GUI Elements
		
			int xpos = SCREENX/4 + i*SCREENX/2;
			
			hpTxt[i] = device->getGUIEnvironment()->addStaticText(L"", recti(position2di(xpos-35,40),position2di(xpos+35,60)));
			hpTxt[i]->setOverrideColor( SColor(200,255,255,255) );
			
			//bloodSplit[i] = driver->addRenderTargetTexture(core::dimension2d<u32>(256,256), "RTT1");
			//device->getGUIEnvironment()->addImage( bloodSplit[i], core::position2d<s32>(xpos-256/2, 70-256/2) );

			// smileys
			good[i] = device->getGUIEnvironment()->addImage( goodTex, core::position2d<s32>(xpos,500), true );
			bad[i] = device->getGUIEnvironment()->addImage( badTex, core::position2d<s32>(xpos,500), true );
			neutral[i] = device->getGUIEnvironment()->addImage( neutralTex, core::position2d<s32>(xpos,500), true );
			
			// health bars
			healthBar[i] = new VxHealthSceneNode(
								smgr->getRootSceneNode(), // parent node
								smgr, // scene manager
								-1, // id
								i==0? 1:-1, false,
								300, // width
								20, // height
								vector3df(xpos, 20, 0), // position in 2d
								SColor(0,0,0,0), // bar color
								SColor(255,220,0,0), // background color
								SColor(200,255,255,255) ); // border color
								
			#define STAMINA_BAR_H 60
			for(int k=0; k<NSTAMINALEVELS; k++)
				staminaBar[i][k] = new VxHealthSceneNode(
									smgr->getRootSceneNode(), // parent node
									smgr, // scene manager
									-1, // id
									1, true,
									10, // width
									STAMINA_BAR_H, // height
									vector3df(i==0 ? 20 : SCREENX-20, SCREENY/3 + (k==0?(STAMINA_BAR_H/2):-(STAMINA_BAR_H/2)), 0), // position in 2d
									SColor(0,0,0,0), // bar color
									SColor(170,30,30,240), // background color
									SColor(150,255,255,255) ); // border color								
			
		}
		
		// glowing when you hit correclty a fret
		for(int k=0;k<NFRETS;k++) {
			glow[i][k] = smgr->addBillboardSceneNode(smgr->getRootSceneNode(), dimension2d<float>(10, 10));
			glow[i][k]->setMaterialFlag(EMF_LIGHTING, false);
			glow[i][k]->setMaterialType(EMT_TRANSPARENT_ADD_COLOR); 
			glow[i][k]->setPosition( vector3df(player[i]->track->getStoneXPos(k),-TRACK_SIZE_Y,player[i]->track->posz) );
			glow[i][k]->setVisible(false);
			((IBillboardSceneNode*)glow[i][k])->setColor(fretColors[k]);
			glow[i][k]->setMaterialTexture(0, glowTex);
			/*glow[i][k] = smgr->addVolumeLightSceneNode(smgr->getRootSceneNode(), -1,
							12,                              // Subdivisions on U axis
							12,                              // Subdivisions on V axis
							fretColors[k], 			// foot color
							SColor(0, 0, 0, 0),      		// tail color
							vector3df(player[i]->track->getStoneXPos(k),-TRACK_SIZE_Y,player[i]->track->posz),
							vector3df(-90,0,0),
							vector3df(10,10,10));
			glow[i][k]->setMaterialTexture(0, glowTex);	
			glow[i][k]->setVisible(false);*/
		}							
	}
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
		
		for(int k=0; k<NFRETS; k++) {
			
			int zdisplace = 0;
			
			if(player[i]->usingSkill) {
				glow[i][k]->setVisible(player[i]->fretting->_hitting[k]==2);				
				
				color = fretColors[k];
				color.setAlpha(255);
			}
			else
			{	
				glow[i][k]->setVisible(player[i]->fretting->_hitting[k]==1);
				
				// what's the state of the fret?
				switch( player[i]->fretting->_hitting[k] )
				{
					case 0:
						// idle
						color = fretColors[k];
						color.setAlpha(255);
						break;				
					case 1:
						// hit
						color = fretColors[k];
						color.setAlpha(255);
						zdisplace=3;
						break;
					default:
						// mising
						color.setAlpha(128);
						color = SColor(255,0,0,0);
						break;
					
				}
			}
				
			// draw the colored bar for this fret
			driver->draw3DLine( vector3df(player[i]->track->getStoneXPos(k) -1, -TRACK_SIZE_Y, player[i]->track->posz - zdisplace),
								vector3df(player[i]->track->getStoneXPos(k) +1, -TRACK_SIZE_Y, player[i]->track->posz - zdisplace),
								color );			
		}
	}
}	

void Screen::update()
// Here we check everything about the game that has to be drawn.
// It's all about HUDs and special effects!
{
	drawHUD();
	drawKeys();
	drawHittingState();
	drawSoloModeState();
	
	sky->getMaterial(0).getTextureMatrix(0).setTextureTranslate( 0, -*musicTime/3 );
	
	effectFactory->handleEffectsQueue();
	effectFactory->shieldmanager->drawAll();
}

void Screen::drawSoloModeState()
{
	for(int i=0; i<NPLAYERS; i++)
		if( player[i]->usingSkill )
			effectFactory->createAreaBorderEffect(i,glowTex,10);		
}

void Screen::drawHUD()
{
	char str[30];
	sprintf(str,"%.1lf",musicTotalTime-*musicTime);
	timeText->setText( stringw(str).c_str() );
	
	for(int i=0; i<NPLAYERS; i++) {
		healthBar[i]->setProgress( player[i]->HP*100/player[i]->maxHP );
		
		staminaBar[i][1]->setProgress( ((player[i]->stamina)-(player[i]->maxStamina/2))*100 / (player[i]->maxStamina/2) );
		
		if(player[i]->stamina > player[i]->maxStamina/2)
			staminaBar[i][0]->setProgress( 100 );
		else
			staminaBar[i][0]->setProgress( player[i]->stamina*100 / (player[i]->maxStamina/2) );
		
		sprintf(str,"%i/%i",player[i]->HP,player[i]->maxHP);
		hpTxt[i]->setText( stringw(str).c_str() );
	}
}

void Screen::drawHittingState()
{
	for(int i=0; i<NPLAYERS; i++) 
	{
		switch( player[i]->fretting->frettingState )
		{
			case 1:
				showGood(i);
				break;
			case -1:
				showBad(i);
				break;
			case 0:
				showNeutral(i);
				break;
		}
		
		if( player[i]->damageTaken ) {
			
			E_GORE_LEVEL gore;
			if(player[i]->damageTaken>100)
				gore = EGL_INSANE;
			else if(player[i]->damageTaken>80)
				gore = EGL_BRUTAL;
			else if(player[i]->damageTaken>20)
				gore = EGL_MEDIUM;
			else
				gore = EGL_MILD;
			
			effectFactory->splitBlood(i, gore);
			player[i]->damageTaken = 0;
		}
	}
}

void Screen::showGood( int i )
{	
	good[i]->setVisible(true);
	bad[i]->setVisible(false);
	neutral[i]->setVisible(false);
}

void Screen::showBad( int i )
{	
	
	bad[i]->setVisible(true);	
	good[i]->setVisible(false);
	neutral[i]->setVisible(false);
}

void Screen::showNeutral( int i )
{
	bad[i]->setVisible(false);	
	good[i]->setVisible(false);
	neutral[i]->setVisible(true);
}	

void Screen::setFps( int fps )
{
	core::stringw tmp(L"fps: ");
	tmp += fps;
	fpsText->setText(tmp.c_str());
}
