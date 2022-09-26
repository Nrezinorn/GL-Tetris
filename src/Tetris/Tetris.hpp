
#ifndef Tetris_h
#define Tetris_h

#define INITGUID

#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_mixer.h>
#include <stdlib.h>			// Randomization (srand, rand)
#include <time.h>			// Computer clock access (for randomization)
#include <fstream>		// File IO (high scores)

//GL Helpers
#include "../GL_Base.hpp"

// related game bits
#include "Block.h"		// Block def
#include "Piece.h"		// Piece def
#include "Grid.h"		// Board def
#include "Block3D.h"	// Block3D def
#include "Menu.h"		// Menu def
#include "RTimer.h"		// Reliable Timer def

// for printing text
#include "../freetype.h"

using namespace std;


enum GAMESTATE { STATE_INTRO, STATE_MAINMENU, STATE_OPTIONS, STATE_GAMETYPE, 
				 STATE_LEVEL, STATE_HEIGHT, STATE_MUSIC, STATE_GAME, 
				 STATE_PAUSED, STATE_HIGHSCORES, STATE_NEWHIGH };
enum GAMETYPE  { TYPE_A = 65, TYPE_B = 66 };

enum SOUNDTYPE { MENU_MOVE = 0, MENU_SELECT, MENU_BACK, PIECE_MOVE, PIECE_ROTATE, 
				 PIECE_LAND, LINE_KILL, TETRIS, LEVELUP, PAUSE, DEATH };

const int LevelSpeedInc = 100;		// Speed increase per level in milliseconds
const float MenuLineHeight = 0.3f;	// Height of menu options

struct highScoreData
{
	char highName[11];
	char highScore[7];
	char highLines[5];
	char highLevel[3];
	char highType[2];
};

extern bool keys[1024];  // use global keys from MAIN.cpp

class Tetris
{
protected:
	Grid* grid;
	Piece* piece;
	Piece* nextPiece;
	RTimer* timer;
	Menu* menu;
	Block3D* menuBlock;

	GAMESTATE gameState;
	GAMETYPE gameType;
	unsigned short level;
	unsigned short musicType;
	unsigned short height;
	unsigned long lines;
	unsigned long score;
	Color3f colorScheme[3];

	bool linesAwaitingClear;
	unsigned short clearStep;
	unsigned short numFullLines;
	short fullLines[4];

	bool gameOver;
	unsigned short numDeadRows;

	bool unpausing;
	double timeAfterResume;

	highScoreData highScores[3];
	uint8_t newHighPos;


	Mix_Music*    pMusic[5];
	Mix_Chunk     pSound[10];

	bool hurry;
	double regTempo;
	double fastTempo;

    // TODO: Music Temp stuff
	//REFERENCE_TIME RTstart;
	//MUSIC_TIME MTsongLen;
	//MUSIC_TIME MTsongResumeStart;
    
	//font party
	freetype_mod::font_data myfont;

public:
	Tetris();
	~Tetris();

	inline GAMESTATE getGameState();
	inline GAMETYPE getGameType();
	inline void setGameState(GAMESTATE newState);

	void InitGame();

	void draw();
	void DrawIntro();
	void DrawMainMenu();
	void DrawGameTypeMenu();
	void DrawLevelMenu();
	void DrawHeightMenu();
	void DrawMusicMenu();
	void DrawGame();
	void DrawLeftInfo();
	void DrawRightInfo();
	void DrawPaused();
	void DrawHighScores();
	void DrawNewHigh();

	bool PathObstructed(unsigned short direction);
	bool RotationObstructed(unsigned short direction);

	void LineCheck();
	void Unify();

	void MainMenuEventHandler();
	void GameTypeMenuEventHandler();
	void LevelMenuEventHandler();
	void HeightMenuEventHandler();
	void MusicMenuEventHandler();
	void GameEventHandler();
	void PauseEventHandler();
	void HighScoresEventHandler();
	void NewHighEventHandler();

	void Update();
	void ColorizePieces();

	void InitAudio();
	void RetempoMusic();
};


Tetris::Tetris()
	:grid(NULL),
	 piece(NULL),
	 nextPiece(NULL),
	 timer(NULL),
	 gameState(STATE_INTRO),
	 gameType(TYPE_A),
	 level(0),
	 height(0),
	 lines(0),
	 score(0),
	 linesAwaitingClear(false),
	 clearStep(0),
	 numFullLines(0),
	 gameOver(false),
	 numDeadRows(0),
	 unpausing(false),
	 timeAfterResume(0),
	 hurry(false)
{
	srand (static_cast <unsigned> (time(new time_t())));

	for (int i = 0; i < 4; i++)
		fullLines[i] = -1;

	for (int i = 0; i < 3; i++)
	{
		colorScheme[i].red = 0.0f;
		colorScheme[i].green = 0.0f;
		colorScheme[i].blue = 0.0f;
	}

	menu = new Menu(3, 0, MenuLineHeight, "1 Player", "Options", "Quit");
	menuBlock = new Block3D(6.0f, 6.0f, 6.0f, 2.0f);

	timer = new RTimer(5000);

	// Load high scores
	fstream inStream("highscores.txt", ios::in);
	if (!inStream.fail())
	{
		for (int i = 0; i < 3; i++)
		{
			inStream.getline(highScores[i].highName, 20);
			inStream.getline(highScores[i].highScore, 10);
			inStream.getline(highScores[i].highLines, 10);
			inStream.getline(highScores[i].highLevel, 10);
			inStream.getline(highScores[i].highType, 10);
		}
	}

	inStream.close();

	myfont.init("courier.ttf", 16);
	InitGL();
 	//InitAudio();
}

Tetris::~Tetris()
{
	// Stop the music, and close down 
   //todo mix music stuff
   /*
	for (int i = 0; i < 5; i++)
		if (pMusic[i])
			pMusic[i]->Release();

	for (i = 0; i < 10; i++)
		if (pSound[i])
			pSound[i]->Release();
    */

	if (grid)
		delete grid;

	if (piece)
		delete piece;

	if (nextPiece)
		delete nextPiece;

	if (timer)
		delete timer;

	if (menu)
		delete menu;

	if (menuBlock)
		delete menuBlock;

	myfont.clean();
}

