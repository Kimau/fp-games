#include <SDL.h>
#include "misc.h"


/*Time getTime() (NOTE: This is probably a more accurate timing method in Windows)
{
  /*static __int64 counterFrequency;

  //Initialize counter frequency once only
  static int isValid1 = QueryPerformanceFrequency((LARGE_INTEGER*)&counterFrequency);*

  __int64 counterFrequency;
  __int64 time;
  QueryPerformanceFrequency((LARGE_INTEGER*)&counterFrequency);
  QueryPerformanceCounter((LARGE_INTEGER*)&time);

  /* TMP DEBUG
  char str[255];
  sprintf(str, "%d", counterFrequency);
  OutputDebugString(str);
  *

  //return time in seconds
  return (float10)time / (float10)counterFrequency / (float10)1000.0;
}*/

/* getTime
 */
Time getTime()
{
	int n = SDL_GetTicks();
	return n/1000.0;
}




