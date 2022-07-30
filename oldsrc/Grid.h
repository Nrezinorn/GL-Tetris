
#ifndef Grid_h
#define Grid_h

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glaux.h>

#include "constants.h"
#include "Block.h"

class Grid
{
public:
	Block* blocks[blocksPerCol][blocksPerRow];
	Color3f color;

	Grid();
	Grid(float r, float g, float b);
	~Grid();

	// Draws the grid / board / playfield
	void draw();
};

Grid::Grid()
{
	for (int row = 0; row < blocksPerCol; row++)
		for (int col = 0; col < blocksPerRow; col++)
			blocks[row][col] = NULL;

	color.red = color.green = color.blue = 0.5f;
}

Grid::Grid(float r, float g, float b)
{
	for (int row = 0; row < blocksPerCol; row++)
		for (int col = 0; col < blocksPerRow; col++)
			blocks[row][col] = NULL;

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

Grid::~Grid()
{
	for (int r = 0; r < blocksPerCol; r++)
		for (int c = 0; c < blocksPerRow; c++)
			delete blocks[r][c];
}

void Grid::draw (GLvoid)
{
	float halfBoardWidth = (blocksPerRow * blockLen) / 2;
	float halfBoardHeight = (blocksPerCol * blockLen) / 2;

	DrawExtrusion (blocksPerRow * blockLen + 2 * blockLen,
				   blocksPerCol * blockLen + 2 * blockLen,
				   boardTaperLen, color);
	DrawIndent (blocksPerRow * blockLen, blocksPerCol * blockLen, boardTaperLen, color);

	float x, y;

	// Draw the blocks
	for (int r = 0; r < blocksPerCol; r++)
		for (int c = 0; c < blocksPerRow; c++)
			if (blocks[r][c])
			{
				x = c * blockLen - halfBoardWidth + blockLen / 2;
				y = halfBoardHeight - (r * blockLen + blockLen / 2);

				glTranslatef (x, y, 0.0f);
				blocks[r][c]->draw();
				glTranslatef (-x, -y, 0.0f);
			}
}


#endif // !defined (Grid_h)