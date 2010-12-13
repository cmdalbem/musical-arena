#include "EffectFactory.h"

using namespace irr::core;
using namespace irr::video;

EffectFactory::EffectFactory( IrrlichtDevice *_device, IVideoDriver *_driver, ISceneManager *_smgr, vector<Player*> _players )
{
	device = _device;
	driver = _driver;
	smgr = _smgr;
	players = _players;
	
	iceStone = smgr->getMesh("stone.3ds");
	
	ITexture *shieldTex0 = driver->getTexture("img/blanktex.png");
	ITexture *shieldTex1 = driver->getTexture("img/gradient.png");
	CBloodShader::instance().createMaterial(device);
	this->fireballTex = driver->getTexture("img/fireball.bmp");
	this->glowTex = driver->getTexture("img/glow2.bmp");
	this->blackTex = driver->getTexture("img/black.png");
	this->iceTex = driver->getTexture("img/icetex.png");
	this->laserTex1 = driver->getTexture("img/laser1.png");
	this->laserTex2 = driver->getTexture("img/laser2.png");
	this->laserTex3 = driver->getTexture("img/laser3.png");
	//this->electricTex = driver->getTexture("img/electrictex.png");
	this->lensFlareTex = driver->getTexture("img/flares.jpg");
	//this->smokeTex1 = driver->getTexture("img/smoke1.jpg");
	//this->smokeTex2 = driver->getTexture("img/smoke2.jpg");
	char c[50];
	for(int i=0; i<5; i++) {
		sprintf(c,"img/water%i.png", i+1);
		waterTex[i] = driver->getTexture( c);
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
	#define NICETEX 30
	for(int i=0; i<NICETEX; i++) {
		sprintf(c,"img/anim/ice_001_%i.png", i+1);
		ice1.push_back( driver->getTexture(c) );
	}
	cout << ice1[0]->getColorFormat() << endl;
	
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
				effectFireball(target,false);
				break;			
			case EFFECT_FIRE_RAIN:
				for(int i=0; i<50; i++)
					queueEffect( i*150, EFFECT_FIREBALL_SKY, target );
				break;
			case EFFECT_FIREBALL_SKY:
				effectFireRain(target);
				break;
			case EFFECT_GLOW_AREA:
				areaEffect(target,glowTex, 700);
				break;
			case EFFECT_FEEDBACK:
				effectFeedback(target);
				break;
			case EFFECT_SHIELD:
				for(int i=0; i<25; i++)
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
				for(int i=0; i<50; i++)
					queueEffect( rand()%3000, EFFECT_THUNDERSTORM_BOLT, target );
				break;
			case EFFECT_THUNDERSTORM_BOLT:
				effectThunderstormBolt(target);
				break;
			case EFFECT_WATER_BEAM:
				effectWaterBeam(target);
				break;	
			case EFFECT_DRUNK_EFFECT:
				for(int i=0; i<10; i++)
					queueEffect( i*500, EFFECT_DRUNK_EFFECT_SINGLE, target );
				break;
			case EFFECT_DRUNK_EFFECT_SINGLE:
				effectDrunkEffect(target,5);
				break;
			case EFFECT_EXPLOSION:
				effectExplosion(players[target]->track->getCentroid());
				break;
			case EFFECT_ELETRIC_GROUND:
				queueEffect( 0, EFFECT_DRUNK_EFFECT, target );
				effectEletrifiedGround(target, 6000);
				break;
			case EFFECT_FLOOD:
				effectFloodEffect(target);
				break;
			case EFFECT_SWAMP:
				effectSwampEffect(target,8000);
				break;
			case EFFECT_BALL_LIGHTNING:
				effectBallLightningEffect(target,8000);
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
                                SColor(0, 0, 0, 0),      		// tail color
                                players[i]->track->getCentroid(),
                                vector3df(-90,0,0),
                                vector3df(70,40,70));
	spellEffect->addAnimator( smgr->createTextureAnimator(feedbackTex, 120) );
	spellEffect->addAnimator( smgr->createDeleteAnimator(NFEEDBACKTEX*120*1.7) );
}

