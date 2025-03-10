#pragma once
#include "Scene.h"
#include <stack>

enum SCENE_TYPE {
    SCENE_MAINMENU,
    SCENE_CARNIVAL,
    SCENE_WIUTEST,
    SCENE_BALLOONPOP,
    SCENE_ARCHERY,
    SCENE_HOLE,
    SCENE_WHACKAMOLE,
    SCENE_BUMPERBALLS,
    SCENE_SPINNINGRING,
    SCENE_FINAL
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

private:
    Scene* CreateScene(SCENE_TYPE sceneType);
    std::stack<Scene*> sceneStack;
    std::stack<SCENE_TYPE> typeStack;
    SCENE_TYPE currentSceneType;
};