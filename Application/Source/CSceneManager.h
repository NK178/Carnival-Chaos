#pragma once
#include <vector>
#include "Scene.h"

class CSceneManager {
public:
    enum SCENE_TYPE {
        SCENE_WIUTEST,
        SCENE_BALLOONPOP,
        SCENE_ARCHERY,
        SCENE_MAIN_MENU,
        SCENE_CARNIVAL,
        SCENE_SPINNING_RING,
        SCENE_TOTAL
    };

    static CSceneManager& GetInstance() {
        static CSceneManager instance;
        return instance;
    }

    void Init();
    void Update(double dt);
    void Render();
    void Exit();

    // Scene management
    void ChangeScene(SCENE_TYPE newScene);
   // void RenderLoadingScreen();
    Scene* GetCurrentScene() { return activeScene; }

private:
    CSceneManager() : activeScene(nullptr), nextScene(nullptr) {}
    ~CSceneManager() { Exit(); }

    // Prevent copying
    CSceneManager(const CSceneManager&) = delete;
    CSceneManager& operator=(const CSceneManager&) = delete;

    Scene* activeScene;
    Scene* nextScene;
    SCENE_TYPE currentSceneType;
};