void EffectFactory::effectIce( int target, int timeMs )
{
	vector3df pos = players[target]->track->getCentroid() + vector3df(0,-TRACK_SIZE_Y/2,0);
	
	#define iceAnimTime 45

	ISceneNode * bill = smgr->addBillboardSceneNode(smgr->getRootSceneNode(),
                                core::dimension2d<f32>(70, 70),
                                pos);
	bill->setMaterialFlag(video::EMF_LIGHTING, false);
	bill->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	bill->addAnimator( smgr->createTextureAnimator(ice1, iceAnimTime, false) );
	bill->addAnimator( smgr->createDeleteAnimator(NICETEX*iceAnimTime) );
	
	/*IVolumeLightSceneNode * spellEffect = smgr->addVolumeLightSceneNode(0, -1,
                                64,                              // Subdivisions on U axis
                                64,                              // Subdivisions on V axis
                                SColor(100, 100, 100, 130), 		// foot color
                                SColor(0, 0, 0, 0),      		// tail color
                                players[i]->track->getCentroid(),
                                vector3df(-90,0,0),
                                vector3df(40,20,40));
	spellEffect->addAnimator( smgr->createTextureAnimator(ice1, 60) );
	spellEffect->addAnimator( smgr->createDeleteAnimator(NICETEX*60*1.7) );
	spellEffect->setMaterialType(EMT_TRANSPARENT_ADD_COLOR );*/	
	
	IMeshSceneNode* stone = smgr->addMeshSceneNode(iceStone,
										0,-1,
										pos,
										vector3df(-90,0,0)
										);
	stone->setMaterialTexture( 0, iceTex );
	stone->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
	stone->setScale( vector3df(100,100,100) );
	
	/*ISceneNodeAnimator* appear = new CSceneNodeAnimatorScale(vector3df(0,0,0), vector3df(30,30,30), 2000, false, device->getTimer()->getTime() );
	stone->addAnimator( appear );
	ISceneNodeAnimator* disappear = new CSceneNodeAnimatorScale(vector3df(30,30,30), vector3df(0,0,0), 2000, false, device->getTimer()->getTime()+timeMs-4000 );
	stone->addAnimator( disappear );*/
}

void EffectFactory::effectExplosion( vector3df pos )
{
	#define expTime 45

	ISceneNode * spellEffect = smgr->addBillboardSceneNode(smgr->getRootSceneNode(),
                                core::dimension2d<f32>(70, 70),
                                pos);
	spellEffect->setMaterialFlag(video::EMF_LIGHTING, false);
	spellEffect->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	spellEffect->addAnimator( smgr->createTextureAnimator(explosion, expTime, false) );
	spellEffect->addAnimator( smgr->createDeleteAnimator(NEXPLOSIONTEX*expTime) );
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
	
	effectFireball(path);
}

void EffectFactory::effectFireball( int attacked, bool randomizeTarget )
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
	
	effectFireball(path);	
}

void EffectFactory::effectFireball( array<vector3df> path )
{
	float speed = 2;
	
	scene::ISceneNode* ball = smgr->addBillboardSceneNode(smgr->getRootSceneNode(), core::dimension2d<f32>(10, 10));
	ball->setMaterialFlag(video::EMF_LIGHTING, false);
	ball->setMaterialTexture(0, fireballTex);
	ball->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	
	// add particle system
	scene::IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false, ball);

	//  and set emitter
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
	
	//  a lens flare node
	CLensFlareSceneNode* lensFlare = new CLensFlareSceneNode(ball, smgr, -1);
	lensFlare->getMaterial(0).setTexture(0, lensFlareTex);

	ps->addAffector( ps->createFadeOutParticleAffector() );

	// adjust some material settings
	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, fireballTex);
	ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
	
	// set animators
	ball->addAnimator( smgr->createFollowSplineAnimator( device->getTimer()->getTime(),path,speed,1,false) );
	ball->addAnimator( smgr->createDeleteAnimator((2/speed)*1000 + 400) );
	lensFlare->addAnimator( smgr->createDeleteAnimator((2/speed)*1000) );	
}

