#ifndef __SEEKER_H__
#define __SEEKER_H__
#include <stdlib.h>
#include "types.h"
#include "misc.h"
#include "vector.h"
#include "controller.h"
#include "cellstack.h"

/* Seeker
 *
 * This is a random walk behaviour. The monster walks in random directions regardless of obstacles 
 * etc.
 */
class Seeker : public Controller 
{
protected:  
  static const float MINIMUM_DEVIATION_TIME;
  static const int RADIUS;
  static const int GRID_SIZE; 
  
  Time oldTime;


  void execute(Pawn* pawn);
  

public:  
  /* constructor  
   */
  Seeker();
  
  /* start
   *
   * See Controller::start
   */
  void start(Pawn* pawn, Time startTime);
};

#endif

