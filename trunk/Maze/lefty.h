#ifndef __LEFTY_H__
#define __LEFTY_H__
#include <stdlib.h>
#include "types.h"
#include "misc.h"
#include "vector.h"
#include "controller.h"
#include "pawn.h"

/* 
	Lefty
	Follows the left hand rule
 */
class Lefty : public Controller 
{
protected:  

  Time oldTime;
  int Dir;

  void execute(Pawn* pawn);
  

public:  
  /* constructor  
   */
  Lefty();
  
  /* start
   *
   * See Controller::start
   */
  void start(Pawn* pawn, Time startTime);
  bool getNewDir(Pawn*);
};

#endif

