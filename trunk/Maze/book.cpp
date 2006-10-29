#include "commongl.h"
#include "book.h"
#include "bookfactory.h"

Book::Book(Controller *controller, float posX, float posY, float posZ) : Monster(controller, posX,posY, posZ) 
{
}

bool Book::biteHim()
{
	return true;
}

void Book::draw() 
{
  //todo: also draw a shadow
//#pragma warning(disable:4800) -- MS specific pragma (Do not use in linux)
  bool oldBlend = glIsEnabled(GL_BLEND);
//#pragma warning(default:4800)
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  //TODO: turn off depth-writing & sort billboards by depth

  glBindTexture(GL_TEXTURE_2D, BookFactory::texture);

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