void Tetris::ColorizePieces()
{
	switch (piece->type)
	{
	case Z:
	case L:
		piece->setColor(colorScheme[0]);
		break;

	case Z2:
	case L2:
		piece->setColor(colorScheme[1]);
		break;

	case Line:
	case Cube:
	case T:
		piece->setColor(colorScheme[2]);
		break;
	}

	switch (nextPiece->type)
	{
	case Z:
	case L:
		nextPiece->setColor(colorScheme[0]);
		break;

	case Z2:
	case L2:
		nextPiece->setColor(colorScheme[1]);
		break;

	case Line:
	case Cube:
	case T:
		nextPiece->setColor(colorScheme[2]);
		break;
	}
}

void Tetris::draw()
{
	//glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	switch (gameState)
	{
	case STATE_INTRO:
		DrawIntro();
		break;

	case STATE_MAINMENU:
		DrawMainMenu();
		break;

	case STATE_OPTIONS:
		break;

	case STATE_GAMETYPE:
		DrawGameTypeMenu();
		break;

	case STATE_LEVEL:
		DrawLevelMenu();
		break;

	case STATE_HEIGHT:
		DrawHeightMenu();
		break;

	case STATE_MUSIC:
		DrawMusicMenu();
		break;

	case STATE_GAME:
		DrawGame();
		break;

	case STATE_PAUSED:
		DrawPaused();
		break;

	case STATE_HIGHSCORES:
		DrawHighScores();
		break;

	case STATE_NEWHIGH:
		DrawNewHigh();
		break;
	}
}

void Tetris::DrawGameTypeMenu()
{
//	glTranslatef (0.0f, 0.0f, -25.0f);
//	menuBlock->draw();
//	glLoadIdentity();

	//glTranslatef (0.0f, 0.0f, -5.0f);

	glColor3f(0.5f, 0.5f, 0.5f);

	glRasterPos2f (-.1f, 0.8f);
	freetype_mod::print(myfont,"1 Player");

	glColor3f(1.0f, 1.0f, 1.0f);

	glRasterPos2f (-0.1f, 0.5f);
	freetype_mod::print(myfont,"Game Type");

	glLoadIdentity();

	//glTranslatef (0.1f, 0.3f, -8.0f);
	menu->draw();
}

void Tetris::DrawHeightMenu()
{
	//	glTranslatef (0.0f, 0.0f, -25.0f);
	//	menuBlock->draw();
	//	glLoadIdentity();

//	glTranslatef (0.0f, 0.0f, -5.0f);

	glColor3f(0.5f, 0.5f, 0.5f);

	glRasterPos2f (-2.3f, 1.3f);
	freetype_mod::print(myfont,"1 Player");

	glRasterPos2f (-2.3f, 0.8f);
	freetype_mod::print(myfont,"Game Type");

	glRasterPos2f (-2.3f, 0.6f);
	freetype_mod::print(myfont,(gameType - 65 ? "Type B" : "Type A"));

	glRasterPos2f (-2.3f,  0.1f);
	freetype_mod::print(myfont,"Difficulty Level");

	glRasterPos2f (-2.3f, -0.1f);
	freetype_mod::print(myfont,"Level - %d", level);

	glColor3f(1.0f, 1.0f, 1.0f);

	glRasterPos2f (-0.35f,  0.5f);
	freetype_mod::print(myfont,"Starting Height");

	glLoadIdentity();
	//glTranslatef (0.5f, 0.3f, -8.0f);
	menu->draw();
}

void Tetris::DrawHighScores()
{
	glTranslatef (0.0f, -4.5f, -25.0f);

	Color3f color = { 0.5f, 0.5f, 0.5f };

	DrawExtrusion(22.0f, 7.0f, 0.5f, color);

	glTranslatef (0.0f, 2.5f, 0.0f);
	DrawIndent(21.0f, 1.0f, 0.3f, color);

	glTranslatef (0.0f, -3.5f, 0.0f);
	DrawIndent(21.0f, 4.0f, 0.3f, color);


	glLoadIdentity();
	glTranslatef (0.0f, 0.0f, -5.0f);

	glColor3f (1.0f, 1.0f, 1.0f);

	glRasterPos2f (-0.8f, 1.0f);
	freetype_mod::print(myfont,"The Tetris Masters");

	glRasterPos2f (-1.7f, -0.44f);
	freetype_mod::print(myfont,"Name");

	glRasterPos2f (-0.4f, -0.44f);
	freetype_mod::print(myfont,"Score");

	glRasterPos2f ( 0.5f, -0.44f);
	freetype_mod::print(myfont,"Lines");

	glRasterPos2f ( 1.2f, -0.44f);
	freetype_mod::print(myfont,"Lv");

	glRasterPos2f ( 1.6f, -0.44f);
	freetype_mod::print(myfont,"Type");

	glTranslatef (0.0f, -0.85f, 0.0f);

	for (int i = 0; i < 3; i++)
	{
		glRasterPos2f (-2.0f, -(i * 0.25f));
		freetype_mod::print(myfont,"%d", i + 1);

		glRasterPos2f (-1.7f, -(i * 0.25f));
		freetype_mod::print(myfont,"%s", highScores[i].highName);

		glRasterPos2f (-0.4f, -(i * 0.25f));
		freetype_mod::print(myfont,"%s", highScores[i].highScore);

		glRasterPos2f ( 0.5f, -(i * 0.25f));
		freetype_mod::print(myfont,"%s", highScores[i].highLines);

		glRasterPos2f ( 1.2f, -(i * 0.25f));
		freetype_mod::print(myfont,"%s", highScores[i].highLevel);

		glRasterPos2f ( 1.6f, -(i * 0.25f));
		freetype_mod::print(myfont,"%s", highScores[i].highType);
	}
}

void Tetris::DrawIntro()
{
	//BUG?:  text cant be translatef'd in Z space 

	glColor3f (1.0f, 1.0f, 1.0f);

	glRasterPos2f (-0.5f, 0.8f);
	freetype_mod::print(myfont, "Original concept, design,");

	glRasterPos2f (-0.3f, 0.6f);
	freetype_mod::print(myfont, "and program");

	glRasterPos2f (-0.45f, 0.4f);
	freetype_mod::print(myfont, "by Alexey Pazhitnov");

	glRasterPos2f (-0.1f, 0.2f);
	freetype_mod::print(myfont, "1987");

	glRasterPos2f (-0.4f, -0.2f);
	freetype_mod::print(myfont, "This incarnation");

	glRasterPos2f (-0.35f, -0.4f);
	freetype_mod::print(myfont, "by Dan Peschman");

	glRasterPos2f (-0.1f, -0.6f);
	freetype_mod::print(myfont,"2002");

	glRasterPos2f (-0.7f, -0.8f);
	freetype_mod::print(myfont,"SDL Port by Jim Gorz (c) 2022");

}

