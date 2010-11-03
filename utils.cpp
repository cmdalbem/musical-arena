// utils.cpp
//

#include "utils.h"

double timeval_to_double(struct timeval t) 
{ 
	return (double)(t.tv_sec + ((double)t.tv_usec / ONE_MILLION)); 
}

double time_diff (timeval base_time)
/* returns the difference between the time NOW and the time on base_time */
{
	timeval end;
	gettimeofday(&end, NULL);
	
	end.tv_sec  = end.tv_sec  - base_time.tv_sec;
	end.tv_usec = end.tv_usec - base_time.tv_usec;
	
	return timeval_to_double(end);
}

int buttonType_to_int (buttonType button)
{
	switch (button)
	{
		case B1: return 0;
		case B2: return 1;
		case B3: return 2;
		case B4: return 3;
		case B5: return 4;
		case NIL: return 5;
	}
}

struct timeval double_to_timeval( double diff_time )
{
	struct timeval t;
	t.tv_sec = (long int)diff_time;
	diff_time -= t.tv_sec;
	
	t.tv_usec = diff_time * ONE_MILLION;
	
	return t;
}
