// main.cpp
//

#include <vector>
#include <iostream>
using namespace std;

#define HAVE_IRRKLANG 1 //comment me to disable irrklang!

#ifdef HAVE_IRRKLANG
  #include <irrKlang.h>
#endif
#include "smf.h"

#include <irrlicht.h>
using namespace irr;

#include "Decoder.h"
#include "stone.h"
#include "utils.h"
#include "eventReceiver.h"


#define STONE_DELAY -2


Decoder decoder; 

// stores an entire MIDI file, so we don't need to keep it opened during too much time
vector<musicEvent> theMusic;

// stores the stones present on the screen
vector<stone*>	stonesOnScreen[NUMBER_OF_FRETS];

// indicates the end of the music for the keyboard polling function
bool endOfMusic;

std::string defaultFile = "example.mid", songFile = "example.ogg", guitarFile = "";
note theScreen[SCREEN_Y][NUMBER_OF_FRETS];

// Irrlicht-related globals
MyEventReceiver receiver;
IrrlichtDevice *device;
video::IVideoDriver* driver;
scene::ISceneManager* smgr;
// stores the stones present on the screen
vector<scene::ISceneNode*> nodes[NUMBER_OF_FRETS];


void matrix_update()
{
	// propagates the lines of the matrix	
	for(int lin=SCREEN_Y-1; lin>0; lin--)
		for(int col=0; col<NUMBER_OF_FRETS; col++)
			theScreen[lin][col] = theScreen[lin-1][col];
}

void matrix_print()
{
	//prints the matrix	
	for(int lin=0; lin<SCREEN_Y; lin++)
	{
		for(int col=0; col<NUMBER_OF_FRETS; col++)
		{
			switch(theScreen[lin][col])
			{
				case NOTHING: cout<<" "; break;
				case STRIKE: cout<<"O"; break;						
				case HOLD: cout<<"|"; break;
			}
			cout<<"  ";
		}
		cout<<endl;
	}
}

void decodeMidiEvent( smf_event_t *event )
{
	buttonType button = decoder.whatButton( decoder.note_from_int(event->midi_buffer[1]), EXPERT );
	//cout << smf_event_decode(event) << endl;
	
	if(button != NIL)  {	// decoded the event successfully
		musicEvent newEvent;
		
		eventType type = decoder.whatEventType( event->midi_buffer[0], event->midi_buffer[2] );
		
		// inicializes newEvent fields and put newEvent on the event vector.
		newEvent.time = event->time_seconds;
		newEvent.button = button;
		newEvent.type  = type;	
		theMusic.push_back(newEvent); // put this new event to the music events list
	}
}

int readMidi()
/* reads the mid file calling decodeMidiEvent() for every event */
{
	smf_t *smf;
	smf_event_t *event;
	bool eof = false;
	
	// load the file
	smf = smf_load( defaultFile.c_str() );
	if (smf == NULL) 
		return 1;
	
	while (!eof) {
		event = smf_get_next_event(smf);
		if( !event ) 
			eof = true;
		else if( smf_event_is_metadata(event) )	// ignores these events
			;
		else
			decodeMidiEvent(event);
	}
	
	cout << endl << "End of file." << endl;
	smf_delete(smf);
		
	return 0;	
}

