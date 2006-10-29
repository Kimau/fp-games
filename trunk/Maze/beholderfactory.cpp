#include "beholderfactory.h"
#include "commongl.h"

using namespace std;

GLuint BeholderFactory::texture(0);
BeholderFactory::BeholderFactory(ControllerFactory* controllerFactory):MonsterFactory(controllerFactory)
{} 

BeholderFactory::~BeholderFactory()
{}
 
Monster* BeholderFactory::createMonster(float posX, float posY, float posZ) const 
{ 
  return new Beholder(controllerFactory->createController(), posX, posY, posZ); 
}  

bool BeholderFactory::reload()
{
	//load image
	#define MASK_COLOR 0xff000000 //Red    

	if(!loadMaskedBMPTexture("Data/monst03.bmp", MASK_COLOR, texture)) //!loadBMPTexture("Data/monst01.bmp", texture))
	{
		#ifdef DEBUG_MODE
			fprintf(stderr, "BeholderFactory: Couldn't load data/monst03.bmp\n");
		#endif
		return false;
	}

  //todo: in future perhaps reload controller factory resources here if necessary

	return true;
}

void BeholderFactory::unload()
{
	glDeleteTextures(1, &texture);

  //todo: in future perhaps unload controller factory resources here if necessary
}