void Tetris::DrawLevelMenu()
{
//	glTranslatef (0.0f, 0.0f, -25.0f);
//	menuBlock->draw();
//	glLoadIdentity();

//	glTranslatef (0.0f, 0.0f, -5.0f);

	glColor3f(0.5f, 0.5f, 0.5f);

	glRasterPos2f (-2.3f, 1.3f);
	freetype_mod::print(myfont,"1 Player");

	glRasterPos2f (-2.3f, 0.8f);
	freetype_mod::print(myfont,"Game Type");

	glRasterPos2f (-2.3f, 0.6f);
	freetype_mod::print(myfont,(gameType - 65 ? "Type B" : "Type A"));

	glColor3f(1.0f, 1.0f, 1.0f);

	glRasterPos2f (-0.4f,  0.5f);
	freetype_mod::print(myfont,"Difficulty Level");

	glLoadIdentity();
//	glTranslatef (0.5f, 0.3f, -8.0f);
	menu->draw();
}

void Tetris::DrawMainMenu()
{
	glTranslatef (0.0f, 4.0f, -40.0f);
	menuBlock->draw();
	glLoadIdentity();

	//glTranslatef (0.0f, 0.0f, -4.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos2f (-0.20f, 0.5f);
	freetype_mod::print(myfont,"Tetris");

	glLoadIdentity();
	//glTranslatef (-0.3f, -2.0f, -8.0f);
	menu->draw();
}

void Tetris::DrawMusicMenu()
{
//	glTranslatef (0.0f, 0.0f, -25.0f);
//	menuBlock->draw();
//	glLoadIdentity();

	//glTranslatef (0.0f, 0.0f, -5.0f);

	glColor3f(0.5f, 0.5f, 0.5f);

	glRasterPos2f (-2.3f, 1.3f);
	freetype_mod::print(myfont,"1 Player");

	glRasterPos2f (-2.3f, 0.8f);
	freetype_mod::print(myfont,"Game Type");

	glRasterPos2f (-2.3f, 0.6f);
	freetype_mod::print(myfont,(gameType - 65 ? "Type B" : "Type A"));

	glRasterPos2f (-2.3f,  0.1f);
	freetype_mod::print(myfont,"Difficulty Level");

	glRasterPos2f (-2.3f, -0.1f);
	freetype_mod::print(myfont,"Level - %d", level);

	if (gameType == TYPE_B)
	{
		glRasterPos2f (-2.3f, -0.6f);
		freetype_mod::print(myfont,"Starting Height");

		glRasterPos2f (-2.3f, -0.8f);
		freetype_mod::print(myfont,"Height - %d", height);
	}

	glColor3f(1.0f, 1.0f, 1.0f);

	glRasterPos2f (-0.1f, 0.5f);
	freetype_mod::print(myfont,"Music Type");

	glLoadIdentity();
	//glTranslatef (0.5f, 0.3f, -8.0f);
	menu->draw();
}

void Tetris::DrawPaused()
{
	glTranslatef (0.0f, 0.0f, -25.0f);

	glColor3f (0.3f, 0.3f, 0.3f);

	glBegin(GL_QUADS);
		glVertex3f (-25.0f,  15.0f, 0.0f);
		glVertex3f (-25.0f, -15.0f, 0.0f);
		glVertex3f ( 25.0f, -15.0f, 0.0f);
		glVertex3f ( 25.0f,  15.0f, 0.0f);
	glEnd();

	glTranslatef (0.0f, 0.0f, 25.0f);  //was 20.f

	glColor3f (1.0f, 1.0f, 1.0f);

	glRasterPos2f (-0.2f, 0.0f);
	freetype_mod::print(myfont,"PAUSE");
}

void Tetris::GameEventHandler ()
{
	if (!gameOver && !linesAwaitingClear)
	{
		if (keys[SDL_SCANCODE_LEFT])
		{
			keys[SDL_SCANCODE_LEFT] = FALSE;

			if (!PathObstructed(LEFT))
			{
				// Play sound
				//todo sound
				//pPerformance->PlaySegmentEx( pSound[PIECE_MOVE], NULL, 
				//							NULL, DMUS_SEGF_SECONDARY, 0, NULL, NULL, NULL );

				piece->move(LEFT);
			}
		}

		if (keys[SDL_SCANCODE_RIGHT])
		{
			keys[SDL_SCANCODE_RIGHT] = FALSE;

			if (!PathObstructed(RIGHT))
			{
				// Play sound
				//todo sound
				//pPerformance->PlaySegmentEx( pSound[PIECE_MOVE], NULL, 
				//							NULL, DMUS_SEGF_SECONDARY, 0, NULL, NULL, NULL );

				piece->move(RIGHT);
			}
		}

		if (keys[SDL_SCANCODE_S])
		{
			keys[SDL_SCANCODE_S] = FALSE;

			if (!RotationObstructed(CW))
			{
				// Play sound
				//todo sound
				//pPerformance->PlaySegmentEx( pSound[PIECE_ROTATE], NULL, 
				//							NULL, DMUS_SEGF_SECONDARY, 0, NULL, NULL, NULL );

				piece->rotate(CW);
			}
		}

		if (keys[SDL_SCANCODE_A])
		{
			keys[SDL_SCANCODE_A] = FALSE;

			if (!RotationObstructed(CCW))
			{
				// Play sound
				//todo sound
				//pPerformance->PlaySegmentEx( pSound[PIECE_ROTATE], NULL, 
				//							NULL, DMUS_SEGF_SECONDARY, 0, NULL, NULL, NULL );

				piece->rotate(CCW);
			}
		}

		if (keys[SDL_SCANCODE_DOWN])
		{
			keys[SDL_SCANCODE_DOWN] = FALSE;

			if (!PathObstructed(DOWN))
				piece->move(DOWN);
			else
				Unify();

			timer->Reset();
		}

		if (keys[SDL_SCANCODE_RETURN] || keys[SDL_SCANCODE_P])
		{
			keys[SDL_SCANCODE_RETURN] = keys[SDL_SCANCODE_P] = FALSE;

			gameState = STATE_PAUSED;

			timeAfterResume = timer->getTimeOut() - timer->ElapsedMillis();

			delete timer;
			timer = NULL;

/*			if (musicType < 3)
			{
				REFERENCE_TIME RTcurrent;
				MUSIC_TIME MTcurrent, MTstart;

				pMusic[musicType]->GetLength( &MTsongLen );
				pPerformance->GetTime( &RTcurrent, &MTcurrent );
				pPerformance->ReferenceToMusicTime( RTstart, &MTstart );

				pPerformance->Stop( NULL, NULL, 0, 0 );

			//	*pMTsongResumeStart = ((*pMTcurrent) - (*pMTstart)) / (*pMTsongLen);
			}
*/		}
	}

	if (keys[SDL_SCANCODE_ESCAPE] || keys[SDL_SCANCODE_RETURN])
	{
		keys[SDL_SCANCODE_ESCAPE] = keys[SDL_SCANCODE_RETURN] = FALSE;

		// Show high scores
		if (score > StoL(highScores[2].highScore))
			setGameState(STATE_NEWHIGH);
		else
			setGameState(STATE_HIGHSCORES);

		// Stop music and unload the band
		//todo music
		//pPerformance->Stop( NULL, NULL, 0, 0 );
		//if (musicType < 3)
		///	pMusic[musicType]->Unload( pPerformance );
	}
}


