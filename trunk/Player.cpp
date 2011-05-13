// Player.cpp
//

#include "Player.h"

#include <ctime>

/////////////////////////////////////////////////////////////////// CONSTRUCTORS
Player::Player( ITimer *_timer, double *_musicTime, EventReceiver *_receiver, Track *_track )
{
	timer = _timer;
	track = _track;
	
	fretting.musicTime = _musicTime;
	fretting.receiver = _receiver;
	
	instrument = NULL;
	
	this->reset();
}

//////////////////////////////////////////////////////////////////// DESTRUCTORS
Player::~Player()
{	}

////////////////////////////////////////////////////////////// GETTERS & SETTERS


////////////////////////////////////////////////////////////////// OTHER METHODS
void Player::reset()
{
	damageTaken = 0;
	isUsingSkill = false;
	lastTimeUpdatedStatus = timer->getTime()/1000.;
	hasMagicBarrier = false;
	hasMirror = false; 
	isChaotic = false;
	isFrozen = false;
	useAI = false;
	
	chaoticCounter = 0;
	poisonCounter = 0;
	XP = 0;
	gold = 0;
	stamina = 0;
	gotAnEvent = 0;
	multiplier = 1;
	streak = 0;
	
	level = 1;
	
	status.clear();
	
	track->initialize();
}

void Player::update()
{
	//SEvent *anEvent=NULL;
	double elapsedTime = timer->getTime()/1000. - lastTimeUpdatedStatus;
	
	if(track)
		track->update();
	
	// This part is executed only 4 times per second
	if (elapsedTime >= ( 1.0 / UPDATE_STATUS_TIME ))
	{
		//cout << "elapsedTime: " << elapsedTime << endl;
		updateStatus();
		
		if( isUsingSkill )
			changeStamina(-STAMINA_LOST_ON_SOLO);
		
		if( fretting.frettingState > 0 ) {
			bool cursed = false;
			for (unsigned int i = 0; i < status.size(); i++)
				if (status[i].status == ST_CURSED)
					cursed = true;
					
			if( cursed )
				takeDamage(DAMAGE_PER_NOTE_MISSED);
			else
				changeStamina(STAMINA_GAINED_BY_NOTE * fretting.frettingState * multiplier);
		}
		
		lastTimeUpdatedStatus = timer->getTime()/1000.;
	}
	
	// Handle when the player left behind notes he should have played
	if ( track->nonPressedChord )
	{
		fretting.lostNote();
		takeDamage(track->notesOnChord * DAMAGE_PER_NOTE_MISSED); //multiplier of doom!
		streak = 0;
		multiplier = 1;
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
		if (useAI && !isFrozen)
			gotAnEvent = fretting.verifyEventsAI(firstStones);
	}
}

void Player::updateEvents()
{
	SEvent *anEvent=NULL;
	Stone* firstStones[NFRETS];
	gotAnEvent = -3;
	int		lastFrettingState = fretting.frettingState;
	if ( !isFrozen && !useAI && (anEvent = fretting.receiver->getEvent()) )
	{
		for(unsigned int i=0; i<NFRETS; i++)
			if(track->stones[i].size() > 0)
				firstStones[i] = track->stones[i].front();
			else
				firstStones[i] = NULL;

		gotAnEvent = fretting.verifyEvents(anEvent, firstStones, &isUsingSkill);
		
		//cout << "frettingState: " << fretting.frettingState << "  lastFrettingState: " << lastFrettingState << endl;
		if ((fretting.frettingState == -1) && (lastFrettingState != -1))
		{
			streak = 0;
			multiplier = 1;
			//cout << "entrou aqui" << endl;
			takeDamage (DAMAGE_PER_NOTE_MISSED);
		}
		
		if ((fretting.frettingState > 0) && ((lastFrettingState == 0) || (lastFrettingState == -2)))
		{
			streak += fretting.frettingState;
			//cout << "streak: " << streak << endl;
			if (multiplier != 4)
			{
				if (streak < 10 && streak >= 0)
					multiplier = 1;
				else if (streak >= 10 && streak < 20)
					multiplier = 2;
				else if (streak >= 20 && streak < 30)
					multiplier = 3;
				else if (streak >= 30)
					multiplier = 4;
			}	
		}
	}
}
double Player::takeDamage( double damage )
{
	bool blessed = false;
	for (unsigned int i = 0; i < status.size(); i++)
		if (status[i].status == ST_BLESSED)
			blessed = true;
			
	if( blessed ) {
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
	bool	isInvencible = false;

	for (unsigned int i = 0; i < status.size(); i++)
	{
		switch (status[i].status)
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
				isInvencible = true;
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
				fretting.setAllNotPressed();
				isFrozen = true;
				break;
			case ST_CURSED:
				//look in Player::update()
				break;
		
		}
	
		// handle finish time of a status
		status[i].timeInStatus -= 1.0 / UPDATE_STATUS_TIME;
		if (status[i].timeInStatus <= 0)
			removeStatus(status[i], i);
	}
	
	if (fretting.frettingState > 0)
		XP += fretting.frettingState;
}

void Player::setStatusNormal()
{
	status.clear();
	
	//resets atributes that may have been modified by the status
	fretting.tolerance = instrument->tolerance;
	armor = instrument->armor;
	
	setStatusAttNormal();
}

void Player::setStatusAttNormal()
{
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

void Player::setInstrument( Instrument *_instrument )
{
	instrument = _instrument;
	
	maxHP = instrument->sumHP;
	HP = maxHP;
	maxStamina = instrument->sumStamina;
	//stamina = maxStamina;
	armor = instrument->armor;
	castedSpell = NULL;	
	
	fretting.setSkills( &instrument->skills );
	
	track->tolerance = instrument->tolerance;
	fretting.tolerance = instrument->tolerance;	
}

void Player::addStatus(statusType newStatus)
{
	status.push_back(newStatus);
}

void Player::removeStatus(statusType oldStatus, int indexInVector)
{
	statusType backup = oldStatus;
	status.erase(status.begin() + indexInVector);
	
	setStatusAttNormal();
}





