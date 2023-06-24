# GL-Tetris
 SDL + OpenGL Port

# History
Original Code was written by Daniel Peschman in 2001 while attending UAT.  It was only built for Windows, using DirectX for Input/Sound and OpenGL for rendering.  

# Legacy
Cross Platform gaming is important to me and Dan allowed me to have his original code and granted me access to publish to github.  I am learning OpenGL with this port.  

# Building

## Windows
My environment on windows uses [MSYS2](https://www.msys2.org/).  I have installed mingw64, SDL2, and a few extra SDL Libraries.  I also installed Freetype2.  That should be the minimum needed to compile.

## OSX
I use the SDL DMG and install the frameworks into `/Library/Frameworks`.  brew install glm and freetype2.  With all dependancies in place running `make -f Makefile.osx` should build the project.

## Linux
Use your package manager to install the packages we use for windows builds:  Freetype2, SDL, SDL_Mixer, glm.  Once I have this setup there will be a Makefile for builds.

## Libraries
freetype font rendering uses the "hacked" nehe lesson 34 from here [https://users.cs.northwestern.edu/~sco590/fonts_tutorial.html]

# Bugs
-  `7/30/2022` ~~Freetype Font rendering currently does not work properly~~
-  `9/30/2022` ~~OSX Crashes on app close, OSX Crash on game end~~