void Tetris::HeightMenuEventHandler()
{
	if (keys[SDL_SCANCODE_DOWN])
	{
		keys[SDL_SCANCODE_DOWN] = FALSE;

		// Play sound
		//todo sound
		//pPerformance->PlaySegmentEx( pSound[MENU_MOVE], NULL, 
		//									NULL, DMUS_SEGF_SECONDARY, 
		//									0, NULL, NULL, NULL );

		menu->setSelectedOp(menu->getSelectedOp() < menu->getNumOps() - 1 ? menu->getSelectedOp() + 1 : 0);
	}

	if (keys[SDL_SCANCODE_UP])
	{
		keys[SDL_SCANCODE_UP] = FALSE;

		// Play sound
		//todo sound
		//pPerformance->PlaySegmentEx( pSound[MENU_MOVE], NULL, 
		//									NULL, DMUS_SEGF_SECONDARY, 
		//									0, NULL, NULL, NULL );

		menu->setSelectedOp(menu->getSelectedOp() > 0 ? menu->getSelectedOp() - 1 : menu->getNumOps() - 1);
	}

	if (keys[SDL_SCANCODE_RETURN])
	{
		keys[SDL_SCANCODE_RETURN] = FALSE;

		// Play sound
		//todo sound
		//pPerformance->PlaySegmentEx( pSound[MENU_SELECT], NULL, 
		//									NULL, DMUS_SEGF_SECONDARY, 
		//									0, NULL, NULL, NULL );

		height = menu->getSelectedOp();

		setGameState(STATE_MUSIC);

		// Download music type 1's instruments to the synthesizer
		//todo music
		//pMusic[0]->Download( pPerformance );

		// Play music type 1
		//todo music
		//pPerformance->PlaySegmentEx( pMusic[0], NULL, 
		//								NULL, 0, 0, NULL, NULL, NULL );
	}

	if (keys[SDL_SCANCODE_ESCAPE])
	{
		keys[SDL_SCANCODE_ESCAPE] = FALSE;

		// Play sound
		//todo sound
		//pPerformance->PlaySegmentEx( pSound[MENU_BACK], NULL, 
		//									NULL, DMUS_SEGF_SECONDARY, 
		//									0, NULL, NULL, NULL );

		setGameState(STATE_LEVEL);
	}
}

void Tetris::HighScoresEventHandler()
{
	if (keys[SDL_SCANCODE_ESCAPE] || keys[SDL_SCANCODE_RETURN])
	{
		keys[SDL_SCANCODE_ESCAPE] = keys[SDL_SCANCODE_RETURN] = FALSE;
		setGameState(STATE_MAINMENU);
	}
}

void Tetris::InitGame()
{
	grid = new Grid();
	piece = new Piece();
	nextPiece = new Piece();
	timer = new RTimer(1000 - (level * LevelSpeedInc));

	lines = 0;
	score = 0;

	for (int i = 0; i < 3; i++)
	{
		colorScheme[i].red = ((rand() % 7) / 10.0f) + 0.2f;
		colorScheme[i].green = ((rand() % 7) / 10.0f) + 0.2f;
		colorScheme[i].blue = ((rand() % 7) / 10.0f) + 0.2f;
	}

	ColorizePieces();

	linesAwaitingClear = false;
	clearStep = 0;
	numFullLines = 0;

	for (int i = 0; i < 4; i++)
		fullLines[i] = -1;

	gameOver = false;
	numDeadRows = 0;

	unpausing = false;
	timeAfterResume = 0;

	if (gameType == TYPE_B)
	{
		unsigned short badRows = 0;		// Number of disarrayed rows

		switch (height)
		{
		case 1:
			badRows = 3;
			break;
		case 2:
			badRows = 5;
			break;
		case 3:
			badRows = 8;
			break;
		case 4:
			badRows = 10;
			break;
		case 5:
			badRows = 12;
		}

		bool badLuck;	// Row completely filled with blocks, despite random insertion

		for (int r = blocksPerCol - 1; r >= blocksPerCol - badRows; r--)
		{
			badLuck = true;

			for (int c = 0; c < blocksPerRow; c++)
			{
				if (rand() % 2)
					grid->blocks[r][c] = new Block(colorScheme[rand() % 3]);
				else if (badLuck)
					badLuck = false;
			}

			// If the row totally fills with blocks, remove one of them
			if (badLuck)
			{
				unsigned short hole = rand() % blocksPerRow;
				delete grid->blocks[r][hole];
				grid->blocks[r][hole] = NULL;
			}
		}
	}

	if (musicType < 3)
	{
		
		//todo music
		//pPerformance->Stop( NULL, NULL, 0, 0 );

		double factor;
		if (musicType == 0)
			factor = 2.0;
		else
			factor = 1.5;
		
		//todo music
		//DMUS_TEMPO_PARAM tempo;
		//pMusic[musicType]->GetParam( GUID_TempoParam, 0xFFFFFFFF, DMUS_SEG_ALLTRACKS, 
		//							 (musicType < 2 ? 0 : 1), NULL, &tempo );

		// Adjust loop for dead space at end of music type 2
		//todo figure this out with mixMusic
		/*
		if (musicType == 1)
		{
			MUSIC_TIME MTstart, MTlen;

			pMusic[1]->GetLength( &MTlen );
			pMusic[1]->GetStartPoint( &MTstart );
			MTlen -= 3000;
			pMusic[1]->SetLoopPoints( MTstart, MTlen );
		}

		regTempo = tempo.dblTempo;
		fastTempo = regTempo * factor;

		pPerformance->GetTime( &RTstart, NULL );
		pPerformance->PlaySegmentEx( pMusic[musicType], NULL, 
											NULL, 0, 0, NULL, NULL, NULL );

		*/
	}
	
}


