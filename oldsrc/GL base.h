
#ifndef GL_Base_h
#define GL_Base_h

#include <windows.h>
#include <stdio.h>			// Header File For Standard Input/Output
#include <stdarg.h>		// Header File For Variable Argument Routines

#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glaux.h>

HGLRC		hRC=NULL;							// Permanent Rendering Context
HDC			hDC=NULL;							// Private GDI Device Context
HWND		hWnd=NULL;							// Holds Our Window Handle
HINSTANCE	hInstance;							// Holds The Instance Of The Application

bool	keys[256];					// Array Used For The Keyboard Routine
bool	active=TRUE;				// Window Active Flag Set To TRUE By Default
bool	fullscreen=FALSE;			// Fullscreen Flag Set To Fullscreen Mode By Default

GLuint	base;				// Base display list for the font set

GLuint	texture[1];			// Storage For One Texture

float LightAmbient[]  =	{ 0.5f, 0.5f, 0.5f, 1.0f };
float LightDiffuse[]  =	{ 0.5f, 0.5f, 0.5f, 1.0f };
float LightPosition[] =	{ 0.0f, 0.0f, 2.0f, 1.0f };

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

AUX_RGBImageRec *LoadBMP(char *Filename)				// Loads A Bitmap Image
{
	FILE *File=NULL;									// File Handle

	if (!Filename)										// Make Sure A Filename Was Given
	{
		return NULL;									// If Not Return NULL
	}

	File=fopen(Filename,"r");							// Check To See If The File Exists

	if (File)											// Does The File Exist?
	{
		fclose(File);									// Close The Handle
		return auxDIBImageLoad(Filename);				// Load The Bitmap And Return A Pointer
	}

	return NULL;										// If Load Failed Return NULL
}

int LoadGLTextures()									// Load Bitmaps And Convert To Textures
{
	int Status=FALSE;									// Status Indicator

	AUX_RGBImageRec *TextureImage[1];					// Create Storage Space For The Texture

	memset(TextureImage,0,sizeof(void *)*1);           	// Set The Pointer To NULL

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if (TextureImage[0]=LoadBMP("Images/typhoon.bmp"))
	{
		Status=TRUE;									// Set The Status To TRUE

		glGenTextures(1, &texture[0]);					// Create The Texture

		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	}

	if (TextureImage[0])									// If Texture Exists
	{
		if (TextureImage[0]->data)							// If Texture Image Exists
		{
			free(TextureImage[0]->data);					// Free The Texture Image Memory
		}

		free(TextureImage[0]);								// Free The Image Structure
	}

	return Status;										// Return The Status
}

GLvoid BuildFont(GLvoid)								// Build Our Bitmap Font
{
	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping

	base = glGenLists(96);								// Storage For 96 Characters

	font = CreateFont(	-18,							// Height Of Font
						0,								// Width Of Font
						0,								// Angle Of Escapement
						0,								// Orientation Angle
						FW_BOLD,						// Font Weight
						FALSE,							// Italic
						FALSE,							// Underline
						FALSE,							// Strikeout
						ANSI_CHARSET,					// Character Set Identifier
						OUT_TT_PRECIS,					// Output Precision
						CLIP_DEFAULT_PRECIS,			// Clipping Precision
						ANTIALIASED_QUALITY,			// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
						"Courier");					// Font Name

	oldfont = (HFONT)SelectObject(hDC, font);           // Selects The Font We Want
	wglUseFontBitmaps(hDC, 32, 96, base);				// Builds 96 Characters Starting At Character 32
	SelectObject(hDC, oldfont);							// Selects The Font We Want
	DeleteObject(font);									// Delete The Font
}

GLvoid KillFont(GLvoid)									// Delete The Font List
{
	glDeleteLists(base, 96);							// Delete All 96 Characters
}

GLvoid glPrint(const char *fmt, ...)					// Custom GL "Print" Routine
{
	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);									// Parses The String For Variables
	    vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(base - 32);								// Sets The Base Character to 32
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits
}

// Resize And Initialize The GL Window
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)
{
	if (height==0)								// Prevent A Divide By Zero By
	{
		height=1;							// Making Height Equal One
	}

	glViewport(0, 0, width, height);					// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();							// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);						// Select The Modelview Matrix
	glLoadIdentity();							// Reset The Modelview Matrix
}

