#ifndef SCENE_ARCHERY_H
#define SCENE_ARCHERY_H

#include "Scene.h"
#include "Mesh.h"
#include "Application.h"
#include "FPCamera.h"
#include "MatrixStack.h"
#include "Light.h"
#include "CSceneManager.h"
#include "GameObject.h"
#include "CollisionDetection.h"
#include <iostream>

class SceneArchery : public Scene
{
public:
    enum GEOMETRY_TYPE
    {
        GEO_AXES,
        // shapes
        GEO_SPHERE,
        GEO_QUAD,
        GEO_PLANE,
        GEO_TARGET,
        GEO_ARROW,
        GEO_CROSSHAIR,
        GEO_CARPET,
        GEO_BARREL,
        // Text
        GEO_TEXT,
        // Skybox
        GEO_LEFT,
        GEO_RIGHT,
        GEO_TOP,
        GEO_BOTTOM,
        GEO_FRONT,
        GEO_BACK,
        NUM_GEOMETRY,
    };

    enum UNIFORM_TYPE
    {
        U_MVP = 0,
        U_MODELVIEW,
        U_MODELVIEW_INVERSE_TRANSPOSE,
        U_MATERIAL_AMBIENT,
        U_MATERIAL_DIFFUSE,
        U_MATERIAL_SPECULAR,
        U_MATERIAL_SHININESS,
        // Light 0 (Spotlight)
        U_LIGHT0_TYPE,
        U_LIGHT0_POSITION,
        U_LIGHT0_COLOR,
        U_LIGHT0_POWER,
        U_LIGHT0_KC,
        U_LIGHT0_KL,
        U_LIGHT0_KQ,
        U_LIGHT0_SPOTDIRECTION,
        U_LIGHT0_COSCUTOFF,
        U_LIGHT0_COSINNER,
        U_LIGHT0_EXPONENT,
        // Light 1 (Spotlight)
        U_LIGHT1_TYPE,
        U_LIGHT1_POSITION,
        U_LIGHT1_COLOR,
        U_LIGHT1_POWER,
        U_LIGHT1_KC,
        U_LIGHT1_KL,
        U_LIGHT1_KQ,
        U_LIGHT1_SPOTDIRECTION,
        U_LIGHT1_COSCUTOFF,
        U_LIGHT1_COSINNER,
        U_LIGHT1_EXPONENT,
        // Light 2 (Spotlight)
        U_LIGHT2_TYPE,
        U_LIGHT2_POSITION,
        U_LIGHT2_COLOR,
        U_LIGHT2_POWER,
        U_LIGHT2_KC,
        U_LIGHT2_KL,
        U_LIGHT2_KQ,
        U_LIGHT2_SPOTDIRECTION,
        U_LIGHT2_COSCUTOFF,
        U_LIGHT2_COSINNER,
        U_LIGHT2_EXPONENT,
        U_NUMLIGHTS,
        // Texture and lighting
        U_COLOR_TEXTURE_ENABLED,
        U_COLOR_TEXTURE,
        U_LIGHTENABLED,
        U_TEXT_ENABLED,
        U_TEXT_COLOR,
        U_TOTAL,
    };

    SceneArchery();
    ~SceneArchery();

    virtual void Init();
    virtual void Update(double dt);
    virtual void Render();
    virtual void Exit();

private:
 
    struct Target : public GameObject {
        float radius;
        bool isHit;

        Target(int id, const glm::vec3& position, float rad) : GameObject(id, GameObject::CUBE) {
            pos = position;
            radius = rad;
            isHit = false;
        }
    };

    struct Arrow : public GameObject {
        float lifetime{ 5.0f };
        float currentTime{ 0.0f };
        bool isActive{ false };
        bool isStuck{ false };
        float radius{ 0.5f };  // Collision radius for arrow
        glm::vec3 stuckPosition{ 0.0f, 0.0f, 0.0f };
        glm::vec3 targetNormal{ 0.0f, 0.0f, 0.0f };

        Arrow(int id) : GameObject(id, GameObject::SPHERE) {  // Changed to SPHERE for collision
            mass = 0.4f;
            bounciness = 0.2f;
            accel = glm::vec3(0, -9.8f, 0);
        }

        void Fire(const glm::vec3& startPos, const glm::vec3& direction, float speed) {
            pos = startPos;
            vel = glm::vec3(0);
            m_totalForces = glm::vec3(0);

            glm::vec3 randomSpread{
                (rand() % 100 - 50) * 0.001f,
                (rand() % 100 - 50) * 0.001f,
                0.0f
            };

            glm::vec3 impulse = (direction + randomSpread) * speed;
            AddImpulse(impulse);
            AddForce(glm::vec3(0, 5.0f, 0));

            isActive = true;
            isStuck = false;
            lifetime = 5.0f + (rand() % 100 - 50) * 0.05f;
            currentTime = 0.0f;
        }

        void StickToTarget(const glm::vec3& hitPos, const glm::vec3& normal) {
            stuckPosition = hitPos;
            targetNormal = normal;
            isStuck = true;
            vel = glm::vec3(0);
            m_totalForces = glm::vec3(0);
        }

        void Update(float dt) {
            if (!isActive || isStuck) return;
            currentTime += dt;
            UpdatePhysics(dt);
            lifetime -= dt;
            if (lifetime <= 0) isActive = false;
            vel *= 0.99f;
        }
    };


    float m_arrowPower;         // Current power level of the arrow
    float m_maxArrowPower;      // Maximum power level
    float m_powerChargeRate;    // How fast power increases
    bool m_isChargingShot;      // Whether we're currently charging a shot

    // Constants
    static const int NUM_LIGHTS = 3;

    int m_arrowsLeft;     // Track remaining arrows
    int m_playerScore;    // Track player's score

    // Utility methods
    void HandleKeyPress();
    void RenderMesh(Mesh* mesh, bool enableLight);
    bool OverlapAABB2AABB(glm::vec3 Obj1, const int Width1, const int Height1,
        glm::vec3 Obj2, const int Width2, const int Height2);
   
    void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey);
    void RenderText(Mesh* mesh, std::string text, glm::vec3 color);
    void RenderTextOnScreen(Mesh* mesh, std::string text, glm::vec3 color, float size, float x, float y);
    void Material(GEOMETRY_TYPE obj, float AmR, float AmG, float AmB,
        float DifA, float DifG, float DifB,
        float SpA, float SpG, float SpB, float Shiny);
    void RenderSkyBox();

    // Arrow-related methods
    void HandleArrowInput();
    void FireArrow();
    void CheckArrowCollisions();

    // Member variables
    unsigned m_vertexArrayID;
    Mesh* meshList[NUM_GEOMETRY];
    unsigned m_programID;
    unsigned m_parameters[U_TOTAL];

    FPCamera camera;
    int projType = 1; // 0 for orthographic, 1 for projection

    MatrixStack modelStack, viewStack, projectionStack;

    Light light[NUM_LIGHTS];
    bool enableLight;

    Application app;

    // Arrow array
    std::vector<Arrow> arrows;
    static const int MAX_ARROWS = 10;

    std::vector<Target> targets;

    static constexpr float TARGET_RADIUS = 15.0f;
    static constexpr float TARGET_HEIGHT = 30.0f;  // Increased to match visual size
    static constexpr float TARGET_WIDTH = 15.0f;   // Increased to match visual size
    static constexpr float TARGET_DEPTH = 2.0f;    // Increased for better collision
    static constexpr float ARROW_RADIUS = 0.8f;    // Increased for better hit detection
};

#endif // SCENE_ARCHERY_H