void Tetris::LineCheck ()
{
	bool lineFull;

	for (int r = 0; r < blocksPerCol; r++)
	{
		lineFull = true;
		for (int c = 0; c < blocksPerRow; c++)
			if (!grid->blocks[r][c])
			{
				lineFull = false;
				break;
			}

		if (lineFull)
			fullLines[numFullLines++] = r;
	}

	// If no complete lines, skip this stuff
	if (numFullLines > 0)
	{
		if (numFullLines < 4)
			// Play sound
			//->PlaySegmentEx( pSound[LINE_KILL], NULL, 
			//								NULL, DMUS_SEGF_SECONDARY, 
			//								0, NULL, NULL, NULL );
			int foo;
		else
			// Play sound
//			pPerformance->PlaySegmentEx( pSound[TETRIS], NULL, 
//											NULL, DMUS_SEGF_SECONDARY, 
//											0, NULL, NULL, NULL );
			int foo;

		switch (numFullLines)
		{
		case 1:
			score += 100 * (level + 1);
			break;
		case 2:
			score += 300 * (level + 1);
			break;
		case 3:
			score += 600 * (level + 1);
			break;
		case 4:
			score += 1000 * (level + 1);
			break;
		}

		lines += numFullLines;

		linesAwaitingClear = true;

		delete timer;
		timer = new RTimer(60);
	}
}

void Tetris::MainMenuEventHandler ()
{
	if (keys[SDL_SCANCODE_DOWN])
	{
		keys[SDL_SCANCODE_DOWN] = FALSE;

		// Play sound
		//pPerformance->PlaySegmentEx( pSound[MENU_MOVE], NULL, 
		//									NULL, DMUS_SEGF_SECONDARY, 
		//									0, NULL, NULL, NULL );

		menu->setSelectedOp(menu->getSelectedOp() < menu->getNumOps() - 1 ? menu->getSelectedOp() + 1 : 0);
	}

	if (keys[SDL_SCANCODE_UP])
	{
		keys[SDL_SCANCODE_UP] = FALSE;

		// Play sound
		//pPerformance->PlaySegmentEx( pSound[MENU_MOVE], NULL, 
		//									NULL, DMUS_SEGF_SECONDARY, 
		//									0, NULL, NULL, NULL );

		menu->setSelectedOp(menu->getSelectedOp() > 0 ? menu->getSelectedOp() - 1 : menu->getNumOps() - 1);
	}

	if (keys[SDL_SCANCODE_RETURN])
	{
		keys[SDL_SCANCODE_RETURN] = FALSE;

		// Play sound
		//pPerformance->PlaySegmentEx( pSound[MENU_SELECT], NULL, 
		//									NULL, DMUS_SEGF_SECONDARY, 
		//									0, NULL, NULL, NULL );

		switch (menu->getSelectedOp())
		{
		// 1 Player
		case 0:
			setGameState(STATE_GAMETYPE);
			break;

		// Options
		case 1:
			break;

		// Quit
		case 2:
			keys[SDL_SCANCODE_ESCAPE] = TRUE;
			break;
		}
	}
}

void Tetris::MusicMenuEventHandler ()
{
	if (keys[SDL_SCANCODE_DOWN])
	{
		keys[SDL_SCANCODE_DOWN] = FALSE;

		menu->setSelectedOp(menu->getSelectedOp() < menu->getNumOps() - 1 ? menu->getSelectedOp() + 1 : 0);

		// Stop previously playing segment
		//todo stop music
		//pPerformance->Stop( NULL, NULL, 0, 0 );

		// Play sound
		//play sound
		//pPerformance->PlaySegmentEx( pSound[MENU_MOVE], NULL, 
		//									NULL, DMUS_SEGF_SECONDARY, 
		//									0, NULL, NULL, NULL );

		// Unload the old segment's instruments from the synthesizer
		
		//todo music stuff
		/*
		if (menu->getSelectedOp() > 0)
			pMusic[menu->getSelectedOp() - 1]->Unload( pPerformance );

		if (menu->getSelectedOp() < menu->getNumOps() - 1)
		{
			// Download the segment's instruments to the synthesizer
			pMusic[menu->getSelectedOp()]->Download( pPerformance );

			// Begin new segment
			pPerformance->PlaySegmentEx( pMusic[menu->getSelectedOp()], NULL, 
											NULL, 0, 0, NULL, NULL, NULL );
		}
		*/
	}

	if (keys[SDL_SCANCODE_UP])
	{
		keys[SDL_SCANCODE_UP] = FALSE;

		menu->setSelectedOp(menu->getSelectedOp() > 0 ? menu->getSelectedOp() - 1 : menu->getNumOps() - 1);

		// Stop previously playing segment
		//todo stop music
		//pPerformance->Stop( NULL, NULL, 0, 0 );

		// Play sound
		//todo sound
		//pPerformance->PlaySegmentEx( pSound[MENU_MOVE], NULL, 
		//									NULL, DMUS_SEGF_SECONDARY, 
		//									0, NULL, NULL, NULL );

		/*
		if (menu->getSelectedOp() < menu->getNumOps() - 2)
			// Unload the old segment's instruments from the synthesizer
			pMusic[menu->getSelectedOp() + 1]->Unload( pPerformance );

		if (menu->getSelectedOp() < menu->getNumOps() - 1)
		{
			// Download the segment's instruments to the synthesizer
			pMusic[menu->getSelectedOp()]->Download( pPerformance );

			// Begin new segment
			pPerformance->PlaySegmentEx( pMusic[menu->getSelectedOp()], NULL, 
											NULL, 0, 0, NULL, NULL, NULL );
		}
		else
			// Unload the old segment's instruments from the synthesizer
			pMusic[0]->Unload( pPerformance );

		*/
	}

	if (keys[SDL_SCANCODE_RETURN])
	{
		keys[SDL_SCANCODE_RETURN] = FALSE;

		// Play sound
		//todo sound
		//pPerformance->PlaySegmentEx( pSound[MENU_SELECT], NULL, 
		//									NULL, DMUS_SEGF_SECONDARY, 
		//									0, NULL, NULL, NULL );

		musicType = menu->getSelectedOp();

		gameState = STATE_GAME;

		delete menuBlock;
		menuBlock = NULL;
		delete menu;
		menu = NULL;

		InitGame();
	}

	if (keys[SDL_SCANCODE_ESCAPE])
	{
		keys[SDL_SCANCODE_ESCAPE] = FALSE;

		// Play sound
		//todo sound
		//pPerformance->PlaySegmentEx( pSound[MENU_BACK], NULL, 
		//									NULL, DMUS_SEGF_SECONDARY, 
		//									0, NULL, NULL, NULL );

		if (gameType == TYPE_A)
			setGameState(STATE_LEVEL);
		else
			setGameState(STATE_HEIGHT);

		//todo music?
		//pPerformance->Stop( NULL, NULL, 0, 0 );
	}
}

