#pragma once
#include "Scene.h"
#include <functional>
#include <string>
#include <vector>

class LoadingScreen : public Scene {
private:
    float progress;
    Scene* sceneToLoad;
    bool loadingStarted;
    size_t currentLoadingStep;
    std::function<void()> loadingFunction;
public:
    bool isLoading;
    std::vector<std::string> loadingMessages;
    LoadingScreen();
    virtual void Init() override;
    virtual void Update(double dt) override;
    virtual void Render() override;
    virtual void Exit() override;
    void SetSceneToLoad(Scene* scene, std::function<void()> loadFunc);
    void AddLoadingMessage(const std::string& message);
    bool IsLoadingComplete() const { return !isLoading; }
};