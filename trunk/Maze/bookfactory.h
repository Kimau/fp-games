#ifndef __BOOKFACTORY_H__
#define __BOOKFACTORY_H__

#include "monsterfactory.h"
#include "book.h"

/* BookFactory 
 *
 * BookFactory is a concrete factory derived from MonsterFactory.
 * TODO: Perhaps make BookFactory into a singleton? 
 */
class BookFactory : public MonsterFactory
{
  friend class Book;
protected:
  static uint texture;
public:
  BookFactory(ControllerFactory* controllerFactory);
  ~BookFactory();

  /* createMonster
   *
   * See MonsterFactory::createMonster
   */
  Monster* createMonster(float posX, float posY, float posZ) const;
  bool reload();
  void unload();
};

#endif

