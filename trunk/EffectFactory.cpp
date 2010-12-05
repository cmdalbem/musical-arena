#include "EffectFactory.h"

using namespace irr::core;
using namespace irr::video;

EffectFactory::EffectFactory( IrrlichtDevice *_device, IVideoDriver *_driver, ISceneManager *_smgr, vector<Player*> _players )
{
	device = _device;
	driver = _driver;
	smgr = _smgr;
	players = _players;
	
	ITexture *shielTex0 = driver->getTexture("img/blanktex.png");
	ITexture *shielTex1 = driver->getTexture("img/gradient_two.png");
	CBloodShader::instance().createMaterial(device);
	this->fireballTex = driver->getTexture("img/fireball.bmp");
	this->glowTex = driver->getTexture("img/glow2.bmp");
	this->laserTex = driver->getTexture("img/laser3.bmp");
	//this->smokeTex1 = driver->getTexture("img/smoke1.jpg");
	//this->smokeTex2 = driver->getTexture("img/smoke2.jpg");
	char c[50];
	for(int i=0; i<5; i++) {
		sprintf(c,"img/water%i.png", i+1);
		waterTex[i] = driver->getTexture(c);
		sprintf(c,"img/blood%i.png", i+1);
		bloodTex[i] = driver->getTexture(c);
	}
	#define NFEEDBACKTEX 7
	for(int i=0; i<NFEEDBACKTEX; i++) {
		sprintf(c,"img/portal%i.bmp", NFEEDBACKTEX-i);
		feedbackTex.push_back( driver->getTexture(c) );
	}
	#define NEXPLOSIONTEX 36
	for(int i=0; i<NEXPLOSIONTEX; i++) {
		sprintf(c,"img/explosion/explosion%i.png", i);
		explosion.push_back( driver->getTexture(c) );
	}
	
	scene::IMeshManipulator *manipulator = smgr->getMeshManipulator();
	
	shieldmanager = new CShieldManager(smgr,device->getTimer()); 

	for(int i=0;i<NPLAYERS;i++) {
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
	}

}

EffectFactory::~EffectFactory()
{

}

void EffectFactory::queueEffect( int msecondsAhead, visualEffectFunction functionToCall, int targetPlayer )
{ 
	effectsQueue.insert( effectEvent(pair<unsigned int,visualEffectFunction>(device->getTimer()->getTime()+msecondsAhead,functionToCall),targetPlayer) );	
}

void EffectFactory::handleEffectsQueue()
{
	unsigned int time = device->getTimer()->getTime();
	
	while( effectsQueue.size()>0  &&  time > effectsQueue.begin()->first.first ) {
		
		int target = effectsQueue.begin()->second;
		
		switch( effectsQueue.begin()->first.second )
		{
			case CREATE_FIREBALL:
				createFireball(target,false);
				break;			
			case CREATE_FIRE_RAIN:
				for(int i=0; i<50; i++)
					queueEffect( i*150, CREATE_FIREBALL_SKY, target );
				break;
			case CREATE_FIREBALL_SKY:
				createFireRain(target);
				break;
			case CREATE_GLOW_AREA:
				createAreaEffect(target,glowTex);
				break;
			case CREATE_FEEDBACK:
				createFeedback(target);
				break;
			case SHOW_SHIELD:
				createShield(target);
				break;
			case CREATE_BOLT:
				createBolt(target);
				break;
			case CREATE_THUNDERSTORM:
				srand(device->getTimer()->getRealTime());
				for(int i=0; i<50; i++)
					queueEffect( rand()%3000, CREATE_THUNDERSTORM_BOLT, target );
				break;
			case CREATE_THUNDERSTORM_BOLT:
				createThunderstormBolt(target);
				break;
			case CREATE_WATER_BEAM:
				createWaterBeam(target);
				break;	
			case CREATE_DRUNK_EFFECT:
				for(int i=0; i<5; i++)
					queueEffect( i*500, CREATE_DRUNK_EFFECT_SINGLE, target );
				break;
			case CREATE_DRUNK_EFFECT_SINGLE:
				createDrunkEffect(target,8);
				break;
			case CREATE_EXPLOSION:
				createExplosion(players[target]->track->getCentroid());
				break;
			case CREATE_ELETRIC_GROUND:
				createEletrifiedGround(target);
				break;
			case CREATE_FLOOD_EFFECT:
				createFloodEffect(target);
				break;
		}
		effectsQueue.erase( effectsQueue.begin() );
	}
}

