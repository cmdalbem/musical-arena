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
	
	//sceneLight = smgr->addLightSceneNode(0, vector3df(0,-80,-30), video::SColorf(1.0f, 1.0f, 1.0f), 30);
	sceneLight = smgr->addLightSceneNode();
	sceneLight->getLightData().SpecularColor = SColorf(0,0,0);
	sceneLight->setLightType(ELT_DIRECTIONAL);
	
	effectFactory = new EffectFactory(device,driver,smgr,player,sceneLight);
	
	initializeScreenElements();
	
	//effectFactory->queueEffect( 4000, EFFECT_LOTUS, 1 );
	
	//effectFactory->effectAnotherDimension(1,5000);
	//effectFactory->queueEffect( 2000, EFFECT_FREEZE, 1 );
	//effectFactory->effectVampireAttack(1,5000);
	//effectFactory->queueEffect( 2000, EFFECT_EXPLOSION, 1 );
	//effectFactory->queueEffect( 1000, EFFECT_BLACKHOLE, 1 );
	//effectFactory->queueEffect( 1000, EFFECT_SUN, 1 );
}

Screen::~Screen()
{

}

void Screen::initializeScreenElements()
{
	loadTextures();
	
	// background
	//sky = smgr->addSkyDomeSceneNode( driver->getTexture("img/stars.tga"), 32, 32 );
	//sky->setRotation( vector3df(90,0,0) );
	//sky->getMaterial(0).getTextureMatrix(0).setTextureScale(6,6);
	
	fpsText = device->getGUIEnvironment()->addStaticText(L"", core::recti(0, 0, 100, 20));
	fpsText->setOverrideColor( SColor(255,255,255,255) );
	
	//#define HUD_BARS_Y (SCREENY-200)
	#define HUD_BARS_Y (SCREENY-80)

	timeText = device->getGUIEnvironment()->addStaticText(L"", core::recti( position2di(SCREENX/2-50, HUD_BARS_Y-15), dimension2di(100,50) ) , false);
	timeText->setTextAlignment( EGUIA_CENTER,EGUIA_CENTER );
	timeText->setOverrideColor( SColor(255,255,255,255) );
	timeText->setOverrideFont( device->getGUIEnvironment()->getFont("img/fontbookman.png") );
	
	for(int i=0;i<NPLAYERS;i++) {
		{
			// GUI Elements
			int xpos = SCREENX/4 + i*SCREENX/2;
			
			#define MULTSIZE 117
			mult[i] = device->getGUIEnvironment()->addImage( recti( position2di(xpos+(i==0?150:-150)-MULTSIZE/2, 100-MULTSIZE/2),dimension2di(MULTSIZE,MULTSIZE)) );
			mult[i]->setUseAlphaChannel(true);
			
			hpText[i] = device->getGUIEnvironment()->addStaticText(L"", recti(position2di(xpos-35,HUD_BARS_Y+10),position2di(xpos+35,HUD_BARS_Y+25)));
			hpText[i]->setOverrideColor( SColor(200,255,255,255) );
			
			for(int k=0; k<5; k++){
				#define ICONSIZE 60
				statusIcon[i][k] = device->getGUIEnvironment()->addImage( recti( position2di(xpos + (i==0?1:-1)*(175 - 60*k) - ICONSIZE/2, HUD_BARS_Y+7.5-ICONSIZE/2),dimension2di(ICONSIZE,ICONSIZE)) );
				statusIcon[i][k]->setUseAlphaChannel(true);
				statusIcon[i][k]->setScaleImage(true);
			}
			
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
								
			#define STAMINA_BAR_H 400
			staminaBar[i] = new VxHealthSceneNode(
								smgr->getRootSceneNode(), // parent node
								smgr, // scene manager
								-1, // id
								1, false,
								STAMINA_BAR_H, // width
								10, // height
								vector3df(xpos, HUD_BARS_Y-15, 0), // position in 2d
								SColor(0,0,0,0), // bar color
								SColor(170,50,50,240), // background color
								SColor(150,200,200,255) ); // border color
			
		}
		
		// glowing when you correclty hit the fret note
		for(int k=0;k<NFRETS;k++) {
			glow[i][k] = smgr->addBillboardSceneNode(smgr->getRootSceneNode(), dimension2d<float>(10, 10));
			glow[i][k]->setMaterialFlag(EMF_LIGHTING, false);
			glow[i][k]->setMaterialType(EMT_TRANSPARENT_ADD_COLOR); 
			glow[i][k]->setPosition( vector3df(player[i]->track->getStoneXPos(k),-TRACK_SIZE_Y,TRACK_POS_Z) );
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
	
	screenFader = device->getGUIEnvironment()->addInOutFader();
	//screenFader->fadeOut(1);
	
	koImage = device->getGUIEnvironment()->addImage(koTex, position2di(0,100), true);
	koImage->setVisible(false);
}

void Screen::loadTextures()
{
	// Pre-load textures
	koTex = driver->getTexture("img/ko.png");
	glowTex = driver->getTexture("img/glow2.bmp");
	mult2xTex = driver->getTexture("img/2x.png");
	mult3xTex = driver->getTexture("img/3x.png");
	mult4xTex = driver->getTexture("img/4x.png");
	
	fireTex = driver->getTexture("img/fireicon.png");
	poisonTex = driver->getTexture("img/poisonicon.png");
	magicBarrierTex = driver->getTexture("img/darkshieldicon.png");
	eletrifiedTex = driver->getTexture("img/thundericon.png");
	drownedTex = driver->getTexture("img/watericon.png");
	frozenTex = driver->getTexture("img/iceicon.png");
	mirrorTex = driver->getTexture("img/mirroricon.png");
	blessedTex = driver->getTexture("img/goldcross.png");
	cursedTex = driver->getTexture("img/darkcross.png");
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
				glow[i][k]->setVisible(player[i]->fretting._hitting[k]==2);				
				
				color = fretColors[k];
				color.setAlpha(255);
			}
			else
			{	
				glow[i][k]->setVisible(player[i]->fretting._hitting[k]==1);
				
				// what's the state of the fret?
				switch( player[i]->fretting._hitting[k] )
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
					case -1:
					case 2:
						// mising
						color.setAlpha(128);
						color = SColor(255,0,0,0);
						break;
					
				}
			}
				
			// draw the colored bar for this fret
			driver->draw3DLine( vector3df(player[i]->track->getStoneXPos(k) -1, -TRACK_SIZE_Y, TRACK_POS_Z - zdisplace),
								vector3df(player[i]->track->getStoneXPos(k) +1, -TRACK_SIZE_Y, TRACK_POS_Z - zdisplace),
								color );			
		}
	}
}	

