// utils.h
//

#ifndef UTILS_H
#define UTILS_H

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
#define SCREEN_Y 30
#define DEBUG 0
#define STONE_DELAY 3
#define MAX_STONE_PER_TRACK 30

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
} musicEvent;

/*
 * MACROS
 */ 
#define DOUBLE_TO_TIMESPEC(diff_time, timer) \
{ \
	timer.tv_sec	= (long int)diff_time; \
	diff_time -= timer.tv_sec; \
	\
	timer.tv_nsec	= diff_time * ONE_BILLION; \
}

#define DOUBLE_TO_TIMEVAL(diff_time, timer) \
{ \
	timer.tv_sec	= (long int)diff_time; \
	diff_time -= timer.tv_sec; \
	\
	timer->tv_nsec	= diff_time * ONE_MILLION; \
}

double timeval_to_double(struct timeval t);
double time_diff (timeval base_time);
int buttonType_to_int (buttonType button);

#endif

