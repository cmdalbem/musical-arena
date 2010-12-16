// Player.cpp
//

#include "Player.h"

#include <ctime>

/////////////////////////////////////////////////////////////////// CONSTRUCTORS
Player::Player()
{
	this->initialize();
}

//////////////////////////////////////////////////////////////////// DESTRUCTORS
Player::~Player()
{	}

////////////////////////////////////////////////////////////// GETTERS & SETTERS


////////////////////////////////////////////////////////////////// OTHER METHODS
void Player::initialize()
{
	srand((unsigned)time(0));
	
	damageTaken = 0;
	isUsingSkill = false;
	gettimeofday (&lastTimeUpdatedStatus, NULL);
	hasMagicBarrier = false;
	hasMirror = false; 
	isChaotic = false;
	isFrozen = false;
	
	chaoticCounter = 0;
	poisonCounter = 0;
	timeInStatus = 0;
	XP = 0;
	gold = 0;
	gotAnEvent = 0;
	
	multiplier = 1;
	level = 1;
	
	status = ST_NORMAL;
}

void Player::initializeAtributes()
{
	maxHP = instrument->sumHP;
	HP = maxHP;
	maxStamina = instrument->sumStamina;
	stamina = maxStamina;///2;
	armor = instrument->armor;
	castedSpell = NULL;	
	
	fretting = new Fretting(&instrument->skills);
	track->fretting = fretting;	
	
	track->tolerance = instrument->tolerance;
	fretting->tolerance = instrument->tolerance;
}

void Player::update()
{
	SEvent *anEvent=NULL;
	double elapsedTime = time_diff (lastTimeUpdatedStatus);
	
	track->update();
	
	// This part is executed only 4 times per second
	if (elapsedTime >= ( 1.0 / UPDATE_STATUS_TIME ))
	{
		//cout << "elapsedTime: " << elapsedTime << endl;
		updateStatus();
		
		if( isUsingSkill )
			changeStamina(-STAMINA_LOST_ON_SOLO);
		
		if( fretting->frettingState==1 ) {
			if(status==ST_CURSED)
				takeDamage(DAMAGE_PER_NOTE_MISSED);
			else
				changeStamina(STAMINA_GAINED_BY_NOTE);
		}
		
		gettimeofday (&lastTimeUpdatedStatus, NULL);
	}
	
	// Handle when the player left behind notes he should have played
	if ( track->nonPressedChord )
	{
		fretting->lostNote();
		takeDamage(track->notesOnChord * DAMAGE_PER_NOTE_MISSED); //multiplier of doom!
		//cout << "the player left " << track->notesOnChord << " behind" << endl;
	}
	else
	{
		// s an vector with only the interesting stones on the screen
		Stone* firstStones[NFRETS];
		gotAnEvent = 1;
		for(unsigned int i=0; i<NFRETS; i++)
			if(track->stones[i].size() > 0)
				firstStones[i] = track->stones[i].front();
			else
				firstStones[i] = NULL;
		if (fretting->receiver->enabled)
		{
			if (activateAI && !isFrozen)
				gotAnEvent = fretting->verifyEventsAI(firstStones);
			/*else if(anEvent = fretting->receiver->getEvent())
				{
					fretting->verifyEvents(anEvent, firstStones, &isUsingSkill);
				for (int i=0; i<fretting->receiver->getEventsSize(); i++)
					fretting->verifyEvents(fretting->receiver->getEvent(i), firstStones, &isUsingSkill);
				while((anEvent = (fretting->receiver->getEvent())) && (gotAnEvent != 0))
				{
					//sem_wait(fretting->receiver->semaphore);
					if (!activateAI)
						gotAnEvent = fretting->verifyEvents( anEvent, firstStones, &usingSkill);
					//castedSpell = fretting->castedSpell;
				
					if (gotAnEvent != 0)
					{
						// removes the first event of the events vector (so we can deal with the others =D)
						fretting->receiver->removeEvent();
					
						// check if the player must lose some HP or earn some XP
						int state = fretting->getFrettingState();
						if (gotAnEvent != SKILLBUTTON_INDEX)
						{
							if (state == -1)
								takeDamage (1 * IS_STATUS_FIRE);	// IS_STATUS_FIRE is defined in utils.h
							else if (state > 0)
								XP += state;
						}
					}
				}
			}*/
				//cout << "vai verificar eventos" << endl;
				//cout << "terminou de verificar eventos" << endl;
		}
	}
}
void Player::updateEvents()
{
	SEvent *anEvent=NULL;
	Stone* firstStones[NFRETS];
	gotAnEvent = 1;
	if ( (!isFrozen) && (fretting->receiver->enabled) && (!activateAI) 
		&& (anEvent = fretting->receiver->getEvent()) )
	{
		for(unsigned int i=0; i<NFRETS; i++)
			if(track->stones[i].size() > 0)
				firstStones[i] = track->stones[i].front();
			else
				firstStones[i] = NULL;
		fretting->verifyEvents(anEvent, firstStones, &isUsingSkill);
	}
}
double Player::takeDamage( double damage )
{
	if( status==ST_BLESSED ) {
		recoverHP(damage);
		return 0;
	}
	else {
		damage -= armor;
		
		if( damage>0 ) {
			damageTaken = damage; //used in Screen
			HP = HP - damage;
			if (HP < 0)
				HP = 0;
		}
		
		return damage;
	}
}

