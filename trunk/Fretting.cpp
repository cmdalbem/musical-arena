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
	}
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

int Fretting::verifyEvents(SEvent event, Stone* stones[NUMBER_OF_FRETS])
{
	// vejo quais botões estão apertados;
	// se algum foi apertado agora;
	// // // se tem alguma nota nova pra ele;
	// // // // se tem uma outra nota com início no mesmo momento;
	// // // // // se tem alguma nota apertada já errada
	// // // // // // errou
	// // // // // se nao tem alguma nota apertada errada
	// // // // // // não faz nada (espera que o jogador ainda pode apertar as outras)
	// // // // se não tem outra nota com o início no mesmo momento;
	// // // // // acertou
	// // // se não tem alguma nota nova pra ele
	// // // // errou
	// se ninguém foi apertado agora
	// // se tem nota tocando agora & acertei (last frame)
	// // // "dá ponto"
	// // se nao tem nota tocando agora
	// // // nao faz nada
	// O caso de o jogador deixar um nota passar será tratado pela track =D
					
	double 	noteCreationTime;
	double 	noteDestructionTime;
	int		usefulButton = -1;
	
	// verifies which button has been pressed
	for (int i = 0; i < NUMBER_OF_FRETS; i++)
		if (event.KeyInput.Key == _events[i])
			usefulButton = i;
	
	if (usefulButton != -1)
	{
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
		 * But if you do, increment the counter below with the time you
		 * lost on it.
		 * 
		 * Total time lost: 3 minutes.
		 */
		
		if (event.KeyInput.PressedDown) // key was pressed down
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
			_hitting[usefulButton] = 0; // it's not being pressed
	}


	switch( _hitting[usefulButton] )
	{
		case 0:
		case 2:
			//	if you ain't doing nothing, nothing happens
			frettingState = 0;
			break;
		case -1:
			//	if your are missing you are missing
			frettingState = -1;
			break;
		case 1:
		{
			// if you are hitting one track but there's others
			//  we check the ones being hit.
			double hitTime[NUMBER_OF_FRETS];
			for(int i=0; i<NUMBER_OF_FRETS; i++)
				if( stones[i]!= NULL )
					hitTime[i] = stones[i]->event.time;
				else
					noteCreationTime = INT_MAX;
			
			for(int i=0; i<NUMBER_OF_FRETS; i++)
				if( *musicTime > hitTime[i] && 	// if there's another note that should be hitting
					*musicTime < hitTime[i] + tolerance)
					switch(_hitting[i])
					{
						case 1: frettingState = 1; break;
						case 0:
						case 2: frettingState = 0; break;
						case -1: frettingState = -1; break;
					}
		}				
				
			
	}
	cout << "frettingState: " << frettingState << endl;

	return 1;
}
