#include "LoadingScreen.h"
#include <GL/glew.h>
#include <thread>
#include <chrono>  // Added for sleep_for

LoadingScreen::LoadingScreen()
    : progress(0.0f),
    isLoading(true),
    sceneToLoad(nullptr),
    loadingStarted(false),
    currentLoadingStep(0) {
}

void LoadingScreen::Init() {
    progress = 0.0f;
    isLoading = true;
    loadingStarted = false;
    currentLoadingStep = 0;
    // Don't clear loadingMessages here as they might be set up before Init() is called
}

void LoadingScreen::Update(double dt) {
    if (!isLoading) return;

    // Check if we have a loading function
    if (!loadingFunction) return;

    // Start loading thread if not already started
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
    // Note: The actual loading completion is now handled by
    // the callback function setting isLoading = false
}

void LoadingScreen::Render() {
    // Clear screen with dark blue background
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);  // Dark blue background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw a styled loading bar background
    glBegin(GL_QUADS);

    // Background panel (darker gray)
    glColor3f(0.1f, 0.1f, 0.15f);
    glVertex2f(-0.9f, -0.3f);
    glVertex2f(0.9f, -0.3f);
    glVertex2f(0.9f, 0.3f);
    glVertex2f(-0.9f, 0.3f);

    // Loading bar background (medium gray)
    glColor3f(0.3f, 0.3f, 0.3f);
    glVertex2f(-0.8f, -0.1f);
    glVertex2f(0.8f, -0.1f);
    glVertex2f(0.8f, 0.1f);
    glVertex2f(-0.8f, 0.1f);

    // Progress bar (blue or green gradient)
    float totalProgress = 0.0f;
    if (loadingMessages.size() > 0) {
        totalProgress = (currentLoadingStep + progress) / loadingMessages.size();
    }

    // Use a blue to green gradient based on progress
    float r = 0.0f;
    float g = 0.5f + (totalProgress * 0.5f); // Increases from 0.5 to 1.0
    float b = 1.0f - (totalProgress * 0.8f); // Decreases from 1.0 to 0.2

    glColor3f(r, g, b);
    glVertex2f(-0.8f, -0.1f);
    glVertex2f(-0.8f + (1.6f * totalProgress), -0.1f);
    glVertex2f(-0.8f + (1.6f * totalProgress), 0.1f);
    glVertex2f(-0.8f, 0.1f);
    glEnd();

    // If you have a simple text rendering function, use it here:
    // DrawSimpleText("LOADING...", -0.3f, 0.2f);

    // If you have a current message to display:
    if (currentLoadingStep < loadingMessages.size()) {
        // DrawSimpleText(loadingMessages[currentLoadingStep], -0.4f, -0.2f);
    }

    // Draw percentage text
    char percentText[10];
    sprintf_s(percentText, "%d%%", (int)(totalProgress * 100.0f));
    // DrawSimpleText(percentText, 0.0f, 0.0f);

    if (!loadingMessages.empty()) {
        glBegin(GL_QUADS);
        for (size_t i = 0; i < loadingMessages.size(); i++) {
            float dotSize = 0.02f;
            float x = -0.1f + (i * 0.05f) - (loadingMessages.size() * 0.025f);

            if (i < currentLoadingStep) {
                glColor3f(0.0f, 1.0f, 0.0f); // Green for completed steps
            }
            else if (i == currentLoadingStep) {
                glColor3f(1.0f, 1.0f, 0.0f); // Yellow for current step
            }
            else {
                glColor3f(0.5f, 0.5f, 0.5f); // Gray for future steps
            }

            glVertex2f(x - dotSize, -0.25f - dotSize);
            glVertex2f(x + dotSize, -0.25f - dotSize);
            glVertex2f(x + dotSize, -0.25f + dotSize);
            glVertex2f(x - dotSize, -0.25f + dotSize);
        }
        glEnd();
    }
}

void LoadingScreen::Exit() {
    sceneToLoad = nullptr;
    loadingFunction = nullptr;
    isLoading = false;
    loadingStarted = false;
}

void LoadingScreen::SetSceneToLoad(Scene* scene, std::function<void()> loadFunc) {
    sceneToLoad = scene;
    loadingFunction = loadFunc;
    loadingStarted = false;  // Reset the loading flag when setting a new scene
}

void LoadingScreen::AddLoadingMessage(const std::string& message) {
    loadingMessages.push_back(message);
}