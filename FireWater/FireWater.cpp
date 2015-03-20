/*
	Fire and Water Exhibit
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glut.h>
#include <math.h>

#pragma comment(lib,"glut32.lib")

#define ESCAPE 		27				// Define ASCII value for special keystrokes.

#define MAP_DIM 	24
#define MAP_SCALE	2

#define VEC_COUNT   5

#define TEX_COUNT	4
#define TEX_SCALE	0.5

#define W_WIDTH 	640
#define W_HEIGHT	480
#define W_FOV		45.0

#define T_CEMENT	0				// Easy indexing into texture array
#define T_LAVA		1
#define T_ROCK		2
#define T_WATER		3

struct MY_IMG {
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
};

struct MY_ANIM {
	GLuint x;
	GLuint y;
	GLfloat z;
};

GLfloat g_rot = 0;
GLfloat g_the = 30;
MY_IMG g_TexDat[TEX_COUNT] = {0};			 // Array with image texture information
GLuint g_TexArr[TEX_COUNT] = {0};			 // Array to hold OpenGL texture information
GLuint g_seed = 0;

GLfloat g_map[MAP_DIM][MAP_DIM] = {0};
GLfloat g_ground[MAP_DIM][MAP_DIM] = {0};

MY_ANIM g_vectors[VEC_COUNT] = {0};
MY_ANIM g_bubbles[VEC_COUNT] = {0};

GLUquadricObj* g_quadric;

bool g_lava = true;

// Texture filenames list
char* g_TexFN[TEX_COUNT] =
{
	"cement.bmp",
	"lava.bmp",
	"rock.bmp",
	"water.bmp"
};

// Loads bitmap data into the Image data structure for use in texturing.
int ImageLoad(const char* _filename, MY_IMG* _img)
{

    FILE* _filePTR;
    unsigned long _isize;
    unsigned long i;
    unsigned short int planes;
    unsigned short int bpp;
    char temp, _finalFN[80];

	strcpy(_finalFN, "" );
	strcat(_finalFN, _filename );

    if ((_filePTR = fopen(_finalFN, "rb"))==NULL)
    {
	printf("File Not Found : %s\n",_finalFN);
	return 0;
    }

    fseek(_filePTR, 18, SEEK_CUR);

    if ((i = fread(&_img->sizeX, 4, 1, _filePTR)) != 1)
    {
	printf("Error reading width from %s.\n", _finalFN);
	return 0;
    }

    if ((i = fread(&_img->sizeY, 4, 1, _filePTR)) != 1)
    {
	printf("Error reading height from %s.\n", _finalFN);
	return 0;
    }

    _isize = _img->sizeX * _img->sizeY * 3;

    if ((fread(&planes, 2, 1, _filePTR)) != 1)
    {
	printf("Error reading planes from %s.\n", _finalFN);
	return 0;
    }

    if (planes != 1)
    {
	printf("Planes from %s is not 1: %u\n", _finalFN, planes);
	return 0;
    }

    if ((i = fread(&bpp, 2, 1, _filePTR)) != 1)
    {
	printf("Error reading bpp from %s.\n", _finalFN);
	return 0;
    }

    if (bpp != 24)
    {
	printf("Bpp from %s is not 24: %u\n", _finalFN, bpp);
	return 0;
    }

    fseek(_filePTR, 24, SEEK_CUR);

    _img->data = (char *) malloc(_isize);
    if (_img->data == NULL) {
		printf("Error allocating memory for color-corrected _img data");
		return 0;
    }

    if ((i = fread(_img->data, _isize, 1, _filePTR)) != 1) {
		printf("Error reading _img data from %s.\n", _finalFN);
		return 0;
    }

    for (i=0;i<_isize;i+=3) {
		temp = _img->data[i];
		_img->data[i] = _img->data[i+2];
		_img->data[i+2] = temp;
    }

    return 1;
}

// Load Bitmaps And Convert To Textures
void LoadGLTextures()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (int k=0; k < TEX_COUNT; k++)
	{
	    if (!ImageLoad(g_TexFN[k], &g_TexDat[k]))
		exit(1);

	    glGenTextures(1, &g_TexArr[k]);
	    glBindTexture(GL_TEXTURE_2D, g_TexArr[k]);

	    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, g_TexDat[k].sizeX, g_TexDat[k].sizeY, GL_RGB, GL_UNSIGNED_BYTE, g_TexDat[k].data);
	}
}

// Switches from perspective to orthographic projection.
// (0,0) is situated at the top left of the screen.
void beginOrtho()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, W_WIDTH, 0, W_HEIGHT);
	glScalef(1, -1, 1);
	glTranslatef(0, -1.0*W_HEIGHT, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Switch back from orthographic to perspective projection.
void endOrtho()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

// Draws a text string on screen
void drawString(float x, float y, char* _str)
{
	beginOrtho();

	if(strlen(_str) < 1)
		return;
	// Complete this function to draw any length string on screen, starting at the given X,Y coordinates.
	// Strings should be terminated by the NULL character ('\0')
	int c = 0;
	glRasterPos2f(x,y);

	while(_str[c] > 0)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (GLint) _str[c]);
		++c;
	}

	endOrtho();
}

// Renders output to screen
void display(void)
{
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();	// Saves current state
    {
		// Positions the surface in front of the default camera
		glTranslatef(0.0,0.0, -3.5);

		// Initial rotation corresponds to isometric drawing angle (30 degrees around x-axis)
		glRotatef(g_the, 1.0, 0.0, 0.0);

		// Initial rotation of 45 degrees around y-axis
		glRotatef(g_rot, 0.0, 1.0,0.0);

		/*------------------------------------------------------
		*		Draw the Land and Water
		*------------------------------------------------------*/
		double _mapWidth = MAP_SCALE;
		double _mapLength = MAP_SCALE;
		double _MWstep = _mapWidth / MAP_DIM;
		double _MLstep = _mapLength / MAP_DIM;
		double _startX = 0 - (_mapWidth / MAP_SCALE);
		double _startZ = 0 - (_mapLength / MAP_SCALE);
		double _endX   = 0 + (_mapWidth / MAP_SCALE);
		double _endZ   = 0 + (_mapLength / MAP_SCALE);
		double _currX = _startX;
		double _currZ = _startZ;

		for (int i=0; i < MAP_DIM - 1; i++)
		{
			_currZ = _startZ;
			for (int j=0; j < MAP_DIM - 1; j++)
			{
				if(g_lava)
					glBindTexture(GL_TEXTURE_2D, g_TexArr[T_LAVA]);
				else
					glBindTexture(GL_TEXTURE_2D, g_TexArr[T_WATER]);

				// The fluid (water/lava) surface
				glBegin(GL_QUADS);
				{
					glTexCoord2f((_currX) / TEX_SCALE,(_currZ + 0) / TEX_SCALE);
					glVertex3f(_currX,		 g_map[i][j],	_currZ);		// 1

					glTexCoord2f((_currX + _MWstep) / TEX_SCALE,(_currZ + 0) / TEX_SCALE);
					glVertex3f(_currX + _MWstep, g_map[i+1][j], _currZ);		// 2

					glTexCoord2f((_currX + _MWstep) / TEX_SCALE,(_currZ + _MLstep) / TEX_SCALE);
					glVertex3f(_currX + _MWstep, g_map[i+1][j+1], _currZ + _MLstep);	// 3

					glTexCoord2f((_currX) / TEX_SCALE,(_currZ + _MLstep) / TEX_SCALE);
					glVertex3f(_currX,		 g_map[i][j+1],	  _currZ + _MLstep);	// 4
				}
				glEnd();

				if(g_lava)
					glBindTexture(GL_TEXTURE_2D, g_TexArr[T_ROCK]);
				else
					glBindTexture(GL_TEXTURE_2D, g_TexArr[T_CEMENT]);

				// The ground/rock surface
				glBegin(GL_QUADS);
				{
					glTexCoord2f((_currX) / TEX_SCALE,(_currZ) / TEX_SCALE);
					glVertex3f(_currX,		 g_ground[i][j],	_currZ);		// 1

					glTexCoord2f((_currX +_MWstep) / TEX_SCALE,(_currZ) / TEX_SCALE);
					glVertex3f(_currX + _MWstep, g_ground[i+1][j],	_currZ);		// 2

					glTexCoord2f((_currX +_MWstep) / TEX_SCALE,(_currZ + _MLstep) / TEX_SCALE);
					glVertex3f(_currX + _MWstep, g_ground[i+1][j+1],	_currZ + _MLstep);	// 3

					glTexCoord2f((_currX) / TEX_SCALE,(_currZ + _MLstep) / TEX_SCALE);
					glVertex3f(_currX,		 g_ground[i][j+1],	_currZ + _MLstep);	// 4
				}
				glEnd();
				_currZ += _MLstep;
			}
			_currX += _MWstep;
		}

		/*------------------------------------------------------
		*		Draw the Bubbles
		*------------------------------------------------------*/
		for(int v = 0; v < VEC_COUNT; ++v)
		{
			if(g_lava)
			{
				glBindTexture(GL_TEXTURE_2D, g_TexArr[T_LAVA]);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, g_TexArr[T_WATER]);
			}

			MY_ANIM& _bub = g_bubbles[v];

			if(_bub.z != 0)
			{
				glPushMatrix();
					glTranslatef((_bub.x * _MWstep) + _startX,_bub.z,(_bub.y * _MLstep) + _startZ);
					gluSphere(g_quadric,0.07f,6,6);
				glPopMatrix();
			}
		}

		/*------------------------------------------------------
		*		Draw the Container
		*------------------------------------------------------*/
		if(g_lava)
			glBindTexture(GL_TEXTURE_2D, g_TexArr[T_ROCK]);
		else
			glBindTexture(GL_TEXTURE_2D, g_TexArr[T_CEMENT]);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0.0f, 0.0f);		glVertex3f(_startX,-0.5f, _startZ);
			glTexCoord2f(0.0f, 1.0f);		glVertex3f(_startX,-0.5f, _endZ  );
			glTexCoord2f(1.0f, 1.0f);		glVertex3f(_startX, 0.5f, _endZ  );
			glTexCoord2f(1.0f, 0.0f);		glVertex3f(_startX, 0.5f, _startZ);

			glTexCoord2f(0.0f, 0.0f);		glVertex3f(_endX  ,-0.5f, _startZ);
			glTexCoord2f(0.0f, 1.0f);		glVertex3f(_endX  ,-0.5f, _endZ  );
			glTexCoord2f(1.0f, 1.0f);		glVertex3f(_endX  , 0.5f, _endZ  );
			glTexCoord2f(1.0f, 0.0f);		glVertex3f(_endX  , 0.5f, _startZ);

			glTexCoord2f(0.0f, 0.0f);		glVertex3f(_startX,-0.5f, _startZ);
			glTexCoord2f(0.0f, 1.0f);		glVertex3f(_startX, 0.5f, _startZ);
			glTexCoord2f(1.0f, 1.0f);		glVertex3f(_endX  , 0.5f, _startZ);
			glTexCoord2f(1.0f, 0.0f);		glVertex3f(_endX  ,-0.5f, _startZ);

			glTexCoord2f(0.0f, 0.0f);		glVertex3f(_startX,-0.5f, _endZ  );
			glTexCoord2f(0.0f, 1.0f);		glVertex3f(_startX, 0.5f, _endZ  );
			glTexCoord2f(1.0f, 1.0f);		glVertex3f(_endX  , 0.5f, _endZ  );
			glTexCoord2f(1.0f, 0.0f);		glVertex3f(_endX  ,-0.5f, _endZ  );

			glTexCoord2f(0.0f, 0.0f);		glVertex3f(_startX,-0.5f, _startZ);
			glTexCoord2f(0.0f, 1.0f);		glVertex3f(_startX,-0.5f, _endZ  );
			glTexCoord2f(1.0f, 1.0f);		glVertex3f(_endX  ,-0.5f, _endZ  );
			glTexCoord2f(1.0f, 0.0f);		glVertex3f(_endX  ,-0.5f, _startZ);
		}
		glEnd();
    }

    glPopMatrix(); // Restores previously saved state

	if(g_lava)
	{
		drawString(250,40,"Lava Exhibition");
	}
	else
	{
		drawString(230,40,"Water Exhibition");
	}

    glutSwapBuffers();
    glFlush();
}



