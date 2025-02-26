#include "Application.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

#include "MainMenu.h"
#include "KeyboardController.h"
#include "MouseController.h"
#include "CSceneManager.h"
#include "SceneArchery.h"
#include "SceneBalloonPop.h"
#include "SceneFinal.h"
#include "SceneHole.h"
#include "SceneMain.h"
#include "SceneSpinningRing.h"
#include "SceneWhackAMole.h"
#include "SceneBumperBalls.h"
#include "SceneWIUtest.h"

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame
bool Application::enablePointer = false;
bool Application::showPointer = false;

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	KeyboardController::GetInstance()->Update(key, action);
}

//Define the mouse button callback
static void mousebtn_callback(GLFWwindow* window, int button, int action,
	int mods)
{
	// Send the callback to the mouse controller to handle
	if (action == GLFW_PRESS)
		MouseController::GetInstance()->UpdateMouseButtonPressed(button);
	else
		MouseController::GetInstance()->UpdateMouseButtonReleased(button);
}
//Define the mouse scroll callback
static void mousescroll_callback(GLFWwindow* window, double xoffset,
	double yoffset)
{
	MouseController::GetInstance()->UpdateMouseScroll(xoffset, yoffset);
}

void Application::SetPointerStatus(bool status)
{
	enablePointer = status;
	if (enablePointer)
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	else
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h); //update opengl the new window size
}