void EffectFactory::areaEffect( int player, ITexture *tex, int timeMs )
{
	vector3df pos = players[player]->track->getCentroid();
	
	// add particle system
	scene::IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false);
	ps->setPosition( pos );	
	
	//  and set emitter
	scene::IParticleEmitter* em = ps->createBoxEmitter(
			core::aabbox3d<f32>(-TRACK_SIZE_X/2,-TRACK_SIZE_Y/2,0,TRACK_SIZE_X/2,TRACK_SIZE_Y/2,1), //minx, miny, minz, maxx, maxy, maxz
			core::vector3df(0.0f,0.015f,0.0f),
			180,200,
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

void EffectFactory::soloEffect( int player, ITexture *tex, int timeMs )
{
	vector3df pos = players[player]->track->getCentroid();
	
	// add particle system
	scene::IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false);
	ps->setPosition( pos );	
	
	//  and set emitter
	scene::IParticleEmitter* em = ps->createBoxEmitter(
			core::aabbox3d<f32>(-TRACK_SIZE_X/2,-TRACK_SIZE_Y/2,0,TRACK_SIZE_X/2,TRACK_SIZE_Y/2,0.5), //minx, miny, minz, maxx, maxy, maxz
			core::vector3df(0.0f,0.015f,0.0f),
			80,80,
			video::SColor(0,100,100,255), video::SColor(0,200,100,255),
			400,2000);			
	em->setMinStartSize(core::dimension2d<f32>(4.0f, 4.0f));
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
	ps->addAnimator( smgr->createDeleteAnimator(timeMs+500) );
}

void EffectFactory::effectBolt( int target )
{
	vector3df initPos = players[!target]->track->getCentroid() + vector3df(0,0,-20);
	vector3df endPos = players[target]->track->getCentroid() * vector3df(2,1,1);
	
	IBillboardSceneNode* ball = smgr->addBillboardSceneNode(smgr->getRootSceneNode(), core::dimension2d<f32>(20, 20));
	ball->setMaterialFlag(video::EMF_LIGHTING, false);
	ball->setMaterialTexture(0, glowTex);
	ball->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
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
	vector3df endPos = players[target]->track->getRandomPos();
	vector3df initPos = endPos + vector3df(0,0,-20);
	
	IBillboardSceneNode* ball = smgr->addBillboardSceneNode(smgr->getRootSceneNode(), core::dimension2d<f32>(20, 20));
	ball->setMaterialFlag(video::EMF_LIGHTING, false);
	ball->setMaterialTexture(0, glowTex);
	ball->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	ball->setPosition(initPos);
	
	CBoltSceneNode* beam = new CBoltSceneNode(smgr->getRootSceneNode(), smgr, -1, laserTex1, EMT_TRANSPARENT_ADD_COLOR); 
	// start, end, updateTime = 300, height = 10, parts = 10, bolts = 1, steddyend = true, thick=5.0f , color
	beam->setLine( initPos,
				   endPos,
				   80, 10, 4, 2, false, 5, SColor(255,0,0,255)); 
				   
	beam->addAnimator( smgr->createDeleteAnimator(1000) );
	ball->addAnimator( smgr->createDeleteAnimator(1000) );
}

void EffectFactory::effectEletrifiedGround( int targetPlayer, int timeMs )
{
	TrackSceneNode* node = (TrackSceneNode*)players[targetPlayer]->track->node;
	
	for(int i=0; i<NFRETS; i++) {
		CBoltSceneNode* beam = new CBoltSceneNode(smgr->getRootSceneNode(), smgr, -1, laserTex1, EMT_TRANSPARENT_ADD_COLOR); 
		// start, end, updateTime = 300, height = 10, parts = 10, bolts = 1, steddyend = true, thick=5.0f , color
		beam->setLine( vector3df(node->getStoneXPos(i), node->getPosition().Y, node->getPosition().Z-3.5),
					   vector3df(node->getStoneXPos(i), node->getPosition().Y-TRACK_SIZE_Y-5, node->getPosition().Z-3.5),
					   40, 2, 17, 1, false, 1, SColor(255,0,0,255)); 
		beam->addAnimator( smgr->createDeleteAnimator(timeMs) );
	}
}

