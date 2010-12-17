#include "EffectFactory.h"

using namespace irr::core;
using namespace irr::video;

EffectFactory::EffectFactory( IrrlichtDevice *_device, IVideoDriver *_driver, ISceneManager *_smgr, vector<Player*> _players, ILightSceneNode *_light )
{
	device = _device;
	driver = _driver;
	smgr = _smgr;
	players = _players;
	light = _light;
	
	stoneMesh1 = smgr->getMesh("stone1.3ds");
	stoneMesh2 = smgr->getMesh("stone2.3ds");
	
	ITexture *shieldTex0 = driver->getTexture("img/shield_tex.png");
	ITexture *shieldTex1 = driver->getTexture("img/gradient.png");
	CBloodShader::instance().createMaterial(device);
	this->gridTex = driver->getTexture("img/grid.jpg");
	this->starTex = driver->getTexture("img/stars.tga");
	this->fireballTex = driver->getTexture("img/fireball.bmp");
	this->glowTex = driver->getTexture("img/glow2.bmp");
	this->glowTex2 = driver->getTexture("img/glowpurple.png");
	this->glowTex3 = driver->getTexture("img/glowblack.png");
	this->blackTex = driver->getTexture("img/black.png");
	this->iceTex = driver->getTexture("img/icetex.png");
	this->laserTex1 = driver->getTexture("img/laser1.png");
	this->laserTex2 = driver->getTexture("img/laser2.png");
	this->laserTex3 = driver->getTexture("img/laser3.png");
	this->lensFlareTex = driver->getTexture("img/flares.jpg");
	char c[50];
	for(int i=0; i<5; i++) {
		sprintf(c,"img/water%i.png", i+1);
		waterTex[i] = driver->getTexture( c);
		sprintf(c,"img/blood%i.png", i+1);
		bloodTex[i] = driver->getTexture(c);
	}
	#define NSNOWTEX 3
	for(int i=0; i<NSNOWTEX; i++) {
		sprintf(c,"img/snow%i.png", i+1);
		snowFlakes.push_back( driver->getTexture(c) );
	}
	
	// ANIMATIONS
	#define NFEEDBACKTEX 7
	for(int i=0; i<NFEEDBACKTEX; i++) {
		sprintf(c,"img/anim/portal%i.bmp", NFEEDBACKTEX-i);
		feedbackTex.push_back( driver->getTexture(c) );
	}
	#define NICETEX 30
	for(int i=0; i<NICETEX; i++) {
		sprintf(c,"img/anim/ice_001_%i.png", i+1);
		ice1.push_back( driver->getTexture(c) );
	}
	#define NMAGICTEX 29
	for(int i=0; i<NMAGICTEX; i++) {
		sprintf(c,"img/anim/magic1_%i.png", i);
		magic1.push_back( driver->getTexture(c) );
	}
	#define NWATERTEX 10
	for(int i=0; i<NWATERTEX; i++) {
		sprintf(c,"img/anim/water_001_%i.png", i);
		water1.push_back( driver->getTexture(c) );
	}
	#define NFIRETEX 20
	for(int i=0; i<NFIRETEX; i++) {
		sprintf(c,"img/anim/fire_001_%i.png", i);
		fire1.push_back( driver->getTexture(c) );
	}
	#define NLOTUSTEX 5
	for(int i=0; i<NLOTUSTEX; i++) {
		sprintf(c,"img/lotuspetal%i.png", i+1);
		lotusTex.push_back( driver->getTexture(c) );
	}
	
	
	/*IImage *iceSprites = driver->createImageFromFile("img/anim/magic_004.png");
	int xs = 5, ys=7;
	for(int i=0; i<xs; i++) {
		for(int k=0; k<ys; k++) {
			sprintf(c,"%i", i*k);
			IImage *temp = driver->createImage(ECF_A8R8G8B8, dimension2d<unsigned int>(192,192));
			iceSprites->copyTo(temp, position2di(192, 192), recti(position2di(192,192), dimension2d<unsigned int>(192,192)),1 );
			magic1.push_back(driver->addTexture(c, temp));
		}
	}*/	
	
	
	IMeshManipulator *manipulator = smgr->getMeshManipulator();
	
	shieldmanager = new CShieldManager(smgr,device->getTimer());  

	for(int i=0;i<NPLAYERS;i++) {		
		// shields
		//spherical shield
		shields[i] = smgr->addSphereSceneNode(1,50,smgr->getRootSceneNode());
		shields[i]->setScale( vector3df(TRACK_SIZE_X*0.7,TRACK_SIZE_Y*0.7,20) );
		
		//rectangular shield
		//shields[i] = smgr->addCubeSceneNode(1,smgr->getRootSceneNode());
		//shields[i]->setScale( vector3df(TRACK_SIZE_X,TRACK_SIZE_Y,7) );
		
		shields[i]->setPosition(vector3df(players[i]->track->posx,-TRACK_SIZE_Y/2,TRACK_POS_Z));
		shields[i]->setMaterialFlag(EMF_BACK_FACE_CULLING,false);
		shields[i]->setMaterialTexture(0,shieldTex0);
		shields[i]->setMaterialTexture(1,shieldTex1);
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
			case EFFECT_FIREBALL:
				effectFireball(target,false,true);
				break;			
			case EFFECT_FIRE_RAIN: {
				for(int i=0; i<50; i++)
					queueEffect( i*150, EFFECT_FIREBALL_SKY, target );
				ISceneNode *light = smgr->addLightSceneNode(0, players[target]->track->getCentroid()+vector3df(0,0,-14), SColorf(1, 0.8, 0.8), 150);
				light->addAnimator( smgr->createDeleteAnimator( 50*150+1000 ) );
			}
				break;
			case EFFECT_FIREBALL_SKY:
				effectFireRain(target);
				break;
			case EFFECT_SILENCE:
				areaEffect(target,glowTex, 150);
				break;
			case EFFECT_GLOW_AREA:
				areaEffect(target,glowTex, 400);
				break;
			case EFFECT_FEEDBACK:
				effectFeedback(target);
				break;
			case EFFECT_SHIELD:
				for(int i=0; i<5; i++)
					queueEffect( i*140, EFFECT_SHIELD_SINGLE, target );
				break;
			case EFFECT_SHIELD_SINGLE:
				effectShield(target);
				break;
			case EFFECT_BOLT:
				effectBolt(target);
				break;
			case EFFECT_THUNDERSTORM:
				srand(device->getTimer()->getRealTime());
				for(int i=0; i<40; i++)
					queueEffect( rand()%8000, EFFECT_THUNDERSTORM_BOLT, target );
				break;
			case EFFECT_THUNDERSTORM_BOLT:
				effectThunderstormBolt(target);
				break;
			case EFFECT_WATER_BEAM:
				effectWaterBeam(target);
				break;	
			case EFFECT_EXPLOSION:
				effectExplosion(target);
				break;
			case EFFECT_ELETRIC_GROUND:
				effectEletrifiedGround(target, 6000);
				break;
			case EFFECT_FLOOD:
				effectFloodEffect(target);
				break;
			case EFFECT_SWAMP:
				effectSwampEffect(target,8000);
				break;
			case EFFECT_BALL_LIGHTNING:
				effectBallLightning(target,8000);
				break;
			case EFFECT_SUN:
				effectSun(target,20000);
				break;
			case EFFECT_DARK_STORM:
				for(int i=0; i<30; i++)
					queueEffect( rand()%5000, EFFECT_DARK_STORM_BOLT, 1 );
				break;
			case EFFECT_DARK_STORM_BOLT:
				effectDarkStormBolt(target);
				break;
			case EFFECT_FREEZE:
				effectIce(target,10000);
				break;
			case EFFECT_BLACKHOLE:
				effectBlackHole(target,10000);
				for(int i=0; i<50; i++)
					queueEffect( 2000 + rand()%10000, EFFECT_BLACKHOLE_BOLT, 1 );
				break;
			case EFFECT_BLACKHOLE_BOLT:
				effectBlackHoleBolt(target);
				break;
			case EFFECT_AURORA:
				effectAurora(target);
				break;
			case EFFECT_VAMPIRIC:
				effectVampiric(target,2000);
				break;
			case EFFECT_LOTUS:
				effectLotus(target,500);
				break;
		}
		effectsQueue.erase( effectsQueue.begin() );
	}
}

