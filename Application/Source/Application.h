
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"

class Application
{
public:
	Application();
	~Application();
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);

	//TEST
	void setEnablePointer(bool Enable);
	void setShowPointer(bool Show);
	bool getEnablePointer();
	bool getShowPointer();

private:

	//Declare a window object
	StopWatch m_timer;

	bool enablePointer = false;
	bool showPointer = true;
};

#endif