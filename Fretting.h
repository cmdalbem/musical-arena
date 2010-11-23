// fretting.h
//

#pragma once

#include "Stone.h"
#include "utils.h"
#include "tree.hh"
#include "Skill.h"
#include "EventReceiver.h"

#include <irrlicht.h>
//#include <semaphore.h>
using namespace irr;

#define ERROR 0
enum {
	HIT = 1, DO_NOTHING, NO_BUTTON_PRESSED
};


class skillTreeNode {
	
	public:
		skillTreeNode( buttonType b, Skill* s ) : button(b), skill(s) {}
		
		buttonType button;
		Skill *skill;
				
};

typedef tree<skillTreeNode> skillSearchTree;

										
class Fretting
{
	public:
		Fretting();
		Fretting( vector<Skill> *skills );
		Fretting(EKEY_CODE events[NUMBER_OF_FRETS]);
		~Fretting();
		
		double 	*musicTime;
		double 	tolerance;
		int 	frettingState;
		int		keyState[5];
		int		_hitting[NUMBER_OF_FRETS];
		//sem_t	semaphore;
		EventReceiver *receiver;

		void 	setEvents(EKEY_CODE events[NUMBER_OF_FRETS]);
		int 	verifyEvents(SEvent *event, Stone* firstStones[NUMBER_OF_FRETS]);
		int		getFrettingState();
		void	printHitFret();
		void	lostNote();

	private:
		skillSearchTree 			skillsTree;
		skillSearchTree::iterator 	actualSkillNode;
		
		vector<EKEY_CODE> 	_events;
		
		bool 				_trackPressed[NUMBER_OF_FRETS];		// tell us the state of the tracks on the last
		bool 				_rightPressed[NUMBER_OF_FRETS];		// frame		
		
		Skill* 				findSkill( buttonType buttonPressed );
		void				generateSkillsTree( vector<Skill> *skills );
		void				initialize();
};

