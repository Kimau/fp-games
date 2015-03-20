#include "ghostfactory.h"
#include "commongl.h"

using namespace std;

GLuint GhostFactory::texture(0);
GhostFactory::GhostFactory(ControllerFactory* controllerFactory):MonsterFactory(controllerFactory)
{} 

GhostFactory::~GhostFactory()
{
}
 
Monster* GhostFactory::createMonster(float posX, float posY, float posZ) const 
{ 
  return new Ghost(controllerFactory->createController(), posX, posY, posZ); 
}  

bool GhostFactory::reload()
{
	//load image
	#define MASK_COLOR 0xff000000 //Red    

	if(!loadMaskedBMPTexture("Data/monst02.bmp", MASK_COLOR, texture)) //!loadBMPTexture("Data/monst01.bmp", texture))
	{
		#ifdef DEBUG_MODE
			fprintf(stderr, "GhostFactory: Couldn't load data/monst02.bmp\n");
		#endif
		return false;
	}

  //todo: in future perhaps reload controller factory resources here if necessary

	return true;
}

void GhostFactory::unload()
{
	glDeleteTextures(1, &texture);

  //todo: in future perhaps unload controller factory resources here if necessary
}


