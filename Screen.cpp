#include "Screen.h"
#include "utils.h"

using namespace irr::core;
using namespace irr::video;

Screen::Screen( IrrlichtDevice *_device, Player* player1, Player* player2 )
{
	this->device = _device;
	this->driver = device->getVideoDriver();
	this->smgr = device->getSceneManager();
	
	players[0] = player1;
	players[1] = player2;
	
	ITexture *glowTex = driver->getTexture("img/glow2.bmp");
	ITexture *goodTex = driver->getTexture("img/good.png");
	ITexture *badTex = driver->getTexture("img/bad.png");
	ITexture *neutralTex = driver->getTexture("img/neutral.png");
	ITexture *shielTex0 = driver->getTexture("img/blanktex.png");
	ITexture *shielTex1 = driver->getTexture("img/gradient_two.png");
	CBloodShader::instance().createMaterial(device);
	spellEffectTex.push_back( driver->getTexture("img/portal7.bmp") );
	spellEffectTex.push_back( driver->getTexture("img/portal6.bmp") );
	spellEffectTex.push_back( driver->getTexture("img/portal5.bmp") );
	spellEffectTex.push_back( driver->getTexture("img/portal4.bmp") );
	spellEffectTex.push_back( driver->getTexture("img/portal3.bmp") );
	spellEffectTex.push_back( driver->getTexture("img/portal2.bmp") );
	spellEffectTex.push_back( driver->getTexture("img/portal1.bmp") );
	this->fireballTex = driver->getTexture("img/fireball.bmp");
	
	fpsText = device->getGUIEnvironment()->addStaticText(L"", core::rect<int>(0, 0, 100, 10));
	
	scene::IMeshManipulator *manipulator = smgr->getMeshManipulator();
	shieldmanager = new CShieldManager(smgr,device->getTimer()); 
	
	for(int i=0;i<NPLAYERS;i++) {
		int xpos = SCREENX/4 + i*SCREENX/2;
		
		hpTxt[i] = device->getGUIEnvironment()->addStaticText(L"", rect<int>(xpos, 50, xpos+50, 100));
		
		// shields
		//spherical shield
		shields[i] = smgr->addSphereSceneNode(1,50,smgr->getRootSceneNode());
		shields[i]->setScale( vector3df(TRACK_SIZE_X*0.7,TRACK_SIZE_Y*0.7,20) );
		
		//rectangular shield
		//shields[i] = smgr->addCubeSceneNode(1,smgr->getRootSceneNode());
		//shields[i]->setScale( vector3df(TRACK_SIZE_X,TRACK_SIZE_Y,7) );
		
		shields[i]->setPosition(vector3df(players[i]->track->posx,-TRACK_SIZE_Y/2,players[i]->track->posz));
		shields[i]->setMaterialFlag(video::EMF_BACK_FACE_CULLING,false);
		shields[i]->setMaterialTexture(0,shielTex0);
		shields[i]->setMaterialTexture(1,shielTex1);
		manipulator->scaleTCoords(shields[i]->getMesh(),core::vector2df(6,6),1);
		shields[i]->setVisible(false);
		shieldmanager->addShield(shields[i]);
		
		// smileys
		good[i] = device->getGUIEnvironment()->addImage( goodTex, core::position2d<s32>(xpos,500), true );
		bad[i] = device->getGUIEnvironment()->addImage( badTex, core::position2d<s32>(xpos,500), true );
		neutral[i] = device->getGUIEnvironment()->addImage( neutralTex, core::position2d<s32>(xpos,500), true );
		
		// health bars
		healthBars[i] = new VxHealthSceneNode(
							smgr->getRootSceneNode(), // parent node
							smgr, // scene manager
							-1, // id
							300, // width
							30, // height
							core::vector3df(xpos, 20, 0), // position in 2d
							video::SColor(255,0,200,0), // bar color
							video::SColor(255,220,0,0), // background color
							video::SColor(128,255,255,255) ); // border color
		
		// keys hitting glows
		for(int k=0;k<NFRETS;k++) {
			/*glow[i][k] = smgr->addVolumeLightSceneNode(smgr->getRootSceneNode(), -1,
							12,                              // Subdivisions on U axis
							12,                              // Subdivisions on V axis
							fretColors[k], 			// foot color
							SColor(0, 0, 0, 0),      		// tail color
							vector3df(players[i]->track->getStoneXPos(k),-TRACK_SIZE_Y,players[i]->track->posz),
							vector3df(-90,0,0),
							vector3df(10,10,10));
			glow[i][k]->setMaterialTexture(0, glowTex);	
			glow[i][k]->setVisible(false);*/			
			
			glow[i][k] = smgr->addBillboardSceneNode(smgr->getRootSceneNode(), dimension2d<float>(10, 10));
			glow[i][k]->setMaterialFlag(EMF_LIGHTING, false);
			glow[i][k]->setMaterialType(EMT_TRANSPARENT_ADD_COLOR); 
			glow[i][k]->setPosition( vector3df(players[i]->track->getStoneXPos(k),-TRACK_SIZE_Y,players[i]->track->posz) );
			glow[i][k]->setVisible(false);
			((IBillboardSceneNode*)glow[i][k])->setColor(fretColors[k]);
			glow[i][k]->setMaterialTexture(0, glowTex);
			//glow[i][k]->addAnimator( smgr->createTextureAnimator(textures, 70) );
		}							
	}		
	
	
	
	//// Special Effects Playground below! :D
	//    
	
	// fireball
	for(int i=0; i<4; i++)
		createFireball(1,true);
	
	// blood effect
	new CBloodEffect(device->getSceneManager(), EGL_INSANE, players[1]->track->getCentroid(), vector3df(0, 0.0f, -1.0f), 1000);
	
	// spell casting effect	
	//showSpellEffect(0);
	
	// lighning bolt
	/*irr::scene::CBoltSceneNode* beam = new CBoltSceneNode(smgr->getRootSceneNode(), smgr, -1,"img/laser3.bmp"); 
	// start, end, updateTime = 300, height = 10, parts = 10, bolts = 1, steddyend = true, thick=5.0f , color
	beam->setLine(vector3df(players[0]->track->posx,-TRACK_SIZE_Y/2,players[0]->track->posz),
				 vector3df(players[1]->track->posx,-TRACK_SIZE_Y/2,players[1]->track->posz),
				 100, 10, 5, 2, false, 5, SColor(255,0,0,255)); */
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
		
		for(int k=0; k<NFRETS; k++) {
			
			glow[i][k]->setVisible( players[i]->fretting->_hitting[k]==1 );
			
			int zdisplace = 0;
			// what's the state of the fret?
			switch( players[i]->fretting->_hitting[k] )
			{
				case 0:
					color = fretColors[k];
					color.setAlpha(255);
					break;				
				case 1: 
					color = fretColors[k];
					color.setAlpha(255);
					zdisplace=3;
					break;
				default:
					color.setAlpha(128);
					color = SColor(255,0,0,0);
					//color.setAlpha(255);
					break;
				
			}
			
			// draw the colored bar for this fret
			driver->draw3DLine( vector3df(players[i]->track->getStoneXPos(k) -1, -TRACK_SIZE_Y, players[i]->track->posz - zdisplace),
								vector3df(players[i]->track->getStoneXPos(k) +1, -TRACK_SIZE_Y, players[i]->track->posz - zdisplace),
								color );
		}
	}
}	

