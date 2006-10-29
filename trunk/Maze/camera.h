#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"
#include "commongl.h"

/* Camera 
 * 
 * The camera object used for blending
 */

class Camera
{
protected:
  Vector<float, 3> rotation; 
  Vector<float, 3> position; 
public:
  /* setRotation
   *
   * Rotations happen in this order: x, y, z. These rotation angles must be in degrees.
   */
  void setRotation(float x, float y, float z)
  {
    rotation[0] = x;
    rotation[1] = y;
    rotation[2] = z;
  }
  void setPosition(float x, float y, float z)
  {
    position[0] = x;
    position[1] = y;
    position[2] = z;
  }
  /* transform 
   *
   * Must be in world transformation mode
   */
  void transform()
  {
    glLoadIdentity();

    //rotate world around camera in reverse camera rotation
    glRotatef(-rotation[0],1.0f,0.0f,0.0f);     //rotation around the x-axis
    glRotatef(-rotation[1],1.0f,0.0f,0.0f);  //rotation around the y-axis
    glRotatef(-rotation[2],1.0f,0.0f,0.0f);  //rotation around the z-axis

    //translate world in reverse of camera position
    glTranslatef(-position[0],-position[1],-position[2]);
  }
};

#endif
