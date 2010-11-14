// utils.h
//
#pragma once

// Standard Libraries
#include <cstdlib>
#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <ctime>
#include <sys/time.h>
#include <pthread.h>

#define ONE_BILLION 1000000000
#define ONE_MILLION 1000000
#define NUMBER_OF_FRETS 5
#define NPLAYERS 2
#define DEBUG 0
#define MAX_STONE_PER_TRACK 30
#define STONE_RADIUS 1
#define NECK_TEXTURE_PROPORTION 2 //how many texture copies fit on a neck

#ifdef HAVE_FMOD
  #include <fmod.hpp>
  #include <fmod_errors.h>
#endif

using namespace std;

typedef unsigned char u_char;

enum difficultyType
{
	EXPERT, HARD, MEDIUM, EASY
};

enum note
{
	STRIKE,
	HOLD,
	NOTHING
};

enum eventType		// Types of MIDI events
{
	ON,
	OFF,
	ERROR
};

enum buttonType		// 5 buttons, five frets. Some idea?
{
	B1, B2, B3, B4, B5, NIL
};

typedef struct musicEvent_t
{
	buttonType button;
	eventType type;
	double time; //	time in seconds
	int mspqn; // microseconds per quarter note
} musicEvent;



/*
 * MACROS
 */ 
#define DOUBLE_TO_TIMESPEC(diff_time, timer) \
{ \
	timer.tv_sec	= (long int)diff_time; \
	diff_time -= timer.tv_sec; \
	\
	timer.tv_usec	= diff_time * ONE_BILLION; \
}

#define ERRCHECK(result) \
	if (result != FMOD_OK) \
	{ \
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result)); \
		exit(-1); \
	}


struct timeval double_to_timeval( double diff_time );
double timeval_to_double(struct timeval t);
double time_diff (timeval base_time);
double timeval_diff(timeval a, timeval b);
int buttonType_to_int (buttonType button);

