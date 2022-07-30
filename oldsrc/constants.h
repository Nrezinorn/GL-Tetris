
#ifndef constants_h
#define constants_h

const float blockLen = 0.8f;		// Total length of a tetris block
const float blockTaperLen = 0.14f;	// Length of the taper on a tetris block

const int blocksPerRow = 10;
const int blocksPerCol = 20;

const float boardTaperLen = 0.4f;	// Length of the taper on the board

typedef unsigned char byte;

struct Color3f
{
	float red;
	float green;
	float blue;

	inline bool operator != (Color3f other)
	{
		if ((red != other.red) || (green != other.green) || (blue != other.blue))
			return true;
		return false;
	}

	inline bool operator == (Color3f other)
	{
		if (*this != other)
			return false;
		return true;
	}
};

struct Point2i
{
	int x;
	int y;
};

const Point2i SHAPES[7][4] =
{
	{ {0,0}, { 1,0}, { 0,-1}, { 1,-1} },	// Cube
	{ {0,0}, {-1,0}, { 0,-1}, { 1,-1} },	// Z
	{ {0,0}, { 1,0}, { 0,-1}, {-1,-1} },	// Z mirrored (Z2)
	{ {0,0}, { 1,0}, {-1, 0}, {-1,-1} },	// L
	{ {0,0}, {-1,0}, { 1, 0}, { 1,-1} },	// L mirrored (L2)
	{ {0,0}, { 1,0}, {-1, 0}, {-2, 0} },	// Line
	{ {0,0}, { 1,0}, {-1, 0}, { 0,-1} }		// T
};


// Shape types
enum shapeType { Cube, Z, Z2, L, L2, Line, T };

// Directions for piece movement
const unsigned short DOWN = 0;
const unsigned short RIGHT = 1;
const unsigned short LEFT = 2;

// Directions for piece rotation
const unsigned short CW = 0;	// Clockwise
const unsigned short CCW = 1;	// Counter-clockwise

void DrawExtrusion (float width, float height, float indentTaperLen, Color3f color)
{
	float halfIndentWidth = width / 2;
	float halfIndentHeight = height / 2;

	glBegin (GL_QUADS);
		// Center
		glColor3f (color.red, color.green, color.blue);
		glVertex3f ( halfIndentWidth,  halfIndentHeight, 0.0f);
		glVertex3f (-halfIndentWidth,  halfIndentHeight, 0.0f);
		glVertex3f (-halfIndentWidth, -halfIndentHeight, 0.0f);
		glVertex3f ( halfIndentWidth, -halfIndentHeight, 0.0f);

		// Top
		glColor3f (color.red + 0.2f, color.green + 0.2f, color.blue + 0.2f);
		glVertex3f ( halfIndentWidth + indentTaperLen,  halfIndentHeight + indentTaperLen, 0.0f);
		glVertex3f (-(halfIndentWidth + indentTaperLen),  halfIndentHeight + indentTaperLen, 0.0f);
		glVertex3f (-halfIndentWidth,  halfIndentHeight, 0.0f);
		glVertex3f ( halfIndentWidth,  halfIndentHeight, 0.0f);

		// Right
		glColor3f (color.red + 0.2f, color.green + 0.2f, color.blue + 0.2f);
		glVertex3f ( halfIndentWidth,  halfIndentHeight, 0.0f);
		glVertex3f ( halfIndentWidth, -halfIndentHeight, 0.0f);
		glVertex3f ( halfIndentWidth + indentTaperLen, -(halfIndentHeight + indentTaperLen), 0.0f);
		glVertex3f ( halfIndentWidth + indentTaperLen,  halfIndentHeight + indentTaperLen, 0.0f);

		// Bottom
		glColor3f (color.red - 0.2f, color.green - 0.2f, color.blue - 0.2f);
		glVertex3f ( halfIndentWidth, -halfIndentHeight, 0.0f);
		glVertex3f (-halfIndentWidth, -halfIndentHeight, 0.0f);
		glVertex3f (-(halfIndentWidth + indentTaperLen), -(halfIndentHeight + indentTaperLen), 0.0f);
		glVertex3f ( halfIndentWidth + indentTaperLen, -(halfIndentHeight + indentTaperLen), 0.0f);

		// Left
		glColor3f (color.red - 0.2f, color.green - 0.2f, color.blue - 0.2f);
		glVertex3f (-(halfIndentWidth + indentTaperLen),  halfIndentHeight + indentTaperLen, 0.0f);
		glVertex3f (-(halfIndentWidth + indentTaperLen), -(halfIndentHeight + indentTaperLen), 0.0f);
		glVertex3f (-halfIndentWidth, -halfIndentHeight, 0.0f);
		glVertex3f (-halfIndentWidth,  halfIndentHeight, 0.0f);
	glEnd();
}

