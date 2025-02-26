
#ifndef SCENE_FINAL_H
#define SCENE_FINAL_H

#include "Scene.h"
#include "Mesh.h"
#include "Application.h"
#include "FPCamera.h"
#include "MatrixStack.h"
#include "Light.h"
#include "CollisionDetection.h"
#include "CSceneManager.h"

class SceneFinal : public Scene
{
public:
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		//shapes
		GEO_SPHERE,
		GEO_QUAD,
		GEO_PLANE,
		GEO_FENCE,
		GEO_BUMPERCAR,
		GEO_CLOWN,
	
		//Text
		GEO_TEXT,
		GEO_TEXT2,
		GEO_KEY_E,
		GEO_KEY_R,
		GEO_KEY_Q,
		GEO_FPS,
		GEO_UI,
		GEO_CROSSHAIR,

		//Skybox
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,

		//Entities
		GEO_CAR,
		GEO_PELLETGUN,
		GEO_BALLOON,
		GEO_HEALTHBAR,


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

	SceneFinal();
	~SceneFinal();

	virtual void Init();
	bool CheckRayBalloonCollision(glm::vec3 rayOrigin, glm::vec3 rayDirection);
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

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

	void RenderDialogue();
	void UpdateDialogue(double dt);
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey);
	void RenderText(Mesh* mesh, std::string text, glm::vec3 color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, glm::vec3 color, float size, float x, float y);
	void Material(GEOMETRY_TYPE obj, float AmR, float AmG, float AmB, float DifA, float DifG, float DifB, float SpA, float SpG, float SpB, float Shiny);
	void RenderSkyBox();

	int fps = 0;
	
	PhysicsObject carPhysics;
	PhysicsObject m_cpu;
	char AIsteer = 'N';
	char AImove = 'F';
	const float CAR_FORCE = 300.0f; // Adjust based on desired acceleration
	const float CAR_TURN_RATE = 180.0f; // Degrees per second
	const float CAR_DRAG = 0.95f; // Air resistance factor

	// Boss battle variables
	float m_battleTimer;          // Timer for the 80-second battle
	int m_bossHealth;             // Boss health (starts at 100)
	bool m_battleStarted;         // Flag to track if the battle has started
	bool m_battleEnded;           // Flag to track if the battle has ended
	bool m_playerWon;             // Flag to track if player won
	bool m_playerLost;

	bool isObjectiveRead;
	float countdownTime;

	// structure for dialogue lines
	struct DialogueLine {
		std::vector<std::string> textLines;
		bool isMultiLine;
	};

	// dialogue displays
	int currentLineIndex = -1;
	float dialogueTimer = 0;
	bool isTyping;
	float typewriterTimer;
	std::string currentText;
	int currentCharIndex;
	
	bool isEnterSceneDialogueActive;
	bool hasESDialogueCompleted;

	std::vector<DialogueLine> enterSceneDialogueLines = {
		{{"What is this place?"}, false},
		{{"Hello, player."}, false},
		{{"A bear?! Who are you?"}, false},
		{{"I am the Ringmaster,",
		  "the overseer of this carnival."}, true},
		{{"I've been watching you closely",
		  "as you played the carnival games."}, true},
		{{"You truly desire the million-dollar",
		  "reward, don't you?"}, true},
		{{"I won't go easy on you!"}, false},
	};

	bool isBossDefeatedDialogueActive;
	bool isWinScreenActive;

	std::vector<DialogueLine> bossDefeatDialogueLines = {
		{{"AHHH!!!"}, false},
		{{"Whoa! That bear just got blasted",
		  "into the sky like a rocket..."}, true},
		{{"I guess I’ve completed the",
		  "final challenge."}, true},
	};

	struct Balloon {
		bool active;           // Is this balloon currently active?
		glm::vec3 pos;         // Current world position of the balloon
		glm::vec3 offset;      // Offset from the boss car's position (if needed)
		float size;            // Scale factor for the balloon model
		float spawnTimer;      // Timer for balloon-specific animations or lifetime control
	};


	static const int MAX_BALLOONS = 3;
	Balloon m_balloons[MAX_BALLOONS];
	float m_balloonSpawnTimer;    // Timer for balloon spawning
	float m_balloonSpawnInterval; // Interval between balloon spawns


};

#endif