void Tetris::NewHighEventHandler()
{
	short len = strlen(highScores[newHighPos].highName);

	if (len < 10)
	{
		// Letters
		for (int k = 65; k <= 90; k++)
		{
			if (keys[k])
			{
				keys[k] = FALSE;

				sprintf (highScores[newHighPos].highName, "%s%c", 
						 highScores[newHighPos].highName, 
						 (keys[SDL_SCANCODE_LSHIFT] ? k : k + 32));  //only LSHIFT works for capitol
			}
		}

		// Numbers
		for (int k = 48; k <= 57; k++)
		{
			if (keys[k])
			{
				keys[k] = FALSE;

				sprintf (highScores[newHighPos].highName, "%s%c", 
						 highScores[newHighPos].highName, k);
			}
		}

		if (keys[SDL_SCANCODE_SPACE])
		{
			keys[SDL_SCANCODE_SPACE] = FALSE;

			sprintf (highScores[newHighPos].highName, "%s ", 
					 highScores[newHighPos].highName);
		}
	}

	if (len > 0)
		if (keys[SDL_SCANCODE_BACKSPACE])
		{
			keys[SDL_SCANCODE_BACKSPACE] = FALSE;

			highScores[newHighPos].highName[len - 1] = '\0';
		}

	if (keys[SDL_SCANCODE_RETURN])
	{
		keys[SDL_SCANCODE_RETURN] = FALSE;

		setGameState(STATE_MAINMENU);
	}

	if (keys[SDL_SCANCODE_ESCAPE])
	{
		keys[SDL_SCANCODE_ESCAPE] = FALSE;

		setGameState(STATE_MAINMENU);
	}
}

void Tetris::PauseEventHandler()
{
	if (keys[SDL_SCANCODE_RETURN] || keys[SDL_SCANCODE_P] || keys[SDL_SCANCODE_ESCAPE])
	{
		keys[SDL_SCANCODE_RETURN] = keys[SDL_SCANCODE_P] = keys[SDL_SCANCODE_ESCAPE] = FALSE;

		gameState = STATE_GAME;

		// Unpause timer
		unpausing = true;
		timer = new RTimer (timeAfterResume);
	}
}

void Tetris::GameTypeMenuEventHandler ()
{
	if (keys[SDL_SCANCODE_DOWN])
	{
		keys[SDL_SCANCODE_DOWN] = FALSE;

		// Play sound
		//pPerformance->PlaySegmentEx( pSound[MENU_MOVE], NULL, 
		//									NULL, DMUS_SEGF_SECONDARY, 
		//									0, NULL, NULL, NULL );

		menu->setSelectedOp(menu->getSelectedOp() < menu->getNumOps() - 1 ? menu->getSelectedOp() + 1 : 0);
	}

	if (keys[SDL_SCANCODE_UP])
	{
		keys[SDL_SCANCODE_UP] = FALSE;

		// Play sound
		//todo:  play sound
		//pPerformance->PlaySegmentEx( pSound[MENU_MOVE], NULL, 
		//									NULL, DMUS_SEGF_SECONDARY, 
		//									0, NULL, NULL, NULL );

		menu->setSelectedOp(menu->getSelectedOp() > 0 ? menu->getSelectedOp() - 1 : menu->getNumOps() - 1);
	}

	if (keys[SDL_SCANCODE_RETURN])
	{
		keys[SDL_SCANCODE_RETURN] = FALSE;

		// Play sound
		//todo:  play sound
		//pPerformance->PlaySegmentEx( pSound[MENU_SELECT], NULL, 
		//									NULL, DMUS_SEGF_SECONDARY, 
	//										0, NULL, NULL, NULL );

		switch (menu->getSelectedOp())
		{
		// Type A
		case 0:
			gameType = TYPE_A;
			break;

		// Type B
		case 1:
			gameType = TYPE_B;
			break;
		}

		setGameState(STATE_LEVEL);
	}

	if (keys[SDL_SCANCODE_ESCAPE])
	{
		keys[SDL_SCANCODE_ESCAPE] = FALSE;

		// Play sound
		//todo:  play sound
		//pPerformance->PlaySegmentEx( pSound[MENU_BACK], NULL, 
		//									NULL, DMUS_SEGF_SECONDARY, 
		//									0, NULL, NULL, NULL );

		setGameState(STATE_MAINMENU);
	}
}

inline GAMESTATE Tetris::getGameState()
{
	return gameState;
}

inline GAMETYPE Tetris::getGameType()
{
	return gameType;
}

void Tetris::LevelMenuEventHandler ()
{
	if (keys[SDL_SCANCODE_DOWN])
	{
		keys[SDL_SCANCODE_DOWN] = FALSE;

		// Play sound
		//todo:: sound
		//pPerformance->PlaySegmentEx( pSound[MENU_MOVE], NULL, 
		//									NULL, DMUS_SEGF_SECONDARY, 
		//									0, NULL, NULL, NULL );

		menu->setSelectedOp(menu->getSelectedOp() < menu->getNumOps() - 1 ? menu->getSelectedOp() + 1 : 0);
	}

	if (keys[SDL_SCANCODE_UP])
	{
		keys[SDL_SCANCODE_UP] = FALSE;

		// Play sound
		//todo sound
		//pPerformance->PlaySegmentEx( pSound[MENU_MOVE], NULL, 
		//									NULL, DMUS_SEGF_SECONDARY, 
		//									0, NULL, NULL, NULL );

		menu->setSelectedOp(menu->getSelectedOp() > 0 ? menu->getSelectedOp() - 1 : menu->getNumOps() - 1);
	}

	if (keys[SDL_SCANCODE_RETURN])
	{
		keys[SDL_SCANCODE_RETURN] = FALSE;

		// Play sound
		//todo: sound
		//pPerformance->PlaySegmentEx( pSound[MENU_SELECT], NULL, 
		//									NULL, DMUS_SEGF_SECONDARY, 
		//									0, NULL, NULL, NULL );

		level = menu->getSelectedOp();

		if (gameType == TYPE_A)
		{
			setGameState (STATE_MUSIC);

			// Download music type 1's instruments to the synthesizer
			//todo music
			//pMusic[0]->Download( pPerformance );

			// Play music type 1
			//today music
			//pPerformance->PlaySegmentEx( pMusic[0], NULL, 
			//								NULL, 0, 0, NULL, NULL, NULL );
		}
		else
			setGameState (STATE_HEIGHT);
	}

	if (keys[SDL_SCANCODE_ESCAPE])
	{
		keys[SDL_SCANCODE_ESCAPE] = FALSE;

		// Play sound
		//todo: sound
		//pPerformance->PlaySegmentEx( pSound[MENU_BACK], NULL, 
		//									NULL, DMUS_SEGF_SECONDARY, 
		//									0, NULL, NULL, NULL );

		setGameState(STATE_GAMETYPE);
	}
}

