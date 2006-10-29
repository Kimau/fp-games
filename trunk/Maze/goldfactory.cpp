#include "goldfactory.h"
#include "gold.h"
#include "commongl.h"


uint GoldFactory::texture;

/* createItem   
 */
Item* GoldFactory::createItem(uint x, uint y, uint z) const
{
  return new Gold(x,y,z);
}

/* reload
 */
bool GoldFactory::reload()
{
  //load image
	#define MASK_COLOR 0xff000000 //Red    

	if(!loadMaskedBMPTexture("Data/gold.bmp", MASK_COLOR, texture))
	{
		#ifdef DEBUG_MODE
			fprintf(stderr, "GoldFactory: Couldn't load data/gold.bmp\n");
		#endif
		return false;
	}

  //todo: in future perhaps reload controller factory resources here if necessary

	return true;
}

/* unload
 */
void GoldFactory::unload()
{
	glDeleteTextures(1, &texture);
}