void EffectFactory::createFeedback( int i )
{
	IVolumeLightSceneNode * spellEffect = smgr->addVolumeLightSceneNode(0, -1,
                                128,                              // Subdivisions on U axis
                                128,                              // Subdivisions on V axis
                                SColor(0, 50, 50, 80), 		// foot color
                                SColor(0, 0, 0, 0),      		// tail color
                                players[i]->track->getCentroid(),
                                vector3df(-90,0,0),
                                vector3df(60,30,60));
	spellEffect->addAnimator( smgr->createTextureAnimator(feedbackTex, 120) );
	spellEffect->addAnimator( smgr->createDeleteAnimator(3000) );
}

void EffectFactory::createExplosion( vector3df pos )
{
	IVolumeLightSceneNode * spellEffect = smgr->addVolumeLightSceneNode(0, -1,
                                256,                              // Subdivisions on U axis
                                256,                              // Subdivisions on V axis
                                SColor(0, 20,20,20), 		// foot color
                                SColor(0, 0,0,0),      		// tail color
                                pos,
                                vector3df(-90,0,0),
                                vector3df(70,20,70));
	spellEffect->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	
	#define animTime 45
	spellEffect->addAnimator( smgr->createTextureAnimator(explosion, animTime, false) );
	spellEffect->addAnimator( smgr->createDeleteAnimator(NEXPLOSIONTEX*animTime) );
}

void EffectFactory::createShield( int player )
{
	shieldmanager->addLocalImpact(shields[player], vector3df(0,0,-1), 20);
}

void EffectFactory::createFireRain( int attacked )
{
	array<vector3df> path;
	vector3df p1,p2;
	
	p2 = players[attacked]->track->getRandomPos();
	p1 = p2 + vector3df(0,0,-40);
	
	path.push_back( p1 );
	path.push_back( p2 );	
	
	createFireball(path);
}

void EffectFactory::createFireball( int attacked, bool randomizeTarget )
{
	array<vector3df> path;
	vector3df p1,p2,p3;
	
	p1 = players[!attacked]->track->getCentroid();
	p3 = players[attacked]->track->getCentroid();
	if(randomizeTarget)
		p3 = vector3df(p3.X + rand()%TRACK_SIZE_X - TRACK_SIZE_X/2, p3.Y + rand()%TRACK_SIZE_Y - TRACK_SIZE_Y/2, p3.Z);
	p2 = vector3df( (p1.X+p3.X)/2, (p1.Y+p3.Y)/2, -20 ); //generic middle-point	
	
	path.push_back( p1 + vector3df(0,0,-20) );
	path.push_back( p2 );
	path.push_back( p3 );	
	
	createFireball(path);	
}

void EffectFactory::createFireball( array<vector3df> path )
{
	float speed = 1.5;
	
	scene::ISceneNode* ball = smgr->addBillboardSceneNode(smgr->getRootSceneNode(), core::dimension2d<f32>(10, 10));
	ball->setMaterialFlag(video::EMF_LIGHTING, false);
	ball->setMaterialTexture(0, fireballTex);
	ball->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	
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
	
	// set animators
	ball->addAnimator( smgr->createFollowSplineAnimator( device->getTimer()->getTime(),path,speed,1,false) );
	ball->addAnimator( smgr->createDeleteAnimator((2/speed)*1000 + 400) );
}

