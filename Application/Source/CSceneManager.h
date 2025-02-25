#pragma once

#include "Scene.h"
#include "LoadingScreen.h"
#include <chrono>   // For time duration (std::chrono::milliseconds)
#include <thread>   // For std::this_thread functions
#include <stack>

enum SCENE_TYPE {
    SCENE_MAINMENU,
    SCENE_CARNIVAL,
    SCENE_WIUTEST,
    SCENE_BALLOONPOP,
    SCENE_ARCHERY,
    SCENE_HOLE,
    SCENE_WHACKAMOLE,
    SCENE_SPINNING_RING,
    SCENE_FINAL
};

enum TRANSITION_ACTION {
    PUSH,
    POP,
    REPLACE
};

struct SceneTransition {
    TRANSITION_ACTION action;
    Scene* newScene;
    SCENE_TYPE newSceneType;
};

class CSceneManager {
public:
    CSceneManager();
    ~CSceneManager();

    void Init(SCENE_TYPE startingScene);
    void Update(double dt);
    void Render();
    void Exit();

    void PushScene(SCENE_TYPE newScene);
    void PopScene();
    void ReplaceScene(SCENE_TYPE newScene);

    Scene* GetCurrentScene();
    SCENE_TYPE GetCurrentSceneType() const;
    bool IsTransitioning() const;

private:
    Scene* CreateScene(SCENE_TYPE sceneType);
    void StartLoadingScreen();
    void PreloadScene();
    void CompleteTransition();

    std::stack<Scene*> sceneStack;
    std::stack<SCENE_TYPE> typeStack;
    SCENE_TYPE currentSceneType;
    bool isTransitioning;

    // Loading screen
    LoadingScreen loadingScreen;
    bool isLoadingActive;

    SceneTransition currentTransition;
};