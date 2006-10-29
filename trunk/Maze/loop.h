#ifndef __LOOP_H__
#define __LOOP_H__
#include <stdlib.h>
#include "types.h"
#include "misc.h"
#include "vector.h"
#include "controller.h"
#include "pawn.h"

/* 
	Loop
	Goes until it hits something then turns right
	Ends up looping
 */
class Loop : public Controller 
{
protected:  

  Time oldTime;
  int Dir;

  void execute(Pawn* pawn);
  

public:  
  /* constructor  
   */
  Loop();
  
  /* start
   *
   * See Controller::start
   */
  void start(Pawn* pawn, Time startTime);
  bool getNewDir(Pawn*);
};

#endif

