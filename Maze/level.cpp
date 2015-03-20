#include "level.h"
#include "player.h"
#include "random.h"

Level::Level(uint8 sizeX, uint8 sizeY, uint8 sizeZ, int bigroom, int medroom, int smlroom) : map(sizeX, sizeY, sizeZ, bigroom, medroom, smlroom) 
{
  Player::getInstance()->moveTo(pointXYZ(0,0,0));  
  minimapCamera.setRotation(0.0f, 0.0f, 0.0f);  
}

Level::~Level() 
{
  { vector<Pawn*>::iterator i;
    for(i = pawnList.begin(); i != pawnList.end(); i++)
      delete *i; }
  { vector<MonsterFactory*>::iterator i;
    for(i = monsterFactoryList.begin(); i != monsterFactoryList.end(); i++)
      delete *i; }
}

void Level::addItemFactory(ItemFactory* itemFactory)
{
  itemFactoryList.push_back(itemFactory);
}

void Level::addMonsterFactory(MonsterFactory* monsterFactory)
{
  monsterFactoryList.push_back(monsterFactory);
}

/*
	Generates Random Items
*/
void Level::generateRandomItems(int num)
{ 
  if(itemFactoryList.empty())
    return;

  Time currentTime = getTime();
  for(uint cGold = 0; cGold < num; cGold++)
  {    
    //get random position    
    uint x, y;
    x = (uint)globalRandom.generate(0,map.getMaze()->getWidth()-1);
    y = (uint)globalRandom.generate(0,map.getMaze()->getHeight()-1);
    //z = (uint)globalRandom.generate(0,map.getMaze()->getDepth()-1);

    
    //get random monster (i.e. obtain a random factory from the avialable list)
    uint itemType = globalRandom.generate(0, itemFactoryList.size()-1);
    
    //create item
    pawnList.push_back(itemFactoryList[itemType]->createItem(x, y, 0));
  }
  
  //update each item
  vector<Pawn*>::iterator i;
  for(i = pawnList.begin(); i != pawnList.end(); ++i)
    (*i)->update(currentTime);  
  
}

/*
	Generates Random Monsters
*/
void Level::generateRandomMonsters(int num)
{ 
  if(monsterFactoryList.empty())
    return;

  Time currentTime = getTime();
  for(uint cMonster = 0; cMonster < num; cMonster++)
  {
    //get random position
    uint x, y;
    x = (uint)globalRandom.generate(0,map.getMaze()->getWidth()-1);
    y = (uint)globalRandom.generate(0,map.getMaze()->getHeight()-1);

    
    //get random monster (i.e. obtain a random factory from the avialable list)
    uint monsterType = globalRandom.generate(0, monsterFactoryList.size()-1);
    
    //create monster
    pawnList.push_back(monsterFactoryList[monsterType]->createMonster(x+0.5f, y+0.5f, 0.0f));
  }

  //start each monster's behaviour
  vector<Pawn*>::iterator i;
  for(i = pawnList.begin(); i != pawnList.end(); ++i)
    (*i)->update(currentTime);    
}

vector<Pawn*>& Level::getPawnList()
{
	return pawnList;
}

void Level::draw()
{
  Vector<float, 2> playerPos = Player::getInstance()->getWorldPos();
  //Set camera positions
  {
    //World camera    
    camera.setPosition(playerPos[0]+0.0f, playerPos[1]-3.0f, 6.0f); //set camera postion
    camera.setRotation(45.0f,-15.0f, 0.0f);
  
    //Minimap position      
    minimapCamera.setPosition((int)playerPos[0], (int)playerPos[1], 10.0f);
  }
  
  //Initialize main map light
  {
    camera.transform();  

    GLfloat LightAmbient[]=		{ 0.65f, 0.6f, 0.5f, 1.0f };
    GLfloat LightDiffuse[]=		{ 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat LightPosition[]=	{ playerPos[0], playerPos[1], 1.5f, 1.0f };

	  glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
	  glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
	  glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);	// Position The Light
  }

  //draw map
  camera.transform();  
    //activate light
    glEnable(GL_TEXTURE_2D);    
	  glEnable(GL_LIGHT1);								// Enable Light One
    glEnable(GL_LIGHTING);  

  map.draw();      
    
  glDisable(GL_LIGHTING);  
  glDisable(GL_LIGHT1);
  
  //draw monsters  
  camera.transform();

	glEnable(GL_TEXTURE_2D);
	vector<Pawn*>::iterator i;
	for(i = pawnList.begin(); i != pawnList.end(); ++i)
		(*i)->draw();
	glDisable(GL_TEXTURE_2D);
	
	//todo draw weapons

  //draw player
  camera.transform();
  glEnable(GL_TEXTURE_2D);
  Player::getInstance()->draw();
  glDisable(GL_TEXTURE_2D);

 //Initialize minimap light
  {
    minimapCamera.transform();  

    GLfloat LightAmbient[]=		{ 0.65f, 0.6f, 0.5f, 1.0f };
    GLfloat LightDiffuse[]=		{ 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat LightPosition[]=	{ playerPos[0], playerPos[1], 1.4f, 1.0f };

	  glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
	  glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
	  glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);	// Position The Light
  }

  //draw minimap  		
  {
    glDisable(GL_DEPTH_TEST);
  
	    //Set minimap viewport
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

	  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glDisable(GL_DEPTH_TEST);
    glViewport(int(viewport[0]+0.75*viewport[2]), int(viewport[1]+0.75*viewport[3]), int(0.25*viewport[2]), int(0.25*viewport[3]));

      //Draw
    minimapCamera.transform();
    
        //activate light
    glEnable(GL_TEXTURE_2D);    

    glEnable(GL_LIGHT1);								// Enable Light One
    glEnable(GL_LIGHTING);  
    map.draw();

    glDisable(GL_LIGHTING);  
    glDisable(GL_LIGHT1);

      //Reset viewport
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
    glEnable(GL_DEPTH_TEST);
  }
}

void Level::performLogic()
{
  Time currentTime = getTime();

  //run monster logic
  vector<Pawn*>::iterator i;
  for(i = pawnList.begin(); i != pawnList.end(); ++i)
    (*i)->update(currentTime);

  //run player's logic
  Player::getInstance()->update(currentTime);

  //check if any pawns are dead and must be deleted and remove them
  for(i = pawnList.begin(); i != pawnList.end(); ++i)
  {
    if((*i)->getHealth() <= 0)
    {       
      (*i)->anihilateMe();
      delete *i;      
      pawnList.erase(i);           
      i = pawnList.begin(); //restart since i is no longer available
    }
  }
}

bool Level::reload()
{
  bool success = true;
  
  //reload moster factories
  {
    vector<MonsterFactory*>::iterator i;
    for(i = monsterFactoryList.begin(); i != monsterFactoryList.end(); ++i)
      success &= (*i)->reload();
  }
  
  //reload item factories
  {
    vector<ItemFactory*>::iterator i;
    for(i = itemFactoryList.begin(); i != itemFactoryList.end(); ++i)
      success &= (*i)->reload();  
  }
  return success;
}
void Level::unload()
{
  //unload monster factories
  {
    vector<MonsterFactory*>::iterator i;
    for(i = monsterFactoryList.begin(); i != monsterFactoryList.end(); ++i)
      (*i)->unload();
  }
  //unload item factories
  {
    vector<ItemFactory*>::iterator i;
    for(i = itemFactoryList.begin(); i != itemFactoryList.end(); ++i)
      (*i)->unload();
  }
}

