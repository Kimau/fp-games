#include "ghost.h"
#include "commongl.h"
#include "ghostfactory.h"

Ghost::Ghost(Controller *controller, float posX, float posY, float posZ) : Monster(controller, posX,posY, posZ) 
{
  isGhost = true;
}

bool Ghost::biteHim()
{
	return true;
}

void Ghost::draw() 
{
  bool oldBlend = glIsEnabled(GL_ADD);
  glBlendFunc(GL_ONE, GL_ONE);
  glEnable(GL_BLEND);

  glBindTexture(GL_TEXTURE_2D, GhostFactory::texture);

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

