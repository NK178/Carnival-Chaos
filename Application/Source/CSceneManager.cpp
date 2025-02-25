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
#include "LoadingScreen.h"
#include <iostream>

CSceneManager::CSceneManager()
    : currentSceneType(SCENE_MAINMENU), isTransitioning(false),
    isLoadingActive(false), loadingScreen()
{
}

CSceneManager::~CSceneManager()
{
    Exit();
}

void CSceneManager::Init(SCENE_TYPE startingScene)
{
    // Initialize loading screen
    loadingScreen.Init();

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
    // If loading screen is active, update it
    if (isLoadingActive) {
        loadingScreen.Update(dt);

        // Check if loading is complete - we'll manually set this flag
        // when the loading is done in our loading thread
        if (!loadingScreen.isLoading) {
            isLoadingActive = false;
            CompleteTransition();
        }
        return; // Skip regular update while loading
    }

    // Handle scene transitions if needed
    if (isTransitioning) {
        // Start loading screen
        StartLoadingScreen();
        isTransitioning = false; // We're now in loading mode instead
        return;
    }

    // Update the current scene
    if (!sceneStack.empty()) {
        sceneStack.top()->Update(dt);
    }
}

void CSceneManager::Render()
{
    // Render the loading screen if active
    if (isLoadingActive) {
        loadingScreen.Render();
        return;
    }

    // Otherwise render the current scene
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

void CSceneManager::StartLoadingScreen()
{
    // Reset and initialize loading screen
    loadingScreen.Init();
    isLoadingActive = true;

    // Clear any previous loading messages
    loadingScreen.loadingMessages.clear();

    // Add loading messages
    loadingScreen.AddLoadingMessage("Preparing scene...");
    loadingScreen.AddLoadingMessage("Loading assets...");
    loadingScreen.AddLoadingMessage("Setting up environment...");
    loadingScreen.AddLoadingMessage("Almost ready...");

    // Set up the loading function based on transition type
    switch (currentTransition.action) {
    case PUSH:
        loadingScreen.SetSceneToLoad(currentTransition.newScene, [this]() {
            // This will run in a separate thread
            PreloadScene();
            // When done, mark loading as complete
            loadingScreen.isLoading = false;
            });
        break;
    case POP:
    case REPLACE:
        loadingScreen.SetSceneToLoad(nullptr, [this]() {
            // Simulate loading time
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            // When done, mark loading as complete
            loadingScreen.isLoading = false;
            });
        break;
    }
}

void CSceneManager::PushScene(SCENE_TYPE newScene)
{
    // Prepare the transition
    currentTransition.action = PUSH;
    currentTransition.newScene = CreateScene(newScene);
    currentTransition.newSceneType = newScene;
    isTransitioning = true;

    std::cout << "Pushing scene: " << newScene << std::endl;
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

    // Otherwise, if for some reason we're not yet at Carnival,
    // prepare a POP transition to reach it.
    currentTransition.action = POP;
    currentTransition.newScene = nullptr;
    currentTransition.newSceneType = SCENE_CARNIVAL;
    isTransitioning = true;

    std::cout << "Popping scene, returning to Carnival scene" << std::endl;
}

void CSceneManager::ReplaceScene(SCENE_TYPE newScene)
{
    // Prepare the transition
    currentTransition.action = REPLACE;
    currentTransition.newScene = CreateScene(newScene);
    currentTransition.newSceneType = newScene;
    isTransitioning = true;

    std::cout << "Replacing scene with: " << newScene << std::endl;
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

bool CSceneManager::IsTransitioning() const
{
    return isTransitioning || isLoadingActive;
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

void CSceneManager::PreloadScene()
{
    // This function is called in a separate thread
    // Scene-specific preloading can go here

    // Simulate loading time
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    // You could add specific preloading based on scene type
    // For example:
    if (currentTransition.newSceneType == SCENE_BALLOONPOP) {
        // Preload balloon pop assets
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    else if (currentTransition.newSceneType == SCENE_CARNIVAL) {
        // Preload carnival assets
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void CSceneManager::CompleteTransition()
{
    // Actually perform the scene transition
    switch (currentTransition.action) {
    case PUSH: {
        // Initialize the new scene
        if (currentTransition.newScene) {
            currentTransition.newScene->Init();

            // Push it onto the stacks
            sceneStack.push(currentTransition.newScene);
            typeStack.push(currentTransition.newSceneType);

            // Update current scene type
            currentSceneType = currentTransition.newSceneType;
        }
        break;
    }
    case POP: {
        // Exit and remove the current scene
        if (!sceneStack.empty()) {
            Scene* currentScene = sceneStack.top();
            currentScene->Exit();
            delete currentScene;  // Manual memory management

            sceneStack.pop();
            typeStack.pop();

            // If there's a scene left, note its type
            if (!typeStack.empty()) {
                currentSceneType = typeStack.top();
            }
        }
        break;
    }
    case REPLACE: {
        // Exit and remove the current scene
        if (!sceneStack.empty()) {
            Scene* currentScene = sceneStack.top();
            currentScene->Exit();
            delete currentScene;  // Manual memory management

            sceneStack.pop();
            typeStack.pop();
        }

        // Initialize the new scene
        if (currentTransition.newScene) {
            currentTransition.newScene->Init();

            // Push it onto the stacks
            sceneStack.push(currentTransition.newScene);
            typeStack.push(currentTransition.newSceneType);

            // Update current scene type
            currentSceneType = currentTransition.newSceneType;
        }
        break;
    }
    }

    // Reset transition data
    currentTransition.newScene = nullptr;
}