#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include "CSceneManager.h"

class Application
{
public:
	Application();
	~Application();
	void Init();
	void Run();
	void Exit();
	static void SetPointerStatus(bool status);
	static bool IsKeyPressed(unsigned short key);

	//TEST
	void setEnablePointer(bool Enable);
	void setShowPointer(bool Show);
	bool getEnablePointer();
	bool getShowPointer();
	static bool enablePointer;

private:

	//Declare a window object
	StopWatch m_timer;
	bool showPointer = true;
};

#endif