// fretting.cpp
//
#include "Fretting.h"
#include "limits.h"
#include "tree_util.hh"


ostream& operator<<(ostream& out, skillTreeNode& node )
{
	switch(node.button)
	{
		case B1: return out << "B1";
		case B2: return out << "B2";
		case B3: return out << "B3";
		case B4: return out << "B4";
		case B5: return out << "B5";
		case NIL: return out << "NIL";
	}
	
	return out;
}


/////////////////////////////////////////////////////////////////// CONSTRUCTORS
Fretting::Fretting()
{
	initialize();
}

Fretting::Fretting( vector<Skill> skills )
{
	initialize();
	
	generateSkillsTree(skills);
}

void Fretting::initialize()
{
	for (int i = 0; i < NUMBER_OF_FRETS; i++) {
		this->trackPressed[i] = false;
		this->rightPressed[i] = false;
	}	
}

Fretting::Fretting(EKEY_CODE events[NUMBER_OF_FRETS], eventReceiver *receiver)
{
	setEvents(&(events[NUMBER_OF_FRETS]));
	setReceiver(receiver);
	initialize();
}

//////////////////////////////////////////////////////////////////// DESTRUCTORS
Fretting::~Fretting()
{	}

////////////////////////////////////////////////////////////// GETTERS & SETTERS
void Fretting::setEvents(EKEY_CODE events[NUMBER_OF_FRETS])
{
	for (int i = 0; i < NUMBER_OF_FRETS; i++)
		this->events.push_back(events[i]);
}

void Fretting::setReceiver (eventReceiver *receiver)
{	this->receiver = receiver;	}

////////////////////////////////////////////////////////////////// OTHER METHODS
int Fretting::verify_event(vector<Stone*> stones[NUMBER_OF_FRETS], double musicTime, const double tolerance)
{
	double  noteCreationTime[NUMBER_OF_FRETS];
	double  noteDestructionTime[NUMBER_OF_FRETS];
	bool	noonePressedNow = true;
	
	//stores on a vector what buttons are pressed
	vector<int> whatPressed;
	
	for(int i = 0; i < NUMBER_OF_FRETS; i++)
	{
		// discovers what buttons are pressed now
		if(receiver->IsKeyDown(events[i]))
			whatPressed.push_back(i);

		// inicializes the times
		if (stones[i].size() > 0)
		{
			// semáforo lock;
			noteCreationTime[i] = stones[i][0]->event.time;
			noteDestructionTime[i] = stones[i][0]->destroyTime;
			// semáforo unlock
		}
		else
		{
			noteCreationTime[i] = INT_MAX;
			noteDestructionTime[i] = INT_MIN;
		}
	}
	
	/*cout << "buttons pressed: " << whatPressed.size()
		 << "\t stones on each track: " << stones[0].size()
		 << " " << stones[1].size() << " " << stones[2].size()
		 << " " << stones[3].size() << " " << stones[4].size()
		 << endl;*/
	
	// if no button was pressed, ignores the rest of the function
	if (whatPressed.size() == 0)
		return NO_BUTTON_PRESSED;		// acho que equivale ao DO_NOTHING
	
	// tests the algorithm written on the end of this function (comment)
	for (int wPIndex = 0; wPIndex < whatPressed.size(); wPIndex++)
	{
		int actualButton = whatPressed[wPIndex];
		
		// if this button was pressed now
		if (trackPressed[actualButton] == false)
		{
			cout << "button " << actualButton << "was pressed now" << endl;
			noonePressedNow = false;
			// if actualButton is the "rightier" button pressed (it is the last button on whatPressed)
			if ( wPIndex == whatPressed.size() - 1)
			{
				// if there was any note on the time the button was pressed
				if( (musicTime > noteCreationTime[actualButton] - tolerance) &&
					(musicTime < noteCreationTime[actualButton] + tolerance) ) // eu acertei AGORA
				{
					// if there's another note starting at the same time
					bool anotherNote = false;
					for (int i = 0; i < NUMBER_OF_FRETS; i++)
					{
						if (stones[i][0]->event.time == noteCreationTime[actualButton] && i != actualButton)
							anotherNote = true;
					}
					
					if (anotherNote)
					{
						// if there's any note wrong pressed (every note must be pressed on its "CreationTime")
						for (int wPIndex2 = 0; wPIndex2 < whatPressed.size(); wPIndex2++)
						{
							int actualButton2 = whatPressed.size();
							if(!((musicTime > noteCreationTime[actualButton2] - tolerance) &&
								 (musicTime < noteCreationTime[actualButton2] + tolerance) ))
								return ERROR;
							else
								return DO_NOTHING;
						}
					}
					else
						return HIT;
				}
				else
					return ERROR;
			}
			else
				return ERROR;
		}
	}
	
	// tests if is there any note bad pressed
	if (noonePressedNow)
	{
		for (int wPIndex = 0; wPIndex < whatPressed.size(); wPIndex++)
		{
			int actualButton = whatPressed[wPIndex];
			if (musicTime > noteCreationTime[actualButton] &&
				musicTime < noteDestructionTime[actualButton])
				return	HIT;
			else
				return	DO_NOTHING;
		}
	}
	
	// vejo quais botões estão apertados;
	// se algum foi apertado agora;
	// // se ele é o mais da direita;
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
	// // se ele não é o mais da direita;
	// // // errou
	// se ninguém foi apertado agora
	// // se tem nota tocando agora
	// // // "dá ponto"
	// // se nao tem nota tocando agora
	// // // nao faz nada
	// O caso de o jogador deixar um nota passar será tratado pela track =D
}

void Fretting::generateSkillsTree( vector<Skill> skills )
{
	tree<skillTreeNode>::iterator top, actual;
	tree<skillTreeNode>::sibling_iterator sib;

	top = skillsTree.begin();
	
	top = skillsTree.insert(top, skillTreeNode(NIL,NULL));
	
	cout<<"Skills:"<<endl;
	for(unsigned int i=0; i<skills.size(); i++)
	{
		for(unsigned int k=0; k<skills[i].keysSequence.size(); k++)
			cout << skills[i].keysSequence[k]+1 << "-";
		cout << endl;
	}
	
	for(unsigned int i=0; i<skills.size(); i++)
	{
		actual = top;
		
		cout<<"- SKILL "<<skills[i].name<<endl;
		for(unsigned int k=0; k<skills[i].keysSequence.size(); k++)
		{
			bool found = false;
			cout<<"searching for: "<<skills[i].keysSequence[k]+1<<endl;
			cout<<"in:";
			for(sib = actual.begin(); sib!=actual.end(); sib++)
			{
				cout<<(*sib).button+1<<",";
				if( (*sib).button == skills[i].keysSequence[k] ) {
					found=true;
					actual = sib;
				}
			}
			
			if( !found ) {
				cout<<"didn't find! inserting..."<<endl;
				if(actual==top)
				{
					cout<<"ROOTS BLOODY ROOTS"<<endl;
					actual = skillsTree.append_child(top, skillTreeNode(skills[i].keysSequence[k],NULL));
				}
				else
					if(k==skills[i].keysSequence.size()-1) //skill leaf
						actual = skillsTree.append_child(actual, skillTreeNode(skills[i].keysSequence[k],&skills[i]));
					else
						actual = skillsTree.append_child(actual, skillTreeNode(skills[i].keysSequence[k],NULL));
			}
			else
				cout<<"ok, found it."<<endl;
		}
	}
	
	cout<<"Arvore:"<<endl;
	kptree::print_tree_bracketed(skillsTree);
	cout<<endl;
	
	actualNode = &(*skillsTree.begin());
}