////////////////////////////
// EFFECTS
////////////////////////////



void EffectFactory::effectFeedback( int i )
{
	IVolumeLightSceneNode * spellEffect = smgr->addVolumeLightSceneNode(0, -1,
                                32,                              // Subdivisions on U axis
                                32,                              // Subdivisions on V axis
                                SColor(0, 100, 100, 130), 		// foot color
                                SColor(0, 10, 10, 20),      		// tail color
                                players[i]->track->getCentroid(),
                                vector3df(-90,0,0),
                                vector3df(70,40,70));
	spellEffect->addAnimator( smgr->createTextureAnimator(feedbackTex, 150) );
	spellEffect->addAnimator( smgr->createDeleteAnimator(NFEEDBACKTEX*150*1.7) );
}

void EffectFactory::effectAurora( int target )
{
	vector3df pos = players[target]->track->getCentroid();
	
	/*ISceneNode * bill = smgr->addBillboardSceneNode(smgr->getRootSceneNode(),
                                core::dimension2d<f32>(70, 70),
                                pos);
	bill->setMaterialFlag(EMF_LIGHTING, false);
	bill->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	bill->addAnimator( smgr->createTextureAnimator(ice1, iceAnimTime, false) );
	bill->addAnimator( smgr->createDeleteAnimator(NICETEX*iceAnimTime) );*/
	
	IVolumeLightSceneNode * spellEffect = smgr->addVolumeLightSceneNode(0, -1,
                                128,                              // Subdivisions on U axis
                                128,                              // Subdivisions on V axis
                                SColor(0, 0, 0, 0), 		// foot color
                                SColor(0, 50, 50, 50),      		// tail color
                                pos,
                                vector3df(-90,0,0),
                                vector3df(40,20,40));
	spellEffect->addAnimator( smgr->createTextureAnimator(magic1, 80, false) );
	spellEffect->addAnimator( smgr->createDeleteAnimator(NMAGICTEX*80) );
	
	ISceneNode *light = smgr->addLightSceneNode(0, pos+vector3df(0,0,-20), SColorf(0.8, 0.8, 1), 100);
	light->addAnimator( smgr->createDeleteAnimator( NMAGICTEX*80 ) );
}

