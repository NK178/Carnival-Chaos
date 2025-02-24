#include "LoadingScreen.h"
#include <GL/glew.h>
#include <thread>

LoadingScreen::LoadingScreen()
    : progress(0.0f), isLoading(true), sceneToLoad(nullptr), currentLoadingStep(0) {
}

void LoadingScreen::Init() {
    progress = 0.0f;
    isLoading = true;
}

void LoadingScreen::Update(double dt) {
    if (!isLoading || !sceneToLoad) return;

    static bool loadingStarted = false;

    if (!loadingStarted) {
        // Start loading in a separate thread
        std::thread loadingThread([this]() {
            loadingFunction();
            });
        loadingThread.detach();
        loadingStarted = true;
    }

    // Update progress based on loading steps
    if (currentLoadingStep < loadingMessages.size()) {
        progress += dt * 0.5f; // Adjust speed as needed
        if (progress >= 1.0f) {
            currentLoadingStep++;
            progress = 0.0f;
        }
    }

    // Check if loading is complete
    //SceneCarnival* carnivalScene = dynamic_cast<SceneCarnival*>(sceneToLoad);
    //if (carnivalScene && carnivalScene->AreModelsLoaded()) {
    //    isLoading = false;
    //    loadingStarted = false;
    //}
}

void LoadingScreen::Render() {
    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render loading bar
    glBegin(GL_QUADS);
    // Background bar (gray)
    glColor3f(0.3f, 0.3f, 0.3f);
    glVertex2f(-0.8f, -0.1f);
    glVertex2f(0.8f, -0.1f);
    glVertex2f(0.8f, 0.1f);
    glVertex2f(-0.8f, 0.1f);

    // Progress bar (green)
    glColor3f(0.0f, 1.0f, 0.0f);
    float totalProgress = (currentLoadingStep + progress) / loadingMessages.size();
    glVertex2f(-0.8f, -0.1f);
    glVertex2f(-0.8f + (1.6f * totalProgress), -0.1f);
    glVertex2f(-0.8f + (1.6f * totalProgress), 0.1f);
    glVertex2f(-0.8f, 0.1f);
    glEnd();

    // Render current loading message
    if (currentLoadingStep < loadingMessages.size()) {
        // Render loading messages[currentLoadingStep] using your text rendering system
        // Example:
        // RenderText(loadingMessages[currentLoadingStep], -0.5f, -0.3f, 1.0f, glm::vec3(1.0f));
    }
}

void LoadingScreen::Exit() {
    if (sceneToLoad) {
        sceneToLoad = nullptr;
    }
}

void LoadingScreen::SetSceneToLoad(Scene* scene, std::function<void()> loadFunc) {
    sceneToLoad = scene;
    loadingFunction = loadFunc;
}

void LoadingScreen::AddLoadingMessage(const std::string& message) {
    loadingMessages.push_back(message);
}