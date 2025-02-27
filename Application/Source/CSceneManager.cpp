#include "CSceneManager.h"
#include "MainMenu.h"
#include "SceneMain.h"
#include "SceneWIUtest.h"
#include "SceneBalloonPop.h"
#include "SceneArchery.h"
#include "SceneHole.h"
#include "SceneWhackAMole.h"
#include "SceneBumperBalls.h"
#include "SceneSpinningRing.h"
#include "SceneFinal.h"
#include <iostream>

CSceneManager::CSceneManager()
    : currentSceneType(SCENE_MAINMENU)
{
}

CSceneManager::~CSceneManager()
{
    Exit();
}

void CSceneManager::Init(SCENE_TYPE startingScene)
{
    // Clear any existing scenes
    Exit();

    // Create and push the starting scene
    Scene* scene = CreateScene(startingScene);
    if (scene) {
        sceneStack.push(scene);
        typeStack.push(startingScene);
        currentSceneType = startingScene;
        scene->Init();
    }
}

void CSceneManager::Update(double dt)
{
    // Update the current scene
    if (!sceneStack.empty()) {
        sceneStack.top()->Update(dt);
    }
}

void CSceneManager::Render()
{
    // Render the current scene
    if (!sceneStack.empty()) {
        sceneStack.top()->Render();
    }
}

void CSceneManager::Exit()
{
    // Exit and cleanup all scenes (with manual memory management)
    while (!sceneStack.empty()) {
        Scene* scene = sceneStack.top();
        scene->Exit();
        delete scene;  // Free the memory

        sceneStack.pop();

        if (!typeStack.empty()) {
            typeStack.pop();
        }
    }
}

void CSceneManager::PushScene(SCENE_TYPE newScene)
{
    // Create the new scene
    Scene* scene = CreateScene(newScene);
    if (scene) {
        scene->Init();
        sceneStack.push(scene);
        typeStack.push(newScene);
        currentSceneType = newScene;

        std::cout << "Pushing scene: " << newScene << std::endl;
    }
}

void CSceneManager::PopScene()
{
    // Check if we're popping from a minigame and if the player won
    bool returningToCarnival = false;
    int minigameIndex = -1;
    bool hasWon = false;

    if (!sceneStack.empty()) {
        Scene* currentScene = sceneStack.top();

        // Check which minigame we're popping from and if it was won
        if (dynamic_cast<SceneArchery*>(currentScene)) {
            minigameIndex = 0;
            hasWon = dynamic_cast<SceneArchery*>(currentScene)->m_hasWon;
        }
        else if (dynamic_cast<SceneBalloonPop*>(currentScene)) {
            minigameIndex = 1;
            hasWon = dynamic_cast<SceneBalloonPop*>(currentScene)->m_hasWon;
        }
        else if (dynamic_cast<SceneHole*>(currentScene)) {
            minigameIndex = 2;
            hasWon = dynamic_cast<SceneHole*>(currentScene);
            if (dynamic_cast<SceneHole*>(currentScene)->gameResult == 1) {
                hasWon = true;
            }
            else {
                hasWon = false;
            }
        }
        else if (dynamic_cast<SceneWhackAMole*>(currentScene)) {
            minigameIndex = 3;
            hasWon = dynamic_cast<SceneWhackAMole*>(currentScene)->gamewin;
        }
        else if (dynamic_cast<SceneSpinningRing*>(currentScene)) {
            minigameIndex = 4;
            hasWon = dynamic_cast<SceneSpinningRing*>(currentScene)->playerWon;
        }
        else if (dynamic_cast<SceneBumperBalls*>(currentScene)) {
            minigameIndex = 5;
            hasWon = dynamic_cast<SceneBumperBalls*>(currentScene)->gamewin;
        }

        // Check if next scene is the carnival scene
        if (sceneStack.size() > 1) {
            Scene* temp = nullptr;
            sceneStack.pop();
            if (!sceneStack.empty()) {
                temp = sceneStack.top();
                if (dynamic_cast<SceneMain*>(temp)) {
                    returningToCarnival = true;

                    // If player won, mark the tent as completed
                    if (hasWon && minigameIndex >= 0) {
                        SceneMain* mainScene = dynamic_cast<SceneMain*>(temp);
                        if (mainScene) {
                            mainScene->SetTentCompleted(minigameIndex, true);
                        }
                    }

                    // Save state before we pop
                    SceneMain* mainScene = dynamic_cast<SceneMain*>(temp);
                    if (mainScene) {
                        mainScene->SaveState();
                    }
                }
            }
            // Restore the stack
            sceneStack.push(currentScene);
        }
    }

    // Pop scenes until we reach the Carnival scene
    while (sceneStack.size() > 1 && currentSceneType != SCENE_CARNIVAL)
    {
        Scene* currentScene = sceneStack.top();
        currentScene->Exit();  // Properly exit the scene to clean up resources
        delete currentScene;
        sceneStack.pop();
        typeStack.pop();

        if (!typeStack.empty()) {
            currentSceneType = typeStack.top();
        }
    }

    // If we're at Carnival, reinitialize it and restore state
    if (currentSceneType == SCENE_CARNIVAL)
    {
        std::cout << "Returning to Carnival scene with preserved state." << std::endl;
        Scene* carnivalScene = sceneStack.top();

        // Exit and reinitialize scene (RestoreState will be called in Init)
        carnivalScene->Exit();
        carnivalScene->Init();

        return;
    }
}

void CSceneManager::ReplaceScene(SCENE_TYPE newScene)
{
    // Exit and remove the current scene
    if (!sceneStack.empty()) {
        Scene* currentScene = sceneStack.top();
        currentScene->Exit();
        delete currentScene;  // Manual memory management

        sceneStack.pop();
        typeStack.pop();
    }

    // Create and push the new scene
    Scene* scene = CreateScene(newScene);
    if (scene) {
        scene->Init();
        sceneStack.push(scene);
        typeStack.push(newScene);
        currentSceneType = newScene;

        std::cout << "Replacing scene with: " << newScene << std::endl;
    }
}

Scene* CSceneManager::GetCurrentScene()
{
    if (!sceneStack.empty()) {
        return sceneStack.top();
    }
    return nullptr;
}

SCENE_TYPE CSceneManager::GetCurrentSceneType() const
{
    return currentSceneType;
}

Scene* CSceneManager::CreateScene(SCENE_TYPE sceneType)
{
    Scene* scene = nullptr;

    switch (sceneType) {
    case SCENE_MAINMENU:
        scene = new MainMenu();
        break;
    case SCENE_CARNIVAL:
        scene = new SceneMain();
        break;
    case SCENE_WIUTEST:
        scene = new SceneWIUtest();
        break;
    case SCENE_BALLOONPOP:
        scene = new SceneBalloonPop();
        break;
    case SCENE_ARCHERY:
        scene = new SceneArchery();
        break;
    case SCENE_HOLE:
        scene = new SceneHole();
        break;
    case SCENE_WHACKAMOLE:
        scene = new SceneWhackAMole();
        break;
    case SCENE_BUMPERBALLS:
        scene = new SceneBumperBalls();
        break;
    case SCENE_SPINNINGRING:
        scene = new SceneSpinningRing();
        break;
    case SCENE_FINAL:
        scene = new SceneFinal();
        break;
    default:
        std::cout << "Unknown scene type: " << sceneType << std::endl;
        break;
    }

    return scene;
}