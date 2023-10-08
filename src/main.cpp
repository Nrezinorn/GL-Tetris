/* Copyright 2002-2023 Dan Peschment, James Gorz */

#include <SDL2/SDL.h>  // not osx happy
#include <SDL2/SDL_opengl.h>
#include "freetype.h"

#include "Tetris/Tetris.hpp"
#include <algorithm>
#include <iostream>

// null pointers
SDL_Renderer* gRenderer = NULL;
SDL_Window* gWindow = NULL;

int running = true;

bool keys[1024];  // Array Used For The Keyboard Routine, extended

int main(int argc, char* args[]) {
    // init SDL, everything!
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0 )
      return 1;

    // Most compatible openGL for what we are doing, nothing fancy
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
      SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    gWindow = SDL_CreateWindow("GL Tetris 2022", SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    // TODO(Nrezinorn): check for null

    // gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    // TODO(Nrezinorn): check for null

    // Scale to 640x480 no matter desktop resolution
    // SDL_RenderSetLogicalSize(gRenderer, 640, 480);

    // create opengl context for our window
    SDL_GLContext g_ctx = SDL_GL_CreateContext(gWindow);

    // if the current context on our window fails, clean up and exit
    if (!SDL_GL_MakeCurrent(gWindow, g_ctx) < 0) running = false;

    Tetris tetris;

    while (running) {
      SDL_Event event;
      while (SDL_PollEvent(&event)) {
        switch (event.type) {
          case SDL_QUIT:              running = false;
          case SDL_KEYDOWN:
            keys[event.key.keysym.scancode] = true;
          // case SDL_MOUSEBUTTONDOWN:   running = false;
          break;
        }
      }

     switch (tetris.getGameState()) {
       case STATE_MAINMENU:
          tetris.MainMenuEventHandler();
          break;

        case STATE_OPTIONS:
          break;

        case STATE_GAMETYPE:
          tetris.GameTypeMenuEventHandler();
          break;

        case STATE_LEVEL:
          tetris.LevelMenuEventHandler();
          break;

        case STATE_HEIGHT:
          tetris.HeightMenuEventHandler();
          break;

        case STATE_MUSIC:
          tetris.MusicMenuEventHandler();
          break;

        case STATE_GAME:
          tetris.GameEventHandler();
          break;

        case STATE_PAUSED:
          tetris.PauseEventHandler();
          break;

        case STATE_HIGHSCORES:
          tetris.HighScoresEventHandler();
          break;

        case STATE_NEWHIGH:
          tetris.NewHighEventHandler();
          break;
        }

      if (keys[SDL_SCANCODE_ESCAPE] &&
        (tetris.getGameState() == STATE_MAINMENU))
      running = false;

      tetris.draw();
      tetris.Update();

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
