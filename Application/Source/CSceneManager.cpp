#include "CSceneManager.h"
#include "SceneBalloonPop.h"
#include "SceneWIUtest.h"
#include "SceneArchery.h"
#include "SceneMain.h"
#include "SceneSpinningRing.h"
#include "SceneHole.h"
#include "MainMenu.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
// Include other scene headers

void CSceneManager::Init() {
    // Initialize with first scene
    activeScene = new SceneBalloonPop;
    currentSceneType = SCENE_MAIN_MENU;
    loadingScreen = new LoadingScreen;
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

//void CSceneManager::Update(double dt) {
//    if (isLoading) {
//        loadingScreen->Update(dt);
//        if (loadingScreen->IsLoadingComplete()) {
//            // Transition from loading screen to next scene
//            if (activeScene != loadingScreen) {
//                delete activeScene;
//            }
//            activeScene = nextScene;
//            nextScene = nullptr;
//            isLoading = false;
//        }
//    }
//    else if (nextScene != nullptr) {
//        // Start loading sequence
//        isLoading = true;
//        loadingScreen->SetSceneToLoad(nextScene);
//        loadingScreen->Init();
//        if (activeScene != loadingScreen) {
//            activeScene->Exit();
//            Scene* oldScene = activeScene;
//            activeScene = loadingScreen;
//            delete oldScene;
//        }
//    }
//    else if (activeScene != nullptr) {
//        activeScene->Update(dt);
//    }
//}

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

    if (loadingScreen != nullptr) {
        delete loadingScreen;
        loadingScreen = nullptr;
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
    case SCENE_CARNIVAL:
        scene = new SceneMain();
        break;
    case SCENE_SPINNING_RING:
        scene = new SceneSpinningRing();
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