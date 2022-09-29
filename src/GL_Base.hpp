#ifndef GL_BASE_HPP
#define GL_BASE_HPP

#include <SDL2/SDL_opengl.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

float LightAmbient[]  =	{ 0.5f, 0.5f, 0.5f, 1.0f };
float LightDiffuse[]  =	{ 0.5f, 0.5f, 0.5f, 1.0f };
float LightPosition[] =	{ 0.0f, 0.0f, 2.0f, 1.0f };

glm::mat4 projection = glm::perspective(
  // FOV & aspect
  45.0f, 640.0f / 480.0f, 
  // Near and far planes
  0.1f, 100.0f);

int InitGL (GLvoid)
{
//	if (!LoadGLTextures())								// Jump To Texture Loading Routine ( NEW )
//	{
//		return FALSE;									// If Texture Didn't Load Return FALSE
//	}

	
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, 640, 480);  //hardcoded viewport
	glLoadIdentity();
	glLoadMatrixf(glm::value_ptr(projection));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

//	
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