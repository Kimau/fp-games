#ifndef __GOLDFACTORY_H__
#define __GOLDFACTORY_H__

#include "types.h"
#include "itemfactory.h"


class GoldFactory : public ItemFactory
{
  friend class Gold;
protected:
  static uint texture;
public:
  /* createItem   
   */
  virtual Item* createItem(uint x, uint y, uint z) const;
  
  /* reload
   */
  virtual bool reload();
  
  /* unload
	 */
  virtual void unload();
};

#endif

