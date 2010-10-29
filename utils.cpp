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

