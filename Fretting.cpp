// fretting.cpp
//
#include "Fretting.h"
#include "limits.h"
#include "tree_util.hh"


ostream& operator<<(ostream& out, skillTreeNode& node )
{
	switch(node.button)
	{
		case B1: out << "B1"; break;
		case B2: out << "B2"; break;
		case B3: out << "B3"; break;
		case B4: out << "B4"; break;
		case B5: out << "B5"; break;
		case NIL: out << "NIL"; break;
	}
	
	//out << "(" << node.skill << ")";
	
	return out;
}


/////////////////////////////////////////////////////////////////// CONSTRUCTORS
Fretting::Fretting()
{
	initialize();
	generateSkillsTree(NULL);
}

Fretting::Fretting( vector<Skill> *skills )
{
	initialize();
	
	generateSkillsTree(skills);
}

void Fretting::initialize()
{
	for (int i = 0; i < NFRETS; i++) {
		_trackPressed[i] = false;
		_rightPressed[i] = false;
		
		_hitting[i] = 0;
		keyState[i] = 0;
	}
	
	skillButtonState = 0;
	frettingState = 0;
	
	castedSpell = NULL;
	//sem_init(&semaphore, 0, 1);
}

Fretting::Fretting(EKEY_CODE events[NFRETS], EKEY_CODE skillButton)
{
	setEvents( &(events[NFRETS]), skillButton);
	initialize();
}

//////////////////////////////////////////////////////////////////// DESTRUCTORS
Fretting::~Fretting()
{	}

////////////////////////////////////////////////////////////// GETTERS & SETTERS
void Fretting::setEvents(EKEY_CODE events[NFRETS], EKEY_CODE _skillButton)
{
	for (int i = 0; i < NFRETS; i++)
		_events.push_back(events[i]);
	
	type = KEYBOARD;
	skillButton = _skillButton;
}

void Fretting::setEvents(int buttons[NFRETS], core::array<SJoystickInfo> 
			joystickInfo, int number, int skButton)
{
	if (number < joystickInfo.size())
	{
		for (int i = 0; i < NFRETS; i++)
			joystickButtons.push_back(buttons[i]);
		joystickNumber = number;
	}
	else
		cout << "Não foi encontrado este joystick\n";
	type = JOYSTICK;
	joystickSkillButton = skButton;
}
////////////////////////////////////////////////////////////////// OTHER METHODS

Skill* Fretting::findSkill( buttonType buttonPressed ) 
{
	bool found = false;
	tree<skillTreeNode>::sibling_iterator sib;

	// search for this key in the actual depth of the search tree
	for(sib = actualSkillNode.begin(); sib!=actualSkillNode.end() && !found; sib++)
		if( (*sib).button == buttonPressed ) {
			found=true;
			actualSkillNode = sib;
		}
		
	if(found) {
		if( (*actualSkillNode).skill!=NULL ) {
			// node is a leaf!
			tree<skillTreeNode>::sibling_iterator temp = actualSkillNode;
			actualSkillNode = skillsTree.begin();
			
			// return Skill found
			return (*temp).skill;
		}
		else {
			// will continue searching in next call
		}
	}
	else
		// there is no more possibilities of skills
		actualSkillNode = skillsTree.begin();
	
	
	// didn't find any Skills yet.
	return NULL;
}

void Fretting::generateSkillsTree( vector<Skill> *skills )
{
	skillSearchTree::iterator top, actual;
	skillSearchTree::sibling_iterator sib;

	top = skillsTree.begin();
	top = skillsTree.insert(top, skillTreeNode(NIL,NULL)); //dummy root

	// for each skill
	for(unsigned int i=0; i<skills->size(); i++) {
		actual = top;
		
		// for each key in the skill
		for(unsigned int k=0; k<skills->at(i).keysSequence.size(); k++) {
			bool found = false;

			// search for actual key in current depth
			for(sib = actual.begin(); sib!=actual.end(); sib++)
				if( (*sib).button == skills->at(i).keysSequence[k] ) {
					found=true;
					actual = sib;
				}

			// we must insert a new key in this depth
			if( !found ) {
				if(k==skills->at(i).keysSequence.size()-1) // it's a leaf, we register with pointer to the Skill
					actual = skillsTree.append_child(actual, skillTreeNode(skills->at(i).keysSequence[k],&(skills->at(i)) ));
				else // it's a node in the middle of the way, so no pointer to any Skill!
					actual = skillsTree.append_child(actual, skillTreeNode(skills->at(i).keysSequence[k],NULL));
			}
		}
	}
	
	cout<<"\nSkills tree:\n";
	kptree::print_tree_bracketed(skillsTree);
	cout<<endl<<endl;
	
	// set actual searching node to the top (root) 
	actualSkillNode = top;
}

void Fretting::lostNote()
{
	for (int i = 0; i < NFRETS; i++)
		if (_hitting[i] == 0)
			_hitting[i] = 0;
		else
			_hitting[i] = 2;
}

