#include "Screen.h"
#include "utils.h"

#include "RibbonTrailSceneNode.h"

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
	
	/*scene::ISceneNode* ball = smgr->addSphereSceneNode(5);
	ball->addAnimator( smgr->FlyCircleAnimator( vector3df(0,0,0), 20) );
	
	ITexture* beamTex = driver->getTexture( "img/beam.png" );

	RibbonTrailSceneNode* rt = new RibbonTrailSceneNode( device, ball, -1 );
	rt->setPosition( core::vector3df( 0, 0, 0 ) );
	rt->setMaterialTexture( 0, beamTex );
	rt->setMaxDistance( 0 );
	rt->setMaxQuads( 10000 );
	rt->setStartingAlpha( 255 );
	rt->setEnabled( true );
	rt->getMaterial(0).MaterialType=video::EMT_TRANSPARENT_ADD_COLOR;*/
	
	
	effectFactory = new EffectFactory(device,driver,smgr,player);
	
	initializeScreenElements();
	
	//effectFactory->effectIce(1,10000);
	
	//effectFactory->effectVampireAttack(1,2000);
					
	//effectFactory->effectBlackHole(1,10000);
	//effectFactory->queueEffect( 1000, EFFECT_SUN, 1 );
	//effectFactory->queueEffect( 0, _BALL_LIGHTNING, 0 );
	//effectFactory->queueEffect( 0, _SWAMP_EFFECT, 1 );
	//effectFactory->queueEffect( 300, _DRUNK_EFFECT, 1 );
	//effectFactory->queueEffect( 0, _FLOOD_EFFECT, 1);
	//effectFactory->queueEffect( 0, _WATER_BEAM, 1);
	//effectFactory->queueEffect( 0, _FLOOD_EFFECT, 1);
	//effectFactory->queueEffect(0, _BOLT, 1);
	//effectFactory->queueEffect(0, _THUNDERSTORM, 0);
	//effectFactory->queueEffect(0, _ELETRIC_GROUND, 0);
	//effectFactory->queueEffect(0, EFFECT_FIREBALL, 1);
	//effectFactory->queueEffect( 0, _FIRE_RAIN, 1 );
	//effectFactory->queueEffect(0, _BOLT, 1);
	//effectFactory->queueEffect(1000, _BOLT, 0);
	//effectFactory->queueEffect( 800, _EXPLOSION, 0 );
	//effectFactory->queueEffect( 0, _FEEDBACK, 1 );
	//effectFactory->queueEffect( 2000, _DRUNK_EFFECT_SINGLE, 0 );
	//effectFactory->queueEffect( 0, _EXPLOSION, 1 );
	//effectFactory->queueEffect( 0, _GLOW_AREA, 0 );
	//effectFactory->queueEffect( 0, _ELECTRIC, 1 );
	//for(int i=0;i<10;i++)
		//effectFactory->queueEffect( 1000 + i*100, EFFECT_FIREBALL, 0 );
}

Screen::~Screen()
{

}

