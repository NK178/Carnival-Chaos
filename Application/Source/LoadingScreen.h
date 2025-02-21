#pragma once
#include "Scene.h"
#include <functional>
#include <string>
#include <vector>

class LoadingScreen : public Scene {
private:
    float progress;
    bool isLoading;
    Scene* sceneToLoad;
    std::vector<std::string> loadingMessages;
    size_t currentLoadingStep;
    std::function<void()> loadingFunction;

public:
    LoadingScreen();
    virtual void Init() override;
    virtual void Update(double dt) override;
    virtual void Render() override;
    virtual void Exit() override;

    void SetSceneToLoad(Scene* scene, std::function<void()> loadFunc);
    void AddLoadingMessage(const std::string& message);
    bool IsLoadingComplete() const { return !isLoading; }
};