int InitGL (GLvoid)
{
//	if (!LoadGLTextures())								// Jump To Texture Loading Routine ( NEW )
//	{
//		return FALSE;									// If Texture Didn't Load Return FALSE
//	}

	glShadeModel (GL_SMOOTH);
	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth (1.0f);
	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LEQUAL);
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	BuildFont();										// Build The Font

	glLightfv (GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv (GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv (GL_LIGHT1, GL_POSITION, LightPosition);

	glEnable (GL_LIGHT1);

	return TRUE;
}


GLvoid KillGLWindow (GLvoid)
{
	if (fullscreen)
	{
		ChangeDisplaySettings(NULL,0);			// If So Switch Back To The Desktop
		ShowCursor(TRUE);						// Show Mouse Pointer
	}

	if (hRC)
	{
		if (!wglMakeCurrent (NULL, NULL))
			MessageBox(NULL,"Release Of DC And RC Failed.",
						"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);

		if (!wglDeleteContext (hRC))
			MessageBox(NULL,"Release Rendering Context Failed.",
						"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);

		hRC = NULL;
	}

	if (hDC && !ReleaseDC(hWnd,hDC))	// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.",
					"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);

		hDC=NULL;							// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))	// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.",
					"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);

		hWnd=NULL;							// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))	// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.",
					"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);

		hInstance=NULL;									// Set hInstance To NULL
	}

	KillFont();
}

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint PixelFormat;

	WNDCLASS wc;

	DWORD		dwExStyle;						// Window Extended Style
	DWORD		dwStyle;						// Window Style

	RECT WindowRect;					// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	fullscreen = fullscreenflag;

	hInstance		= GetModuleHandle(NULL);			// Grab An Instance For Our Window
	wc.style		= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;		// Redraw On Move, And Own DC For Window
	wc.lpfnWndProc		= (WNDPROC) WndProc;				// WndProc Handles Messages
	wc.cbClsExtra		= 0;						// No Extra Window Data
	wc.cbWndExtra		= 0;						// No Extra Window Data
	wc.hInstance		= hInstance;					// Set The Instance
	wc.hIcon		= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor		= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;						// No Background Required For GL
	wc.lpszMenuName		= NULL;						// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";					// Set The Class Name

	if (!RegisterClass(&wc))						// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.",
					"ERROR",MB_OK|MB_ICONEXCLAMATION);

		return FALSE;							// Exit And Return FALSE
	}

	if (fullscreen)
	{
		DEVMODE dmScreenSettings;					// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));		// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;			// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;			// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;				// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","Warning",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;				// Select Windowed Mode (Fullscreen=FALSE)
			}
			else
			{
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;					// Exit And Return FALSE
			}
		}
	}

	if (fullscreen)
	{
		dwExStyle=WS_EX_APPWINDOW;					// Window Extended Style
		dwStyle=WS_POPUP;						// Windows Style
		ShowCursor(FALSE);						// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;					// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	if (!(hWnd=CreateWindowEx(	dwExStyle,				// Extended Style For The Window
					"OpenGL",				// Class Name
					title,					// Window Title
					WS_CLIPSIBLINGS |			// Required Window Style
					WS_CLIPCHILDREN |			// Required Window Style
					dwStyle,				// Selected Window Style
					0, 0,					// Window Position
					WindowRect.right-WindowRect.left,	// Calculate Adjusted Window Width
					WindowRect.bottom-WindowRect.top,	// Calculate Adjusted Window Height
					NULL,					// No Parent Window
					NULL,					// No Menu
					hInstance,				// Instance
					NULL)))					// Don't Pass Anything To WM_CREATE
	{
		KillGLWindow();							// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=					// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),					// Size Of This Pixel Format Descriptor
		1,								// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,						// Must Support Double Buffering
		PFD_TYPE_RGBA,							// Request An RGBA Format
		bits,								// Select Our Color Depth
		0, 0, 0, 0, 0, 0,						// Color Bits Ignored
		0,								// No Alpha Buffer
		0,								// Shift Bit Ignored
		0,								// No Accumulation Buffer
		0, 0, 0, 0,							// Accumulation Bits Ignored
		16,								// 16Bit Z-Buffer (Depth Buffer)
		0,								// No Stencil Buffer
		0,								// No Auxiliary Buffer
		PFD_MAIN_PLANE,							// Main Drawing Layer
		0,								// Reserved
		0, 0, 0								// Layer Masks Ignored
	};

	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();							// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))				// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();							// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))				// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();							// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))					// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();							// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))						// Try To Activate The Rendering Context
	{
		KillGLWindow();							// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);								// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);						// Set Up Our Perspective GL Screen

	if (!InitGL())								// Initialize Our Newly Created GL Window
	{
		KillGLWindow();							// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}

	return TRUE;
}

#endif // !defined(GL_Base_h)