#include "bookfactory.h"
GLuint BookFactory::texture(0);
BookFactory::BookFactory(ControllerFactory* controllerFactory):MonsterFactory(controllerFactory)
{} 

BookFactory::~BookFactory()
{}
 
Monster* BookFactory::createMonster(float posX, float posY, float posZ) const 
{ 
  return new Book(controllerFactory->createController(), posX, posY, posZ); 
}  

bool BookFactory::reload()
{
	//load image
	#define MASK_COLOR 0xff000000 //Red    

	if(!loadMaskedBMPTexture("Data/monst04.bmp", MASK_COLOR, texture))
	{
		#ifdef DEBUG_MODE
			fprintf(stderr, "BookFactory: Couldn't load data/monst04.bmp\n");
		#endif
		return false;
	}

  //todo: in future perhaps reload controller factory resources here if necessary

	return true;
}

void BookFactory::unload()
{
	glDeleteTextures(1, &texture);

  //todo: in future perhaps unload controller factory resources here if necessary
}