void EffectFactory::effectIce( int target, int timeMs )
{
	vector3df pos = players[target]->track->getCentroid();// + vector3df(0,-TRACK_SIZE_Y/2,0);
	
	#define iceAnimTime 50
	
	IAnimatedMeshSceneNode* stone = smgr->addAnimatedMeshSceneNode(stoneMesh1,
										0,-1,
										pos,
										vector3df(-90,0,90),
										vector3df(0,0,0)
										);
	stone->setMaterialTexture( 0, iceTex );
	stone->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
	stone->setMaterialFlag(EMF_LIGHTING, false);
	
	IVolumeLightSceneNode * spellEffect = smgr->addVolumeLightSceneNode(0, -1,
                                64,                              // Subdivisions on U axis
                                64,                              // Subdivisions on V axis
                                SColor(0, 100, 100, 100), 		// foot color
                                SColor(0, 0, 0, 0),      		// tail color
                                pos,
                                vector3df(-90,0,0),
                                vector3df(45,10,45));
	spellEffect->addAnimator( smgr->createTextureAnimator(ice1, 80) );
	spellEffect->addAnimator( smgr->createDeleteAnimator(NICETEX*80) );
	spellEffect->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	
	#define FINALSCALE vector3df(6,6,4)
	//stone->setScale( FINALSCALE );
	
	#define TIME_ANIM1 2300
	#define TIME_ANIM2 700
	ISceneNodeAnimator* appear = new CSceneNodeAnimatorScale(vector3df(2,0,1), FINALSCALE, TIME_ANIM1, false, device->getTimer()->getTime()+1000 );
	stone->addAnimator( appear );
	ISceneNodeAnimator* disappear = new CSceneNodeAnimatorScale(FINALSCALE, vector3df(3,0,0), TIME_ANIM2, false, device->getTimer()->getTime()+timeMs-(TIME_ANIM1+TIME_ANIM2) );
	stone->addAnimator( disappear );
	stone->addAnimator( smgr->createDeleteAnimator(timeMs) );
	
	IParticleSystemSceneNode* ps;
	IParticleEmitter* em;
	for(int i=0; i<3; i++)
	{
		// add particle system
		ps = smgr->addParticleSystemSceneNode(false,0,-1,pos);
		
		em = ps->createPointEmitter(
				vector3df(0,0,-0.02),
				100,700,
				SColor(0,200,200,255), SColor(0,255,255,255),
				1,100+ i*1000,
				180,
				dimension2d<f32>(2, 2),
				dimension2d<f32>(6, 6));
		ps->setEmitter(em);
		em->drop();

		// adjust some material settings
		ps->setMaterialFlag(EMF_LIGHTING, false);
		ps->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
		if(i<2) {
			ps->setMaterialTexture(0, snowFlakes[i]);
			ps->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
		}
		else {
			ps->setMaterialTexture(0, glowTex);
			ps->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
		}
		
		new CDeleteParticleAffector(ps, TIME_ANIM1);
		ps->addAnimator( smgr->createDeleteAnimator(TIME_ANIM1+3000) );
	}
}

void EffectFactory::effectExplosion( int target )
{
	vector3df pos = players[target]->track->getCentroid();
	
	#define expTime 60

	ISceneNode * spellEffect = smgr->addBillboardSceneNode(smgr->getRootSceneNode(),
                                core::dimension2d<f32>(80, 80),
                                pos);
	spellEffect->setMaterialFlag(EMF_LIGHTING, false);
	spellEffect->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	spellEffect->addAnimator( smgr->createTextureAnimator(fire1, expTime, false) );
	spellEffect->addAnimator( smgr->createDeleteAnimator(NFIRETEX*expTime) );
}

void EffectFactory::effectShield( int player )
{
	shieldmanager->addLocalImpact(shields[player], vector3df(0,0,-1), 70);
}

void EffectFactory::effectFireRain( int attacked )
{
	array<vector3df> path;
	vector3df p1,p2;
	
	p2 = players[attacked]->track->getRandomPos();
	p1 = p2 + vector3df(0,0,-40);
	
	path.push_back( p1 );
	path.push_back( p2 );	
	
	effectFireballGeneric(path,false);
}

void EffectFactory::effectFireball( int attacked, bool randomizeTarget, bool emitLight )
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
	
	effectFireballGeneric(path,emitLight);	
}

