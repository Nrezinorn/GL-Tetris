
#ifndef Block3D_h
#define Block3D_h

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glaux.h>

#include <stdlib.h>			// Randomization (srand, rand)

#include "constants.h"

class Block3D
{
public:
	float dimensions[3];		// 0 = w, 1 = h, 2 = d
	float taperLen;
	float rotation[3];			// 0 = x, 1 = y, 2 = z
	float rotationSpeed[3];		// 0 = x, 1 = y, 2 = z
	Color3f nextColor;
	Color3f currentColor;
	float colorSpeed;
	bool redUp;					// red of currentColor increasing?
	bool greenUp;				// green of currentColor increasing?
	bool blueUp;				// blue of currentColor increasing?

public:
	Block3D();
	Block3D(float width, float height, float depth, float taper);
	~Block3D();

	void draw();
	inline Color3f getCurrentColor();
};

Block3D::Block3D()
{
	dimensions[0] = dimensions[1] = dimensions[2] = 1.5f;

	taperLen = 0.5f;

	rotation[0] = rotation[1] = rotation[2] = 0.0f;

	// (-0.51 to -1.00) OR (0.51 to 1.00)
	for (int i = 0; i < 3; i++)
		rotationSpeed[i] = (rand() % 2 ? -1 : 1) * (((rand() % 50) + 51) / 100.0f);

	// 0.2 to 0.8
	currentColor.red   = ((rand() % 7) / 10.0f) + 0.2f;
	currentColor.green = ((rand() % 7) / 10.0f) + 0.2f;
	currentColor.blue  = ((rand() % 7) / 10.0f) + 0.2f;

	// 0.2 to 0.8
	nextColor.red   = ((rand() % 7) / 10.0f) + 0.2f;
	nextColor.green = ((rand() % 7) / 10.0f) + 0.2f;
	nextColor.blue  = ((rand() % 7) / 10.0f) + 0.2f;

	redUp = (currentColor.red <= nextColor.red ? true : false);
	greenUp = (currentColor.green <= nextColor.green ? true : false);
	blueUp = (currentColor.blue <= nextColor.blue ? true : false);

	colorSpeed = 0.0002f;
}

Block3D::Block3D(float width, float height, float depth, float taper)
{
	dimensions[0] = width;
	dimensions[1] = height;
	dimensions[2] = depth;

	taperLen = taper;

	rotation[0] = rotation[1] = rotation[2] = 0.0f;

	// (-0.51 to -1.00) OR (0.51 to 1.00)
	for (int i = 0; i < 3; i++)
		rotationSpeed[i] = (rand() % 2 ? -1 : 1) * (((rand() % 50) + 51) / 100.0f);

	// 0.2 to 0.8
	currentColor.red   = ((rand() % 7) / 10.0f) + 0.2f;
	currentColor.green = ((rand() % 7) / 10.0f) + 0.2f;
	currentColor.blue  = ((rand() % 7) / 10.0f) + 0.2f;

	// 0.2 to 0.8
	nextColor.red   = ((rand() % 7) / 10.0f) + 0.2f;
	nextColor.green = ((rand() % 7) / 10.0f) + 0.2f;
	nextColor.blue  = ((rand() % 7) / 10.0f) + 0.2f;

	redUp = (currentColor.red <= nextColor.red ? true : false);
	greenUp = (currentColor.green <= nextColor.green ? true : false);
	blueUp = (currentColor.blue <= nextColor.blue ? true : false);

	colorSpeed = 0.0002f;
}

Block3D::~Block3D()
{}

