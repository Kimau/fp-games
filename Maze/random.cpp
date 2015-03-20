#include "random.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "types.h"

/* Random globals
 */
Random globalRandom;

/* Random constructor 
 */
Random::Random(int seed):seed(seed),num(seed)
{
}

/* Random constructor 
 *
 * Note: Just seeding the random number generator with the system time is not good enough because
 * time() can only return a number in the order of seconds, which means that any number of
 * random number generators, each constructed rapidly after the other, would generate the same sets of
 * numbers. The solution is simply to have a global seed, originally taken from system time, 
 * that is modified with each new random number generator's creation
 *
 * Note: I'm not entirely sure what would happen if you seed srand with a number larger than RAND_MAX, but it 
 * shouldn't make a difference since the docs don't mention anything.
 */
Random::Random()
{
  static int globalSeed = time(null);
  globalSeed++;
  num = seed = globalSeed;
}

/* Random reset
 */ 
void Random::reset()
{
  num = seed;
}

/* Random generate
 */
float Random::generate(float min, float max)
{
  generate();
  return (num/((float)RAND_MAX))*(max-min) + min;
  
}

int Random::generate(int min, int max)
{
  generate();
  return (int)((num/(float)(RAND_MAX))*(max-min+1)) + min;
}

int Random::generate()
{
  srand(num);
  return num = rand();
}

/* Random setSeed
 */
void Random::setSeed(int seed)
{
  num = Random::seed = seed;
}

/* Random setResetPoint
 */
void Random::setResetPoint()
{
  seed = num;
}