void Fretting::setAllNotPressed()
{
	for (int i = 0; i < NFRETS; i++)
		_hitting[i] = 0;
}

int Fretting::joystickPreFretting(SEvent *event)
{
	int usefullButton = -1;
	if (event->EventType == irr::EET_JOYSTICK_INPUT_EVENT && event->JoystickEvent.Joystick == joystickNumber)
        {
                JoystickState = event->JoystickEvent;
		for (int i = 0; i < NFRETS; i++)
		{
			//cout << JoystickState.IsButtonPressed(joystickButtons[i]) << _trackPressed[i] << " ";
			if (JoystickState.IsButtonPressed(joystickButtons[i]) != _trackPressed[i])
			{
				usefullButton = i;
			}
		}
		//cout << endl;
	}
	//cout << usefullButton << endl;
	_trackPressed[usefullButton] = JoystickState.IsButtonPressed(joystickButtons[usefullButton]);
	
	if (JoystickState.IsButtonPressed(joystickSkillButton) != skillButtonState)
	{
		//skillButtonState = JoystickState.IsButtonPressed(joystickSkillButton);
		usefullButton = SKILLBUTTON_INDEX;
	}
		//cout << JoystickState.IsButtonPressed(joystickSkillButton) << "\t" << skillButtonState << endl;
	return usefullButton;
}

int Fretting::keyboardPreFretting(SEvent *event)
{
	int usefullButton = -1;
	// verifies which button has been pressed -- TEM ALGUMA COISA ERRADA AQUI!!!
	for (int i = 0; i < NFRETS; i++)
		if (event->KeyInput.Key == _events[i])
			usefullButton = i;
			
	if (event->KeyInput.Key == skillButton)
		usefullButton = SKILLBUTTON_INDEX;
	
	return usefullButton;
}