void EffectFactory::effectFireballGeneric( array<vector3df> path, bool emitLight )
{
	float speed = 2;
	float deathTime = (2/speed)*1000;
	
	ISceneNode* ball = smgr->addBillboardSceneNode(smgr->getRootSceneNode(), core::dimension2d<f32>(10, 10));
	ball->setMaterialFlag(EMF_LIGHTING, false);
	ball->setMaterialTexture(0, fireballTex);
	ball->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	
	// add particle system
	IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false, ball);
	ps->addAffector( ps->createFadeOutParticleAffector() );
	//  and set emitter
	IParticleEmitter* em = ps->createBoxEmitter(
			core::aabbox3d<f32>(-3,0,-3,3,1,3),
			core::vector3df(0.0f,0.03f,0.0f),
			80,100,
			SColor(0,255,255,255), SColor(0,255,255,255),
			400,1100);
	em->setMinStartSize(core::dimension2d<f32>(5.0f, 7.0f));
	em->setMaxStartSize(core::dimension2d<f32>(5.0f, 7.0f));
	ps->setEmitter(em);
	em->drop();
	
	ps->addAffector( ps->createFadeOutParticleAffector() );
	
	//  a lens flare node
	//CLensFlareSceneNode* lensFlare = new CLensFlareSceneNode(ball, smgr, -1);
	//lensFlare->getMaterial(0).setTexture(0, lensFlareTex);
	
	// light source
	if(emitLight) {
		ISceneNode *light = smgr->addLightSceneNode(ball, vector3df(0,0,0), SColorf(0.8, 0.1, 0.1), 50);
		light->addAnimator( smgr->createDeleteAnimator( deathTime ) );
	}

	// adjust some material settings
	ps->setMaterialFlag(EMF_LIGHTING, false);
	ps->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, fireballTex);
	ps->setMaterialType(EMT_TRANSPARENT_VERTEX_ALPHA);
	
	// set animators
	ball->addAnimator( smgr->createFollowSplineAnimator( device->getTimer()->getTime(),path,speed,1,false) );
	ball->addAnimator( smgr->createDeleteAnimator(deathTime + 400) );
	//lensFlare->addAnimator( smgr->createDeleteAnimator(deathTime) );	
}

void EffectFactory::areaEffect( int player, ITexture *tex, int timeMs )
{
	vector3df pos = players[player]->track->getCentroid();
	
	// add particle system
	IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false);
	ps->setPosition( pos );	
	
	//  and set emitter
	ps->setEmitter( ps->createBoxEmitter(
			core::aabbox3d<f32>(-TRACK_SIZE_X/2,-TRACK_SIZE_Y/2,0,TRACK_SIZE_X/2,TRACK_SIZE_Y/2,1), //minx, miny, minz, maxx, maxy, maxz
			core::vector3df(0.0f,0.015f,0.0f),
			300,400,
			SColor(0,255,255,255), SColor(0,255,255,255),
			100,2000,
			0,
			dimension2d<f32>(1.0f, 1.0f),
			dimension2d<f32>(10.0f, 10.0f)) );
	ps->addAffector( ps->createFadeOutParticleAffector() );

	// adjust some material settings
	ps->setMaterialFlag(EMF_LIGHTING, false);
	ps->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, tex);
	ps->setMaterialType(EMT_TRANSPARENT_VERTEX_ALPHA);
	
	new CDeleteParticleAffector(ps, timeMs);
	ps->addAnimator( smgr->createDeleteAnimator(timeMs+2000) );
}

void EffectFactory::soloEffect( int player, ITexture *tex, int timeMs )
{
	vector3df pos = players[player]->track->getCentroid();
	
	// add particle system
	IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false);
	ps->setPosition( pos );	
	
	//  and set emitter
	ps->setEmitter( ps->createBoxEmitter(
			core::aabbox3d<f32>(-TRACK_SIZE_X/2,-TRACK_SIZE_Y/2,0,TRACK_SIZE_X/2,TRACK_SIZE_Y/2,0.5), //minx, miny, minz, maxx, maxy, maxz
			core::vector3df(0.0f,0.015f,0.0f),
			80,80,
			SColor(0,100,100,255), SColor(0,200,100,255),
			400,2000,
			0,
			dimension2d<f32>(4.0f, 4.0f),
			dimension2d<f32>(6.0f, 6.0f)) );
	ps->addAffector( ps->createFadeOutParticleAffector() );

	// adjust some material settings
	ps->setMaterialFlag(EMF_LIGHTING, false);
	ps->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, tex);
	ps->setMaterialType(EMT_TRANSPARENT_VERTEX_ALPHA);
	
	new CDeleteParticleAffector(ps, timeMs);
	ps->addAnimator( smgr->createDeleteAnimator(timeMs+500) );
}

void EffectFactory::effectBolt( int target )
{
	vector3df initPos = players[!target]->track->getCentroid() + vector3df(0,0,-20);
	vector3df endPos = players[target]->track->getCentroid() * vector3df(2,1,1);
	
	IBillboardSceneNode* ball = smgr->addBillboardSceneNode(smgr->getRootSceneNode(), core::dimension2d<f32>(20, 20));
	ball->setMaterialFlag(EMF_LIGHTING, false);
	ball->setMaterialTexture(0, glowTex);
	ball->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	ball->setPosition(initPos);
	
	CBoltSceneNode* beam = new CBoltSceneNode(smgr->getRootSceneNode(), smgr, -1, laserTex1, EMT_TRANSPARENT_ADD_COLOR); 
	// start, end, updateTime = 300, height = 10, parts = 10, bolts = 1, steddyend = true, thick=5.0f , color
	beam->setLine( initPos,
				   endPos,
				   100, 20, 5, 5, false, 5, SColor(255,0,0,255)); 
				   
	beam->addAnimator( smgr->createDeleteAnimator(1000) );
	ball->addAnimator( smgr->createDeleteAnimator(1000) );
}

