#include "CSceneManager.h"
#include "SceneBalloonPop.h"
#include "SceneWIUtest.h"
#include "SceneArchery.h"
#include "SceneMain.h"
#include "SceneSpinningRing.h"
#include "MainMenu.h"
// Include other scene headers

void CSceneManager::Init() {
    // Initialize with first scene
    activeScene = new SceneSpinningRing();
    currentSceneType = SCENE_SPINNING_RING;
    activeScene->Init();
}

void CSceneManager::Update(double dt) {
    // Handle scene transition if needed
    if (nextScene != nullptr) {
        if (activeScene != nullptr) {
            activeScene->Exit();
            delete activeScene;
        }
        activeScene = nextScene;
        nextScene = nullptr;
        activeScene->Init();
    }

    // Update active scene
    if (activeScene != nullptr) {
        activeScene->Update(dt);
    }
}

void CSceneManager::Render() {
    if (activeScene != nullptr) {
        activeScene->Render();
    }
}

void CSceneManager::Exit() {
    if (activeScene != nullptr) {
        activeScene->Exit();
        delete activeScene;
        activeScene = nullptr;
    }

    if (nextScene != nullptr) {
        delete nextScene;
        nextScene = nullptr;
    }
}

void CSceneManager::ChangeScene(SCENE_TYPE newScene) {
    // Create the new scene
    Scene* scene = nullptr;

    switch (newScene) {
    case SCENE_WIUTEST:
        scene = new SceneWIUtest();
        break;
    case SCENE_BALLOONPOP:
        scene = new SceneBalloonPop();
        std::cout << "In Balloon Pop!" << std::endl;
        break;
    case SCENE_ARCHERY:
        scene = new SceneArchery();
        std::cout << "In Archery!" << std::endl;
        break;
    case SCENE_SPINNING_RING:
        scene = new SceneSpinningRing();
        break;
    case SCENE_CARNIVAL:
        scene = new SceneMain();
        break;
    default:
        return;
    }

    // Queue the scene change
    if (scene != nullptr) {
        if (nextScene != nullptr) {
            delete nextScene;
        }
        nextScene = scene;
        currentSceneType = newScene;
    }
}