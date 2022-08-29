
#ifndef Tetris_h
#define Tetris_h

#define INITGUID

#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_mixer.h>
#include <stdlib.h>			// Randomization (srand, rand)
#include <time.h>			// Computer clock access (for randomization)
#include <fstream>		// File IO (high scores)

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
		//DrawMainMenu();
		break;

	case STATE_OPTIONS:
		break;

	case STATE_GAMETYPE:
		//DrawGameTypeMenu();
		break;

	case STATE_LEVEL:
		//DrawLevelMenu();
		break;

	case STATE_HEIGHT:
		//DrawHeightMenu();
		break;

	case STATE_MUSIC:
		//DrawMusicMenu();
		break;

	case STATE_GAME:
		//DrawGame();
		break;

	case STATE_PAUSED:
		//DrawPaused();
		break;

	case STATE_HIGHSCORES:
		//DrawHighScores();
		break;

	case STATE_NEWHIGH:
		//DrawNewHigh();
		break;
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

#endif // !defined (Tetris_h)