void Screen::update() 
// Here we check everything about the game that has to be drawn.
// It's all about HUDs and special effects!
{	
	drawOSD();
	drawKeys();
	drawSoloModeState();
	drawSplitBlood();
	drawStatus();
	drawMultiplier();
	
	// animate background
	//sky->getMaterial(0).getTextureMatrix(0).setTextureTranslate( 0, -*musicTime/3 );
	
	effectFactory->handleEffectsQueue();
	effectFactory->shieldmanager->drawAll();
	
	// FPS
	int fps = driver->getFPS();
	static int lastFPS = 0;
	if (lastFPS != fps) {
		setFps(fps);
		lastFPS = fps;
	}
}

void Screen::drawMultiplier()
{
	for(int i=0; i<NPLAYERS; i++)
		if(player[i]->multiplier==1)
			mult[i]->setVisible(false);
		else {
			mult[i]->setVisible(true);
			switch(player[i]->multiplier) {
				case 2:
					mult[i]->setImage(mult2xTex);
					break;			
				case 3:
					mult[i]->setImage(mult3xTex);
					break;
				case 4:
					mult[i]->setImage(mult4xTex);
					break;
			}
		}
}

void Screen::drawStatus()
{
	for(int i=0; i<NPLAYERS; i++) 
	{
		for(unsigned int k=player[i]->status.size(); k<5; k++)
			statusIcon[i][k]->setVisible(false);
			
		for(unsigned int k=0; k<player[i]->status.size(); k++)
			switch(player[i]->status[k].status)
			{
				case ST_POISON:
					statusIcon[i][k]->setVisible(true);
					statusIcon[i][k]->setImage(poisonTex);
					break;
				case ST_FIRE:
					statusIcon[i][k]->setVisible(true);
					statusIcon[i][k]->setImage(fireTex);
					break;
				case ST_MAGIC_BARRIER:
					statusIcon[i][k]->setVisible(true);
					statusIcon[i][k]->setImage(magicBarrierTex);
					break;
				case ST_MIRROR:
					statusIcon[i][k]->setVisible(true);
					statusIcon[i][k]->setImage(mirrorTex);
					break;
				case ST_ELETRIFIED:
					statusIcon[i][k]->setVisible(true);
					statusIcon[i][k]->setImage(eletrifiedTex);
					break;
				case ST_DROWNED:
					statusIcon[i][k]->setVisible(true);
					statusIcon[i][k]->setImage(drownedTex);
					break;			
				case ST_FROZEN:
					statusIcon[i][k]->setVisible(true);
					statusIcon[i][k]->setImage(frozenTex);
					break;
				case ST_CURSED:
					statusIcon[i][k]->setVisible(true);
					statusIcon[i][k]->setImage(cursedTex);
					break;			
				case ST_BLESSED:
					statusIcon[i][k]->setVisible(true);
					statusIcon[i][k]->setImage(blessedTex);
					break;
				default:
					statusIcon[i][k]->setVisible(false);
			}
	}
}

void Screen::drawSoloModeState()
{
	for(int i=0; i<NPLAYERS; i++)
		if( player[i]->isUsingSkill )
			effectFactory->soloEffect(i,glowTex,10);		
}

void Screen::drawOSD()
{
	//music time counter
	char str[30];
	int seconds = (int)(musicTotalTime-*musicTime) % 60;
	int minutes = (musicTotalTime-*musicTime) / 60.;
	if(seconds<10)
		sprintf(str,"%i:0%i",minutes,seconds);
	else
		sprintf(str,"%i:%i",minutes,seconds);
	timeText->setText( stringw(str).c_str() );
	
	// health and stamina bars
	for(int i=0; i<NPLAYERS; i++) {
		healthBar[i]->setProgress( player[i]->HP*100/player[i]->maxHP );		
		armorBar[i]->setProgress( (player[i]->HP+player[i]->getArmor())*100/player[i]->maxHP );
		
		staminaBar[i]->setProgress( player[i]->stamina*100/player[i]->maxStamina );	
		
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
			//cout << player[i]->damageTaken << endl;
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

void Screen::showKO()
{
	koImage->setVisible(true);
}

void Screen::hideKO()
{
	koImage->setVisible(false);
}
