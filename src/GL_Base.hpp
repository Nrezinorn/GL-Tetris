#ifndef GL_BASE_HPP
#define GL_BASE_HPP

#include <SDL2/SDL_opengl.h>

float LightAmbient[]  =	{ 0.5f, 0.5f, 0.5f, 1.0f };
float LightDiffuse[]  =	{ 0.5f, 0.5f, 0.5f, 1.0f };
float LightPosition[] =	{ 0.0f, 0.0f, 2.0f, 1.0f };

int InitGL (GLvoid)
{
//	if (!LoadGLTextures())								// Jump To Texture Loading Routine ( NEW )
//	{
//		return FALSE;									// If Texture Didn't Load Return FALSE
//	}

	
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, 640, 480);  //hardcoded viewport
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

//	gluPerspective(45.0f, (float)640 / (float)480, 0.1f, 100.0f);
	glShadeModel (GL_SMOOTH);
	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth (1.0f);
	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LEQUAL);
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glLightfv (GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv (GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv (GL_LIGHT1, GL_POSITION, LightPosition);

	glEnable (GL_LIGHT1);

	return TRUE;
}

#endif