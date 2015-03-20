#ifndef __BLIND_H__
#define __BLIND_H__
#include "controller.h"
/* Blind
 *
 * This is a random walk behaviour. The monster walks in random directions regardless of obstacles 
 * etc.
 */
class Blind : public Controller 
{
protected:
  void execute(Pawn* pawn);
public:  
  /* constructor
   */
  Blind();
  
  /* start
   *
   * See Controller::start
   */
  void start(Pawn* pawn, Time startTime);
};

#endif 