void DrawIndent (float width, float height, float indentTaperLen, Color3f color)
{
	float halfIndentWidth = width / 2;
	float halfIndentHeight = height / 2;

	glBegin (GL_QUADS);
		// Center
		glColor3f (color.red, color.green, color.blue);
		glVertex3f ( halfIndentWidth,  halfIndentHeight, 0.0f);
		glVertex3f (-halfIndentWidth,  halfIndentHeight, 0.0f);
		glVertex3f (-halfIndentWidth, -halfIndentHeight, 0.0f);
		glVertex3f ( halfIndentWidth, -halfIndentHeight, 0.0f);

		// Top
		glColor3f (color.red - 0.2f, color.green - 0.2f, color.blue - 0.2f);
		glVertex3f ( halfIndentWidth + indentTaperLen,  halfIndentHeight + indentTaperLen, 0.0f);
		glVertex3f (-(halfIndentWidth + indentTaperLen),  halfIndentHeight + indentTaperLen, 0.0f);
		glVertex3f (-halfIndentWidth,  halfIndentHeight, 0.0f);
		glVertex3f ( halfIndentWidth,  halfIndentHeight, 0.0f);

		// Right
		glColor3f (color.red - 0.2f, color.green - 0.2f, color.blue - 0.2f);
		glVertex3f ( halfIndentWidth,  halfIndentHeight, 0.0f);
		glVertex3f ( halfIndentWidth, -halfIndentHeight, 0.0f);
		glVertex3f ( halfIndentWidth + indentTaperLen, -(halfIndentHeight + indentTaperLen), 0.0f);
		glVertex3f ( halfIndentWidth + indentTaperLen,  halfIndentHeight + indentTaperLen, 0.0f);

		// Bottom
		glColor3f (color.red + 0.2f, color.green + 0.2f, color.blue + 0.2f);
		glVertex3f ( halfIndentWidth, -halfIndentHeight, 0.0f);
		glVertex3f (-halfIndentWidth, -halfIndentHeight, 0.0f);
		glVertex3f (-(halfIndentWidth + indentTaperLen), -(halfIndentHeight + indentTaperLen), 0.0f);
		glVertex3f ( halfIndentWidth + indentTaperLen, -(halfIndentHeight + indentTaperLen), 0.0f);

		// Left
		glColor3f (color.red + 0.2f, color.green + 0.2f, color.blue + 0.2f);
		glVertex3f (-(halfIndentWidth + indentTaperLen),  halfIndentHeight + indentTaperLen, 0.0f);
		glVertex3f (-(halfIndentWidth + indentTaperLen), -(halfIndentHeight + indentTaperLen), 0.0f);
		glVertex3f (-halfIndentWidth, -halfIndentHeight, 0.0f);
		glVertex3f (-halfIndentWidth,  halfIndentHeight, 0.0f);
	glEnd();
}

unsigned long StoL (char* str)
{
	unsigned long integer = 0, place = 1;

	for (int i = strlen(str); i >= 0; i--)
		if ((str[i] >= 48) && (str[i] <= 57))
		{
			integer += (str[i] - 48) * place;
			place *= 10;
		}

	return integer;
}


#endif // !defined(constants_h)