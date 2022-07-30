#include <SDL2/SDL.h> //not osx happy
#include <SDL2/SDL_opengl.h> 
#include <gl\gl.h>			// Header File For The OpenGL32 Library
//#include <gl\glu.h>			// Header File For The GLu32 Library
//#include <gl\glaux.h>		// Header File For The GLaux Library
//#include <glfw\glfw3.h>
#include <gl\glut.h>
#include <algorithm>
#include <iostream>

// null pointers
SDL_Renderer* gRenderer = NULL;
SDL_Window* gWindow = NULL;

int running = true;

int main(int argc, char* args[]) {
    // init SDL, everything!
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
      return 1;
   
    gWindow = SDL_CreateWindow("GL Tetris 2022", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    // TODO: check for null

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    // TODO: check for null

    // Scale to 640x480 no matter desktop resolution
    SDL_RenderSetLogicalSize(gRenderer, 640, 480);

    // Most compatible openGL for what we are doing, nothing fancy
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    // create opengl context for our window
    SDL_GLContext g_ctx = SDL_GL_CreateContext(gWindow);
    
    // if the current context on our window fails, clean up and exit
    if(!SDL_GL_MakeCurrent(gWindow, g_ctx) < 0) running=false;  

     float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    }; 

    while(running){
      SDL_Event event;
      while(SDL_PollEvent(&event)) {
        switch (event.type) {
          case SDL_QUIT:              running = false;
          case SDL_KEYDOWN:           running = false;
          //case SDL_MOUSEBUTTONDOWN:   running = false;
          break;
        }
      }
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	  glLoadIdentity();
    
    glPushMatrix();
    glBegin(GL_TRIANGLES);
      glColor3f(1.f, 0.f, 0.f);  //red
      glVertex2f(-0.f, 0.5f);
      glColor3f(0.f, 0.f, 1.f);  // blue
      glVertex2f(-0.5f, -0.5f);
      glVertex2f(0.5f, -0.5f);
    glEnd();
    glPopMatrix();
 

    glRasterPos2i(0,0);
    glColor3f(0.f, 1.f, 0.f);  // white
    glDisable(GL_DEPTH_TEST);
    glPrint("ABCDabcd1234!@#$");
    glEnable(GL_DEPTH_TEST);
       
    SDL_GL_SwapWindow(gWindow);
    SDL_Delay(200);
    }

    // destroy context, renderer, and window
    SDL_GL_DeleteContext(g_ctx);  
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
    
    return 0;
}