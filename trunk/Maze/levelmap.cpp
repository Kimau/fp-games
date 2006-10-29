#include "levelmap.h"
#include "random.h"
#include "commongl.h"
#include "player.h"

Maze* LevelMap::maze = Maze::getInstance();

LevelMap::LevelMap(uint8 sizeX, uint8 sizeY, uint8 sizeZ, int bigroom, int medroom, int smlroom)
{
  maze = Maze::getInstance();
  //generate maze
  maze->Create(sizeY, sizeX, sizeZ, globalRandom.generate(), bigroom, medroom, smlroom);
  maze->Generate();

  //Load Texture data (temp: later put in a load function)
  bool error = false;
	error |= !loadBMPTexture("Data/wall01.bmp", wallTextures[0]);
  error |= !loadBMPTexture("Data/wall02.bmp", wallTextures[1]);
  error |= !loadBMPTexture("Data/wall03.bmp", wallTextures[2]);
  error |= !loadBMPTexture("Data/wall04.bmp", wallTextures[3]);
  error |= !loadBMPTexture("Data/wall05.bmp", wallTextures[4]);
  error |= !loadBMPTexture("Data/wall06.bmp", wallTextures[5]);

  error |= !loadBMPTexture("Data/door01.bmp", doorTexture);

  error |= !loadBMPTexture("Data/solid01.bmp", indestructableWallTexture);

  error |= !loadBMPTexture("Data/floor01.bmp", floorTextures[0]);
  error |= !loadBMPTexture("Data/floor02.bmp", floorTextures[1]);
  error |= !loadBMPTexture("Data/floor03.bmp", floorTextures[2]);
  error |= !loadBMPTexture("Data/floor04.bmp", floorTextures[3]);
  
  error |= !Player::getInstance()->reload();
  
  if(error)
  {
		#ifdef DEBUG_MODE
			fprintf(stderr, "LevelMap: Couldn't load texture data\n");
		#endif
		//return false;
	}  
}

LevelMap::~LevelMap()
{
  glDeleteTextures(5, wallTextures);
  glDeleteTextures(4, floorTextures);
}


/* LevelMap draw
 *
 * Optimization note: it is better to draw every "type" of object on the map separately so that
 * primitives can be grouped according to texture.
 *
 * Todo: check for adjacent walls so as to eliminate unnecessary polygons
 */
