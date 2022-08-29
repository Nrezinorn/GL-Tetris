
#ifndef RTimer_h
#define RTimer_h

//TODO use SDL frequency counters for compat
#include <SDL2/SDL.h>

// Reliable timer
class RTimer
{
public:
    uint64_t Freq;
	uint64_t Start;

	double TimeOut;
	void (*TimedFunc)(void);

	RTimer(double timeOut, void(*timedFunc)(void) = NULL);
	virtual ~RTimer();

	double getTimeOut();
	void Reset();
	void Update();
	bool TimeToUpdate();
	double ElapsedMillis ();
};

RTimer::RTimer (double timeOut, void(*timedFunc)(void))
	:TimeOut(timeOut),
	 TimedFunc(timedFunc)
{
	Freq = SDL_GetPerformanceFrequency();
	Start = SDL_GetPerformanceCounter();
}

RTimer::~RTimer()
{
	TimedFunc = NULL;
}

void RTimer::Reset()
{
	Start = SDL_GetPerformanceCounter();
}

void RTimer::Update()
{
	if (ElapsedMillis() >= TimeOut)
	{
		TimedFunc();
		Reset();
	}
}

bool RTimer::TimeToUpdate()
{
	if (ElapsedMillis() >= TimeOut)
		return true;

	return false;
}

double RTimer::ElapsedMillis ()
{
	uint64_t current;
	double seconds;

	current = SDL_GetPerformanceCounter();

	seconds = ((double)(current-Start) / 
				(double)(Freq));

	return ((seconds * 1000));
}

double RTimer::getTimeOut()
{
	return TimeOut;
}

#endif // !defined (RTimer_h)