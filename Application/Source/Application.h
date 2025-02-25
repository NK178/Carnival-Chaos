#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include "CSceneManager.h"

class Application
{
private:
    // The scene manager instance
    CSceneManager sceneManager;

    // Timer for the game loop
    StopWatch m_timer;

    // For cursor control
    static bool enablePointer;
    static bool showPointer;

    // Process input for scene navigation and game control
    void ProcessInput();

public:
    // Constructor and Destructor
    Application();
    ~Application();

    // Initialize the application
    void Init();

    // Main game loop
    void Run();

    // Cleanup
    void Exit();

    // Input checking
    static bool IsKeyPressed(unsigned short key);
    static void SetPointerStatus(bool status);

    // Pointer visibility controls
    static void setEnablePointer(bool Enable);
    static void setShowPointer(bool Show);
    static bool getEnablePointer();
    static bool getShowPointer();
    static bool getBuffer();
};

#endif // APPLICATION_H
