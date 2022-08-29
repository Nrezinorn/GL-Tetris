
#ifndef TTimer_h
#define TTimer_h

#include "RTimer.h"
#include "Tetris.h"

class Tetris;

class TTimer: public RTimer
{
public:
	Tetris* Master;

	TTimer(Tetris* game, double timeOut, void(*timedFunc)(void));
	virtual ~TTimer();

	virtual void Update();
};

TTimer::TTimer(Tetris* game, double timeOut, void(*timedFunc)(void))
	:RTimer(timeOut, timedFunc),
	 Master(game)
{}

TTimer::~TTimer()
{
	Master = NULL;
}

void TTimer::Update()
{
	if (ElapsedMillis() >= TimeOut)
	{
		Master->TimedFunc();
		Reset();
	}
}

#endif // !defined(TTimer_h)