void EffectFactory::createAreaEffect( int player, ITexture *tex, int timeMs )
{
	vector3df pos = players[player]->track->getCentroid();
	
	// add particle system
	scene::IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false);
	ps->setPosition( pos );	
	
	// create and set emitter
	scene::IParticleEmitter* em = ps->createBoxEmitter(
			core::aabbox3d<f32>(-TRACK_SIZE_X/2,-TRACK_SIZE_Y/2,0,TRACK_SIZE_X/2,TRACK_SIZE_Y/2,1), //minx, miny, minz, maxx, maxy, maxz
			core::vector3df(0.0f,0.03f,0.0f),
			100,100,
			video::SColor(0,255,255,255), video::SColor(0,255,255,255),
			400,2000);
	em->setMinStartSize(core::dimension2d<f32>(3.0f, 3.0f));
	em->setMaxStartSize(core::dimension2d<f32>(6.0f, 6.0f));
	ps->setEmitter(em);
	em->drop();

	ps->addAffector( ps->createFadeOutParticleAffector() );

	// adjust some material settings
	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, tex);
	ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
	
	new CDeleteParticleAffector(ps, timeMs);
	ps->addAnimator( smgr->createDeleteAnimator(timeMs+1300) );
}

void EffectFactory::createBolt( int target )
{
	vector3df initPos = players[!target]->track->getCentroid() + vector3df(0,0,-20);
	vector3df endPos = players[target]->track->getCentroid() * vector3df(2,1,1);
	
	IBillboardSceneNode* ball = smgr->addBillboardSceneNode(smgr->getRootSceneNode(), core::dimension2d<f32>(20, 20));
	ball->setMaterialFlag(video::EMF_LIGHTING, false);
	ball->setMaterialTexture(0, glowTex);
	ball->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	ball->setPosition(initPos);
	
	CBoltSceneNode* beam = new CBoltSceneNode(smgr->getRootSceneNode(), smgr, -1, laserTex); 
	// start, end, updateTime = 300, height = 10, parts = 10, bolts = 1, steddyend = true, thick=5.0f , color
	beam->setLine( initPos,
				   endPos,
				   100, 20, 5, 3, false, 5, SColor(255,0,0,255)); 
				   
	beam->addAnimator( smgr->createDeleteAnimator(1000) );
	ball->addAnimator( smgr->createDeleteAnimator(1000) );
}

void EffectFactory::createThunderstormBolt( int target )
{
	vector3df endPos = players[target]->track->getRandomPos();
	vector3df initPos = endPos + vector3df(0,0,-20);
	
	IBillboardSceneNode* ball = smgr->addBillboardSceneNode(smgr->getRootSceneNode(), core::dimension2d<f32>(20, 20));
	ball->setMaterialFlag(video::EMF_LIGHTING, false);
	ball->setMaterialTexture(0, glowTex);
	ball->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	ball->setPosition(initPos);
	
	CBoltSceneNode* beam = new CBoltSceneNode(smgr->getRootSceneNode(), smgr, -1, laserTex); 
	// start, end, updateTime = 300, height = 10, parts = 10, bolts = 1, steddyend = true, thick=5.0f , color
	beam->setLine( initPos,
				   endPos,
				   80, 10, 4, 2, false, 5, SColor(255,0,0,255)); 
				   
	beam->addAnimator( smgr->createDeleteAnimator(1000) );
	ball->addAnimator( smgr->createDeleteAnimator(1000) );
}

void EffectFactory::createEletrifiedGround( int targetPlayer )
{
	TrackSceneNode* node = (TrackSceneNode*)players[targetPlayer]->track->node;
	for(int i=0; i<NFRETS; i++) {
		CBoltSceneNode* beam = new CBoltSceneNode(smgr->getRootSceneNode(), smgr, -1, laserTex); 
		// start, end, updateTime = 300, height = 10, parts = 10, bolts = 1, steddyend = true, thick=5.0f , color
		beam->setLine( vector3df(node->getStoneXPos(i), node->getPosition().Y, node->getPosition().Z-3.5),
					   vector3df(node->getStoneXPos(i), node->getPosition().Y-TRACK_SIZE_Y-5, node->getPosition().Z-3.5),
					   40, 2, 17, 1, false, 1, SColor(255,0,0,255)); 
		beam->addAnimator( smgr->createDeleteAnimator(5000) );
	}
}