bool Application::IsKeyPressed(unsigned short key)
{
	return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

Application::Application() : sceneManager()
{
}

Application::~Application()
{
}

void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

	//Create a window and create its OpenGL context
	m_window = glfwCreateWindow(1920, 1080, "CARNIVAL CHAOS by x86 Assembly", NULL, NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	glfwSetKeyCallback(m_window, key_callback);

	//Sets the mouse button callback
	glfwSetMouseButtonCallback(m_window, mousebtn_callback);
	//Sets the mouse scroll callback
	glfwSetScrollCallback(m_window, mousescroll_callback);

	//Sets the resize callback to handle window resizing
	glfwSetWindowSizeCallback(m_window, resize_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}

	if (enablePointer == false)
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else if (showPointer == false)
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// Initialize scene manager with main menu scene
	sceneManager.Init(SCENE_BUMPERBALLS);
}

void Application::Run()
{
	//Main Loop
	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (!glfwWindowShouldClose(m_window) && !IsKeyPressed(VK_ESCAPE))
	{
		// Process application input
		ProcessInput();

		// Update the scene manager
		sceneManager.Update(m_timer.getElapsedTime());

		// Render the current scene
		sceneManager.Render();

		//Swap buffers
		glfwSwapBuffers(m_window);

		KeyboardController::GetInstance()->PostUpdate();

		MouseController::GetInstance()->PostUpdate();
		double mouse_x, mouse_y;
		glfwGetCursorPos(m_window, &mouse_x, &mouse_y);
		MouseController::GetInstance()->UpdateMousePosition(mouse_x, mouse_y);

		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();
		m_timer.waitUntil(frameTime);       // Frame rate limiter
	}
}

void Application::ProcessInput()
{
    // Global go back to carnival
    if (KeyboardController::GetInstance()->IsKeyPressed('Q')) {
        // If not in main menu or carnival scene, pop back to carnival
        if (sceneManager.GetCurrentSceneType() != SCENE_MAINMENU &&
            sceneManager.GetCurrentSceneType() != SCENE_CARNIVAL) {
            sceneManager.PopScene();
        }
    }


    // Example: Handle special game transitions based on current scene
    Scene* currentScene = sceneManager.GetCurrentScene();
    if (currentScene) {
        // Check for specific scene transitions based on current minigame
        switch (sceneManager.GetCurrentSceneType()) {
        case SCENE_MAINMENU:
        {
            MainMenu* menuScene = static_cast<MainMenu*>(currentScene);
            if (menuScene && menuScene->shouldStartGame) {
                // Push SceneMain first
                sceneManager.PushScene(SCENE_MAINMENU);
                // Then push Carnival scene
                sceneManager.PushScene(SCENE_CARNIVAL);
                menuScene->shouldStartGame = false;
            }
            break;
        }
        case SCENE_CARNIVAL:
        {
            SceneMain* carnivalScene = static_cast<SceneMain*>(currentScene);
            if (carnivalScene) {
                // Process any minigame entrance requests
                if (carnivalScene->shouldEnterArchery) {
                    sceneManager.PushScene(SCENE_ARCHERY);
                    carnivalScene->shouldEnterArchery = false;
                }
                else if (carnivalScene->shouldEnterBalloonPop) {
                    sceneManager.PushScene(SCENE_BALLOONPOP);
                    carnivalScene->shouldEnterBalloonPop = false;
                }
                else if (carnivalScene->shouldEnterHole) {
                    sceneManager.PushScene(SCENE_HOLE);
                    carnivalScene->shouldEnterHole = false;
                }
                else if (carnivalScene->shouldEnterWhackAMole) {
                    sceneManager.PushScene(SCENE_WHACKAMOLE);
                    carnivalScene->shouldEnterWhackAMole = false;
                }
                else if (carnivalScene->shouldEnterSpinningRing) {
                    sceneManager.PushScene(SCENE_SPINNING_RING);
                    carnivalScene->shouldEnterSpinningRing = false;
                }
                else if (carnivalScene->shouldEnterWIUTest) {
                    sceneManager.PushScene(SCENE_WIUTEST);
                    carnivalScene->shouldEnterWIUTest = false;
                }
                // Check if player completed all required minigames to access final scene
                else if (carnivalScene->shouldEnterFinal) {
                    sceneManager.PushScene(SCENE_FINAL);
                    carnivalScene->shouldEnterFinal = false;
                }
            }
            break;
        }
        case SCENE_ARCHERY:
        {
            SceneArchery* archeryScene = static_cast<SceneArchery*>(currentScene);
            // Return to carnival scene only when player wins and presses E
            if (archeryScene->m_hasWon && KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_E)) {
                sceneManager.PopScene();
            }
            break;
        }
        case SCENE_BALLOONPOP:
        {
            SceneBalloonPop* balloonScene = static_cast<SceneBalloonPop*>(currentScene);
            if (balloonScene->m_hasWon && KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_E)) {
                sceneManager.PopScene();
            }
            break;
        }
        case SCENE_HOLE:
        {
            SceneHole* holeScene = static_cast<SceneHole*>(currentScene);
            // For SceneHole, check if gameResult > 0 (indicating a win)
            if (holeScene->gameResult > 0 && KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_E)) {
                sceneManager.PopScene();
            }
            break;
        }
        case SCENE_WHACKAMOLE:
        {
            SceneWhackAMole* whackAMoleScene = static_cast<SceneWhackAMole*>(currentScene);
            // For WhackAMole, you need to add a win condition flag in the scene
            // if (whackAMoleScene->hasWon && KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_E)) {
            //     sceneManager.PopScene();
            // }
            break;
        }
        case SCENE_SPINNING_RING:
        {
            SceneSpinningRing* spinningRingScene = static_cast<SceneSpinningRing*>(currentScene);
            if (spinningRingScene->playerWon && KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_E)) {
                sceneManager.PopScene();
            }
            break;
        }
        case SCENE_WIUTEST:
        {
            SceneWIUtest* wiuTestScene = static_cast<SceneWIUtest*>(currentScene);

            // if (wiuTestScene->hasWon && KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_E)) {
            //     sceneManager.PopScene();
            // }
            break;
        }
        case SCENE_FINAL:
        {
            SceneFinal* finalScene = static_cast<SceneFinal*>(currentScene);

            /*if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_R)) {
                sceneManager.ReplaceScene(SCENE_MAINMENU);
            }
            break;*/
        }
        default:
            break;
        }
    }
}

void Application::Exit()
{
	// Clean up scene manager
	sceneManager.Exit();

	KeyboardController::GetInstance()->DestroyInstance();
	MouseController::GetInstance()->DestroyInstance();

	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}

void Application::setEnablePointer(bool Enable)
{
	enablePointer = Enable;
}

void Application::setShowPointer(bool Show)
{
	showPointer = Show;
}

bool Application::getEnablePointer()
{
	return enablePointer;
}

bool Application::getShowPointer()
{
	return showPointer;
}