void EffectFactory::effectWaterBeam( int targetPlayer )
{
	vector3df initPos = players[!targetPlayer]->track->getCentroid()  + vector3df(0,0,-20);
	vector3df direction(targetPlayer*2-1,0,1);
	
	new CBloodEffect(device->getSceneManager(), waterTex, EGL_INSANE, initPos, direction, 3000);
}

void EffectFactory::effectFloodEffect( int player )
{
	vector3df pos = players[player]->track->getCentroid();
	
	//  and set emitter
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
		
		#define FLOOD_T 10000
		new CDeleteParticleAffector(ps, FLOOD_T);
		ps->addAnimator( smgr->createDeleteAnimator(FLOOD_T*2) );
	}
}

void EffectFactory::effectSwampEffect( int player, int timeMs )
{
	vector3df pos = players[player]->track->getCentroid();
	
	//  and set emitter
	for(int i=0; i<5; i++)
	{
		// add particle system
		scene::IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false,0,-1,pos);
		
		scene::IParticleEmitter* em = ps->createBoxEmitter(
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
							players[targetPlayer]->track->getCentroid()+vector3df(targetPlayer==0? 16.5:-16.5,-48.5,-10), //source
							vector3df(0, -0.0f, -3), //direction
							35); //duration
	blood->setVisible(false);
}

void EffectFactory::effectDrunkEffect ( int targetPlayer, int times )
{
	int k;
	for(int i=0; i<NFRETS; i++) {
		vector<Stone*> stones = players[targetPlayer]->track->stones[i];
		
		for(int xtimes=0;xtimes<times; xtimes++)
			if(stones.size()) {
				k = rand()%stones.size();
				stones[k]->displace = vector3df(rand()%4-2,0,0);
				effectParticlesExplosion( stones[k]->getPosition()+stones[k]->displace,glowTex );
			}
	}
}

void EffectFactory::effectParticlesExplosion( vector3df pos, ITexture *tex )
{
	// add particle system
	scene::IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false,0,-1,pos);
	
	//  and set emitter
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

	//ps->addAffector( ps->FadeOutParticleAffector() );
	ps->addAffector( ps->createGravityAffector(vector3df(0, -0.25, 0)) );

	// adjust some material settings
	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, tex);
	ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
	
	new CDeleteParticleAffector(ps, 100);
	ps->addAnimator( smgr->createDeleteAnimator(1000) );	
}

void EffectFactory::effectBallLightningEffect( int target, int timeMs )
{
	vector3df initPos = players[target]->track->getCentroid() + vector3df(0,0,-15);
	
	ISceneNode* ball = smgr->addSphereSceneNode(1.75);
	ball->setPosition(initPos);
	ball->addAnimator( smgr->createDeleteAnimator(timeMs) );

	scene::IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false,ball);
	
	//  and set emitter
	scene::IParticleEmitter* em = ps->createPointEmitter(
			core::vector3df(0.0f,0.0f,0.1f),
			200,400, // particles/second
			video::SColor(0,255,255,255), video::SColor(0,255,255,255),
			300,300, // lifetime
			360,
			dimension2d<f32>(1, 1),
			dimension2d<f32>(2, 2)
			);
	ps->setEmitter(em);
	em->drop();
	// adjust some material settings
	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, glowTex);
	ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
	new CDeleteParticleAffector(ps, timeMs);
	ps->addAnimator( smgr->createDeleteAnimator(timeMs) );	
	
	/*IBillboardSceneNode* glow = smgr->addBillboardSceneNode(smgr->getRootSceneNode(), core::dimension2d<f32>(40, 40));
	glow->setMaterialFlag(video::EMF_LIGHTING, false);
	glow->setMaterialTexture(0, feedbackTex[5]);
	glow->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	glow->setPosition(initPos);
	glow->addAnimator( smgr->createDeleteAnimator(timeMs) );
	glow->addAnimator( smgr->createTextureAnimator(feedbackTex, 80) );*/
	
	#define SUBDIV 7
	#define BALLR 35
	for(int i=0; i<SUBDIV; i++) {
		CBoltSceneNode* beam = new CBoltSceneNode(ball, smgr, -1, laserTex1, EMT_TRANSPARENT_ADD_COLOR); 
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
	
	// add particle system
	IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false, ball);

	//  and set emitter
	IParticleEmitter* em = ps->createSphereEmitter(
			vector3df(0,0,0), 0.8,
			vector3df(0.001,0.001,0.001),
			1500,2500,
			video::SColor(0,255,255,255), video::SColor(0,255,255,255),
			100,2000,
			360,
			dimension2d<f32>(0.0f, 0.0f),
			dimension2d<f32>(8.0f, 8.0f) );
	ps->setEmitter(em);
	em->drop();

	//  a lens flare node
	CLensFlareSceneNode* lensFlare = new CLensFlareSceneNode(ball, smgr, -1);
	lensFlare->getMaterial(0).setTexture(0, lensFlareTex);

	// adjust some material settings
	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, fireballTex);
	ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
	
	// set animators
	ISceneNodeAnimator* appear = new CSceneNodeAnimatorScale(vector3df(0,0,0), vector3df(30,30,30), 1000, false, device->getTimer()->getTime() );
	ball->addAnimator( appear );
	ISceneNodeAnimator* disappear = new CSceneNodeAnimatorScale(vector3df(30,30,30), vector3df(0,0,0), timeMs, false, device->getTimer()->getTime()+1000 );
	ball->addAnimator( disappear );
	new CDeleteParticleAffector(ps, timeMs);
	lensFlare->addAnimator( smgr->createFlyCircleAnimator( vector3df(0,0,0), 0.1) );
	lensFlare->addAnimator( smgr->createDeleteAnimator( timeMs ) );
	ball->addAnimator( smgr->createDeleteAnimator( timeMs+5000 ) );
}