int Fretting::verifyEvents(SEvent *event, Stone* stones[NFRETS], bool *usingSkill)
{
	if ((event->EventType == irr::EET_JOYSTICK_INPUT_EVENT && type == JOYSTICK) || 
		(event->EventType == irr::EET_KEY_INPUT_EVENT  && type == KEYBOARD))
	{
	double 	noteCreationTime;
	double 	noteDestructionTime;
	int		usefulButton = -1;
	int		lastState;
	int		nextFrettingState = -3;
	

	if (event != NULL)
	{
		if (type == JOYSTICK)
			usefulButton = joystickPreFretting(event);
		else if (type == KEYBOARD)
			usefulButton = keyboardPreFretting(event);
		else
			cout << "no fretting type defined\n";
	}
	else
		return -1;

	if (usefulButton == -1)
		return 0;
	
	// treatment of the SkillButton (toggle the usingSkill variable)
	if (usefulButton == SKILLBUTTON_INDEX)
	{
		if ((type == KEYBOARD && event->KeyInput.PressedDown) || 
			(type == JOYSTICK && JoystickState.IsButtonPressed(joystickSkillButton)))
		{
			if(skillButtonState == 0)
			{
				if (!*usingSkill) {
					actualSkillNode = skillsTree.begin();
					*usingSkill = true;
				}
				else
					*usingSkill = false;
				skillButtonState = 1;
			}
		}
		else
			skillButtonState = 0;
		return SKILLBUTTON_INDEX;
	}
	
	//cout << "1" << endl;
	if( stones[usefulButton]!= NULL ) {
		noteCreationTime = stones[usefulButton]->event.time;
		noteDestructionTime = stones[usefulButton]->destroyTime;
	}
	else {
		noteCreationTime = INT_MAX;
		noteDestructionTime = INT_MIN;
	}
	
	/* What follows is a very intricated and badly programmed state
	 * machines system.
	 * Please don't try to understand it.
	 * 
	 * But if you do try, increment the counter below with the time you
	 * lost on it.
	 * 
	 * Total time lost: 6h23 minutes
	 */
	lastState = _hitting[usefulButton];	// stores the old value of the actual pressed button
	if 	((type == KEYBOARD && event->KeyInput.PressedDown) || // key was pressed down
		(type == JOYSTICK && _trackPressed[usefulButton])) // joystick button was pressed down
	{
		switch (_hitting[usefulButton])
		{
		case 0: // wasn't pressing
		{
			// search skills
			if (*usingSkill == 1)
			{
				castedSpell = findSkill( (buttonType)usefulButton );
				
				if(castedSpell) {
					//casted!
					*usingSkill = 0;
				}
				
				else if( actualSkillNode == skillsTree.begin() ) 
					//missed! aborting Solo
					*usingSkill = 0;
				else
					;//still has chances to cast something
			}
			
			if (*musicTime > noteCreationTime - tolerance &&
				*musicTime < noteCreationTime + tolerance)
				// hit strike
				_hitting[usefulButton] = 1;
			else
				// missed strike
				_hitting[usefulButton] = -1;
			break;
		}	
		case 1: // hitting
			if (*musicTime < noteDestructionTime &&
				*musicTime > noteCreationTime)
				// holding the button
				_hitting[usefulButton] = 1;
			else
				// didnt press in the strike
				_hitting[usefulButton] = 2;
			break;
		case 2: // holding "do nothing" state
			if (*musicTime > noteCreationTime &&
				*musicTime < noteCreationTime + tolerance)
				_hitting[usefulButton] = -1;
			else
				_hitting[usefulButton] = 2;
			break;
		case -1: // missed
			if (*musicTime > noteCreationTime &&
				*musicTime < noteCreationTime + tolerance)
				_hitting[usefulButton] = -1;
			else
				_hitting[usefulButton] = 2;
			break;
		}
	}
	else // key was released
	{
		_hitting[usefulButton] = 0; // it's not being pressed
		//cout << "setou pra 0\t" << endl;
	}
	//cout << "2" << endl;
	
	// tests if I am pressing a chord in this moment
	if (lastState == 0 && _hitting[usefulButton] == 1)	// i.e., I hit a note NOW
	// find out if the note belongs to a chord
	{
		int 	notesOnChord = 0;
		int 	hitNotes;
		double	stonesCreationTime[NFRETS];
		bool	wrongNote = false;
		
		for (int i = 0; i < NFRETS; i++)
			if (_hitting[i] == 2 || _hitting[i] == -1)
				wrongNote = true;

		if ((!wrongNote) && (stones[usefulButton]->pressed == false))
		{		
			stones[usefulButton]->pressed = true;
			
			// CHORD DETECTION
			// takes the times of the chord
			for (int i = 0; i < NFRETS; i++)
				if (stones[i] != NULL)
					stonesCreationTime[i] = stones[i]->event.time;
				else
					stonesCreationTime[i] = INT_MAX;
			// finds out how many notes we have on the chord
			for (int i = 0; i < NFRETS; i++)
				if (stonesCreationTime[usefulButton] == stonesCreationTime[i])
					notesOnChord++;

			hitNotes = notesOnChord;
			// counts how many pressed notes we have on the chord
			for (int i = 0; i < NFRETS; i++)
				if (_hitting[i] == 1)
					hitNotes--;

			if (hitNotes == 0)
				nextFrettingState = notesOnChord;	// nextFrettingState <- how many notes are right
			else
				nextFrettingState = -2;	// waiting for the next notes to be pressed
		}
		else
		{
			lostNote();
			nextFrettingState = -1;
		}
	}
	//cout << "3" << endl;	
	if (nextFrettingState == -3)
	{
		switch( _hitting[usefulButton] )
		{
		case 0:
			nextFrettingState = 0;
			break;
		case 1:
			nextFrettingState = frettingState;	// continues hitting
			break;
		case 2:
			nextFrettingState = 0;
			break;
		case -1:
			nextFrettingState = -1;
			lostNote();
			break;
		}
	}
	
	if (nextFrettingState != -3)
		frettingState = nextFrettingState;
	return 1;
	}
	return 0;
}

int Fretting::verifyEventsAI(Stone* stones[NFRETS])
{
	double 	noteCreationTime;
	double 	noteDestructionTime;

	for (int i = 0; i < NFRETS; i++)
	{
		if( stones[i]!= NULL ) {
			noteCreationTime = stones[i]->event.time;
			noteDestructionTime = stones[i]->destroyTime;
		} else {
			noteCreationTime = INT_MAX;
			noteDestructionTime = INT_MIN;
		}

		if (_hitting[i] == 0)	// wasn't pressing
		{
			if ((*musicTime >= noteCreationTime - (tolerance/2)) && (stones[i]->pressed == false))
			{
				int prob = rand()%1000;
				if(prob<10) {
					_hitting[i] = 1;
					stones[i]->pressed = true;
				}
			}
			else
				_hitting[i] = 0;
		}
		else					// was pressing
		{
			if (*musicTime >= noteDestructionTime - (tolerance/2))
				_hitting[i] = 0;
			else
				_hitting[i] = 1;
		}
	}
	
	for (int i = 0; i < NFRETS; i++)
	{
		frettingState = 0;
		if (_hitting[i] == 1)
		{
			frettingState = 1;
			break;
		}
	}
	return 0;
}

void Fretting::printHitFret()
{
	// printing-time!
	for(int i=0; i<NFRETS; i++)
		cout<<_hitting[i]<<"\t";
	cout << "  =[" << frettingState << "]" << "  vector_size: "  << receiver->events.size();
		// << "  1: " << _events[0] << " 2: " << _events[1] << " 3: " << _events[2] << " 4: " << _events[3] << " 5: " << _events[4] << endl;
	//<< "  tolerance: " << tolerance << endl;
}
