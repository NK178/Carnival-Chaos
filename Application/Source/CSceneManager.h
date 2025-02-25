#ifndef CSCENEMANAGER_H
#define CSCENEMANAGER_H

#include <stack>
#include <vector>
#include "Scene.h"

// Scene type enum
enum SCENE_TYPE {
    SCENE_MAINMENU,
    SCENE_CARNIVAL,
    SCENE_WIUTEST,
    SCENE_BALLOONPOP,
    SCENE_ARCHERY,
    SCENE_HOLE,
    SCENE_WHACKAMOLE,
    SCENE_SPINNING_RING,
    SCENE_FINAL,
    SCENE_COUNT
};

class CSceneManager {
private:
    // Stack of raw scene pointers (we'll manage memory manually)
    std::stack<Scene*> sceneStack;

    // Stack to track scene types (parallel to sceneStack)
    std::stack<SCENE_TYPE> typeStack;

    // Current scene type
    SCENE_TYPE currentSceneType;

    // Flag for scene transitions
    bool isTransitioning;

    // Action to perform during transition
    enum TransitionAction {
        PUSH,
        POP,
        REPLACE
    };

    // Transition data
    struct Transition {
        TransitionAction action;
        Scene* newScene;
        SCENE_TYPE newSceneType;
    };

    Transition currentTransition;

    // Private method to create scene instances
    Scene* CreateScene(SCENE_TYPE sceneType);

public:
    CSceneManager();
    ~CSceneManager();

    // Initialize the scene manager with a starting scene
    void Init(SCENE_TYPE startingScene = SCENE_MAINMENU);

    // Main update loop
    void Update(double dt);

    // Render the current scene
    void Render();

    // Clean up all scenes
    void Exit();

    // Stack operations
    void PushScene(SCENE_TYPE newScene);  // Add scene to top of stack
    void PopScene();                      // Remove top scene and go back to previous
    void ReplaceScene(SCENE_TYPE newScene); // Replace current scene

    // Scene access
    Scene* GetCurrentScene();
    SCENE_TYPE GetCurrentSceneType() const;

    // Returns true if transitioning between scenes
    bool IsTransitioning() const;
};

#endif // CSCENEMANAGER_H
