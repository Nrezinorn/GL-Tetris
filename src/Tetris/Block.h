
#ifndef Block_h
#define Block_h

#include <SDL2/SDL_opengl.h>
#include "constants.h"

class Block
{
public:
	Color3f color;

	Block();
	Block(float r, float g, float b);
	Block(Color3f INcolor);

	GLvoid draw (GLvoid);		// Draws the tetris block
	GLvoid explode (GLvoid);	// Explodes the tetris block
};

Block::Block ()
{
/*	// Random color
	color.red = ((rand() % 7) / 10.0f) + 0.2f;
	color.green = ((rand() % 7) / 10.0f) + 0.2f;
	color.blue = ((rand() % 7) / 10.0f) + 0.2f;
	*/

	color.red = color.green = color.blue = 0.5f;
}

Block::Block (float r, float g, float b)
{
	r = (r > 0.8f ? 0.8f : r);
	r = (r < 0.2f ? 0.2f : r);
	g = (g > 0.8f ? 0.8f : g);
	g = (g < 0.2f ? 0.2f : g);
	b = (b > 0.8f ? 0.8f : b);
	b = (b < 0.2f ? 0.2f : b);

	color.red = r;
	color.green = g;
	color.blue = b;
}

Block::Block (Color3f INcolor)
{
	color = INcolor;
}

// Draws the tetris block
GLvoid Block::draw (GLvoid)
{
	float halfLen = (blockLen - 2 * blockTaperLen) / 2;

	glBegin (GL_QUADS);
		// Center
		glColor3f (color.red, color.green, color.blue);
		glVertex3f ( halfLen,  halfLen, 0.0f);
		glVertex3f (-halfLen,  halfLen, 0.0f);
		glVertex3f (-halfLen, -halfLen, 0.0f);
		glVertex3f ( halfLen, -halfLen, 0.0f);

		// Top
		glColor3f (color.red + 0.2f, color.green + 0.2f, color.blue + 0.2f);
		glVertex3f ( halfLen + blockTaperLen,  halfLen + blockTaperLen, 0.0f);
		glVertex3f (-(halfLen + blockTaperLen),  halfLen + blockTaperLen, 0.0f);
		glVertex3f (-halfLen,  halfLen, 0.0f);
		glVertex3f ( halfLen,  halfLen, 0.0f);

		// Right
		glColor3f (color.red + 0.2f, color.green + 0.2f, color.blue + 0.2f);
		glVertex3f ( halfLen,  halfLen, 0.0f);
		glVertex3f ( halfLen, -halfLen, 0.0f);
		glVertex3f ( halfLen + blockTaperLen, -(halfLen + blockTaperLen), 0.0f);
		glVertex3f ( halfLen + blockTaperLen,  halfLen + blockTaperLen, 0.0f);

		// Bottom
		glColor3f (color.red - 0.2f, color.green - 0.2f, color.blue - 0.2f);
		glVertex3f ( halfLen, -halfLen, 0.0f);
		glVertex3f (-halfLen, -halfLen, 0.0f);
		glVertex3f (-(halfLen + blockTaperLen), -(halfLen + blockTaperLen), 0.0f);
		glVertex3f ( halfLen + blockTaperLen, -(halfLen + blockTaperLen), 0.0f);

		// Left
		glColor3f (color.red - 0.2f, color.green - 0.2f, color.blue - 0.2f);
		glVertex3f (-(halfLen + blockTaperLen),  halfLen + blockTaperLen, 0.0f);
		glVertex3f (-(halfLen + blockTaperLen), -(halfLen + blockTaperLen), 0.0f);
		glVertex3f (-halfLen, -halfLen, 0.0f);
		glVertex3f (-halfLen,  halfLen, 0.0f);
	glEnd();
}

GLvoid Block::explode (GLvoid)
{
}

#endif // !defined (Block_h)