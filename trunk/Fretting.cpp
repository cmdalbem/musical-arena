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
}

Fretting::Fretting( vector<Skill> *skills )
{
	initialize();
	
	generateSkillsTree(skills);
}

void Fretting::initialize()
{
	for (int i = 0; i < NUMBER_OF_FRETS; i++) {
		_trackPressed[i] = false;
		_rightPressed[i] = false;
		
		_hitting[i] = 0;
		keyState[i] = 0;
	}
	
	frettingState = 0;
	
	//sem_init(&semaphore, 0, 1);
}

Fretting::Fretting(EKEY_CODE events[NUMBER_OF_FRETS])
{
	setEvents( &(events[NUMBER_OF_FRETS]) );
	initialize();
}

//////////////////////////////////////////////////////////////////// DESTRUCTORS
Fretting::~Fretting()
{	}

////////////////////////////////////////////////////////////// GETTERS & SETTERS
void Fretting::setEvents(EKEY_CODE events[NUMBER_OF_FRETS])
{
	for (int i = 0; i < NUMBER_OF_FRETS; i++)
		_events.push_back(events[i]);
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
		if( (*actualSkillNode).skill!=NULL ) { // node is a leaf!
			tree<skillTreeNode>::sibling_iterator temp = actualSkillNode;
			actualSkillNode = skillsTree.begin();
			
			cout << "Player casted " << (*temp).skill->name << "!!!" << endl;
			
			return (*temp).skill; // return Skill found
		}
		else {
			// will continue searching in next call
		}
	}
	else
		actualSkillNode = skillsTree.begin(); // didn't find any keys here, going back to the root!
	
	
	return NULL; // didn't find any Skills.
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

int	Fretting::getFrettingState()
{
	switch( frettingState )
	{
		case -2: return 0; 				break;
		default: return frettingState;	break;
	}
}

void Fretting::lostNote()
{
	for (int i = 0; i < NUMBER_OF_FRETS; i++)
		if (_hitting[i] == 0)
			_hitting[i] = 0;
		else
			_hitting[i] = 2;
}

int Fretting::verifyEvents(SEvent *event, Stone* stones[NUMBER_OF_FRETS])
{
	double 	noteCreationTime;
	double 	noteDestructionTime;
	int		usefulButton = -1;
	int		lastState;
	int		nextFrettingState = -3;
	
	if (event != NULL)
	{	
		// verifies which button has been pressed -- TEM ALGUMA COISA ERRADA AQUI!!!
		for (int i = 0; i < NUMBER_OF_FRETS; i++)
			if (event->KeyInput.Key == _events[i])
				usefulButton = i;
	}
	else
		return -1;

	if (usefulButton == -1)
	{
//		cout << "useless button: " << event->KeyInput.Key
//			 << "  1: " << _events[0] << " 2: " << _events[1] << " 3: " << _events[2] << " 4: " << _events[3] << " 5: " << _events[4] << endl;
		return 0;
	}
	
	//cout << "1" << endl;
	//findSkill( (buttonType)usefulButton );
	
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
	 * 9h10
	 */
	lastState = _hitting[usefulButton];	// stores the old value of the actual pressed button
	if (event->KeyInput.PressedDown) // key was pressed down
	{
		switch (_hitting[usefulButton])
		{
		case 0: // wasn't pressing
			if (*musicTime > noteCreationTime - tolerance &&
				*musicTime < noteCreationTime + tolerance)
				// hit strike
				_hitting[usefulButton] = 1;
			else
				// missed strike
				_hitting[usefulButton] = -1;
			break;
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
		double	stonesCreationTime[NUMBER_OF_FRETS];
		bool	wrongNote = false;
		
		for (int i = 0; i < NUMBER_OF_FRETS; i++)
			if (_hitting[i] == 2 || _hitting[i] == -1)
				wrongNote = true;

		if ((!wrongNote) && (stones[usefulButton]->pressed == false))
		{		
			stones[usefulButton]->pressed = true;
			
			// CHORD DETECTION
			// takes the times of the chord
			for (int i = 0; i < NUMBER_OF_FRETS; i++)
				if (stones[i] != NULL)
					stonesCreationTime[i] = stones[i]->event.time;
				else
					stonesCreationTime[i] = INT_MAX;
			// finds out how many notes we have on the chord
			for (int i = 0; i < NUMBER_OF_FRETS; i++)
				if (stonesCreationTime[usefulButton] == stonesCreationTime[i])
					notesOnChord++;

			hitNotes = notesOnChord;
			// counts how many pressed notes we have on the chord
			for (int i = 0; i < NUMBER_OF_FRETS; i++)
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
	
	frettingState = nextFrettingState;
	return 1;
}

void Fretting::printHitFret()
{
	// printing-time!
	for(int i=0; i<NUMBER_OF_FRETS; i++)
		cout<<_hitting[i]<<"\t";
	cout << "  =[" << frettingState << "]" << "  vector_size: "  << receiver->events.size() << endl;
		// << "  1: " << _events[0] << " 2: " << _events[1] << " 3: " << _events[2] << " 4: " << _events[3] << " 5: " << _events[4] << endl;
	//<< "  tolerance: " << tolerance << endl;
}
