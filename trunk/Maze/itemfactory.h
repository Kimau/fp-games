#ifndef __ITEMFACTORY_H__
#define __ITEMFACTORY_H__

#include "item.h"

class ItemFactory
{
public:
  /* constructor                       
   */
  ItemFactory() {}
  
  /* destructor
   */
  ~ItemFactory() {}

  /* createItem   
   */
  virtual Item* createItem(uint x, uint y, uint z) const = 0;
  
  /* reload
   *
   * Reload factory (and item) resources.
   */
  virtual bool reload() = 0;
  
  /* unload
   *
	 * Clean up all resources in use (i.e. delete textures etc.)
	 */
  virtual void unload() = 0;
};

#endif