void EffectFactory::effectThunderstormBolt( int target )
{
	vector3df endPos = players[target]->track->getRandomPos()+vector3df(0,0,10);
	vector3df initPos = endPos + vector3df(0,0,-30);
	
	/*vector3df initPos = players[target]->track->getCentroid() + vector3df(0,0,-30);
	vector3df endPos = players[target]->track->getRandomPos() + vector3df(0,0,10);*/
	
	int time = rand()%1500;
	
	IBillboardSceneNode* ball = smgr->addBillboardSceneNode(smgr->getRootSceneNode(), core::dimension2d<f32>(15, 15));
	ball->setMaterialFlag(EMF_LIGHTING, false);
	ball->setMaterialTexture(0, glowTex);
	ball->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	ball->setPosition(initPos);
	ball->addAnimator( smgr->createDeleteAnimator(time) );
	
	ISceneNode *light = smgr->addLightSceneNode(ball, vector3df(0,0,0), SColorf(0.8, 0.8, 0.8), 100);
	light->addAnimator( smgr->createDeleteAnimator( time ) );
	
	CBoltSceneNode* beam = new CBoltSceneNode(smgr->getRootSceneNode(), smgr, -1, laserTex1, EMT_TRANSPARENT_ADD_COLOR); 
	// start, end, updateTime = 300, height = 10, parts = 10, bolts = 1, steddyend = true, thick=5.0f , color
	beam->setLine( initPos,
				   endPos,
				   80, 7, 6, 2, false, 5, SColor(255,0,0,255)); 
				   
	beam->addAnimator( smgr->createDeleteAnimator(time) );
}

void EffectFactory::effectEletrifiedGround( int targetPlayer, int timeMs )
{
	float xpos;
	CBoltSceneNode* beam;
	for(int i=0; i<NFRETS; i++) {
		xpos = players[targetPlayer]->track->node->getStoneXPos(i);
		beam = new CBoltSceneNode(smgr->getRootSceneNode(), smgr, -1, laserTex1, EMT_TRANSPARENT_ADD_COLOR); 
		// start, end, updateTime = 300, height = 10, parts = 10, bolts = 1, steddyend = true, thick=5.0f , color
		beam->setLine( vector3df(xpos, 0, TRACK_POS_Z-3.5),
					   vector3df(xpos, -TRACK_SIZE_Y-5, TRACK_POS_Z-3.5),
					   40, 2, 17, 1, false, 1, SColor(255,0,0,255)); 
		beam->addAnimator( smgr->createDeleteAnimator(timeMs) );
	}
}

void EffectFactory::effectWaterBeam( int targetPlayer )
{
	vector3df initPos = players[!targetPlayer]->track->getCentroid()  + vector3df(0,0,-20);
	vector3df direction(targetPlayer==0?-0.7:0.7,-1,0);
	
	#define waterAnimTime 70

	ISceneNode * spellEffect = smgr->addBillboardSceneNode(smgr->getRootSceneNode(),
                                core::dimension2d<f32>(70, 70),
                                initPos);
	spellEffect->setMaterialFlag(EMF_LIGHTING, false);
	spellEffect->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	spellEffect->addAnimator( smgr->createTextureAnimator(water1, waterAnimTime, false) );
	spellEffect->addAnimator( smgr->createDeleteAnimator(NWATERTEX*waterAnimTime) );
	
	new CBloodEffect(device->getSceneManager(), waterTex, EGL_INSANE, initPos, direction, 2000);
}

