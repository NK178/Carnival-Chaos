#include "CSceneManager.h"
#include "SceneBalloonPop.h"
#include "SceneWIUtest.h"
#include "SceneArchery.h"
#include "SceneMain.h"
#include "SceneSpinningRing.h"
#include "SceneHole.h"
#include "MainMenu.h"
#include "MatrixStack.h" 

#include <GL/glew.h>
#include <GLFW/glfw3.h>
// Include other scene headers

void CSceneManager::Init() {
    // Initialize with first scene
    activeScene = new MainMenu;
    currentSceneType = SCENE_MAIN_MENU;
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

//void CSceneManager::RenderLoadingScreen() {
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    RenderTextOnScreen(meshList[GEO_TEXT], "Loading...", glm::vec3(1, 1, 1), 30, 400, 300);
//    glfwSwapBuffers(Application::m_window);
//}
//
//void CSceneManager::RenderTextOnScreen(Mesh* mesh, std::string
//    text, glm::vec3 color, float size, float x, float y)
//{
//    if (!mesh || mesh->textureID <= 0) //Proper error check
//        return;
//    // Enable blending
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glDisable(GL_DEPTH_TEST);
//    glm::mat4 ortho = glm::ortho(0.f, 800.f, 0.f, 600.f, -100.f, 100.f); // dimension of screen UI
//    projectionStack.PushMatrix();
//    projectionStack.LoadMatrix(ortho);
//    viewStack.PushMatrix();
//    viewStack.LoadIdentity(); //No need camera for ortho mode
//    modelStack.PushMatrix();
//    modelStack.LoadIdentity(); //Reset modelStack
//    modelStack.Translate(x, y, 0);
//    modelStack.Scale(size, size, size);
//    glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
//    glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
//    glUniform1i(m_parameters[U_LIGHTENABLED], 0);
//    glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, mesh->textureID);
//    glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
//    for (unsigned i = 0; i < text.length(); ++i)
//    {
//        glm::mat4 characterSpacing = glm::translate(
//            glm::mat4(1.f),
//            glm::vec3(0.5f + i * 1.0f, 0.5f, 0)
//        );
//        glm::mat4 MVP = projectionStack.Top() *
//            viewStack.Top() * modelStack.Top() * characterSpacing;
//        glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
//            glm::value_ptr(MVP));
//        mesh->Render((unsigned)text[i] * 6, 6);
//    }
//    glBindTexture(GL_TEXTURE_2D, 0);
//    glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
//    projectionStack.PopMatrix();
//    viewStack.PopMatrix();
//    modelStack.PopMatrix();
//    glEnable(GL_DEPTH_TEST);
//    glDisable(GL_BLEND);
//}