void EffectFactory::effectBlackHole( int target, int timeMs )
{
	vector3df pos = players[target]->track->getCentroid() + vector3df(0,0,-10);
	
	ISceneNode* ball = smgr->addSphereSceneNode(1, 64, smgr->getRootSceneNode(), -1, pos, vector3df(0,0,0) );
	ball->setMaterialFlag(video::EMF_LIGHTING, false);
	ball->setMaterialTexture(0, blackTex);
	
	/*// add particle system
	IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false, ball);
	//  and set emitter
	IParticleEmitter* em = ps->SphereEmitter(
			vector3df(0,0,0), 0.8,
			vector3df(0.001,0.001,0.001),
			1500,2500,
			video::SColor(0,255,255,255), video::SColor(0,255,255,255),
			100,2000,
			360,
			dimension2d<f32>(0.0f, 0.0f),
			dimension2d<f32>(8.0f, 8.0f) );
	ps->setEmitter(em);
	em->drop();
	// adjust some material settings
	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, fireballTex);
	ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
	new CDeleteParticleAffector(ps, timeMs);*/
	
	// set animators
	ISceneNodeAnimator* appear = new CSceneNodeAnimatorScale(vector3df(0,0,0), vector3df(25,0,25), timeMs-1000, false, device->getTimer()->getTime() );
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

void EffectFactory::effectVampireAttack( int target, int timeMs )
{
	vector3df pos = players[target]->track->getCentroid();
	
	// add particle system
	scene::IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false);
	ps->setPosition( pos );	
	
	//  and set emitter
	scene::IParticleEmitter* em = ps->createBoxEmitter(
			core::aabbox3d<f32>(-TRACK_SIZE_X/2,-TRACK_SIZE_Y/2,0,TRACK_SIZE_X/2,TRACK_SIZE_Y/2,1), //minx, miny, minz, maxx, maxy, maxz
			core::vector3df(0.0f,0.0f,0.0f),
			500,700,
			video::SColor(0,255,255,255), video::SColor(0,255,255,255),
			700,700);
	em->setMinStartSize(core::dimension2d<f32>(2.0f, 2.0f));
	em->setMaxStartSize(core::dimension2d<f32>(4.0f, 4.0f));
	ps->setEmitter(em);
	em->drop();

	ps->addAffector( ps->createGravityAffector(vector3df(0, 0, -0.1)) );

	// adjust some material settings
	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, glowTex);
	ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
	
	new CDeleteParticleAffector(ps, timeMs);
	ps->addAnimator( smgr->createDeleteAnimator(timeMs+3000) );
}
