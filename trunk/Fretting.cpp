// fretting.cpp
//
#include "Fretting.h"
#include "limits.h"
#include "tree_util.hh"


ostream& operator<<(ostream& out, skillTreeNode& node )
{
	switch(node.button)
	{
		case B1: return out << "B1(" << node.skill << ")";
		case B2: return out << "B2(" << node.skill << ")";
		case B3: return out << "B3(" << node.skill << ")";
		case B4: return out << "B4(" << node.skill << ")";
		case B5: return out << "B5(" << node.skill << ")";
		case NIL: return out << "NIL";
	}
	
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
		this->trackPressed[i] = false;
		this->rightPressed[i] = false;
	}	
	
	acertadas = 0;
	erradas	= 0;
	neutras = 0;
}

Fretting::Fretting(EKEY_CODE events[NUMBER_OF_FRETS])
{
	setEvents(&(events[NUMBER_OF_FRETS]));
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

////////////////////////////////////////////////////////////////// OTHER METHODS


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
	tree<skillTreeNode>::iterator top, actual;
	tree<skillTreeNode>::sibling_iterator sib;

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
	
	cout<<"Skills tree:"<<endl;
	kptree::print_tree_bracketed(skillsTree);
	cout<<endl;
	
	// set actual searching node to the top (root)
	actualSkillNode = top;
}
