#ifndef __COMMONGL_H__
#define __COMMONGL_H__
//#pragma once

#include <iostream>
#include <fstream>
#ifdef WIN32
  #include <windows.h>
#endif
#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
/*#ifdef WIN32
#include "GL/glaux.h"
#endif*/

#include "types.h"

/*#ifdef WIN32
AUX_RGBImageRec* loadBMP(char *filename);
#endif*/


//TODO: Add texture filtering parameter i.e. enum Filtering { POINT, LINEAR, MIPMAP}
bool loadBMPTexture(const char* filename, uint& texture);
bool loadMaskedBMPTexture(const char* filename, uint32 maskColor, uint& texture);

/* drawBox
 *
 * 
 * x, y, z - center-point position
 * w, h, d - box dimensions along the x-, y-, and z-axis respectively 
 * 
 *
enum BoxQuads
{
  FRONT  = 1,
  BACK   = 2,
  LEFT   = 4,
  RIGHT  = 8,
  TOP    = 16,
  BOTTOM = 32
};

union BoxFlags
{
  struct
  {
    unsigned FRONT  : 1; //bit 0
    unsigned BACK   : 1; //bit 1
    unsigned LEFT   : 1; //bit 2 ..etc 
    unsigned RIGHT  : 1;
    unsigned TOP    : 1;
    unsigned BOTTOM : 1;
  }
  uint8 quadFlags;
};
struct BoxUV
{  
  uv
  operator()
}

float drawBox(float x, float y, float z, float w, float h, float d, int uv[3][3], )
{
}
*/

/* drawBillboard
 *
 * 
 * sizeX, sizeY
 * posX, posY, posZ
 * Assumption: World matrix is active
 */

enum BillboardPivot
{
  CENTER,
  TOP_CENTER,
  TOP_LEFT,
  TOP_RIGHT,
  BOTTOM_CENTER,
  BOTTOM_LEFT,
  BOTTOM_RIGHT
};


/*void drawBillboard(float posX, float posY, float posZ, float sizeX, float sizeY, float u, float v, BillboardPivot pivot = CENTER)
{
  //get centre point
  switch(pivot)
  {
  case CENTER:    
    break:
  case BOTTOM_CENTER:
    posY += sizeY/2.0f;
    break:
  case BOTTOM_RIGHT:
    posY += sizeY/2.0f;
    posX -= sizeX/2.0f;
    break:
  case BOTTOM_LEFT:
    posY += sizeY/2.0f;
    posX += sizeX/2.0f;
    break:
  case TOP_CENTER:
    posY -= sizeY/2.0f;
    break:
  case TOP_RIGHT:
    posY -= sizeY/2.0f;
    posX -= sizeX/2.0f;
    break:
  case TOP_LEFT:
    posY -= sizeY/2.0f;
    posX += sizeX/2.0f;
    break:
  }

  glPushMatrix();
    glLoadIdentity();
    glTranslatef(posX, posY, posZ);		
    glRotatef(-30.0,1.0f,0.0f,0.0f);	
  glPopMatrix();
}
*/
#endif