static void *updater(void *argument) 
{
	struct	timeval start;
	double	musicTime;

	// get the time before starting the music (so we can know how much time passed in each note)
	gettimeofday(&start, NULL);
	
	while( !endOfMusic )
	{
		// In order to do framerate independent movement, we have to know how long it was since the last frame
        u32 then = device->getTimer()->getTime();
		const u32 now = device->getTimer()->getTime();
		const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
		then = now;

		musicTime = time_diff(start);// + 10;
		
//		cout << "music time: "	   << musicTime		 << endl
	//		 << "upcoming event: " << theMusic[0].time << endl;

		while( ((musicTime + STONE_DELAY) > theMusic[0].time) && !endOfMusic) {
			int track = buttonType_to_int(theMusic[0].button); //0~4
			
			switch(theMusic[0].type) { 
				case ON:
				{	
					// creates a new stone
					stone *newStone = new stone(theMusic[0], -1);
					
					// creates a new scene node
					scene::ISceneNode *newNode = smgr->addSphereSceneNode();
					newNode->setPosition( core::vector3df(track*10 - 25, //initial x
														  70,			 //initial y
														  0) );          //initial z
					newNode->setScale( core::vector3df(0.1,0.1,0.1) );
					nodes[track].push_back(newNode);

					// puts the stone on the end of the queue, i.e., it is, now, the
					// last stone to be destroyed
					stonesOnScreen[track].push_back(newStone);
					
					break;
				}
				case OFF:
				{
					int vector_size = stonesOnScreen[track].size();
					
					// sets the destroy_time of the last element of the vector of
					// stones of the referred track.
					if(vector_size > 0)
						stonesOnScreen[track][vector_size-1]->destroy_time = theMusic[0].time;
					else
						puts ("received an OFF without an ON O.o"); //lol 8D
					
					break;
				}
				default:
					break;

			}
			if(theMusic.size() > 0)
				theMusic.erase(theMusic.begin());
			else
				endOfMusic = true;
		}
		
		// updates nodes positions
		for (int ti = 0; ti < NUMBER_OF_FRETS; ti++)
			for(unsigned int ni=0; ni<nodes[ti].size(); ni++) {
				core::vector3df pos = nodes[ti][ni]->getPosition();
				//pos.Y -= 300*frameDeltaTime;
				pos.Y -= 0.00001;
				nodes[ti][ni]->setPosition(pos);
			}
		
		// desalocate the stones for which the time has already gone
		// for each track
		for (int i = 0; i < NUMBER_OF_FRETS; i++)
			// if we have stones to be destroyed (avoiding seg faults =D)
			if (stonesOnScreen[i].size () > 0)
				// while we have stones AND it is time to destroy them
				while ( (stonesOnScreen[i].size() > 0) &&
						(stonesOnScreen[i][0]->destroy_time != -1) &&
						(musicTime > 10*stonesOnScreen[i][0]->destroy_time) )
				
						stonesOnScreen[i].erase(stonesOnScreen[i].begin());
						//this is where we delete the node too (method drop())

	}
	
	return NULL;	
}

static void* drawer(void *argument) 
{
	struct	timeval start;
	double	musicTime;
	
	// this is needed because, while we are using drawer() and updater()
	// 	at the same time, both are editing theMusic vector.
	// we solve the problem simply creating a copy because there's no
	//  apparent reason need for them to be synchronized.
	vector<musicEvent> theMusicCopied = theMusic;


	// get the time before starting the music (so we can know how much time passed in each note)
	gettimeofday(&start, NULL);
	
	while( !endOfMusic )
	{
		usleep(80000);
		system("clear");
		musicTime = time_diff(start);
		
		cout << "music time: "	   << musicTime		 << endl
			 << "upcoming event: " << theMusicCopied[0].time << endl;

		matrix_update();

		while( ((musicTime /*+ STONE_DELAY*/) > theMusicCopied[0].time) && !endOfMusic) {
			// updates the first line of the matrix with the actual configuration
			switch(theMusicCopied[0].type) {
				case ON:
					theScreen[0][theMusicCopied[0].button] = STRIKE;
					break;
				case OFF:	
					theScreen[0][theMusicCopied[0].button] = NOTHING;		
					break;
				default: break;
			}
			
			// avoiding segmentation faults =D
			if(theMusicCopied.size() > 0)
				theMusicCopied.erase(theMusicCopied.begin());
			else
				endOfMusic = true;
		}
		
		matrix_print();
	}
	
	return NULL;	
}

