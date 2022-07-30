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
TBD - I dont have this project building on OSX yet.  I use the SDL DMG and install the frameworks into `/Library/Frameworks`.  I need to get freetype working, which may come from brew.

## Linux
Use your package manager to install the packages we use for windows builds:  Freetype2, SDL, SDL_Mixer.  Once I have this setup there will be a Makefile for builds.

# Bugs
- `7/30/2022` Freetype Font rendering currently does not work properly.