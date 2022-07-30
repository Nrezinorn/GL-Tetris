
#ifndef RTimer_h
#define RTimer_h

#include <windows.h>

// Reliable timer
class RTimer
{
public:
	LARGE_INTEGER Freq;
	LARGE_INTEGER Start;

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
	QueryPerformanceFrequency (&Freq);
	QueryPerformanceCounter (&Start);
}

RTimer::~RTimer()
{
	TimedFunc = NULL;
}

void RTimer::Reset()
{
	QueryPerformanceCounter (&Start);
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
	LARGE_INTEGER current;
	double seconds;

	QueryPerformanceCounter (&current);

	seconds = ((double)(current.QuadPart - Start.QuadPart) / 
				(double)(Freq.QuadPart));

	return ((seconds * 1000));
}

double RTimer::getTimeOut()
{
	return TimeOut;
}

#endif // !defined (RTimer_h)