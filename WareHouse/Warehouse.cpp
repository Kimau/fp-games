//-----------------------------------------------------------
//	Warehouse
//-----------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glut.h>
#include <math.h>

#define ESCAPE 		27				// Define ASCII value for special keystrokes.

#define W_WIDTH 	640
#define W_HEIGHT	480
#define W_FOV		45.0

#define TEX_COUNT	3

#define K_LEFT			100
#define K_UP			101
#define K_RIGHT			102
#define K_DOWN			103

//---------- BEGIN WAREHOUSE DEFINES --------------
#define L_GRID		101
#define L_FLOOR		102
#define L_CUBE		103
#define L_AVATAR	104

#define V_FIRST 	201
#define V_THIRD 	203

#define BOX_COUNT	10
//----------  END WAREHOSE DEFINES --------------

struct MY_IMG {
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
};

char* g_TexFN[TEX_COUNT] = {
	"wall.bmp",
	"floor.bmp",
	"ava.bmp"};

GLfloat g_Cam[6] = {-5.0f,-5.0f,5.0f,  0,0,0};

MY_IMG g_TexDat[TEX_COUNT] = {0};			 // Array with image texture information
GLuint g_TexArr[TEX_COUNT] = {0};			 // Array to hold OpenGL texture information
GLuint g_seed = 0;

//---------- BEGIN WAREHOUSE VARIBLES --------------
GLfloat g_Ava[3] = {0.0f,0.0f,0.0f};
GLfloat g_Move[2] = {0.0f,0.0f};
GLfloat g_Pixie[3] = {0.0f,0.0f,0.0f};

GLfloat g_Boxes[BOX_COUNT*2] =
	{
		0,0, 0,0, 0,0, 0,0, 0,0,	0,0, 0,0, 0,0, 0,0, 0,0
	};
int g_Mode = V_THIRD;
int g_Floor = L_FLOOR;
GLUquadricObj* g_quadric;
//----------  END  WAREHOUSE VARIBLES --------------

int ImageLoad(const char* _filename, MY_IMG* _img);

void myinit();
void changeParentWindow(GLsizei width, GLsizei height);

void display(void);
void beginOrtho();
void endOrtho();
void drawString(float x, float y, char* _str);

void keys(unsigned char key, int x, int y);

double getRandomDouble(double Low, double High);
void idle();
void timer(int v);
void rightMenu(int _id);

//=======================================================================================================
//=======================================================================================================
//=======================================================================================================

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
	glCallList(L_FLOOR);

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
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_LIGHTING_BIT | GL_ENABLE_BIT);

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

	glColor3f(1.0f,1.0f,1.0f);

	while(_str[c] > 0)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (GLint) _str[c]);
		++c;
	}

	glPopAttrib();

	endOrtho();
}

// Renders output to screen
void display(void)
{
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//------------------------------------
	//  BEGIN WAREHOUSE
	//------------------------------------

	//-----------------------------
	//	Move Camera
	//-----------------------------
	if(g_Mode == V_FIRST)
	{
		g_Cam[0] = g_Ava[0];
		g_Cam[1] = g_Ava[1];
		g_Cam[2] = 0.3f;

		g_Cam[3] = g_Ava[0] - (2.5f * cosf(g_Ava[2]));
		g_Cam[4] = g_Ava[1] - (2.5f * sinf(g_Ava[2]));
		g_Cam[5] = 0.3f;
	}
	else
	{
		g_Cam[0] = g_Ava[0] + (2.5f * cosf(g_Ava[2]));
		g_Cam[1] = g_Ava[1] + (2.5f * sinf(g_Ava[2]));
		g_Cam[2] = 1.5f;

		g_Cam[3] = g_Ava[0];
		g_Cam[4] = g_Ava[1];
		g_Cam[5] = 0.3f;
	}
	gluLookAt(g_Cam[0],g_Cam[1],g_Cam[2],g_Cam[3],g_Cam[4],g_Cam[5],0,0,1);

	//-----------------------------
	//	Add Light
	//-----------------------------
	{
		GLfloat _temp[4] = {g_Ava[0],g_Ava[1],0.25f,1};
		glLightfv(GL_LIGHT0,GL_POSITION,_temp);
	}

	{
		GLfloat _temp[4] = {g_Pixie[0],g_Pixie[1],0.02f,1};
		glLightfv(GL_LIGHT1,GL_POSITION,_temp);
	}


	if(g_Mode != V_FIRST)
	{
		glPushMatrix();
			glTranslatef(g_Ava[0],g_Ava[1],0.3f);
			glCallList(L_AVATAR);
		glPopMatrix();
	}

	// Draw Floor
	glCallList(g_Floor);

	// Draw Maze
	for(int c = 0; c < BOX_COUNT*2; ++c)
	{
		glPushMatrix();
			glTranslatef(g_Boxes[c*2],g_Boxes[c*2 + 1], 0);
			glCallList(L_CUBE);
		glPopMatrix();
	}

	//------------------------------------
	//  END WAREHOUSE
	//------------------------------------

    glutSwapBuffers();
    glFlush();
}

