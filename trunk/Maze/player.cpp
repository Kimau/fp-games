#include "player.h"
#include "game.h"
#include "commongl.h"


Player Player::instance;

Player::KeyboardController::KeyboardController()
{
  memset(keymap, 0, 5*sizeof(bool));
}

/* execute
 */
void Player::KeyboardController::execute(Pawn* pawn)
{
  if(keymap[LEFT])
  {
    pawn->moveDir(Maze::WEST);
    keymap[LEFT] = false;
    return;
  }

  if(keymap[RIGHT])
  {
    pawn->moveDir(Maze::EAST);
    keymap[RIGHT] = false;
    return;
  }

  if(keymap[UP])
  {
    pawn->moveDir(Maze::NORTH);
    keymap[UP] = false;
    return;
  }

  if(keymap[DOWN])
  {
    pawn->moveDir(Maze::SOUTH);
    keymap[DOWN] = false;
    return;
  }

  
  if(keymap[USE])
  {
    keymap[USE] = false; //set key to false so it doesn't repeat    
    //check if there are any doors around player and open them if there are
    bool isNextToDoor = false;
    isNextToDoor |= Maze::getInstance()->isDoor(pawn->getX(),pawn->getY(),pawn->getZ(), Maze::NORTH);
    isNextToDoor |= Maze::getInstance()->isDoor(pawn->getX(),pawn->getY(),pawn->getZ(), Maze::EAST);
    isNextToDoor |= Maze::getInstance()->isDoor(pawn->getX(),pawn->getY(),pawn->getZ(), Maze::WEST);
    isNextToDoor |= Maze::getInstance()->isDoor(pawn->getX(),pawn->getY(),pawn->getZ(), Maze::SOUTH);
    if(isNextToDoor)
      ((Player*)pawn)->openDoors();
    return;
  }      

  //pawn->moveDir(0);
}

/* keyPressed
 */
bool Player::KeyboardController::keyPressed(SDLKey key)
{
  ControllerKeys controllerKey; 
  switch(key)
  {
  case SDLK_w:
    controllerKey = UP;
    break;
  case SDLK_s:
    controllerKey = DOWN;
    break;
  case SDLK_d:
    controllerKey = RIGHT;
    break;
  case SDLK_a:
    controllerKey = LEFT;
    break;
  case SDLK_e:
    controllerKey = USE;
    break;
  default:
    return false;
  }
  keymap[controllerKey] = true;
  return true;
}

/* keyUp
 */
bool Player::KeyboardController::keyUp(SDLKey key)
{
  ControllerKeys controllerKey; 
  switch(key)
  {
  case SDLK_w:
    controllerKey = UP;
    break;
  case SDLK_s:
    controllerKey = DOWN;
    break;
  case SDLK_d:
    controllerKey = RIGHT;
    break;
  case SDLK_a:
    controllerKey = LEFT;
    break;
  case SDLK_e:
    controllerKey = USE;
    break;
  default:
    return false;
  }
  keymap[controllerKey] = false;
  return true;      
}

Player::Player() : Pawn(0.0f, 0.0f, 0.0f) , prints(pointXYZ(0,0,0))
{     
  setController(new KeyboardController());     
  baseSpeed = 0.8f;
	name = "hero";
  maxHealth = health = 100;
}

void Player::draw()
{
  //todo: also draw a shadow
//#pragma warning(disable:4800) -- MS specific pragma (Do not use in linux)
  bool oldBlend = glIsEnabled(GL_BLEND);
//#pragma warning(default:4800)
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  //TODO: turn off depth-writing & sort billboards by depth

  glBindTexture(GL_TEXTURE_2D, texture);

  glBegin(GL_QUADS);
    glColor4f(1.0f,1.0f,1.0f,1.0f);
    Vector<float, 2> pos = Pawn::getWorldPos();
    glTexCoord2f(0.0f, 1.0f); glVertex3f(pos[0]-0.5f, pos[1], 1.2f); //tl
    glTexCoord2f(1.0f, 1.0f); glVertex3f(pos[0]+0.5f, pos[1], 1.2f); //tr
    glTexCoord2f(1.0f, 0.0f); glVertex3f(pos[0]+0.5f, pos[1], 0.0f); //br
    glTexCoord2f(0.0f, 0.0f); glVertex3f(pos[0]-0.5f, pos[1], 0.0f); //bl	  
  glEnd();


  glBlendFunc(GL_ONE, GL_ZERO);

  if(!oldBlend)
    glDisable(GL_BLEND);
}

bool Player::biteHim()
{
	return true;
}

/* reload
 *
 * Reload resources.
 */
bool Player::reload()
{
  //load image
	#define MASK_COLOR 0xff000000 //Red    

	if(!loadMaskedBMPTexture("Data/player.bmp", MASK_COLOR, texture)) 
	{
		#ifdef DEBUG_MODE
			fprintf(stderr, "Player: Couldn't load Data/player.bmp\n");
		#endif
		return false;
	}

  //todo: in future perhaps reload controller factory resources here if necessary

	return true;
}

/* unload
 *
 * Clean up all resources in use (i.e. delete textures etc.)
 */
void Player::unload()
{
  glDeleteTextures(1, &texture);
}

/*
	Collision Checker
*/
bool Player::checkCollide(Pawn& mover)
{
	if (mover.getGridPos() == getGridPos())
	{
		/*
			Did Player Move ?
			If So check player against EVERYTHING
		*/
		if (mover.getName() == "hero")
		{
			bool flag_ = true;
			vector<Pawn*>& mylilMon_ = Game::getLevel()->getPawnList();
			vector<Pawn*>::iterator i;
			for(i = mylilMon_.begin(); i != mylilMon_.end(); i++)
			{
				Pawn& myMon = (**i);

				// HIT!!!
				if (myMon.getGridPos() == getGridPos())
				{
					flag_ = (flag_ && myMon.biteHim());
				}
			}
			return flag_;

		}
		/*
			If Player is not the mover then check only Mover against Player
			And we already know player and mover occupate same space
			return true;
		*/
		return true;
	}
	return false;
}

void Player::openDoors()
{
  Maze& maze = *Maze::getInstance();
  
  int x = getX();
  int y = getY();
  int z = getZ();

  if(Maze::getInstance()->isDoor(x,y,z, Maze::NORTH))
    if(!Maze::getInstance()->isOpen(x,y,z, Maze::NORTH))
      maze.openDoor(x,y,z,Maze::NORTH);
    else
      maze.closeDoor(x,y,z,Maze::NORTH);

  if(Maze::getInstance()->isDoor(x,y,z, Maze::SOUTH))
    if(!Maze::getInstance()->isOpen(x,y,z, Maze::SOUTH))
      maze.openDoor(x,y,z,Maze::SOUTH);
    else
      maze.closeDoor(x,y,z,Maze::SOUTH);
  
  if(Maze::getInstance()->isDoor(x,y,z, Maze::WEST))
    if(!Maze::getInstance()->isOpen(x,y,z, Maze::WEST))
      maze.openDoor(x,y,z,Maze::WEST);
    else
      maze.closeDoor(x,y,z,Maze::WEST);
  
  if(Maze::getInstance()->isDoor(x,y,z, Maze::EAST))
    if(!Maze::getInstance()->isOpen(x,y,z, Maze::EAST))
      maze.openDoor(x,y,z,Maze::EAST);
    else
      maze.closeDoor(x,y,z,Maze::EAST);
}

void Player::anihilateMe()
{
  Game::quit();
}

void Player::addGold(uint value)
{
  gold += value; 
}

