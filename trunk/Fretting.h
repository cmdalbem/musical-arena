// fretting.h
//

#pragma once

#include "Stone.h"
#include "utils.h"
#include "tree.hh"
#include "Skill.h"

#include <irrlicht.h>
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


class Fretting
{
	public:
		double *musicTime;
		double tolerance;
		
		int acertadas;
		int erradas;
		int neutras;
	
		Fretting();
		Fretting( vector<Skill> *skills );
		Fretting(EKEY_CODE events[NUMBER_OF_FRETS]);

		~Fretting();

		void 	setEvents(EKEY_CODE events[NUMBER_OF_FRETS]);

		int 	verify_events(SEvent event, vector<Stone*> stones[NUMBER_OF_FRETS])
		{
			double noteCreationTime[NUMBER_OF_FRETS];
			double noteDestructionTime[NUMBER_OF_FRETS];
			//Skill*		findSkill( buttonType buttonPressed );
			int		usefulButton = -1;
			

			for (int i = 0; i < NUMBER_OF_FRETS; i++)
			{
				if (event.KeyInput.Key == events[i])
				{
					//code = event.KeyInput.Key;
					usefulButton = i;
				}
			}

			// verifies what buttons are pressed
			for (unsigned int index = 0; index < events.size(); index++)
			{
				if (stones[index].size() > 0)
				{
					noteCreationTime[index] = stones[index][0]->event.time;
					noteDestructionTime[index] = stones[index][0]->destroyTime;
				}
				else
				{
					noteCreationTime[index] = INT_MAX;
					noteDestructionTime[index] = INT_MIN;
				}
			}

			
			if (usefulButton != -1)
			{
				if (event.KeyInput.PressedDown)
				{
					switch (hitting[usefulButton])
					{
					case 0: // wasn't pressing
						if (*musicTime > noteCreationTime[usefulButton] - tolerance &&
							*musicTime < noteCreationTime[usefulButton] + tolerance)
							// hit strike
							hitting[usefulButton] = 1;
						else
							// missed strike
							hitting[usefulButton] = -1;
						break;
					case 1: // hitting
						if (*musicTime < noteDestructionTime[usefulButton] &&
							*musicTime > noteCreationTime[usefulButton])
							// holding the button
							hitting[usefulButton] = 1;
						else
							// didnt press in the strike
							hitting[usefulButton] = 2;
						break;
					case 2: // holding "do nothing" state
						if (*musicTime > noteCreationTime[usefulButton] &&
							*musicTime < noteCreationTime[usefulButton] + tolerance)
							hitting[usefulButton] = -1;
						else
							hitting[usefulButton] = 2;
						break;
					case -1: // missed
						if (*musicTime > noteCreationTime[usefulButton] &&
							*musicTime < noteCreationTime[usefulButton] + tolerance)
							hitting[usefulButton] = -1;
						else
							hitting[usefulButton] = 2;
						break;
					}
				}
				else //key was released
					hitting[usefulButton] = 0; // pressed nothing				
			}
			

			cout << "hitting: " << hitting[usefulButton] << endl;

			return 1;
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

		}

	private:
		tree<skillTreeNode> 			skillsTree;
		tree<skillTreeNode>::iterator 	actualSkillNode;
		
		vector<EKEY_CODE> 	events;
		int					hitting[5];
		
		bool 				trackPressed[NUMBER_OF_FRETS];		// tell us the state of the tracks on the last
		bool 				rightPressed[NUMBER_OF_FRETS];		// frame		
		
		Skill* 				findSkill( buttonType buttonPressed );
		void				generateSkillsTree( vector<Skill> *skills );
		void				initialize();
};

