
#ifndef Piece_h
#define Piece_h

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
//#include <gl/glaux.h>

#include <stdlib.h>		// Randomization (srand, rand)

#include "constants.h"
#include "Block.h"

class Piece
{
public:
	Color3f color;
	Point2i position;		// Pivot position relative to top-left of board grid
	shapeType type;
	Point2i layout[4];		// Current layout of blocks; affected by rotation
	bool rotated;			// Silly hack for Z and Z2 rotation

	Piece();

	void draw ();
	void drawForNext ();

	void rotate (unsigned short int direction);		// 0 = CW, 1 = CCW
	void move (unsigned short int direction);		// 0 = down, 1 = right, 2 = left

	void setColor (Color3f newColor);
};

Piece::Piece()
{
	// Random color from 0.2f to 0.8f
	color.red = ((rand() % 7) / 10.0f) + 0.2f;
	color.green = ((rand() % 7) / 10.0f) + 0.2f;
	color.blue = ((rand() % 7) / 10.0f) + 0.2f;

	// Piece begins at top and middle of board
	position.x = 5;
	position.y = 0;

	// Random shape type
	type = static_cast <shapeType> (rand() % 7);

	if (type == Cube)
		position.x = 4;			// Adjust for smaller cube width

	for (int i = 0; i < 4; i++)
		layout[i] = SHAPES[type][i];

	rotated = false;
}

void Piece::draw()
{
	Block b;
	b.color = color;

	float x = ((position.x) * blockLen) - ((blocksPerRow * blockLen) / 2) + blockLen / 2;
	float y = ((blocksPerCol * blockLen) / 2) - ((position.y) * blockLen) - blockLen / 2;

	glLoadIdentity();
	glTranslatef (x, y, -25.0f);

	for (int i = 0; i < 4; i++)
	{
		if (position.y - layout[i].y >= 0)
		{
			glTranslatef (layout[i].x * blockLen, 
							(layout[i].y * blockLen), 0.0f);
			b.draw();
			glTranslatef (-(layout[i].x * blockLen), 
							-(layout[i].y * blockLen), 0.0f);
		}
	}
}

void Piece::drawForNext()
{
	Block b;
	b.color = color;

	if (type == Line)
		glTranslatef (blockLen/2, -(blockLen/2), 0.0f);

	if (type == Cube)
		glTranslatef (-(blockLen/2), 0.0f, 0.0f);

	for (int i = 0; i < 4; i++)
	{
		glTranslatef (layout[i].x * blockLen, 
						(layout[i].y * blockLen), 0.0f);
		b.draw();
		glTranslatef (-(layout[i].x * blockLen), 
						-(layout[i].y * blockLen), 0.0f);
	}

	if (type == Line)
		glTranslatef (-(blockLen/2), blockLen/2, 0.0f);

	if (type == Cube)
		glTranslatef (blockLen/2, 0.0f, 0.0f);
}

void Piece::rotate(unsigned short int direction)
{
	int i, temp;

	// For everything except the Cube (Cube never changes shape)
	if (type != Cube)
	{
		// *Hack*  Z and Z2 do not rotate as normal - only have 2 rotation states
		if ((type == Z) || (type == Z2))
			direction = (rotated ? CW : CCW);

		switch (direction)
		{
		case CCW:
			// Flip the piece along the line y = -x
			for (i = 0; i < 4; i++)
			{
				temp = layout[i].x;
				layout[i].x = -layout[i].y;
				layout[i].y = -temp;
			}

			// For everything except the Line
			if (type != Line)
			{
				// Flip the piece along the line y = 0
				for (i = 0; i < 4; i++)
					layout[i].y = -layout[i].y;
			}

			break;

		case CW:
			// For everything except the Line
			if (type != Line)
			{
				// Flip the piece along the line y = 0
				for (i = 0; i < 4; i++)
					layout[i].y = -layout[i].y;
			}

			// Flip the piece along the line y = -x
			for (i = 0; i < 4; i++)
			{
				temp = layout[i].x;
				layout[i].x = -layout[i].y;
				layout[i].y = -temp;
			}

			break;
		};
	}
	rotated = !rotated;
}

void Piece::move (unsigned short int direction)
{
	switch (direction)
	{
	case DOWN:
		position.y++;
		break;

	case RIGHT:
		position.x++;
		break;

	case LEFT:
		position.x--;
		break;
	};
}

void Piece::setColor (Color3f newColor)
{
	color.red = newColor.red;
	color.green = newColor.green;
	color.blue = newColor.blue;
}

#endif // !defined (Piece_h)