void EffectFactory::createWaterBeam( int targetPlayer )
{
	vector3df initPos = players[!targetPlayer]->track->getCentroid()  + vector3df(0,0,-20);
	vector3df direction(targetPlayer*2-1,0,1);
	
	new CBloodEffect(device->getSceneManager(), waterTex, EGL_INSANE, initPos, direction, 3000);
}

void EffectFactory::createFloodEffect( int player )
{
	vector3df pos = players[player]->track->getCentroid();
	
	// create and set emitter
	for(int i=0; i<5; i++)
	{
		// add particle system
		scene::IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false,0,-1,pos);
		
		scene::IParticleEmitter* em = ps->createBoxEmitter(
				core::aabbox3d<f32>(-TRACK_SIZE_X/2,-TRACK_SIZE_Y/2,0,TRACK_SIZE_X/2,TRACK_SIZE_Y/2,1), //minx, miny, minz, maxx, maxy, maxz
				core::vector3df(0.0f,0.015f,0.0f),
				20,80,
				video::SColor(0,255,255,255), video::SColor(0,255,255,255),
				400,3000,
				10,
				dimension2d<f32>(0.5f, 0.5f),
				dimension2d<f32>(5.0f, 5.0f));
		ps->setEmitter(em);
		em->drop();

		ps->addAffector( ps->createScaleParticleAffector( dimension2df(10,10) ) );

		// adjust some material settings
		ps->setMaterialFlag(EMF_LIGHTING, false);
		ps->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
		ps->setMaterialTexture(0, waterTex[i]);
		ps->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
		
		new CDeleteParticleAffector(ps, 6000);
		ps->addAnimator( smgr->createDeleteAnimator(12000) );
	}
}

void EffectFactory::splitBlood( int targetPlayer )
{
	// EGL_MILD, EGL_MEDIUM, EGL_BRUTAL, EGL_INSANE
	blood = new CBloodEffect(device->getSceneManager(), bloodTex, EGL_MILD, players[targetPlayer]->track->getCentroid()+vector3df(targetPlayer==0? 14:-14,0,-30), vector3df(0, -0.5f, -0.05), 35);
	blood->setVisible(false);
}

void EffectFactory::createDrunkEffect ( int targetPlayer, int times )
{
	srand(time(0));
	int k;
	for(int i=0; i<NFRETS; i++) {
		vector<Stone*> stones = players[targetPlayer]->track->stones[i];
		
		for(int xtimes=0;xtimes<times; xtimes++)
			if(stones.size()) {
				k = rand()%stones.size();
				stones[k]->displace = vector3df(rand()%6-3,0,0);
				createParticlesExplosion( stones[k]->getPosition()+stones[k]->displace,glowTex );
			}
	}
}

void EffectFactory::createParticlesExplosion( vector3df pos, ITexture *tex )
{
	// add particle system
	scene::IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false,0,-1,pos);
	
	// create and set emitter
	scene::IParticleEmitter* em = ps->createPointEmitter(
			core::vector3df(0.0f,0.08f,0.0f),
			20,40, // particles/second
			video::SColor(0,255,255,255), video::SColor(0,255,255,255),
			100,300, // lifetime
			10,
			dimension2d<f32>(0.5, 0.5),
			dimension2d<f32>(1, 1)
			);
	ps->setEmitter(em);
	em->drop();

	//ps->addAffector( ps->createFadeOutParticleAffector() );
	ps->addAffector( ps->createGravityAffector(vector3df(0, -0.25, 0)) );

	// adjust some material settings
	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, tex);
	ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
	
	new CDeleteParticleAffector(ps, 100);
	ps->addAnimator( smgr->createDeleteAnimator(1000) );	
}
