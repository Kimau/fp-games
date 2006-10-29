#include "gold.h"
#include "commongl.h"
#include "goldfactory.h"
#include "random.h"
#include "player.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Gold::Gold(uint x, uint y, uint z) : Item(x,y,z)
{	
	  //Initialize stats to default values	
	name	= "Gold";
	value	= globalRandom.generate(0,10);
}

Gold::~Gold()
{

}

bool Gold::biteHim()
{
  Player::getInstance()->addGold(value);
  Item::biteHim();
  return true;
}

void Gold::draw()
{
  bool oldBlend = glIsEnabled(GL_BLEND);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  glBindTexture(GL_TEXTURE_2D, GoldFactory::texture);

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

