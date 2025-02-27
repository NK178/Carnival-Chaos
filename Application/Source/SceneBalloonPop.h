
#ifndef SCENE_BalloonPop_H
#define SCENE_BalloonPop_H

#include "Scene.h"
#include "Mesh.h"
#include "Application.h"
#include "FPCamera.h"
#include "MatrixStack.h"
#include "Light.h"
#include "CollisionDetection.h"
#include "CSceneManager.h"
#include "CAudioManager.h"

class SceneBalloonPop : public Scene
{
public:
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		//shapes
		GEO_SPHERE,
		GEO_QUAD,
		GEO_PLANE,
		GEO_CRATE,
		GEO_BALLOONS,
		GEO_PRESENT,
		GEO_BALLOON,
		GEO_CROSSHAIR,
		GEO_DART,
		GEO_PELLETGUN,
		GEO_GAMEOVER,

		//Text
		GEO_TEXT,
		GEO_TEXT2,
		GEO_KEY_E,
		GEO_KEY_R,
		GEO_FPS,
		GEO_UI,

		//SKybox
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
		/*FLASHLIGHT*/
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
		/*FLASHLIGHT*/
		/*MOON*/
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
		/*MOON*/
		/*LANTERN*/
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
		/*LANTERN*/
		U_NUMLIGHTS,
		// Step 5
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,

		U_LIGHTENABLED,

		U_TEXT_ENABLED,
		U_TEXT_COLOR,

		U_TOTAL,
	};

	SceneBalloonPop();
	~SceneBalloonPop();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

       bool m_hasWon;

	   
private:
	void HandleKeyPress();
	void RenderMesh(Mesh* mesh, bool enableLight);

	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	//AltAzCamera camera;
	FPCamera camera;
	int projType = 1; // fix to 0 for orthographic, 1 for projection

	MatrixStack modelStack, viewStack, projectionStack;

	static const int NUM_LIGHTS = 3;
	Light light[NUM_LIGHTS];
	bool enableLight;

	Application app;

	bool OverlapAABB2AABB(glm::vec3 Obj1, const int Width1, const int Height1, glm::vec3 Obj2, const int Width2, const int Height2);

	void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey);
	void RenderText(Mesh* mesh, std::string text, glm::vec3 color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, glm::vec3 color, float size, float x, float y);
	void Material(GEOMETRY_TYPE obj, float AmR, float AmG, float AmB, float DifA, float DifG, float DifB, float SpA, float SpG, float SpB, float Shiny);
	void RenderSkyBox();

	float fps = 0;

	bool m_isGameOver;
	

	bool m_isObjectiveRead;
	float countdownTime;

	struct Balloon {
		PhysicsObject physics;
		bool isPopped;
		float timeAlive;

		Balloon() : isPopped(false), timeAlive(0.0f) {
			physics.mass = 0.1f;  // Light mass for balloon
			physics.bounciness = 0.5f;
		}
	};

	std::vector<Balloon> balloons;
	float gameTimer;
	int playerScore;
	bool gameOver;
	const float GAME_TIME_LIMIT = 30.0f;
	const int WIN_SCORE = 10;
	const float SPAWN_INTERVAL = 2.0f;
	float spawnTimer;

	// Add these constants
	const float BALLOON_UP_FORCE = 5.0f;        // Increased upward force
	const float BALLOON_RIGHT_FORCE = 2.0f;     // Increased rightward force
	const float BALLOON_WOBBLE_FORCE = 1.0f;    // Force for side-to-side movement
	const float CEILING_HEIGHT = 50.0f;
	const float FLOOR_HEIGHT = 5.0f;


	const float DART_RADIUS = 3.0f;
	const float BALLOON_RADIUS = 5.0f;
	const int MAX_DARTS = 1000;

	float m_shootCooldown;        // Current cooldown timer
	const float SHOOT_COOLDOWN_DURATION = 1.0f;  // One second cooldown

	void SpawnBalloon() {
		Balloon newBalloon;
		// Random position at bottom of stage
		newBalloon.physics.pos = glm::vec3(
			-100.0f + static_cast<float>(rand() % 200), // Random x between -100 and 100
			FLOOR_HEIGHT,  // Start at floor height
			-40.0f + static_cast<float>(rand() % 80)    // Random z between -40 and 40
		);
		balloons.push_back(newBalloon);
	}



	// DISCLAIMER: THE "DARTS" WERE CHANGED TO PELLETS FROM A PELLET GUN!! DIDNT HAVE TIME TO CHANGE THE FUNCTION NAMES SO KEEP THIS IN MIND!! - Mathea
	class Dart {
	public:
		bool isActive;
		PhysicsObject physics;
		float mass;

		Dart() : isActive(false), mass(1.0f) {
			physics.mass = mass;
		}

		void Fire(const glm::vec3& pos, const glm::vec3& dir, float speed) {
			physics.pos = pos;
			physics.vel = dir * speed;
			isActive = true;
		}

		void Update(float dt) {
			if (isActive) {
				// Add gravity
				physics.AddForce(glm::vec3(0, -9.81f * mass, 0));
				physics.UpdatePhysics(dt);
			}
		}
	};


	std::vector<Dart> darts;
	float m_dartPower;
	float m_maxDartPower;
	float m_powerChargeRate;
	bool m_isChargingShot;
	int m_dartsLeft;

	void HandleDartInput();
	void FireDart();
	void CheckDartCollisions();



};

#endif
