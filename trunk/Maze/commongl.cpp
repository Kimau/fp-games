#include "commongl.h"

/*#ifdef WIN32
AUX_RGBImageRec *loadBMP(char *filename)
{
  //Check whether file exists
  {
    std::ifstream file(filename, std::ios::in /*| std::ios::nocreate* | std::ios::binary);
    if(!file)
      return null; //error: can't open file for input
  }

  //Load image into system memory
  return auxDIBImageLoad(filename);
}
#endif*/

/*Note: Uses sdl to load the bitmap image format
 */
bool loadBMPTexture(const char* filename, uint& texture)
{
	SDL_Surface *sdlImage;
	
	// Load The Bitmap, Check For Errors, If the file wasn't found, quit
	if(sdlImage = SDL_LoadBMP(filename))
	{
		/*
		NOTE: SDL loads the pixels from a bitmap file in the same format as it is stored in the BMP
		supposeedly BMP files store pixels in reverse order BGR format, so the data must be reversed and converted 
		to RGB for use with OpenGL.
		*/
		
		// Convert data ordering to RGB, TopLeft->DownRight order
		uint8* img = new uint8[sdlImage->w*sdlImage->h*3]; 
		for(uint c = 0; c < sdlImage->w*sdlImage->h; c++)
		{
			img[c*3] = ((uint8*)sdlImage->pixels)[sdlImage->w*sdlImage->h*3-c*3 + 2]; //R
			img[c*3+1] = ((uint8*)sdlImage->pixels)[sdlImage->w*sdlImage->h*3-c*3 + 1]; //G
			img[c*3+2] = ((uint8*)sdlImage->pixels)[sdlImage->w*sdlImage->h*3-c*3]; //B
		}
		
		// Create the GL texture
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, sdlImage->w,	sdlImage->h, 0, GL_RGB,	GL_UNSIGNED_BYTE, img);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		delete[] img;
	}
	else return false;
	
	// Free temporary sdl image
	if(sdlImage)
		SDL_FreeSurface(sdlImage);	

	return true;
}

bool loadMaskedBMPTexture(const char* filename, uint32 maskColor, uint& texture)
{
	SDL_Surface *sdlImage;
	
	// Load The Bitmap, Check For Errors, If the file wasn't found, quit
	if(sdlImage = SDL_LoadBMP(filename))
	{
    // Convert data ordering to RGB, TopLeft->DownRight order. Also add an alpha channel (i.e. convert 24bit->32bit) for masking.
		uint8* img = new uint8[sdlImage->w*sdlImage->h*4]; 
		for(uint c = 0; c < sdlImage->w*sdlImage->h; c++)
		{
			img[(c<<2)] =   ((uint8*)sdlImage->pixels)[sdlImage->w*sdlImage->h*3-c*3 + 2]; //R
			img[(c<<2)+1] = ((uint8*)sdlImage->pixels)[sdlImage->w*sdlImage->h*3-c*3 + 1]; //G
			img[(c<<2)+2] = ((uint8*)sdlImage->pixels)[sdlImage->w*sdlImage->h*3-c*3]; //B
      img[(c<<2)+3] = 255; //A
		}

		//mask out the maskcolor in the alpha channel		
		#ifndef _WIN32
		uint c; //scoping of variables changed for ISO (gcc obviously complies to a new version of ANSI than MSVC++ 6)
		#endif
		for(c = 0; c < sdlImage->w*sdlImage->h; c++)
		
		{					
			if(((uint32*)img)[c] == (maskColor|0xff)) //check if mask color (note: OR-ING maskColor with 255 so that alpha is ignored in the comparison)
			{
				((uint32*)img)[c] = 0; //set both alpha & color to 0				
			}
		}
		// Create the GL texture
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);		
		
		#define LINEAR_FILTERING
		#ifdef MIPMAP_FILTERING
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
  		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, sdlImage->w, sdlImage->h, GL_RGBA, GL_UNSIGNED_BYTE, img);
		#elif defined(LINEAR_FILTERING)
			glTexImage2D(GL_TEXTURE_2D, 0, 4, sdlImage->w, sdlImage->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
  		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		#else //POINT_FILTERING
			glTexImage2D(GL_TEXTURE_2D, 0, 4, sdlImage->w, sdlImage->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		#endif
		
		delete[] img;
	}
	else 
		return false;
	
	// Free temporary sdl image
	if(sdlImage)
		SDL_FreeSurface(sdlImage);

  return true;
}

