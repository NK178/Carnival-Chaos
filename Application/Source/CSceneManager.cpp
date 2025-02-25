#include "CSceneManager.h"
#include "MainMenu.h"
#include "SceneMain.h"
#include "SceneWIUtest.h"
#include "SceneBalloonPop.h"
#include "SceneArchery.h"
#include "SceneHole.h"
#include "SceneWhackAMole.h"
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
    // Pop scenes until we reach the Carnival scene
    while (sceneStack.size() > 1 && currentSceneType != SCENE_CARNIVAL)
    {
        Scene* currentScene = sceneStack.top();
        currentScene->Exit();
        delete currentScene;
        sceneStack.pop();
        typeStack.pop();

        if (!typeStack.empty())
        {
            currentSceneType = typeStack.top();
        }
    }

    // If we're already at Carnival, reinitialize it to reset its state
    if (currentSceneType == SCENE_CARNIVAL)
    {
        std::cout << "Already at Carnival scene. Reinitializing the carnival scene." << std::endl;
        Scene* carnivalScene = sceneStack.top();
        carnivalScene->Exit();  // Optionally clean up the current state
        carnivalScene->Init();  // Reinitialize to reset camera, cutsceneStage, etc.
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
    case SCENE_SPINNING_RING:
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