void Player::recoverHP( double howMuch )
{
	HP = HP + howMuch;
	if (HP > maxHP)
		HP = maxHP;
}

void Player::changeStamina( int howMuch )
{
	stamina = stamina + howMuch;
	if (stamina < 0)
		stamina = 0;
	else if(stamina>maxStamina)
			stamina = maxStamina;
}

int Player::getArmor()
{
	return armor;
}

void Player::changeArmor( double howMuch )
{
	instrument->armor += howMuch;
	armor += howMuch;
	
	if( instrument->armor < 0 )
		instrument->armor = 0;
	if( armor < 0 )
		armor = 0;
}

void Player::updateStatus()
/* Handle all players status cases */
{
	switch (status)
	{
		case ST_NORMAL:
			break;
		case ST_BROKEN_DEFENSE:
			armor = 0;
			break;
		case ST_POISON:
			poisonCounter = (poisonCounter + 1) % 5;
			if (poisonCounter == 0)
				takeDamage( this->HP*0.10 ); //percentage of the HP
			break;
		case ST_FIRE:
			takeDamage(8);
			break;
		case ST_INVENCIBLE:
			armor = 9999;
			break;
		case ST_MAGIC_BARRIER:
			hasMagicBarrier = true;
			break;
		case ST_MIRROR:
			hasMirror = true;
			break;
		case ST_ELETRIFIED: {
			int k, times=2;
			for(int i=0; i<NFRETS; i++) {					
				for(int xtimes=0;xtimes<times; xtimes++)
					if(track->stones[i].size()) {
						k = rand()%track->stones[i].size();
						track->stones[i][k]->displace = vector3df(rand()%4-2,0,0);
					}
			}
			takeDamage(13);
			break;
		}
		case ST_DROWNED:
			changeStamina(-7);
			if (stamina == 0)
				takeDamage (17);
			break;
		case ST_DEFENSE_DOWN:
			// decrease armor
			break;
		case ST_CHAOTIC_SPEED:
			if(!isChaotic) {
				lastSpeed = track->getSpeed();
				isChaotic = true;
			}
			else {
				chaoticCounter = (chaoticCounter + 1) % 8;
				if (chaoticCounter == 0) {
					track->setSpeed( rand()%50 + 1 );
				}
			}
			break;
		case ST_BLESSED:
			//look in Player::takeDamage()
			break;
		case ST_FROZEN:
			isUsingSkill = false;
			fretting->setAllNotPressed();
			isFrozen = true;
			break;
		case ST_CURSED:
			//look in Player::update()
			break;
		
	}
		
	// handle finish time of a status
	timeInStatus = timeInStatus - (1.0 / UPDATE_STATUS_TIME);
	if (timeInStatus <= 0)
		setStatusNormal();
}

void Player::setStatusNormal()
{
	status = ST_NORMAL;
	
	//resets atributes that may have been modified by the status
	fretting->tolerance = instrument->tolerance;
	armor = instrument->armor;

	if(hasMagicBarrier) {
		hasMagicBarrier = false;
	}
	if(hasMirror) {
		hasMirror = false;
	}
	if(isChaotic) {
		isChaotic = false;
		track->setSpeed(lastSpeed);
	}
	if(isFrozen) {
		isFrozen = false;
	}
}

