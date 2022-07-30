
#include <windows.h>								// Header File For Windows

#include "GL Base.h"
#include "Tetris.h"

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int		nCmdShow)				// Window Show State
{
	Tetris tetris;


	MSG	msg;								// Windows Message Structure
	BOOL	done=FALSE;							// Bool Variable To Exit Loop

	if (!CreateGLWindow("Tetris",640,480,16,fullscreen))
		return 0;							// Quit If Window Was Not Created

	while(!done)								// Loop That Runs Until done=TRUE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))			// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;					// If So done=TRUE
			}
			else							// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else								// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if (active)						// Program Active?
			{
				switch (tetris.getGameState())
				{
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

				if (keys[VK_F1])					// Is F1 Being Pressed?
				{
					keys[VK_F1]=FALSE;				// If So Make Key FALSE
					KillGLWindow();					// Kill Our Current Window
					fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
				
					// Recreate Our OpenGL Window
					if (!CreateGLWindow("Tetris",640,480,16,fullscreen))
						return 0;				// Quit If Window Was Not Created
				}

				if (keys[VK_ESCAPE] && (tetris.getGameState() == STATE_MAINMENU))
					done = TRUE;

				tetris.draw();
				SwapBuffers(hDC);			// Swap Buffers (Double Buffering)

				tetris.Update();
			}
		}
	}

	// Shutdown
	KillGLWindow();				// Kill The Window
	return (msg.wParam);		// Exit The Program
}

LRESULT CALLBACK WndProc(	HWND	hWnd,		// Handle For This Window
							UINT	uMsg,		// Message For This Window
							WPARAM	wParam,		// Additional Message Information
							LPARAM	lParam)		// Additional Message Information
{
	switch (uMsg)
	{
		case WM_ACTIVATE:						// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
				active=TRUE;					// Program Is Active
			else
				active=FALSE;					// Program Is No Longer Active

			return 0;						// Return To The Message Loop
		}

		case WM_SYSCOMMAND:						// Intercept System Commands
		{
			switch (wParam)						// Check System Calls
			{
				case SC_SCREENSAVE:				// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;					// Prevent From Happening
			}
			break;							// Exit
		}

		case WM_CLOSE:							// Did We Receive A Close Message?
		{
			PostQuitMessage(0);					// Send A Quit Message
			return 0;						// Jump Back
		}

		case WM_KEYDOWN:						// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE

			return 0;						// Jump Back
		}

		case WM_KEYUP:							// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE

			return 0;						// Jump Back
		}

		case WM_SIZE:							// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));		// LoWord=Width, HiWord=Height
			return 0;						// Jump Back
		}
	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}