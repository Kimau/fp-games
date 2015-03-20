#include "beholder.h"
#include "commongl.h"
#include "beholderfactory.h"

Beholder::Beholder(Controller *controller, float posX, float posY, float posZ) : Monster(controller, posX,posY, posZ) 
{
}

bool Beholder::biteHim() 
{
	return true;
}

void Beholder::draw() 
{
  bool oldBlend = glIsEnabled(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  glBindTexture(GL_TEXTURE_2D, BeholderFactory::texture);

  glBegin(GL_QUADS);
    glColor4f(1.0f,1.0f,1.0f,1.0f);
    Vector<float, 2> pos = getWorldPos();
    glTexCoord2f(0.0f, 1.0f); glVertex3f(pos[0]-0.5f, pos[1], 1.2f); //tl
    glTexCoord2f(1.0f, 1.0f); glVertex3f(pos[0]+0.5f, pos[1], 1.2f); //tr
    glTexCoord2f(1.0f, 0.0f); glVertex3f(pos[0]+0.5f, pos[1], 0.0f); //br
    glTexCoord2f(0.0f, 0.0f); glVertex3f(pos[0]-0.5f, pos[1], 0.0f); //bl
  glEnd();

  glBlendFunc(GL_ONE, GL_ZERO);

  if(!oldBlend)
    glDisable(GL_BLEND);
}

