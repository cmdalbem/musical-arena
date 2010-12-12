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
	isTimePassenger = false;
	
	isChaotic = 0;
	chaoticCounter = 0;
	poisonCounter = 0;
	timeInStatus = 0;
	XP = 0;
	level = 1;
	gold = 0;
	gotAnEvent = 0;
	
	status = ST_NORMAL;
}

void Player::initializeAtributes()
{
	maxHP = 300 + (rand() % 150) + instrument->sumHP;
	HP = maxHP;
	maxStamina = 200 + (rand() % 100) + instrument->sumStamina;
	stamina = maxStamina;
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
	
	// Apply Status effects
	if (elapsedTime >= ( 1.0 / UPDATE_STATUS_TIME ))
	{
		//cout << "elapsedTime: " << elapsedTime << endl;
		updateStatus();
		gettimeofday (&lastTimeUpdatedStatus, NULL);
	}
	
	// Handle when the player left behind notes he should have played
	if ( track->nonPressedChord && (!activateAI) )
	{
		fretting->lostNote();
		takeDamage (track->notesOnChord);
		//cout << "the player left " << track->notesOnChord << " behind" << endl;
	}
	else
	{
		// Creates an vector with only the interesting stones on the screen
		Stone* firstStones[NFRETS];
		gotAnEvent = 1;
		for(unsigned int i=0; i<NFRETS; i++)
			if(track->stones[i].size() > 0)
				firstStones[i] = track->stones[i].front();
			else
				firstStones[i] = NULL;
		if (fretting->receiver->enabled)
		{
			if (activateAI)
				gotAnEvent = fretting->verifyEventsAI( anEvent, firstStones );
			else
				{
				for (int i=0; i<fretting->receiver->getEventsSize(); i++)
					fretting->verifyEvents(fretting->receiver->getEvent(i), firstStones, &isUsingSkill);
				/*while((anEvent = (fretting->receiver->getEvent())) && (gotAnEvent != 0))
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
				}*/
				//cout << "vai verificar eventos" << endl;
				//cout << "terminou de verificar eventos" << endl;
			}
		}
	}
}

void Player::takeDamage( double damage )
{
	damageTaken = damage; //used in Screen
	
	HP = HP - damage;
	if (HP < 0)
		HP = 0;
}

void Player::recoverHP( double howMuch )
{
	HP = HP + howMuch;
	if (HP > maxHP)
		HP = maxHP;
}

void Player::decreaseStamina( int howMuch )
{
	stamina = stamina - howMuch;
	if (stamina < 0)
		stamina = 0;
}

void Player::decreaseArmor( double howMuch )
{
	instrument->armor -= howMuch;
	armor -= howMuch;
	
	if( instrument->armor < 0 )
		instrument->armor = 0;
	if( armor < 0 )
		armor = 0;
}

void Player::staminaRecover()
{
	stamina++;
	if (stamina > maxStamina)
		stamina = maxStamina;
}

void Player::updateStatus()
/* Handle all players status cases */
{
	switch (status)
	{
		case ST_NORMAL:
			break;
		case ST_TIME_PASSENGER:
			if(!isTimePassenger)
				track->musicPos = track->musicPos*2;
			else
				isTimePassenger = true;
			break;
		case ST_POISON:
			poisonCounter = (poisonCounter + 1) % 5;
			if (poisonCounter == 0)
				takeDamage( this->HP*0.10 ); //percentage of the HP
			break;
		case ST_FIRE:
			takeDamage (10);
			break;
		case ST_INVENCIBLE:
			armor = INT_MAX;
			break;
		case ST_MAGIC_BARRIER:
			hasMagicBarrier = true;
			break;
		case ST_MIRROR:
			hasMirror = true;
			break;
		case ST_ELETRIFIED:
			takeDamage(3);
			break;
		case ST_DROWNED:
			decreaseStamina(7);
			if (stamina == 0)
				takeDamage (7);
			break;
		case ST_DEFENSE_DOWN:
			// decrease armor
			break;
		case ST_CHAOTIC_SPEED:
			if(!isChaotic) {
				speedBeforeChaotic = track->getSpeed();
				isChaotic = true;
			}
			else {
				chaoticCounter = (chaoticCounter + 1) % 8;
				if (chaoticCounter == 0) {
					track->setSpeed( rand()%50 + 1 );
				}
			}
			break;
		
	}
	
	if( isUsingSkill )
		decreaseStamina(2.5);
	else
		staminaRecover();
		
	// handle finish time of a status
	timeInStatus = timeInStatus - (1.0 / UPDATE_STATUS_TIME);
	if (timeInStatus <= 0)
		setStatusNormal();
}

void Player::setStatusNormal()
{
	status = ST_NORMAL;
	
	//resets atributes that may have been modified by the status
	armor = instrument->armor;
	fretting->tolerance = instrument->tolerance;

	if(hasMagicBarrier) {
		hasMagicBarrier = false;
	}
	if(hasMirror) {
		hasMirror = false;
	}
	
	if(isChaotic) {
		isChaotic = false;
		track->setSpeed(speedBeforeChaotic);
	}
}

