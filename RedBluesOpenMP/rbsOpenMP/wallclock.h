/* wallclock.h for CIS*4450

#include or copy/paste the following functions into your program.
They implement Pilot's PI_StartTime(), PI_EndTime() style of timing.

For A2, call StartTime() after populating your board, just before starting the
simulation. Call EndTime() after the simulation finishes. Print the time using
"%lf" format.
*/

#include <time.h>

// helper function, don't call this directly
static double TimeHelper( int reset )
{
	static clock_t start = 0.0;
	if ( reset ) {
		start = clock();
		return 0.0;
	}
	return (double)(clock()-start)/CLOCKS_PER_SEC;
}

// StartTime resets timer
// EndTime returns no. of wallclock seconds elapsed since StartTime
void StartTime()
{
	TimeHelper( 1 ); // reset timer
}

double EndTime()
{
	return TimeHelper( 0 );
}