inline void Tetris::setGameState (GAMESTATE newState)
{
	if (gameState == newState)
		return;

	switch (gameState)
	{
	case STATE_MAINMENU:
	case STATE_OPTIONS:
	case STATE_GAMETYPE:
	case STATE_LEVEL:
	case STATE_HEIGHT:
	case STATE_MUSIC:
		delete menuBlock;
		menuBlock = NULL;
		delete menu;
		menu = NULL;

		break;

	case STATE_GAME:
		delete grid;
		delete piece;
		delete nextPiece;
		delete timer;

		grid = NULL;
		piece = NULL;
		nextPiece = NULL;
		timer = NULL;

		break;

	case STATE_NEWHIGH:

		// Save high scores
		fstream outStream("highscores.txt", ios::out);

		if (!outStream.fail())
		{
			for (int i = 0; i < 3; i++)
			{
				outStream.write(highScores[i].highName, strlen(highScores[i].highName));
				outStream << '\n';
				outStream.write(highScores[i].highScore, strlen(highScores[i].highScore));
				outStream << '\n';
				outStream.write(highScores[i].highLines, strlen(highScores[i].highLines));
				outStream << '\n';
				outStream.write(highScores[i].highLevel, strlen(highScores[i].highLevel));
				outStream << '\n';
				outStream.write(highScores[i].highType, strlen(highScores[i].highType));
				outStream << '\n';
			}
		}

		outStream.close();

		break;
	}

	gameState = newState;

	switch (newState)
	{
	case STATE_MAINMENU:
		menuBlock = new Block3D(6.0f, 6.0f, 6.0f, 2.0f);
		menu = new Menu(3, 0, MenuLineHeight, "1 Player", "Options", "Quit");

		break;

	case STATE_OPTIONS:
		break;

	case STATE_GAMETYPE:
		menuBlock = new Block3D(6.0f, 6.0f, 6.0f, 2.0f);
		menu = new Menu(2, 0, MenuLineHeight, "Type A", "Type B");

		break;

	case STATE_LEVEL:
		menuBlock = new Block3D(6.0f, 6.0f, 6.0f, 2.0f);
		menu = new Menu(10, 0, MenuLineHeight, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9");

		break;

	case STATE_HEIGHT:
		menuBlock = new Block3D(6.0f, 6.0f, 6.0f, 2.0f);
		menu = new Menu(6, 0, MenuLineHeight, "0", "1", "2", "3", "4", "5");

		break;

	case STATE_MUSIC:
		menuBlock = new Block3D(6.0f, 6.0f, 6.0f, 2.0f);
		menu = new Menu(4, 0, MenuLineHeight, "1", "2", "3", "Off");

		break;

	case STATE_NEWHIGH:
		newHighPos = 2;

		if (score > StoL(highScores[1].highScore))
		{
			newHighPos = 1;

			// Move 1 to 2
			sprintf(highScores[2].highName, "%s", highScores[1].highName);
			sprintf(highScores[2].highScore, "%s", highScores[1].highScore);
			sprintf(highScores[2].highLines, "%s", highScores[1].highLines);
			sprintf(highScores[2].highLevel, "%s", highScores[1].highLevel);
			sprintf(highScores[2].highType, "%s", highScores[1].highType);
		}
		if (score > StoL(highScores[0].highScore))
		{
			newHighPos = 0;

			// Move 0 to 1
			sprintf(highScores[1].highName, "%s", highScores[0].highName);
			sprintf(highScores[1].highScore, "%s", highScores[0].highScore);
			sprintf(highScores[1].highLines, "%s", highScores[0].highLines);
			sprintf(highScores[1].highLevel, "%s", highScores[0].highLevel);
			sprintf(highScores[1].highType, "%s", highScores[0].highType);
		}

		// Put new high in position
		sprintf(highScores[newHighPos].highName, "");
		sprintf(highScores[newHighPos].highScore, "%d", score);
		sprintf(highScores[newHighPos].highLines, "%d", lines);
		sprintf(highScores[newHighPos].highLevel, "%d", level);
		sprintf(highScores[newHighPos].highType, "%c", gameType);

		break;

	case STATE_GAME:
		break;
	}
}

// Checks to see if a block exists in the piece's rotation destination
bool Tetris::RotationObstructed (unsigned short int direction)
{
	Point2i testLayout[4];
	int i;

	// Cube never rotates
	if (piece->type == Cube)
		return false;

	// *Hack*  Z and Z2 do not rotate as normal - only have 2 rotation states
	if ((piece->type == Z) || (piece->type == Z2))
		direction = (piece->rotated ? CW : CCW);

	switch (direction)
	{
	case CCW:
		// Flip the piece along the line y = -x
		for (i = 0; i < 4; i++)
		{
			testLayout[i].x = -piece->layout[i].y;
			testLayout[i].y = -piece->layout[i].x;
		}
			// For everything except the Line
		if (piece->type != Line)
		{
			// Flip the piece along the line y = 0
			for (i = 0; i < 4; i++)
				testLayout[i].y = -testLayout[i].y;
		}

		break;

	case CW:
		int temp;

		for (i = 0; i < 4; i++)
			testLayout[i] = piece->layout[i];

		// For everything except the Line
		if (piece->type != Line)
		{
			// Flip the piece along the line y = 0
			for (i = 0; i < 4; i++)
				testLayout[i].y = -piece->layout[i].y;
		}

		// Flip the piece along the line y = -x
		for (i = 0; i < 4; i++)
		{
			temp = piece->layout[i].x;
			testLayout[i].x = -testLayout[i].y;
			testLayout[i].y = -temp;
		}

		break;
	}

	for (i = 0; i < 4; i++)
	{
		if (piece->position.y - testLayout[i].y >= 0)
		{
			if (piece->position.x + testLayout[i].x < 0)
				return true;

			if (piece->position.x + testLayout[i].x >= blocksPerRow)
				return true;

			if (grid->blocks[piece->position.y - testLayout[i].y][piece->position.x + testLayout[i].x])
				return true;
		}
	}
	
	return false;
}


void Tetris::Update()
{
	if (timer)
	{
		if (timer->TimeToUpdate())
		{
			if (gameState == STATE_INTRO)
			{
				gameState = STATE_MAINMENU;

				delete timer;
				timer = NULL;

				return;
			}

			if (linesAwaitingClear)
			{
				if (clearStep < 5)
				{
					short a = 4 - clearStep;
					short b = 5 + clearStep;

					for (int i = 0; i < numFullLines; i++)
					{
						delete grid->blocks[fullLines[i]][a];
						delete grid->blocks[fullLines[i]][b];

						grid->blocks[fullLines[i]][a] = NULL;
						grid->blocks[fullLines[i]][b] = NULL;
					}

					clearStep++;
				}
				// Finished lines are gone
				else
				{
					// Lower the blocks in the rows above the recently deceased
					for (int line = 0; line < numFullLines; line++)
						for (int r = fullLines[line]; r >= 0; r--)
							for (int c = 0; c < blocksPerRow; c++)
							{
								delete grid->blocks[r][c];
								grid->blocks[r][c] = NULL;

								// Can't drop rows above the top one
								if (r != 0)
									if (grid->blocks[r - 1][c])
										grid->blocks[r][c] = new Block(grid->blocks[r - 1][c]->color);
							}

					linesAwaitingClear = false;
					clearStep = 0;
					numFullLines = 0;
					for (int i = 0; i < 4; i++)
						fullLines[i] = -1;

					RetempoMusic();

					//	New level
					if ((static_cast <int> (lines / 10) - level > 0) && (level < 19))
					{
						level++;

						// Play sound
						//pPerformance->PlaySegmentEx( pSound[LEVELUP], NULL, 
						//							NULL, DMUS_SEGF_SECONDARY, 
						//							0, NULL, NULL, NULL );

						Color3f oldScheme[3];

						for (int i = 0; i < 3; i++)
						{
							oldScheme[i] = colorScheme[i];

							colorScheme[i].red = ((rand() % 7) / 10.0f) + 0.2f;;
							colorScheme[i].green = ((rand() % 7) / 10.0f) + 0.2f;;
							colorScheme[i].blue = ((rand() % 7) / 10.0f) + 0.2f;;
						}

						for (int r = 0; r < blocksPerCol; r++)
							for (int c = 0; c < blocksPerRow; c++)
								if (grid->blocks[r][c])
								{
									if (grid->blocks[r][c]->color == oldScheme[0])
										grid->blocks[r][c]->color = colorScheme[0];
									else if (grid->blocks[r][c]->color == oldScheme[1])
										grid->blocks[r][c]->color = colorScheme[1];
									else
										grid->blocks[r][c]->color = colorScheme[2];
								}
					}

					if (level < 10)
					{
						delete timer;
						timer = new RTimer (1000 - ((level < 10 ? level : 9) * LevelSpeedInc));
					}
					
					piece = nextPiece;
					nextPiece = new Piece();

					ColorizePieces();
				}

				timer->Reset();
				return;
			}
			
			if (gameOver)
			{
				if (numDeadRows < blocksPerCol)
					numDeadRows++;

				timer->Reset();
				return;
			}

			if (unpausing)
			{
				unpausing = false;
				timeAfterResume = 0;

				delete timer;
				timer = new RTimer (1000 - ((level < 10 ? level : 9) * LevelSpeedInc));
			}

			if (piece)
			{
				if (!PathObstructed(DOWN))
					piece->move(DOWN);
				else
					Unify();
			}

			timer->Reset();
		}
	}
}


bool Tetris::PathObstructed (unsigned short direction)
{
	for (int i = 0; i < 4; i++)
	{
		if (piece->position.y - piece->layout[i].y >= 0)
		{
			switch (direction)
			{
			case DOWN:
				if (piece->position.y - piece->layout[i].y + 1 == blocksPerCol)
					return true;

				if (grid->blocks[piece->position.y - piece->layout[i].y + 1][piece->position.x + piece->layout[i].x])
					return true;

				break;

			case RIGHT:
				if (piece->position.x + piece->layout[i].x + 1 == blocksPerRow)
					return true;

				if (grid->blocks[piece->position.y - piece->layout[i].y][piece->position.x + piece->layout[i].x + 1])
					return true;

				break;

			case LEFT:
				if (piece->position.x + piece->layout[i].x == 0)
					return true;

				if (grid->blocks[piece->position.y - piece->layout[i].y][piece->position.x + piece->layout[i].x - 1])
					return true;

				break;
			}
		}
	}

	return false;
}

void Tetris::RetempoMusic() {

}

// Puts the blocks of the piece into the grid, destroys the piece, and makes a new one
void Tetris::Unify ()
{
	for (int i = 0; i < 4; i++)
	{
		grid->blocks[piece->position.y - piece->layout[i].y][piece->position.x + piece->layout[i].x] =
			new Block(piece->color);
	}

	LineCheck();
	RetempoMusic();

	// Play sound
	//pPerformance->PlaySegmentEx( pSound[PIECE_LAND], NULL, 
	//								NULL, DMUS_SEGF_SECONDARY, 0, NULL, NULL, NULL );

	// The piece has landed - generate a new one
	delete piece;

	if (linesAwaitingClear)
		piece = NULL;
	else
	{
		piece = nextPiece;
		nextPiece = new Piece();

		ColorizePieces();
	}

	// Check for block overwrite (game over)
	if (piece)
		for (int i = 0; i < 4; i++)
			if (piece->position.y - piece->layout[i].y >= 0)
				if (grid->blocks[piece->position.y - piece->layout[i].y][piece->position.x + piece->layout[i].x])
					gameOver = true;

	if (gameOver)
	{
		delete timer;
		timer = new RTimer(200);

		// Cut the music
		//pPerformance->Stop( NULL, NULL, 0, 0 );

		// Unload the segment's band
		//if (musicType < 3)
			//pMusic[musicType]->Unload( pPerformance );

		// Play "death" sound
//		pPerformance->PlaySegmentEx( pSound[0], NULL, NULL, 0, 0, NULL, NULL, NULL );
	}
}

#endif // !defined (Tetris_h)