void Block3D::draw()
{
	glRotatef(rotation[0], 1.0f, 0.0f, 0.0f);
	glRotatef(rotation[1], 0.0f, 1.0f, 0.0f);
	glRotatef(rotation[2], 0.0f, 0.0f, 1.0f);

	float faceW = dimensions[0] - taperLen;
	float faceH = dimensions[1] - taperLen;
	float faceD = dimensions[2] - taperLen;

	glEnable (GL_COLOR_MATERIAL);
	glEnable (GL_LIGHTING);

	glColor3f (currentColor.red, currentColor.green, currentColor.blue);

	glBegin (GL_QUADS);

	//*** FACES ***//
		// Front
		glNormal3f ( 0.0f,  0.0f,  1.0f);
		glVertex3f ( faceW,  faceH,  dimensions[2]);
		glVertex3f ( faceW, -faceH,  dimensions[2]);
		glVertex3f (-faceW, -faceH,  dimensions[2]);
		glVertex3f (-faceW,  faceH,  dimensions[2]);

		// Back
		glNormal3f ( 0.0f,  0.0f, -1.0f);
		glVertex3f ( faceW,  faceH, -dimensions[2]);
		glVertex3f (-faceW,  faceH, -dimensions[2]);
		glVertex3f (-faceW, -faceH, -dimensions[2]);
		glVertex3f ( faceW, -faceH, -dimensions[2]);

		// Top
		glNormal3f ( 0.0f,  1.0f,  0.0f);
		glVertex3f ( faceW,  dimensions[1], -faceD);
		glVertex3f ( faceW,  dimensions[1],  faceD);
		glVertex3f (-faceW,  dimensions[1],  faceD);
		glVertex3f (-faceW,  dimensions[1], -faceD);

		// Bottom
		glNormal3f ( 0.0f, -1.0f,  0.0f);
		glVertex3f ( faceW, -dimensions[1], -faceD);
		glVertex3f (-faceW, -dimensions[1], -faceD);
		glVertex3f (-faceW, -dimensions[1],  faceD);
		glVertex3f ( faceW, -dimensions[1],  faceD);

		// Right
		glNormal3f ( 1.0f,  0.0f,  0.0f);
		glVertex3f ( dimensions[0],  faceH, -faceD);
		glVertex3f ( dimensions[0], -faceH, -faceD);
		glVertex3f ( dimensions[0], -faceH,  faceD);
		glVertex3f ( dimensions[0],  faceH,  faceD);

		// Left
		glNormal3f (-1.0f,  0.0f,  0.0f);
		glVertex3f (-dimensions[0],  faceH, -faceD);
		glVertex3f (-dimensions[0],  faceH,  faceD);
		glVertex3f (-dimensions[0], -faceH,  faceD);
		glVertex3f (-dimensions[0], -faceH, -faceD);

	//*** JOINTS ***//
		// Front-top joint
		glNormal3f ( 0.0f,  0.7f,  0.7f);
		glVertex3f ( faceW,  faceH,  dimensions[2]);
		glVertex3f (-faceW,  faceH,  dimensions[2]);
		glVertex3f (-faceW,  dimensions[1],  faceD);
		glVertex3f ( faceW,  dimensions[1],  faceD);

		// Front-right joint
		glNormal3f ( 0.7f,  0.0f,  0.7f);
		glVertex3f ( faceW,  faceH,  dimensions[2]);
		glVertex3f ( faceW, -faceH,  dimensions[2]);
		glVertex3f ( dimensions[0], -faceH,  faceD);
		glVertex3f ( dimensions[0],  faceH,  faceD);

		// Front-bottom joint
		glNormal3f ( 0.0f, -0.7f,  0.7f);
		glVertex3f ( faceW, -faceH,  dimensions[2]);
		glVertex3f (-faceW, -faceH,  dimensions[2]);
		glVertex3f (-faceW, -dimensions[1],  faceD);
		glVertex3f ( faceW, -dimensions[1],  faceD);

		// Front-left joint
		glNormal3f (-0.7f,  0.0f,  0.7f);
		glVertex3f (-faceW,  faceH,  dimensions[2]);
		glVertex3f (-faceW, -faceH,  dimensions[2]);
		glVertex3f (-dimensions[0], -faceH,  faceD);
		glVertex3f (-dimensions[0],  faceH,  faceD);

		// Back-top joint
		glNormal3f ( 0.0f,  0.7f, -0.7f);
		glVertex3f (-faceW,  dimensions[1], -faceD);
		glVertex3f ( faceW,  dimensions[1], -faceD);
		glVertex3f ( faceW,  faceH, -dimensions[2]);
		glVertex3f (-faceW,  faceH, -dimensions[2]);
		
		// Back-right joint
		glNormal3f ( 0.7f,  0.0f, -0.7f);
		glVertex3f ( faceW, -faceH, -dimensions[2]);
		glVertex3f ( faceW,  faceH, -dimensions[2]);
		glVertex3f ( dimensions[0],  faceH, -faceW);
		glVertex3f ( dimensions[0], -faceH, -faceW);

		// Back-bottom joint
		glNormal3f ( 0.0f, -0.7f, -0.7f);
		glVertex3f ( faceW, -faceH, -dimensions[2]);
		glVertex3f (-faceW, -faceH, -dimensions[2]);
		glVertex3f (-faceW, -dimensions[1], -faceD);
		glVertex3f ( faceW, -dimensions[1], -faceD);

		// Back-left joint
		glNormal3f (-0.7f,  0.0f, -0.7f);
		glVertex3f (-faceW,  faceH, -dimensions[2]);
		glVertex3f (-faceW, -faceH, -dimensions[2]);
		glVertex3f (-dimensions[0], -faceH, -faceD);
		glVertex3f (-dimensions[0], faceH, -faceD);

		// Top-right joint
		glNormal3f ( 0.7f,  0.7f,  0.0f);
		glVertex3f ( faceW,  dimensions[1],  faceD);
		glVertex3f ( faceW,  dimensions[1], -faceD);
		glVertex3f ( dimensions[0],  faceH, -faceD);
		glVertex3f ( dimensions[0],  faceH,  faceD);

		// Top-left joint
		glNormal3f (-0.7f,  0.7f,  0.0f);
		glVertex3f (-faceW,  dimensions[1], -faceD);
		glVertex3f (-faceW,  dimensions[1],  faceD);
		glVertex3f (-dimensions[0],  faceH,  faceD);
		glVertex3f (-dimensions[0],  faceH, -faceD);

		// Bottom-right joint
		glNormal3f ( 0.7f, -0.7f,  0.0f);
		glVertex3f ( faceW, -dimensions[1], -faceD);
		glVertex3f ( faceW, -dimensions[1],  faceD);
		glVertex3f ( dimensions[0], -faceH,  faceD);
		glVertex3f ( dimensions[0], -faceH, -faceD);

		// Bottom-left joint
		glNormal3f (-0.7f, -0.7f,  0.0f);
		glVertex3f (-faceW, -dimensions[1], -faceD);
		glVertex3f (-faceW, -dimensions[1],  faceD);
		glVertex3f (-dimensions[0], -faceH,  faceD);
		glVertex3f (-dimensions[0], -faceH, -faceD);

	glEnd();

	//*** CORNERS ***//
	glBegin (GL_TRIANGLES);

		// Front-top-right corner
		glNormal3f ( 0.5f,  0.5f,  0.5f);
		glVertex3f ( faceW,  faceH,  dimensions[2]);
		glVertex3f ( faceW,  dimensions[1],  faceD);
		glVertex3f ( dimensions[0],  faceH,  faceD);

		// Front-bottom-right corner
		glNormal3f ( 0.5f, -0.5f,  0.5f);
		glVertex3f ( faceW, -faceH,  dimensions[2]);
		glVertex3f ( faceW, -dimensions[1],  faceD);
		glVertex3f ( dimensions[0], -faceH,  faceD);

		// Front-bottom-left corner
		glNormal3f (-0.5f, -0.5f,  0.5f);
		glVertex3f (-faceW, -faceH,  dimensions[2]);
		glVertex3f (-faceW, -dimensions[1],  faceD);
		glVertex3f (-dimensions[0], -faceH,  faceD);

		// Front-top-left corner
		glNormal3f (-0.5f,  0.5f,  0.5f);
		glVertex3f (-faceW, faceH,  dimensions[2]);
		glVertex3f (-faceW, dimensions[1],  faceD);
		glVertex3f (-dimensions[0], faceH,  faceD);

		// Back-top-left corner
		glNormal3f (-0.5f,  0.5f, -0.5f);
		glVertex3f (-faceW,  faceH, -dimensions[2]);
		glVertex3f (-faceW,  dimensions[1], -faceD);
		glVertex3f (-dimensions[0],  faceH, -faceD);

		// Back-top-right corner
		glNormal3f ( 0.5f,  0.5f, -0.5f);
		glVertex3f ( faceW,  faceH, -dimensions[2]);
		glVertex3f ( faceW,  dimensions[1], -faceD);
		glVertex3f ( dimensions[0],  faceH, -faceD);

		// Back-bottom-right corner
		glNormal3f ( 0.5f, -0.5f, -0.5f);
		glVertex3f ( faceW, -faceH, -dimensions[2]);
		glVertex3f ( faceW, -dimensions[1], -faceD);
		glVertex3f ( dimensions[0], -faceH, -faceD);

		// Back-bottom-left corner
		glNormal3f (-0.5f, -0.5f, -0.5f);
		glVertex3f (-faceW, -faceH, -dimensions[2]);
		glVertex3f (-faceW, -dimensions[1], -faceD);
		glVertex3f (-dimensions[0], -faceH, -faceD);

	glEnd();

	glDisable (GL_COLOR_MATERIAL);
	glDisable (GL_LIGHTING);

	rotation[0] += rotationSpeed[0];
	rotation[1] += rotationSpeed[1];
	rotation[2] += rotationSpeed[2];

	if (currentColor != nextColor)
	{
		if (currentColor.red != nextColor.red)
			currentColor.red += (currentColor.red < nextColor.red ? 1 : -1) * colorSpeed;
		
		if ((redUp && (currentColor.red > nextColor.red)) ||
			(!redUp && (currentColor.red < nextColor.red)))
			currentColor.red = nextColor.red;

		if (currentColor.green != nextColor.green)
			currentColor.green += (currentColor.green < nextColor.green ? 1 : -1) * colorSpeed;

		if ((greenUp && (currentColor.green > nextColor.green)) ||
			(!greenUp && (currentColor.green < nextColor.green)))
			currentColor.green = nextColor.green;

		if (currentColor.blue != nextColor.blue)
			currentColor.blue += (currentColor.blue < nextColor.blue ? 1 : -1) * colorSpeed;

		if ((blueUp && (currentColor.blue > nextColor.blue)) ||
			(!blueUp && (currentColor.blue < nextColor.blue)))
			currentColor.blue = nextColor.blue;
	}
	else
	{
		// 0.2 to 0.8
		nextColor.red   = ((rand() % 7) / 10.0f) + 0.2f;
		nextColor.green = ((rand() % 7) / 10.0f) + 0.2f;
		nextColor.blue  = ((rand() % 7) / 10.0f) + 0.2f;

		redUp = (currentColor.red <= nextColor.red ? true : false);
		greenUp = (currentColor.green <= nextColor.green ? true : false);
		blueUp = (currentColor.blue <= nextColor.blue ? true : false);
	}
}

inline Color3f Block3D::getCurrentColor()
{
	return currentColor;
}

#endif // !defined (Block3D_h)