void* fretting (void *arg)
// Poll the keyboard testing if the player has pressed the right notes.
{
	while(!endOfMusic)
	{		
		// example of how to get handle keyboard keys with Irrlicht
		/*
		core::vector3df nodePosition = node->getPosition();
		
		// receiver is a global EventHandler
		if(receiver.IsKeyDown(irr::KEY_KEY_W))
				nodePosition.Y += MOVEMENT_SPEED * frameDeltaTime;
		else if(receiver.IsKeyDown(irr::KEY_KEY_S))
				nodePosition.Y -= MOVEMENT_SPEED * frameDeltaTime;

		if(receiver.IsKeyDown(irr::KEY_KEY_A))
				nodePosition.X -= MOVEMENT_SPEED * frameDeltaTime;
		else if(receiver.IsKeyDown(irr::KEY_KEY_D))
				nodePosition.X += MOVEMENT_SPEED * frameDeltaTime;

		node->setPosition(nodePosition); */
	}
	
	return NULL;
}

void musa_init()
{
	readMidi();
	
	cout<<"MIDI parsed. This is your music:"<<endl;
	for(unsigned int i=0; i<theMusic.size(); i++) {
		cout<<theMusic[i].time<<" - "<<theMusic[i].button << ": ";
		switch(theMusic[i].type)
		{
			case ON: cout << "ON"; break;
			case OFF: cout << "OFF"; break;
			default: break;
		}
		cout<<endl;
	}
		
	//initializes the matrix
	for(int lin=0; lin<SCREEN_Y; lin++)
		for(int col=0; col<5; col++)
			theScreen[lin][col] = NOTHING;
			
	endOfMusic = false;
}

void initializeIrrlicht()
{
// Graphical engine initializing
	device = createDevice( video::EDT_OPENGL, core::dimension2d<u32>(800,600), 32,
												false, //fullscreen?
												false, //used stencil buffer?
												false, //use vsync?
												&receiver //event receiver
						);
	device->setWindowCaption(L"Musical Arena (MusA): The Adventures of Lucy in the Enchanted Realm of Diamond Sky");

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	
    // like the real game camera
    ///*
    smgr->addCameraSceneNode (
		0,					  // Camera parent
		core::vector3df(0, -50, -30), // Look from
		core::vector3df(0, 1, 0), // Look to
		1);						  // Camera ID
	//*/
		
	// debugging camera
	//smgr->addCameraSceneNode ( 0, core::vector3df(0, 0, -120), core::vector3df(0, 1, 0), 1);

}

int main(int argc, char *argv[])
{
	#ifdef HAVE_IRRKLANG
		irrklang::ISoundEngine* soundEngine = irrklang::createIrrKlangDevice();
		irrklang::ISoundSource* oggMusic = soundEngine->addSoundSourceFromFile(songFile.c_str());
		irrklang::ISoundSource* guitar;
		if( guitarFile.length()>0 )
			guitar = soundEngine->addSoundSourceFromFile(guitarFile.c_str());
		else
			guitar = NULL;
	#endif
	
	initializeIrrlicht();


	// Game engine initializing
	pthread_t thread[3];
	int arg = 1;

	musa_init();

	//cout<<"Press enter to start the music!"<<endl; getchar();

	#ifdef HAVE_IRRKLANG
		soundEngine->play2D(oggMusic, true);
		if(guitar)
			soundEngine->play2D(guitar, true);
	#endif
	//pthread_create(&thread[0], NULL, drawer, (void *) arg);
	pthread_create(&thread[1], NULL, fretting, (void *) arg);
	pthread_create(&thread[2], NULL, updater, (void *) arg);
	
	// Irrklang Main Loop
	int lastFPS = -1;
	while(device->run()) {
			driver->beginScene(true, true, video::SColor(255,113,113,133));

			smgr->drawAll(); // draw the 3d scene
			device->getGUIEnvironment()->drawAll(); // draw the gui environment (the logo)

			driver->endScene();

			int fps = driver->getFPS();

			if (lastFPS != fps)
			{
					core::stringw tmp(L"Movement Example - Irrlicht Engine [");
					tmp += driver->getName();
					tmp += L"] fps: ";
					tmp += fps;

					device->setWindowCaption(tmp.c_str());
					lastFPS = fps;
			}
	}
	device->drop();
	
	
	// wait for threads to complete
	pthread_join(thread[0], NULL);
	pthread_join(thread[1], NULL);
	pthread_join(thread[2], NULL);

	#ifdef HAVE_IRRKLANG
		soundEngine->drop();
	#endif
	return 0;
}