void Screen::initializeScreenElements()
{
	// Pre-load textures
	this->glowTex = driver->getTexture("img/glow2.bmp");
	this->fireballTex = driver->getTexture("img/fireball.bmp");
	
	// background
	//sky = smgr->addSkyDomeSceneNode( driver->getTexture("img/stars.tga"), 32, 32 );
	//sky->setRotation( vector3df(90,0,0) );
	//sky->getMaterial(0).getTextureMatrix(0).setTextureScale(6,6);
	
	fpsText = device->getGUIEnvironment()->addStaticText(L"", core::recti(0, 0, 100, 20));
	fpsText->setOverrideColor( SColor(255,255,255,255) );
	
	
	#define HUD_BARS_Y (SCREENY-80)
	
	timeText = device->getGUIEnvironment()->addStaticText(L"", core::recti( position2di(SCREENX/2-50, HUD_BARS_Y-10), dimension2di(100,50) ) , false);
	timeText->setTextAlignment( EGUIA_CENTER,EGUIA_CENTER );
	timeText->setOverrideColor( SColor(255,255,255,255) );
	
	//  render target
	for(int i=0;i<NPLAYERS;i++) {
		{
			// GUI Elements
		
			int xpos = SCREENX/4 + i*SCREENX/2;
			
			hpText[i] = device->getGUIEnvironment()->addStaticText(L"", recti(position2di(xpos-35,HUD_BARS_Y+10),position2di(xpos+35,HUD_BARS_Y+25)));
			hpText[i]->setOverrideColor( SColor(200,255,255,255) );
			
			// health bars
			armorBar[i] = new VxHealthSceneNode(
								smgr->getRootSceneNode(), // parent node
								smgr, // scene manager
								-1, // id
								i==0? 1:-1, false,
								400, // width
								30, // height
								vector3df(xpos, HUD_BARS_Y, 0), // position in 2d
								SColor(0,0,0,0), // bar color
								SColor(150,0,220,0), // background color
								SColor(0,255,255,255) ); // border color				
			healthBar[i] = new VxHealthSceneNode(
								smgr->getRootSceneNode(), // parent node
								smgr, // scene manager
								-1, // id
								i==0? 1:-1, false,
								400, // width
								30, // height
								vector3df(xpos, HUD_BARS_Y, 0), // position in 2d
								SColor(0,0,0,0), // bar color
								SColor(200,220,0,0), // background color
								SColor(100,255,255,255) ); // border color							
								
			#define STAMINA_BAR_H 60
			#define STAMINA_DISPLACEX 25
			for(int k=0; k<NSTAMINALEVELS; k++)
				staminaBar[i][k] = new VxHealthSceneNode(
									smgr->getRootSceneNode(), // parent node
									smgr, // scene manager
									-1, // id
									1, true,
									15, // width
									STAMINA_BAR_H, // height
									vector3df(i==0 ? STAMINA_DISPLACEX : SCREENX-STAMINA_DISPLACEX,
											  SCREENY*1.5/3 + (k==0?(STAMINA_BAR_H/2):-(STAMINA_BAR_H/2)),
											  0), // position in 2d
									SColor(0,0,0,0), // bar color
									SColor(170,30,30,240), // background color
									SColor(150,200,200,255) ); // border color								
			
		}
		
		// glowing when you correclty hit the fret note
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
			
			if(player[i]->isUsingSkill) {
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
					case 2:
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
					case -1:
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
	drawBars();
	drawKeys();
	drawSoloModeState();
	drawSplitBlood();
	
	// animate background
	//sky->getMaterial(0).getTextureMatrix(0).setTextureTranslate( 0, -*musicTime/3 );
	
	effectFactory->handleEffectsQueue();
	effectFactory->shieldmanager->drawAll();
}

void Screen::drawStatus()
{
	for(int i=0; i<NPLAYERS; i++) 
	{
		if( player[i]->status==ST_FIRE )
			effectFactory->areaEffect(i, fireballTex, 50);
	}
}

void Screen::drawSoloModeState()
{
	for(int i=0; i<NPLAYERS; i++)
		if( player[i]->isUsingSkill )
			effectFactory->soloEffect(i,glowTex,10);		
}

void Screen::drawBars()
{
	char str[30];
	sprintf(str,"%.1lf",musicTotalTime-*musicTime);
	timeText->setText( stringw(str).c_str() );
	
	for(int i=0; i<NPLAYERS; i++) {
		healthBar[i]->setProgress( player[i]->HP*100/player[i]->maxHP );
		armorBar[i]->setProgress( (player[i]->HP+player[i]->getArmor())*100/player[i]->maxHP );
		
		
		staminaBar[i][1]->setProgress( ((player[i]->stamina)-(player[i]->maxStamina/2))*100 / (player[i]->maxStamina/2) );
		
		if(player[i]->stamina > player[i]->maxStamina/2)
			staminaBar[i][0]->setProgress( 100 );
		else
			staminaBar[i][0]->setProgress( player[i]->stamina*100 / (player[i]->maxStamina/2) );
		
		
		sprintf(str,"%i/%i",player[i]->HP,player[i]->maxHP);
		hpText[i]->setText( stringw(str).c_str() );
	}
}

void Screen::drawSplitBlood()
{
	for(int i=0; i<NPLAYERS; i++) 
	{
		if( player[i]->damageTaken ) {
			E_GORE_LEVEL gore;
			
			if(player[i]->damageTaken>=50)
				gore = EGL_INSANE;
			else if(player[i]->damageTaken>=30)
				gore = EGL_BRUTAL;
			else if(player[i]->damageTaken>=10)
				gore = EGL_MEDIUM;
			else
				gore = EGL_MILD;
			
			effectFactory->splitBlood(i, gore);
			player[i]->damageTaken = 0;
		}
	}
}

void Screen::setFps( int fps )
{
	core::stringw tmp(L"fps: ");
	tmp += fps;
	fpsText->setText(tmp.c_str());
}
