#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
  #include <windows.h>
#endif
#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "game.h"
#include "player.h"
#include "main.h"

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP     16

#ifdef WIN32 
  //Microsoft-specific (Comment out if not using a microsoft win32 compiler)
  #pragma comment(lib, "OpenGL32.lib")
  #pragma comment(lib, "GLu32.lib")
  #pragma comment(lib, "SDL.lib")
  //#pragma comment( lib, "GLaux.lib" )
#endif

// Main rendering surface (back-buffer)
SDL_Surface *surface;

// Release/destroy resources and restore the desktop
void quit(int returnCode)
{
	//Unload all resources in use
	Game::unload();
	
	// Shut down the window  
	SDL_Quit();

	// Exit program
	exit(returnCode);
}

// Setup gl viewport when the window size change
int resizeWindow(int width, int height)
{
	GLfloat ratio;
	
	// Protect against a divide by zero
	if(height == 0)
		height = 1;

	ratio = (GLfloat)width / (GLfloat)height;

	// Set viewport
	glViewport(0, 0, (GLint)width, (GLint)height);

	// Change to the projection matrix and set viewing volume
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();	
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	return(true);
}

void handleKeyPress(SDL_keysym *keysym)
{
	switch (keysym->sym)
	{
	case SDLK_ESCAPE:	    
		quit(0);
		break;
	case SDLK_F1:
		// Toggle fullscreen mode
		SDL_WM_ToggleFullScreen(surface);
		break;
	default:    
    		Player::getInstance()->getController()->keyPressed(keysym->sym);
		break;
	}
	return;
}

/* initGL
 *
 * general OpenGL initialization function
 */
int initGL(GLvoid)
{    
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

  glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	return(true);
}


#ifdef WIN32
  int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)	
#else
  int main(int argc, char **argv)
#endif
{    
	int videoFlags;		// SDL_SetVideoMode flags    
	int done = false; 	// main loop variable    
	SDL_Event event; 		// For collecting events    
	const SDL_VideoInfo *videoInfo;    
	int isActive = true;	// Is window active? (i.e should rendering be taking place?)

	// initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0)
	{
		fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
		quit(1);
	}    
	videoInfo = SDL_GetVideoInfo();

	if(!videoInfo)
	{
		fprintf(stderr, "Video query failed: %s\n",
			SDL_GetError());
		quit(1);
	}

	videoFlags  = SDL_OPENGL;          // Enable OpenGL in SDL
	videoFlags |= SDL_GL_DOUBLEBUFFER; // Enable double buffering
	videoFlags |= SDL_HWPALETTE;       // Store the palette in hardware
	videoFlags |= SDL_RESIZABLE;       // Enable window resizing

	// This checks to see if surfaces can be stored in memory
	if(videoInfo->hw_available)
	  videoFlags |= SDL_HWSURFACE;
	else
	  videoFlags |= SDL_SWSURFACE;

	// This checks if hardware blits can be done
	if(videoInfo->blit_hw)
		videoFlags |= SDL_HWACCEL;

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// get a SDL surface
	surface = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
				videoFlags);

	// Verify there is a surface
	if(!surface)
	{
		#ifdef DEBUG_MODE
			fprintf(stderr,  "Video mode set failed: %s\n", SDL_GetError());
		#endif
		quit(1);
	}	
	// initialize OpenGL
	if(!initGL())
	{
		#ifdef DEBUG_MODE
			fprintf(stderr, "Could not initialize OpenGL.\n");
		#endif
		quit(1);
	}
	
	if(!Game::init())
	{
		#ifdef DEBUG_MODE
			fprintf(stderr, "Could not initialize the game.\n");
		#endif
		quit(1);
	}

	// Resize the initial window
	resizeWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
	
	//#ifndef DEBUG_MODE
	//	SDL_WM_ToggleFullScreen(surface); //start in fullscreen if not debugging
	//#endif

	// wait for events
	while (!done)
	{
		// handle the events in the queue

		while (SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_ACTIVEEVENT:
				// Lost\gained focus or iconified				
				if(event.active.gain == 0)
					isActive = false;
				else
					isActive = true;
				break;
			case SDL_VIDEORESIZE:				
				surface = SDL_SetVideoMode(event.resize.w, event.resize.h, 16, videoFlags);
				if(!surface)
				{
					#ifdef DEBUG_MODE
						fprintf(stderr, "Could not get a surface after resize: %s\n", SDL_GetError());
					#endif
					quit(1);
				}
				resizeWindow(event.resize.w, event.resize.h);
				break;
			case SDL_KEYDOWN:				
				handleKeyPress(&event.key.keysym);
				break;
			case SDL_QUIT: //quit request				
				done = true;
				break;
			default:
				break;
			}
		}

		// Draw the scene
		if(isActive)
		{		
			Game::run();
			SDL_GL_SwapBuffers(); //display backbuffer
			#ifdef DEBUG_MODE
				// Calculate fps
				static GLint T0     = 0;
				static GLint Frames = 0;	
				Frames++;
				{
					GLint t = SDL_GetTicks();
					if(t - T0 >= 5000) 
					{
						GLfloat seconds = (t - T0) / 1000.0;
						GLfloat fps = Frames / seconds;
						printf("%d frames in %g seconds = %g FPS\n", Frames, seconds, fps);
						T0 = t;
						Frames = 0;
					}
				}
			#endif
		}
	}

	quit(0);	
	return 0; //just to ward off compiler warning 
}