void EffectFactory::effectFloodEffect( int player )
{
	vector3df pos = players[player]->track->getCentroid();
	
	//  and set emitter
	IParticleSystemSceneNode* ps;
	IParticleEmitter* em;
	for(int i=0; i<5; i++)
	{
		// add particle system
		ps = smgr->addParticleSystemSceneNode(false,0,-1,pos);
		
		em = ps->createBoxEmitter(
				core::aabbox3d<f32>(-TRACK_SIZE_X/2,-TRACK_SIZE_Y/2,0,TRACK_SIZE_X/2,TRACK_SIZE_Y/2,1), //minx, miny, minz, maxx, maxy, maxz
				core::vector3df(0.0f,0.015f,0.0f),
				20,80,
				SColor(0,255,255,255), SColor(0,255,255,255),
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
		
		#define FLOOD_T 10000
		new CDeleteParticleAffector(ps, FLOOD_T);
		ps->addAnimator( smgr->createDeleteAnimator(FLOOD_T*2) );
	}
}

void EffectFactory::effectSwampEffect( int player, int timeMs )
{
	vector3df pos = players[player]->track->getCentroid();
	
	//  and set emitter
	IParticleSystemSceneNode* ps;
	IParticleEmitter* em;
	for(int i=0; i<5; i++)
	{
		// add particle system
		ps = smgr->addParticleSystemSceneNode(false,0,-1,pos);
		
		em = ps->createBoxEmitter(
				aabbox3d<f32>(-TRACK_SIZE_X/2,-TRACK_SIZE_Y/2,0,TRACK_SIZE_X/2,TRACK_SIZE_Y/2,1), //minx, miny, minz, maxx, maxy, maxz
				vector3df(0.0f,0.002f,0.0f),
				20,30,
				SColor(0,100,255,90), SColor(i*30,255,50,128),
				10,2000,
				15,
				dimension2df(1.0f, 1.0f),
				dimension2df(3.0f, 3.0f));
		ps->setEmitter(em);
		em->drop();

		ps->addAffector( ps->createScaleParticleAffector( dimension2df(4,4) ) );

		// adjust some material settings
		ps->setMaterialFlag(EMF_LIGHTING, false);
		ps->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
		ps->setMaterialTexture(0, waterTex[i]);
		ps->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
		
		new CDeleteParticleAffector(ps, timeMs);
		ps->addAnimator( smgr->createDeleteAnimator(timeMs*2) );
	}
}

void EffectFactory::splitBlood( int targetPlayer, E_GORE_LEVEL gore )
{
	// EGL_MILD, EGL_MEDIUM, EGL_BRUTAL, EGL_INSANE
	blood = new CBloodEffect(device->getSceneManager(), bloodTex, gore,
							players[!targetPlayer]->track->getCentroid()+vector3df(targetPlayer==1? 21:-21,-48.5,-10), //source
							vector3df(0, -0.7, -3), //direction
							35); //duration
	blood->setVisible(false);
}

void EffectFactory::effectParticlesExplosion( vector3df pos, ITexture *tex )
{
	// add particle system
	IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false,0,-1,pos);
	
	//  and set emitter
	IParticleEmitter* em = ps->createPointEmitter(
			core::vector3df(0.0f,0.08f,0.0f),
			20,40, // particles/second
			SColor(0,255,255,255), SColor(0,255,255,255),
			100,300, // lifetime
			10,
			dimension2d<f32>(0.5, 0.5),
			dimension2d<f32>(1, 1)
			);
	ps->setEmitter(em);
	em->drop();

	//ps->addAffector( ps->FadeOutParticleAffector() );
	ps->addAffector( ps->createGravityAffector(vector3df(0, -0.25, 0)) );

	// adjust some material settings
	ps->setMaterialFlag(EMF_LIGHTING, false);
	ps->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, tex);
	ps->setMaterialType(EMT_TRANSPARENT_VERTEX_ALPHA);
	
	new CDeleteParticleAffector(ps, 100);
	ps->addAnimator( smgr->createDeleteAnimator(1000) );	
}

