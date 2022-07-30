
#ifndef Menu_h
#define Menu_h

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glaux.h>

#include "constants.h"
#include "Block3D.h"
#include "GL Base.h"

class Menu
{
protected:
	char** options;
	unsigned short numOps;
	unsigned short selectedOp;
	Block3D selector;
	float lineHeight;

public:
	Menu();
	Menu(int numOps, int defSelOp, float lineHeightIn, 
		 char* op1, char* op2 = NULL, char* op3 = NULL, char* op4 = NULL, 
		 char* op5 = NULL, char* op6 = NULL, char* op7 = NULL, char* op8 = NULL,
		 char* op9 = NULL, char* op10 = NULL);
	~Menu();

	void draw();
	void setOps(int numOps, int defSelOp, 
				char* op1, char* op2 = NULL, char* op3 = NULL, char* op4 = NULL, 
				char* op5 = NULL, char* op6 = NULL, char* op7 = NULL, char* op8 = NULL,
				char* op9 = NULL, char* op10 = NULL);
	
	inline unsigned short getSelectedOp();
	inline unsigned short getNumOps();
	inline void setSelectedOp (unsigned short selOp);

protected:
	void clearOps();
};

Menu::Menu()
	:options(NULL),
	 numOps(0),
	 selectedOp(0),
	 selector(1.5f / 40.0f, 1.5f / 40.0f, 1.5f / 40.0f, 0.5f / 40.0f),
	 lineHeight(0.5f)
{}

Menu::Menu(int numOpsIn, int defSelOp, float lineHeightIn, 
		   char* op1, char* op2, char* op3, char* op4, 
		   char* op5, char* op6, char* op7, char* op8,
		   char* op9, char* op10)
	:numOps(numOpsIn),
	 selectedOp(defSelOp),
	 selector(1.5f / 8.0f, 1.5f / 8.0f, 1.5f / 8.0f, 0.5f / 8.0f),
	 lineHeight(lineHeightIn)
{
	options = new char* [numOps];

	char* ops[] = { op1, op2, op3, op4, op5, op6, op7, op8, op9, op10 };

	for (int i = 0; i < numOps; i++)
	{
		options[i] = new char [strlen(ops[i]) + 1];
		strcpy(options[i], ops[i]);
	}
}

Menu::~Menu()
{
	if (options)
		clearOps();
}

void Menu::clearOps()
{
	int i = 0;

	while (i < numOps)
	{
		delete [] options[i];
		options[i] = NULL;
		i++;
	}

	delete [] options;
	options = NULL;

	numOps = 0;
	selectedOp = 0;
}

void Menu::setOps(int numOpsIn, int defSelOp, char* op1, char* op2, char* op3, char* op4, 
											  char* op5, char* op6, char* op7, char* op8,
											  char* op9, char* op10)
{
	if (options)
		clearOps();

	numOps = numOpsIn;
	selectedOp = defSelOp;

	options = new char* [numOps];

	char* ops[] = { op1, op2, op3, op4, op5, op6, op7, op8, op9, op10 };

	for (int i = 0; i < numOps; i++)
	{
		options[i] = new char [strlen(ops[i]) + 1];
		strcpy(options[i], ops[i]);
	}
}

void Menu::draw()
{
//	Color3f selColor = selector.getCurrentColor();

	glColor3f (1.0f, 1.0f, 1.0f);

	// Draw each option
	for (int i = 0; i < numOps; i++)
	{
/*		glColor3f (1.0f, 1.0f, 1.0f);

		if (i == selectedOp)
			glColor3f (selColor.red, selColor.green, selColor.blue);
*/
		glRasterPos2f (0.0f, -(i * lineHeight));
		glPrint(options[i]);
	}

	// Draw the selector next to the selected option
	glTranslatef (-0.5f, -(selectedOp * lineHeight) + 0.08f, 0.0f);
	selector.draw();
}

inline unsigned short Menu::getNumOps()
{
	return numOps;
}

inline unsigned short Menu::getSelectedOp ()
{
	return selectedOp;
}

inline void Menu::setSelectedOp (unsigned short selOp)
{
	if (selOp < numOps)
		selectedOp = selOp;
}

#endif // !defined (Menu_h)