void Screen::update()
// Here we check everything about the game that has to be drawn.
// It's all about HUDs and special effects!
{
	drawHP();
	drawKeys();
	drawHittingState();
}

void Screen::drawHP()
{
	for(int i=0; i<NPLAYERS; i++) {
		healthBars[i]->setProgress( players[i]->HP*100/players[i]->maxHP );
			
		char str[30];
		sprintf(str,"%i/%i",players[i]->HP,players[i]->maxHP);
		hpTxt[i]->setText( stringw(str).c_str() );
	}
}

void Screen::drawHittingState()
{
	for(int i=0; i<NPLAYERS; i++) 
	{
		switch( players[i]->fretting->frettingState )
		{
			case  1:
				showGood(i);
				showShield(i);  //temp! just for demonstration
				break;
			case -1:
				showBad(i);
				showShield(!i); //temp! just for demonstration
				break;
			case 0:
				showNeutral(i);
				break;
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

void Screen::showSpellEffect( int i )
{
	IVolumeLightSceneNode * spellEffect = smgr->addVolumeLightSceneNode(0, -1,
                                128,                              // Subdivisions on U axis
                                128,                              // Subdivisions on V axis
                                SColor(0, 50, 50, 80), 		// foot color
                                SColor(0, 0, 0, 0),      		// tail color
                                players[i]->track->getCentroid(),
                                vector3df(-90,0,0),
                                vector3df(60,30,60));
	spellEffect->addAnimator( smgr->createTextureAnimator(spellEffectTex, 125) );
	spellEffect->addAnimator( smgr->createDeleteAnimator(5000) );
}

void Screen::showShield( int player )
{
	shieldmanager->addLocalImpact(shields[player], vector3df(0,0,-1), 20);
}

void Screen::createFireball( int attacked, bool randomize )
{
	float speed = 1;
	vector3df p1,p2,p3;
	
	scene::ISceneNode* ball = smgr->addBillboardSceneNode(smgr->getRootSceneNode(), core::dimension2d<f32>(10, 10));
	ball->setMaterialFlag(video::EMF_LIGHTING, false);
	ball->setMaterialTexture(0, fireballTex);
	ball->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	
	// set points of movement
	p1 = players[!attacked]->track->getCentroid();
	p3 = players[attacked]->track->getCentroid();
	if(randomize)
		p3 = vector3df(p3.X + rand()%TRACK_SIZE_X - TRACK_SIZE_X/2, p3.Y + rand()%TRACK_SIZE_Y - TRACK_SIZE_Y/2, p3.Z);
	p2 = vector3df( (p1.X+p3.X)/2, (p1.Y+p3.Y)/2, -10 );
	
	// fill points on vector
	array<vector3df> points;
	points.push_back( p1 );
	points.push_back( p2 );
	points.push_back( p3 );
	
	// set animators
	ball->addAnimator( smgr->createFollowSplineAnimator( device->getTimer()->getTime(),points,speed,1,false) );
	ball->addAnimator( smgr->createDeleteAnimator((2/speed)*1000 + 800) );
	
	// add particle system
	scene::IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false, ball);

	// create and set emitter
	scene::IParticleEmitter* em = ps->createBoxEmitter(
			core::aabbox3d<f32>(-3,0,-3,3,1,3),
			core::vector3df(0.0f,0.03f,0.0f),
			80,100,
			video::SColor(0,255,255,255), video::SColor(0,255,255,255),
			400,1100);
	em->setMinStartSize(core::dimension2d<f32>(5.0f, 7.0f));
	em->setMaxStartSize(core::dimension2d<f32>(5.0f, 7.0f));
	ps->setEmitter(em);
	em->drop();

	ps->addAffector( ps->createFadeOutParticleAffector() );

	// adjust some material settings
	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, fireballTex);
	ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
}
