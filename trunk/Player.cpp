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
	usingSkill = false;
	gettimeofday (&lastTimeUpdatedStatus, NULL);
	magicBarrier = false;
	mirror = false;
	
	counterPoison = 0;
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
	
	fretting = new Fretting(&instrument->skills);
	
	castedSpell = NULL;
	
	track->tolerance = instrument->tolerance;
	fretting->tolerance = instrument->tolerance;
}

void Player::update()
{
	SEvent *anEvent;
	double elapsedTime = time_diff (lastTimeUpdatedStatus);
	
	track->update();
	
	if (elapsedTime >= ( 1.0 / UPDATE_STATUS_TIME ))
	{
		//cout << "elapsedTime: " << elapsedTime << endl;
		updateStatus();
		gettimeofday (&lastTimeUpdatedStatus, NULL);
	}
	
	if ( track->nonPressedChord && (!activateAI) ) // the player left behind a chord he should have played
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
			//for (int i=0; i<fretting->receiver->getEventsSize(); i++)
				//fretting->verifyEvents(fretting->receiver->getEvent(i), firstStones);
			if (activateAI)
				gotAnEvent = fretting->verifyEventsAI( anEvent, firstStones );
			else
			{
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
				//cout << "vai verificar eventos" << endl;
				//cout << "terminou de verificar eventos" << endl;
			}
		}
	}
}

void Player::takeDamage( double damage )
{
	damageTaken = damage;
	
	HP = HP - damage;
	if (HP < 0)
		HP = 0;
}

void Player::HPRecover (double howMuch)
{
	HP = HP + howMuch;
	if (HP > maxHP)
		HP = maxHP;
}

void Player::staminaDecrease(int howMuch)
{
	stamina = stamina - howMuch;
	if (stamina < 0)
		stamina = 0;
}

void Player::staminaRecover()
{
	stamina++;
	if (stamina > maxStamina)
		stamina = maxStamina;
}

void Player::updateStatus()
{
	switch (status)
	{
		case ST_POISON:
			counterPoison = (counterPoison + 1) % 5;
			if (counterPoison == 0)
				takeDamage ( 25 );
			break;
		case ST_FIRE:
			takeDamage (4);
			break;
		case ST_BARRIER:
			// increase armor
			break;
		case ST_MAGIC_BARRIER:
			magicBarrier = true;
			break;
		case ST_MIRROR:
			mirror = true;
			break;
		case ST_ELETRIFIED:
			takeDamage (4);
			break;
		case ST_DROWNED:
			staminaDecrease (3);
			if (stamina == 0)
				takeDamage (10);
			break;
		case ST_DEFENSE_DOWN:
			// decrease armor
			break;
		default:
			break;
	}
	
	if (usingSkill)
		staminaDecrease(2.5);
	else
		staminaRecover();
		
	timeInStatus = timeInStatus - (1.0 / UPDATE_STATUS_TIME);
	if (timeInStatus <= 0)
	{
		setStatusNormal();
	}
}

void Player::setStatusNormal()
{
	status = ST_NORMAL;
	armor = instrument->armor;
	fretting->tolerance = instrument->tolerance;
}

