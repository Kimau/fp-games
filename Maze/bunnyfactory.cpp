#include "bunnyfactory.h"
#include "commongl.h"

using namespace std;

GLuint BunnyFactory::texture(0);
BunnyFactory::BunnyFactory(ControllerFactory* controllerFactory):MonsterFactory(controllerFactory)
{} 

BunnyFactory::~BunnyFactory()
{}
 
Monster* BunnyFactory::createMonster(float posX, float posY, float posZ) const 
{ 
  return new Bunny(controllerFactory->createController(), posX, posY, posZ); 
}  

bool BunnyFactory::reload()
{
	//load image
	#define MASK_COLOR 0xff000000 //Red    

	if(!loadMaskedBMPTexture("Data/monst01.bmp", MASK_COLOR, texture)) //!loadBMPTexture("Data/monst01.bmp", texture))
	{
		#ifdef DEBUG_MODE
			fprintf(stderr, "BunnyFactory: Couldn't load data/monst01.bmp\n");
		#endif
		return false;
	}

  //todo: in future perhaps reload controller factory resources here if necessary

	return true;
}

void BunnyFactory::unload()
{
	glDeleteTextures(1, &texture);

  //todo: in future perhaps unload controller factory resources here if necessary
}