void calcAva()	// WAREHOUSE SPECIFIC
{
	g_Move[0] = cosf(g_Ava[2]);
	g_Move[1] = sinf(g_Ava[2]);
}

// Handles all normal keyboard input
void keys(unsigned char key, int x, int y)
{

	// Insert your keyboard handling code here.
	switch(key)
	{
		case 'a':	g_Ava[2] += 0.1f;	if((g_Move[0] + g_Move[1]) != 0)	calcAva();	break;
		case 'd':	g_Ava[2] -= 0.1f;	if((g_Move[0] + g_Move[1]) != 0)	calcAva();	break;
		case 'w':	calcAva();	break;
		case 's':	g_Move[0] = 0.0f;				g_Move[1] = 0.0f; 			break;
	};

	glutPostRedisplay();	// Sends signal to redraw the viewport
							// This should be the last call in this
							// function.
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

void idle()
{
	g_Pixie[2] += 0.1f;
	g_Pixie[0] = g_Ava[0] + (2*cosf(g_Pixie[2]));
	g_Pixie[1] = g_Ava[1] + (2*sinf(g_Pixie[2]));
	glutPostRedisplay();
}

void timer(int v)
{
	g_Ava[0] -= g_Move[0] * 0.1f;
	g_Ava[1] -= g_Move[1] * 0.1f;

	glutTimerFunc(10,timer,0);
}

void rightMenu(int _id)
{
	switch(_id)
	{
		case L_FLOOR:		g_Floor = L_FLOOR;		break;
		case L_GRID:		g_Floor = L_GRID;		break;
		case V_FIRST:		g_Mode = V_FIRST;		break;
		case V_THIRD:		g_Mode = V_THIRD;		break;
	};
}

// Handles basic initialisation and setup of OpenGL parameters.
void myinit()
{
    glClearColor(0.2,0.2,0.2,1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
	LoadGLTextures();

	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
    glLineWidth(2.1f);

	//------------------------------------
	//  BEGIN WAREHOUSE
	//------------------------------------
	for(int c = 0; c < BOX_COUNT; ++c)
	{
		g_Boxes[c*2    ] = (rand() % 20) - 10;
		g_Boxes[c*2 + 1] = (rand() % 20) - 10;
	}

	//-----------------------------------
	//		Make Point Light
	//-----------------------------------
	{
		GLfloat _amb[4]	 = {0.0f,0.0f,0.0f,0.0f};
		GLfloat _diff[4] = {1.0f,1.0f,1.0f,1.0f};

		glLightfv(GL_LIGHT0,GL_AMBIENT,_amb);
		glLightfv(GL_LIGHT0,GL_DIFFUSE,_diff);

		glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,180.0f);
		glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,0);
		glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,0);
		glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,0.1f);

		glEnable(GL_LIGHT0);
	}

	{
		GLfloat _amb[4]	 = {0,0,0,0};
		GLfloat _spec[4] = {1,1,1,1};
		GLfloat _diff[4] = {1,0,0,1};

		glLightfv(GL_LIGHT1,GL_AMBIENT,_amb);
		glLightfv(GL_LIGHT1,GL_DIFFUSE,_diff);
		glLightfv(GL_LIGHT1,GL_SPECULAR,_amb);

		glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,180.0f);
		glLightf(GL_LIGHT1,GL_CONSTANT_ATTENUATION,0);
		glLightf(GL_LIGHT1,GL_LINEAR_ATTENUATION,0);
		glLightf(GL_LIGHT1,GL_QUADRATIC_ATTENUATION,0.5f);

		glEnable(GL_LIGHT1);
	}

	//-----------------------------------
	//	Draw Grid
	//-----------------------------------
	glGenLists(4);
	glNewList(L_GRID,GL_COMPILE);
		glPushAttrib(GL_LIGHTING_BIT | GL_COLOR_BUFFER_BIT);

		glDisable(GL_DITHER);
		glDisable(GL_LIGHTING);

		glLineWidth(1.2f);
		glColor3f(0.0f,1.0f,0.0f);

		glBegin(GL_LINES);
			for(float _start = -10.0f; _start < 10.1f; _start += 0.5f)
			{
				glVertex3f(-10.0f,_start,0.0f);
				glVertex3f( 10.0f,_start,0.0f);

				glVertex3f(_start,-10.0f,0.0f);
				glVertex3f(_start, 10.0f,0.0f);
			}
		glEnd();

		glPopAttrib();
	glEndList();
	//-----------------------------------
	//	Draw Floor
	//-----------------------------------
	glNewList(L_FLOOR,GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, g_TexArr[1]);

		glColor3f(1.0f,1.0f,1.0f);
		glBegin(GL_QUADS);
			for(float _sX = -10.0f; _sX < 10.0f; _sX += 0.5f)
			for(float _sY = -10.0f; _sY < 10.0f; _sY += 0.5f)
			{
				glNormal3f(0,0,1);
				glTexCoord2f(0,0);	glVertex3f(_sX+0.0f,_sY+0.0f,0.0f);
				glTexCoord2f(0,1);	glVertex3f(_sX+0.0f,_sY+0.5f,0.0f);
				glTexCoord2f(1,1);	glVertex3f(_sX+0.5f,_sY+0.5f,0.0f);
				glTexCoord2f(1,0);	glVertex3f(_sX+0.5f,_sY+0.0f,0.0f);
			}
		glEnd();
	glEndList();

	//-----------------------------------
	//	Draw Cube
	//-----------------------------------
	glNewList(L_CUBE,GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, g_TexArr[0]);

		glColor3f(1.0f,1.0f,1.0f);
		glBegin(GL_QUADS);
			glNormal3f(0,0, 1);
			glTexCoord2f(0.0f,0.0f);	glVertex3f(0.0f,0.0f,0.5f); // Draw Top
			glTexCoord2f(0.0f,1.0f);	glVertex3f(0.0f,0.5f,0.5f);
			glTexCoord2f(1.0f,1.0f);	glVertex3f(0.5f,0.5f,0.5f);
			glTexCoord2f(1.0f,0.0f);	glVertex3f(0.5f,0.0f,0.5f);

			glNormal3f(0,0,-1);
			glTexCoord2f(0.0f,0.0f);	glVertex3f(0.0f,0.0f,0.0f); // Draw Bottom
			glTexCoord2f(0.0f,1.0f);	glVertex3f(0.0f,0.5f,0.0f);
			glTexCoord2f(1.0f,1.0f);	glVertex3f(0.5f,0.5f,0.0f);
			glTexCoord2f(1.0f,0.0f);	glVertex3f(0.5f,0.0f,0.0f);

			glNormal3f(0, 1,0);
			glTexCoord2f(0.0f,0.0f);	glVertex3f(0.0f,0.5f,0.0f); // Draw North
			glTexCoord2f(0.0f,1.0f);	glVertex3f(0.0f,0.5f,0.5f);
			glTexCoord2f(1.0f,1.0f);	glVertex3f(0.5f,0.5f,0.5f);
			glTexCoord2f(1.0f,0.0f);	glVertex3f(0.5f,0.5f,0.0f);

			glNormal3f(0,-1,0);
			glTexCoord2f(0.0f,0.0f);	glVertex3f(0.0f,0.0f,0.0f); // Draw South
			glTexCoord2f(0.0f,1.0f);	glVertex3f(0.0f,0.0f,0.5f);
			glTexCoord2f(1.0f,1.0f);	glVertex3f(0.5f,0.0f,0.5f);
			glTexCoord2f(1.0f,0.0f);	glVertex3f(0.5f,0.0f,0.0f);

			glNormal3f( 1,0,0);
			glTexCoord2f(0.0f,0.0f);	glVertex3f(0.5f,0.0f,0.0f); // Draw East
			glTexCoord2f(0.0f,1.0f);	glVertex3f(0.5f,0.0f,0.5f);
			glTexCoord2f(1.0f,1.0f);	glVertex3f(0.5f,0.5f,0.5f);
			glTexCoord2f(1.0f,0.0f);	glVertex3f(0.5f,0.5f,0.0f);

			glNormal3f(-1,0,0);
			glTexCoord2f(0.0f,0.0f);	glVertex3f(0.0f,0.0f,0.0f); // Draw West
			glTexCoord2f(0.0f,1.0f);	glVertex3f(0.0f,0.0f,0.5f);
			glTexCoord2f(1.0f,1.0f);	glVertex3f(0.0f,0.5f,0.5f);
			glTexCoord2f(1.0f,0.0f);	glVertex3f(0.0f,0.5f,0.0f);

		glEnd();
	glEndList();

	//-----------------------------------
	//	Draw Avatar
	//-----------------------------------
	glNewList(L_AVATAR,GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, g_TexArr[2]);
		g_quadric = gluNewQuadric();
		gluQuadricDrawStyle(g_quadric,GLU_FILL);
		gluQuadricNormals(g_quadric,GLU_SMOOTH);
		gluQuadricTexture(g_quadric,GLU_TRUE);
		gluSphere(g_quadric,0.2f,6,6);
	glEndList();
	//------------------------------------
	//  END WAREHOUSE
	//------------------------------------
}

// Main function (creates the OpenGL window, sets up OpenGL parameters
// and calls the display function.
int main(int argc, char **argv)
{
	srand(56);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(W_WIDTH,W_HEIGHT);
    glutCreateWindow("Warehouse of DOOOM! - Claire Blackshaw");
    glutReshapeFunc(changeParentWindow);
    glutDisplayFunc(display);
    glutKeyboardFunc(keys);
    glutIdleFunc(idle);
	glutTimerFunc(10,timer,60);

	glutCreateMenu(rightMenu);
	glutAddMenuEntry("Use Floor",L_FLOOR);
	glutAddMenuEntry("Use Grid",L_GRID);
	glutAddMenuEntry("Third Person",V_THIRD);
	glutAddMenuEntry("First Person",V_FIRST);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

    myinit();

    glutMainLoop();
}
