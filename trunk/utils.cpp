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

double timeval_diff(timeval a, timeval b)
/* returns the difference between two times */
{
	timeval result;
	result.tv_sec  = a.tv_sec  - b.tv_sec;
	result.tv_usec = a.tv_usec - b.tv_usec;
	return timeval_to_double(result);
}

struct timeval double_to_timeval( double diff_time )
{
	struct timeval t;
	t.tv_sec = (long int)diff_time;
	diff_time -= t.tv_sec;
	
	t.tv_usec = diff_time * ONE_MILLION;
	
	return t;
}