void EffectFactory::effectBallLightning( int target, int timeMs )
{
	vector3df initPos = players[target]->track->getCentroid() + vector3df(0,0,-15);
	
	ISceneNode* ball = smgr->addSphereSceneNode(1.75);
	ball->setPosition(initPos);
	ball->addAnimator( smgr->createDeleteAnimator(timeMs) );

	IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false,ball);
	
	//  and set emitter
	IParticleEmitter* em = ps->createPointEmitter(
			core::vector3df(0.0f,0.0f,0.1f),
			200,400, // particles/second
			SColor(0,200,200,255), SColor(0,200,200,255),
			300,300, // lifetime
			360,
			dimension2d<f32>(1, 1),
			dimension2d<f32>(2, 2)
			);
	ps->setEmitter(em);
	em->drop();
	
	// adjust some material settings
	ps->setMaterialFlag(EMF_LIGHTING, false);
	ps->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, glowTex);
	ps->setMaterialType(EMT_TRANSPARENT_VERTEX_ALPHA);
	new CDeleteParticleAffector(ps, timeMs);
	ps->addAnimator( smgr->createDeleteAnimator(timeMs) );	
	
	IBillboardSceneNode* glow = smgr->addBillboardSceneNode(smgr->getRootSceneNode(), core::dimension2d<f32>(40, 40));
	glow->setMaterialFlag(EMF_LIGHTING, false);
	glow->setMaterialTexture(0, feedbackTex[5]);
	glow->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	glow->setPosition(initPos);
	glow->addAnimator( smgr->createDeleteAnimator(timeMs) );
	glow->addAnimator( smgr->createTextureAnimator(feedbackTex, 80) );
	
	ISceneNode *light = smgr->addLightSceneNode(ball, vector3df(0,0,0), SColorf(1, 1, 1), 100);
	light->addAnimator( smgr->createDeleteAnimator( timeMs ) );
	
	#define SUBDIV 7
	#define BALLR 35
	CBoltSceneNode* beam;
	for(int i=0; i<SUBDIV; i++) {
		beam = new CBoltSceneNode(ball, smgr, -1, laserTex1, EMT_TRANSPARENT_ADD_COLOR); 
		// start, end, updateTime, height, parts, bolts, steddyend = true, thick=5.0f , color
		beam->setLine( vector3df(0,0,0),
					   //vector3df(cos((2*M_PI/SUBDIV)*i)*BALLR, sin((2*M_PI/SUBDIV)*i)*BALLR, 0), //horizontal
					   vector3df(cos((2*M_PI/SUBDIV)*i)*BALLR, 0, sin((2*M_PI/SUBDIV)*i)*BALLR),  //vertical
					   70, 7, 6, 2, false, 3, SColor(255,0,0,255)); 
					   
		beam->addAnimator( smgr->createDeleteAnimator(timeMs) );
	}
}
void EffectFactory::effectSun( int target, int timeMs )
{
	vector3df pos = players[target]->track->getCentroid() + vector3df(0,0,-20);
	
	ISceneNode* ball = smgr->addSphereSceneNode(1, 32, smgr->getRootSceneNode(), -1, pos, vector3df(90,0,0));
	ball->getMaterial(0).EmissiveColor = SColor(0,255,255,80);
	
	IParticleSystemSceneNode* ps;
	IParticleEmitter* em;
	for(int i=0; i<3; i++) {
		// add particle system
		ps = smgr->addParticleSystemSceneNode(false, ball);

		//  and set emitter
		em = ps->createSphereEmitter(
				vector3df(0,0,0), 0.8,
				vector3df(0.001,0.001,0.001),
				1500/3,2500/3,
				SColor(0,255,255,255), SColor(0,255,255,255),
				100,2000,
				360,
				dimension2d<f32>(0.0f, 0.0f),
				dimension2d<f32>(8.0f, 8.0f) );
		ps->setEmitter(em);
		em->drop();

		// adjust some material settings
		ps->setMaterialFlag(EMF_LIGHTING, false);
		ps->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
		ps->setMaterialTexture(0, fireballTex);
		ps->setMaterialType(EMT_TRANSPARENT_VERTEX_ALPHA);
		
		new CDeleteParticleAffector(ps, timeMs/3 * (i+1));
	}
	
	//  a lens flare node
	CLensFlareSceneNode* lensFlare = new CLensFlareSceneNode(ball, smgr, -1);
	lensFlare->getMaterial(0).setTexture(0, lensFlareTex);
	
	ISceneNode *light = smgr->addLightSceneNode(ball, vector3df(0,0,0), SColorf(0.8, 0.1, 0.1), 100);
	light->addAnimator( smgr->createDeleteAnimator( timeMs ) );
	
	// set animators
	ISceneNodeAnimator* appear = new CSceneNodeAnimatorScale(vector3df(0,0,0), vector3df(30,30,30), 1500, false, device->getTimer()->getTime() );
	ball->addAnimator( appear );
	ISceneNodeAnimator* disappear = new CSceneNodeAnimatorScale(vector3df(30,30,30), vector3df(0,0,0), timeMs, false, device->getTimer()->getTime()+1500 );
	ball->addAnimator( disappear );
	lensFlare->addAnimator( smgr->createFlyCircleAnimator( vector3df(0,0,0), 0.1) );
	lensFlare->addAnimator( smgr->createDeleteAnimator( timeMs ) );
	ball->addAnimator( smgr->createDeleteAnimator( timeMs+5000 ) );
}

void EffectFactory::effectBlackHoleBolt( int target )
{
	vector3df initPos = players[target]->track->getCentroid() + vector3df(0,0,-10);
	vector3df endPos = initPos + vector3df(rand()%120-60, rand()%120-60, rand()%120-60);
	
	CBoltSceneNode* beam = new CBoltSceneNode(smgr->getRootSceneNode(), smgr, -1, laserTex3, EMT_TRANSPARENT_ALPHA_CHANNEL); 
	// start, end, updateTime, height, parts, bolts, steddyend = true, thick=5.0f , color
	beam->setLine( initPos,
				   endPos,
				   100, 6, 7, 1, false, 6, SColor(0,0,0,255)); 
				   
	beam->addAnimator( smgr->createDeleteAnimator(200+rand()%3000) );
}

void EffectFactory::effectBlackHole( int target, int timeMs )
{
	vector3df pos = players[target]->track->getCentroid() + vector3df(0,0,-10);
	
	ISceneNode* ball = smgr->addSphereSceneNode(1, 64, smgr->getRootSceneNode(), -1, pos, vector3df(0,0,0) );
	ball->setMaterialFlag(EMF_LIGHTING, false);
	ball->setMaterialTexture(0, starTex);
	ball->getMaterial(0).getTextureMatrix(0).setTextureScale(4,4);
	
	// add particle system
	IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false, ball);
	//  and set emitter
	ps->setEmitter( ps->createPointEmitter(
			vector3df(0.01,0.01,0.01),
			1,30,
			SColor(0,255,255,255), SColor(0,255,255,255),
			1000,3000,
			360,
			dimension2d<f32>(10.0f, 10.0f),
			dimension2d<f32>(15.0f, 15.0f) ));
			
	ps->setMaterialFlag(EMF_LIGHTING, false);
	ps->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, glowTex3);
	ps->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
	
	//ball->addAnimator( smgr->createRotationAnimator(vector3df(0.2,0.3,0.5)) );
	
	ps->addAffector( ps->createScaleParticleAffector(dimension2df(-10,-10)) );
	//ps->addAffector( ps->createScaleParticleAffector(dimension2df(11,11)) );
	//ps->addAffector( ps->createAttractionAffector( pos, 70 ) );
	new CDeleteParticleAffector(ps, timeMs);
	
	// set animators
	ISceneNodeAnimator* appear = new CSceneNodeAnimatorScale(vector3df(0,0,0), vector3df(10,10,10), timeMs-1000, false, device->getTimer()->getTime() );
	ball->addAnimator( appear );
	//ISceneNodeAnimator* disappear = new CSceneNodeAnimatorScale(vector3df(25,25,25), vector3df(0,0,0), 1000, false, device->getTimer()->getTime()+timeMs );
	//ball->addAnimator( disappear );
	ball->addAnimator( smgr->createDeleteAnimator( timeMs+5000 ) );
}