void drawWall(int ix, int iy, int iz, Maze::Direction direction)
{
  //NOTE: I refer to width, length an height as the measurements along the x-, y-, and z-axis respectively

  static const float CELL_WIDTH  = 1.0f;
  static const float CELL_LENGTH = 1.0f; 
  static const float CELL_HEIGHT = 1.3f;

  static const float WALL_THICKNESS = 0.2f;
  static const float WALL_LENGTH    = 1.0f;
  static const float WALL_HEIGHT    = 1.3f;
  
  float w, l;
  float h = WALL_HEIGHT; //(This never changes)

  //calculate world-position according to cell index positions
  float x = ix + 0.5f;
  float y = -(iy)  - 0.5f; //negative, because graphics y-axis direction is opposite from "array index y-axis".
  float z = WALL_HEIGHT/2.0f; //(NOTE: this would be different if player can see bottom\top levels)

  //reposition & calculate dimensions
  switch(direction)
  {
  case Maze::NORTH:
    y += CELL_LENGTH/2; //top wall (using opengl axis)
    w = WALL_LENGTH;
    l = WALL_THICKNESS;
    break;
  case Maze::SOUTH:
    y -= CELL_LENGTH/2;
    w = WALL_LENGTH;
    l = WALL_THICKNESS;
    break;
  case Maze::WEST:
    x -= CELL_WIDTH/2;
    w = WALL_THICKNESS;
    l = WALL_LENGTH;
    break;
  case Maze::EAST:
    x += CELL_WIDTH/2;
    w = WALL_THICKNESS;
    l = WALL_LENGTH;
    break;
  default:
    return;
  }

	//Back quad                    
	glNormal3f(0.0f, 1.0f, 0.0f); glTexCoord2f(0.0f, 1.0f); glVertex3f(x+w/2, y+l/2, z+h/2); //tl (note: clockwise as seen from back)
	glNormal3f(0.0f, 1.0f, 0.0f); glTexCoord2f(1.0f, 1.0f); glVertex3f(x-w/2, y+l/2, z+h/2); //tr
	glNormal3f(0.0f, 1.0f, 0.0f); glTexCoord2f(1.0f, 0.0f); glVertex3f(x-w/2, y+l/2, z-h/2); //br
	glNormal3f(0.0f, 1.0f, 0.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(x+w/2, y+l/2, z-h/2); //bl
	
	//Left quad  			          
	glNormal3f(-1.0f, 0.0f, 0.0f); glTexCoord2f(0.0f, 1.0f); glVertex3f(x-w/2, y-l/2, z+h/2); //tl (note: clockwise as seen from the left)
	glNormal3f(-1.0f, 0.0f, 0.0f); glTexCoord2f(1.0f, 1.0f); glVertex3f(x-w/2, y+l/2, z+h/2); //tr
	glNormal3f(-1.0f, 0.0f, 0.0f); glTexCoord2f(1.0f, 0.0f); glVertex3f(x-w/2, y+l/2, z-h/2); //br
	glNormal3f(-1.0f, 0.0f, 0.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(x-w/2, y-l/2, z-h/2); //bl
	
	//Right quad  			          
	glNormal3f(1.0f, 0.0f, 0.0f); glTexCoord2f(0.0f, 1.0f); glVertex3f(x+w/2, y+l/2, z+h/2); //tl (note: clockwise as seen from the right)
	glNormal3f(1.0f, 0.0f, 0.0f); glTexCoord2f(1.0f, 1.0f); glVertex3f(x+w/2, y-l/2, z+h/2); //tr
	glNormal3f(1.0f, 0.0f, 0.0f); glTexCoord2f(1.0f, 0.0f); glVertex3f(x+w/2, y-l/2, z-h/2); //br
	glNormal3f(1.0f, 0.0f, 0.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(x+w/2, y+l/2, z-h/2); //bl
	
	//Front quad          
	glNormal3f(0.0f, -1.0f, 0.0f); glTexCoord2f(0.0f, 1.0f); glVertex3f(x-w/2, y-l/2, z+h/2); //tl (etc...)
	glNormal3f(0.0f, -1.0f, 0.0f); glTexCoord2f(1.0f, 1.0f); glVertex3f(x+w/2, y-l/2, z+h/2); //tr
	glNormal3f(0.0f, -1.0f, 0.0f); glTexCoord2f(1.0f, 0.0f); glVertex3f(x+w/2, y-l/2, z-h/2); //br
	glNormal3f(0.0f, -1.0f, 0.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(x-w/2, y-l/2, z-h/2); //bl
	
	//Top quad          
	glNormal3f(0.0f, 0.0f, 1.0f); glTexCoord2f(0.0f, 1.0f); glVertex3f(x-w/2, y-l/2, z+h/2); //tl
	glNormal3f(0.0f, 0.0f, 1.0f); glTexCoord2f(1.0f, 1.0f); glVertex3f(x+w/2, y-l/2, z+h/2); //tr
	glNormal3f(0.0f, 0.0f, 1.0f); glTexCoord2f(1.0f, 0.0f); glVertex3f(x+w/2, y+l/2, z+h/2); //br
	glNormal3f(0.0f, 0.0f, 1.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(x-w/2, y+l/2, z+h/2); //bl  
}

void drawDoor(int ix, int iy, int iz, Maze::Direction direction, bool isOpen)
{
  //NOTE: I refer to width, length an height as the measurements along the x-, y-, and z-axis respectively

  static const float CELL_WIDTH  = 1.0f;
  static const float CELL_LENGTH = 1.0f;
  static const float CELL_HEIGHT = 1.3f;  

  static const float WALL_THICKNESS = 0.12f;
  static const float WALL_LENGTH    = 1.0f;
  static const float WALL_HEIGHT    = 1.3f;
  
  float w, l;
  float h = isOpen? WALL_HEIGHT*0.2 : WALL_HEIGHT;
  

  //calculate world-position according to cell index positions
  float x = ix + 0.5f;
  float y = -(iy)  - 0.5f; //negative, because graphics y-axis direction is opposite from "array index y-axis".
  float z = WALL_HEIGHT - h/2.0f; //(NOTE: this would be different if player can see bottom\top levels)

  //reposition & calculate dimensions
  switch(direction)
  {
  case Maze::NORTH:
    y += CELL_LENGTH/2; //top wall (using opengl axis)
    w = WALL_LENGTH;
    l = WALL_THICKNESS;
    break;
  case Maze::SOUTH:
    y -= CELL_LENGTH/2;
    w = WALL_LENGTH;
    l = WALL_THICKNESS;
    break;
  case Maze::WEST:
    x -= CELL_WIDTH/2;
    w = WALL_THICKNESS;
    l = WALL_LENGTH;
    break;
  case Maze::EAST:
    x += CELL_WIDTH/2;
    w = WALL_THICKNESS;
    l = WALL_LENGTH;
    break;
  default:
    return;
  }

	//Back quad                    
	glNormal3f(0.0f, 1.0f, 0.0f); glTexCoord2f(0.0f, 1.0f); glVertex3f(x+w/2, y+l/2, z+h/2); //tl (note: clockwise as seen from back)
	glNormal3f(0.0f, 1.0f, 0.0f); glTexCoord2f(1.0f, 1.0f); glVertex3f(x-w/2, y+l/2, z+h/2); //tr
	glNormal3f(0.0f, 1.0f, 0.0f); glTexCoord2f(1.0f, 0.0f); glVertex3f(x-w/2, y+l/2, z-h/2); //br
	glNormal3f(0.0f, 1.0f, 0.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(x+w/2, y+l/2, z-h/2); //bl
	
	//Left quad  			          
	glNormal3f(-1.0f, 0.0f, 0.0f); glTexCoord2f(0.0f, 1.0f); glVertex3f(x-w/2, y-l/2, z+h/2); //tl (note: clockwise as seen from the left)
	glNormal3f(-1.0f, 0.0f, 0.0f); glTexCoord2f(1.0f, 1.0f); glVertex3f(x-w/2, y+l/2, z+h/2); //tr
	glNormal3f(-1.0f, 0.0f, 0.0f); glTexCoord2f(1.0f, 0.0f); glVertex3f(x-w/2, y+l/2, z-h/2); //br
	glNormal3f(-1.0f, 0.0f, 0.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(x-w/2, y-l/2, z-h/2); //bl
	
	//Right quad  			          
	glNormal3f(1.0f, 0.0f, 0.0f); glTexCoord2f(0.0f, 1.0f); glVertex3f(x+w/2, y+l/2, z+h/2); //tl (note: clockwise as seen from the right)
	glNormal3f(1.0f, 0.0f, 0.0f); glTexCoord2f(1.0f, 1.0f); glVertex3f(x+w/2, y-l/2, z+h/2); //tr
	glNormal3f(1.0f, 0.0f, 0.0f); glTexCoord2f(1.0f, 0.0f); glVertex3f(x+w/2, y-l/2, z-h/2); //br
	glNormal3f(1.0f, 0.0f, 0.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(x+w/2, y+l/2, z-h/2); //bl
	
	//Front quad          
	glNormal3f(0.0f, -1.0f, 0.0f); glTexCoord2f(0.0f, 1.0f); glVertex3f(x-w/2, y-l/2, z+h/2); //tl (etc...)
	glNormal3f(0.0f, -1.0f, 0.0f); glTexCoord2f(1.0f, 1.0f); glVertex3f(x+w/2, y-l/2, z+h/2); //tr
	glNormal3f(0.0f, -1.0f, 0.0f); glTexCoord2f(1.0f, 0.0f); glVertex3f(x+w/2, y-l/2, z-h/2); //br
	glNormal3f(0.0f, -1.0f, 0.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(x-w/2, y-l/2, z-h/2); //bl
	
	//Top quad          
	glNormal3f(0.0f, 0.0f, 1.0f); glTexCoord2f(0.0f, 1.0f); glVertex3f(x-w/2, y-l/2, z+h/2); //tl
	glNormal3f(0.0f, 0.0f, 1.0f); glTexCoord2f(1.0f, 1.0f); glVertex3f(x+w/2, y-l/2, z+h/2); //tr
	glNormal3f(0.0f, 0.0f, 1.0f); glTexCoord2f(1.0f, 0.0f); glVertex3f(x+w/2, y+l/2, z+h/2); //br
	glNormal3f(0.0f, 0.0f, 1.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(x-w/2, y+l/2, z+h/2); //bl  
}

void LevelMap::draw()
{
  static Random random; //Note: This random number generator is used for repeatedly generating the same random
                        //       resources in the maze every frame (e.g. different wall textures etc.)
  random.reset();	  
  Maze& maze = *Maze::getInstance();
  
  //Draw Floors    
  for(int cy = 0; cy < maze.getHeight(); cy++)
  {
    for(int cx = 0; cx < maze.getWidth(); cx++)
    {
      //Set texture
      glBindTexture(GL_TEXTURE_2D, floorTextures[random.generate(0,3)]);
      glBegin(GL_QUADS);

      //calculate position
      float x = cx;// - (maze.getWidth()-1)/2.0f*CELL_WIDTH;
      float y = -(cy);// - (maze.getHeight()-1)/2.0f*CELL_LENGTH); //negative, because graphics y-axis direction is opposite from "array index y-axis".
      
      //draw
      glNormal3f(0.0f, 0.0f, 1.0f); glTexCoord2f(0.0f, 1.0f); glVertex3f(x     , y     , 0.0f); //random.generate(0.0f,0.2f)); //tl
			glNormal3f(0.0f, 0.0f, 1.0f); glTexCoord2f(1.0f, 1.0f); glVertex3f(x+1.0f, y     , 0.0f); //random.generate(0.0f,0.2f)); //tr
			glNormal3f(0.0f, 0.0f, 1.0f); glTexCoord2f(1.0f, 0.0f); glVertex3f(x+1.0f, y-1.0f, 0.0f); //random.generate(0.0f,0.2f)); //br
			glNormal3f(0.0f, 0.0f, 1.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(x     , y-1.0f, 0.0f); //random.generate(0.0f,0.2f)); //bl

      glEnd();
    }
  }

  //Draw Walls      
  #ifndef _WIN32
  int cy, cx;
  #endif	

  //TODO: Perhaps change to "scanline circle" algorithm (where only a circular portion around the player is rendered)
  //TODO: Can optimize a LOT by bundling wall textures into one and simply generating different uv's.
  for(cy = 0; cy < maze.getHeight(); cy++)
		for(int cx = 0; cx < maze.getWidth(); cx++)
		{    
      //draw breakable walls
      {
        glBindTexture(GL_TEXTURE_2D, wallTextures[random.generate(0,5)]);
        glBegin(GL_QUADS);

        //up
        if(!maze.isPassable(cx,cy,0,Maze::NORTH))
          if(maze.isBreakable(cx,cy,0,Maze::NORTH))
            if(!maze.isDoor(cx,cy,0,Maze::NORTH))
              drawWall(cx,cy,0,Maze::NORTH);

        //left
        if(!maze.isPassable(cx,cy,0,Maze::WEST))
          if(maze.isBreakable(cx,cy,0,Maze::WEST))
            if(!maze.isDoor(cx,cy,0,Maze::WEST))
              drawWall(cx,cy,0,Maze::WEST);      
      
        //down (only for last cells)
        if(cy == maze.getHeight()-1)
          if(!maze.isPassable(cx,cy,0,Maze::SOUTH))
            if(maze.isBreakable(cx,cy,0,Maze::SOUTH))
              if(!maze.isDoor(cx,cy,0,Maze::SOUTH))
                drawWall(cx,cy,0,Maze::SOUTH);

        //right (only for last cells)
        if(cx == maze.getWidth()-1)
          if(!maze.isPassable(cx,cy,0,Maze::EAST))
            if(maze.isBreakable(cx,cy,0,Maze::EAST))
              if(!maze.isDoor(cx,cy,0,Maze::EAST))
                drawWall(cx,cy,0,Maze::EAST);
        glEnd();
      }

      //draw solid walls
      {
        glBindTexture(GL_TEXTURE_2D, indestructableWallTexture);
        glBegin(GL_QUADS);

        //up      
        if(!maze.isPassable(cx,cy,0,Maze::NORTH))
          if(!maze.isBreakable(cx,cy,0,Maze::NORTH))
            if(!maze.isDoor(cx,cy,0,Maze::NORTH))
              drawWall(cx,cy,0,Maze::NORTH);

        //left
        if(!maze.isPassable(cx,cy,0,Maze::WEST))
          if(!maze.isBreakable(cx,cy,0,Maze::WEST))
            if(!maze.isDoor(cx,cy,0,Maze::WEST))
              drawWall(cx,cy,0,Maze::WEST);      
      
        //down (only for last cells)
        if(cy == maze.getHeight()-1)
          if(!maze.isPassable(cx,cy,0,Maze::SOUTH))
            if(!maze.isBreakable(cx,cy,0,Maze::SOUTH))
              if(!maze.isDoor(cx,cy,0,Maze::SOUTH))
                drawWall(cx,cy,0,Maze::SOUTH);

        //right (only for last cells)
        if(cx == maze.getWidth()-1)
          if(!maze.isPassable(cx,cy,0,Maze::EAST))
            if(!maze.isBreakable(cx,cy,0,Maze::EAST))
              if(!maze.isDoor(cx,cy,0,Maze::EAST))
                drawWall(cx,cy,0,Maze::EAST);
        glEnd();
      }
      
      //draw doors (note: don't have to consider boundary cases)
      {
        glBindTexture(GL_TEXTURE_2D, doorTexture);
        glBegin(GL_QUADS);

        //up              
        if(maze.isDoor(cx,cy,0,Maze::NORTH))
          drawDoor(cx,cy,0,Maze::NORTH,maze.isOpen(cx,cy,0,Maze::NORTH));

        //left       
        if(maze.isDoor(cx,cy,0,Maze::WEST))
          drawDoor(cx,cy,0,Maze::WEST,maze.isOpen(cx,cy,0,Maze::WEST));      
       
        glEnd();
      }      
    }    

  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING); 
  glDisable(GL_LIGHT1);
}

