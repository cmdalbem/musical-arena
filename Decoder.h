#include <string>
#include <map>
#include <iostream>
using namespace std;


enum buttonsType
{
	B1, B2, B3, B4, B5, NIL
};

enum difficultyType
{
	EXPERT, MEDIUM, EASY, SUPEREASY
};
					
						
class Decoder
{
		private:
				map< string,pair<buttonsType,difficultyType> > notes;
				
				void initHash();
				
		public:
				Decoder();
				~Decoder();
		
				string note_from_int(int note_number);
				buttonsType whatButton( string note );	
				buttonsType whatButton( string note, difficultyType difficulty);	
		
		
};
	