void EffectFactory::effectDarkStormBolt( int target )
{
	vector3df endPos = players[target]->track->getRandomPos() + vector3df(0,0,50);
	vector3df initPos = endPos + vector3df(0,0,-100);
	
	CBoltSceneNode* beam = new CBoltSceneNode(smgr->getRootSceneNode(), smgr, -1, laserTex3, EMT_TRANSPARENT_ALPHA_CHANNEL); 
	// start, end, updateTime, height, parts, bolts, steddyend = true, thick=5.0f , color
	beam->setLine( initPos,
				   endPos,
				   100, 6, 7, 2, false, 6, SColor(0,0,0,255)); 
				   
	beam->addAnimator( smgr->createDeleteAnimator(1000) );
}

void EffectFactory::effectVampiric( int target, int timeMs )
{
	vector3df pos = players[target]->track->getCentroid();
	vector3df pos2 = players[!target]->track->getCentroid()+vector3df(0,0,-10);
	
	CBoltSceneNode* beam = new CBoltSceneNode(smgr->getRootSceneNode(), smgr, -1, laserTex2, EMT_TRANSPARENT_ALPHA_CHANNEL); 
	// start, end, updateTime, height, parts, bolts, steddyend = true, thick=5.0f , color
	beam->setLine( pos,
				   pos2,
				   150, 4, 6, 3, true, 10, SColor(0,0,0,255)); 
	beam->addAnimator( smgr->createDeleteAnimator(timeMs) );

	// add particle system
	IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false);
	ps->setPosition( pos );	
	
	//  and set emitter
	ps->setEmitter( ps->createBoxEmitter(
			core::aabbox3d<f32>(-TRACK_SIZE_X/2,-TRACK_SIZE_Y/2,0,TRACK_SIZE_X/2,TRACK_SIZE_Y/2,1), //minx, miny, minz, maxx, maxy, maxz
			core::vector3df(0.02f,0.02f,0.02f),
			100,300,
			SColor(0,255,255,255), SColor(0,255,255,255),
			2000,timeMs,
			360,
			dimension2d<f32>(1.0f, 1.0f),
			dimension2d<f32>(3.0f, 3.0f)
			));
	ps->addAffector( ps->createAttractionAffector( pos2, 70 ) );

	// adjust some material settings
	ps->setMaterialFlag(EMF_LIGHTING, false);
	ps->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, glowTex);
	ps->setMaterialType(EMT_TRANSPARENT_VERTEX_ALPHA);
	
	ps->addAffector( ps->createScaleParticleAffector(dimension2df(20,20)) );
	
	new CDeleteParticleAffector(ps, timeMs);
	ps->addAnimator( smgr->createDeleteAnimator(timeMs+5000) );
}

void EffectFactory::effectAnotherDimension( int target, int timeMs )
{
	ISceneNode *sky = smgr->addSkyDomeSceneNode( gridTex, 2, 2, 0.9, 2, 10 );
	sky->setRotation( vector3df(90,0,0) );
	sky->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	sky->getMaterial(0).getTextureMatrix(0).setTextureScale(10,10);
	
	sky->addAnimator( smgr->createRotationAnimator( vector3df(0,0,0.1) ) );
	sky->addAnimator( smgr->createDeleteAnimator(timeMs) );
}

void EffectFactory::effectLotus( int target, int timeMs )
{
	vector3df pos = players[target]->track->getCentroid();

	IBillboardSceneNode* ball = smgr->addBillboardSceneNode(smgr->getRootSceneNode(), core::dimension2d<f32>(30, 30));
	ball->setMaterialFlag(EMF_LIGHTING, false);
	ball->setMaterialTexture(0, glowTex2);
	ball->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	ball->setPosition(pos);
	ball->addAnimator( smgr->createDeleteAnimator(timeMs) );
	
	// add particle system
	IParticleSystemSceneNode* ps;
	IParticleEmitter* em;
	for(int i=0; i<NLOTUSTEX; i++) {
		ps = smgr->addParticleSystemSceneNode(false,0,-1,pos);
		
		em = ps->createPointEmitter(
				vector3df(0,0,0.01*(i+1)),
				10,300,
				SColor(0,10,10,10), SColor(0,255,255,255),
				100,500,
				100,
				dimension2d<f32>(1, 1),
				dimension2d<f32>(5, 5));
		ps->setEmitter(em);
		em->drop();

		// adjust some material settings
		ps->setMaterialFlag(EMF_LIGHTING, false);
		ps->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
		ps->setMaterialTexture(0, lotusTex[i]);
		ps->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
		
		new CDeleteParticleAffector(ps, timeMs);
		ps->addAnimator( smgr->createDeleteAnimator(timeMs+1000) );
	}
}