// Handles all normal keyboard input
void keys(unsigned char key, int x, int y)
{

	// Insert your keyboard handling code here.
	switch(key)
	{
		case 'a':
			g_rot += 1;
			break;
		case 'd':
			g_rot -= 1;
			break;
		case 'w':
			g_the += 1;
			break;
		case 's':
			g_the -= 1;
			break;
	};


	glutPostRedisplay();	// Sends signal to redraw the viewport
							// This should be the last call in this
							// function.

}




// Handles basic initialisation and setup of OpenGL parameters.
void myinit()
{

    glClearColor(0.2,0.2,0.2,1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glLineWidth(2.1f);

    LoadGLTextures();

	g_quadric = gluNewQuadric();
	gluQuadricDrawStyle(g_quadric,GLU_FILL);
	gluQuadricNormals(g_quadric,GLU_SMOOTH);
	gluQuadricTexture(g_quadric,GLU_TRUE);
}




// Rendered window's reshape function - manages changes to size of the
// window, correctly adjusting aspect ratio.
void changeParentWindow(GLsizei width, GLsizei height)
{

	if (height == 0) height = 1;
	glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Perspective projection
	gluPerspective(W_FOV, (GLfloat)width/height, 0.1, 50.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}


// Returns a random double value between the provided bounds
double getRandomDouble(double Low, double High)
{
    return ((double) rand() / (RAND_MAX+1.0)) * (High-Low) + Low;
}


// Creates a randomly perturbed fluid surface, and a more structured rock/ground surface.
void setupHeights()
{
    for (int i=0; i < MAP_DIM; i++)
    {
		for (int j=0; j < MAP_DIM; j++)
		{
			g_map[i][j] = getRandomDouble(0.0, 0.1);
			g_ground[i][j] = sin(0.25*(sqrt((double)(i)*(i) + (j)*(j)))) / 4.0 - 0.075;
		}
    }
}

void setPt(GLint x, GLint y, GLfloat z)
{
	if((x < 0) || (y < 0) || (x > MAP_DIM) || (y > MAP_DIM))
		return;

	g_map[x][y] = z;
}

void animPt(GLint x, GLint y, GLfloat z)
{
	if((x < 0) || (y < 0) || (x > MAP_DIM) || (y > MAP_DIM))
		return;

	g_map[x][y] += z;
}

void idle()
{

	/*-------------------------------------------
			Animate Liquid
	--------------------------------------------*/
	srand(g_seed++);
	for (int v=0; v < VEC_COUNT; v++)
    {
		MY_ANIM& _vec = g_vectors[v];
		GLfloat& _mPT = g_map[_vec.x][_vec.y];
		if(_vec.z == 0)
		{
			_vec.x = rand() % MAP_DIM;
			_vec.y = rand() % MAP_DIM;
			_vec.z = getRandomDouble(0.01,0.03);
		}
		else if(_mPT > 0.3f)
		{
			// Spawn Bubble
			g_bubbles[v] = _vec;
			g_bubbles[v].z = _mPT;
			// Reverse Vector
			_mPT = 0.19f;
			_vec.z = -0.03f;
		}
		else if(_mPT < -0.2f)
		{
			// Kill Vector
			_mPT = -0.09;
			_vec.z = 0;
		}
		else
		{
			// Add Vector to main then teriatary
			_mPT += _vec.z;

			animPt((_vec.x  ), (_vec.y-1), (_vec.z / 2));
			animPt((_vec.x  ), (_vec.y+1), (_vec.z / 2));
			animPt((_vec.x-1), (_vec.y  ), (_vec.z / 2));
			animPt((_vec.x+1), (_vec.y  ), (_vec.z / 2));
			animPt((_vec.x+1), (_vec.y-1), (_vec.z / 3));
			animPt((_vec.x+1), (_vec.y+1), (_vec.z / 3));
			animPt((_vec.x-1), (_vec.y-1), (_vec.z / 3));
			animPt((_vec.x-1), (_vec.y+1), (_vec.z / 3));
			animPt((_vec.x  ), (_vec.y-2), (_vec.z / 3));
			animPt((_vec.x  ), (_vec.y+2), (_vec.z / 3));
			animPt((_vec.x-2), (_vec.y  ), (_vec.z / 3));
			animPt((_vec.x+2), (_vec.y  ), (_vec.z / 3));
			animPt((_vec.x+1), (_vec.y-2), (_vec.z / 4));
			animPt((_vec.x+1), (_vec.y+2), (_vec.z / 4));
			animPt((_vec.x-2), (_vec.y+1), (_vec.z / 4));
			animPt((_vec.x+2), (_vec.y+1), (_vec.z / 4));
			animPt((_vec.x-1), (_vec.y-2), (_vec.z / 4));
			animPt((_vec.x-1), (_vec.y+2), (_vec.z / 4));
			animPt((_vec.x-2), (_vec.y-1), (_vec.z / 4));
			animPt((_vec.x+2), (_vec.y-1), (_vec.z / 4));
		}
    }
	/*-------------------------------------------
			Animate Bubbles
	--------------------------------------------*/
	for (int v=0; v < VEC_COUNT; v++)
	{
		MY_ANIM& _bubble = g_bubbles[v];
		if(_bubble.z != 0)
		{
			_bubble.z += 0.01f;
		}
	}

	/*-------------------------------------------
			IF WATER SMOOTH
	--------------------------------------------*/
	if(!g_lava)
	{
		for (int i=0; i < MAP_DIM; i++)
		{
			for (int j=0; j < MAP_DIM; j++)
			{
				GLfloat& _point = g_map[i][j];
				_point -= (_point / 100.0f);
			}
		}
	}

	glutPostRedisplay();
}

void rightMenu(int id)
{
	switch(id)
	{
		case 1: g_lava = true; break;
		case 2: g_lava = false; break;
	};
}

// Main function (creates the OpenGL window, sets up OpenGL parameters
// and calls the display function.
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(W_WIDTH,W_HEIGHT);
    glutCreateWindow("Fire and Water - Claire Blackshaw");
    glutReshapeFunc(changeParentWindow);
    glutDisplayFunc(display);
    glutKeyboardFunc(keys);
    glutIdleFunc(idle);

	glutCreateMenu(rightMenu);
	glutAddMenuEntry("Lava",1);
	glutAddMenuEntry("Water",2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

    myinit();

    setupHeights();

    glutMainLoop();
}
