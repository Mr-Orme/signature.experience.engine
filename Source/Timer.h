#ifndef TIMER_H
#define TIMER_H

#include"SDL.h"
#include"Definitions.h"

class Timer
{
public:
	//Initializes variables
	Timer();
	bool Initialize(EngineDefs::Int);

	//The various clock actions
	void start();
	void stop();
	void pause();
	void unpause();

	//Gets the timer's time
	EngineDefs::Int getTicks();

	//Checks the status of the timer
	bool isStarted();
	bool isPaused();
	
	//Regulate
	void fpsRegulate();

private:
	//The clock time when the timer started
	EngineDefs::Int startTicks;
	EngineDefs::Float mpf; //millisecond per frame

	//The ticks stored when the timer was paused
	EngineDefs::Int pausedTicks;

	//The timer status
	bool paused;
	bool started;
};

#endif