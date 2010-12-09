// utils.h
//
#pragma once

// Standard Libraries
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <sys/time.h>
#include <pthread.h>
#include "stdio.h"

#include "types.h"

#define ONE_BILLION 1000000000
#define ONE_MILLION 1000000
#define NFRETS 5
#define NPLAYERS 2
#define DEBUG 0
#define MAX_STONE_PER_TRACK 30
#define STONE_RADIUS 1
#define NECK_TEXTURE_PROPORTION 2 //how many texture copies fit on a neck

#define NSTAMINALEVELS 2

#define SCREENX 800 
#define SCREENY 600

#define TRACK_SIZE_X 30
#define TRACK_SIZE_Y 70

#ifdef HAVE_FMOD
  #include <fmod.hpp>
  #include <fmod_errors.h>
#endif

using namespace std;



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
#define IS_STATUS_FIRE \
	(status == ST_FIRE ? 2 : 1)


struct timeval double_to_timeval( double diff_time );
double timeval_to_double(struct timeval t);
double time_diff (timeval base_time);
double timeval_diff(timeval a, timeval b);
int buttonType_to_int (buttonType button);

