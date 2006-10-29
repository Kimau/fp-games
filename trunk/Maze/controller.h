#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__
#include <stdlib.h>
#include "types.h"
#include "misc.h"
#include "vector.h"
#include "pointXYZ.h"
#include "cellstack.h"

/*#ifndef _WIN32 template class Vector<float,2>; //NOTE: still need this?? (for g++) #endif*/

/* Controller
 *
 * Assigns an AI controller to a pawn. This is a state class (i.e. using the state design pattern) 
 * Assign one of its derived classes to the monster to give it it's current AI.
 */
class Pawn;
class Controller
{
private:
  bool isInitialized;
protected:
  CellStack path;

  float lastPos[2];
  Time startTime;
  Time previousTime;
  Time currentTime;


  virtual void execute(Pawn *pawn) = 0;

public:
  Controller() : isInitialized(false)
  {}

  void update(Pawn *pawn, Time currentTime); //prototype design pattern??

  
  virtual void start(Pawn *pawn, Time startTime)
  {
    isInitialized = true;
    Controller::startTime = startTime;
    previousTime = startTime;    
	path.push(pointXYZ(-1,-1,-1));
  }

  virtual bool getNewDir(Pawn*);

  /* stop
   *
   * This is used when the program must be paused (e.g. if it is minimized, or menu is opened
   * or pause button pressed etc.)
   * restart() must be called before update is called again, so that the timeline can be adjusted 
   * properly
   */
  void stop(Pawn *pawn, Time stopTime)
  {
    currentTime = stopTime;
  }

  void restart(Time startTime)
  {    

  }
};

#include "pawn.h"

#endif

