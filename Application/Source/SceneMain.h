#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H


#include "Scene.h"
#include "Mesh.h"
#include "Application.h"
#include "FPCamera.h"
#include "MatrixStack.h"
#include "Light.h"
#include "CollisionDetection.h"
#include "GameObject.h"
#include "CAudioManager.h"


class SceneMain : public Scene
{
public:
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		//shapes
		GEO_SPHERE,
		GEO_CUBE,
		GEO_QUAD,
		GEO_PLANE,

		//Text
		GEO_TEXT,
		GEO_FPS,

		//Models
		GEO_TENT,
		GEO_TREE,
		GEO_FENCE,
		GEO_SIGN,
		GEO_HOUSE,
		GEO_ROAD,
		GEO_MONEYBAG,

		//GUI
		GEO_KEY_E,
		GEO_KEY_F,
		GEO_KEY_Q,
		GEO_UI,

		//Skybox
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,

		//Skybox 2 (Sunset)
		GEO_LEFT2,
		GEO_RIGHT2,
		GEO_TOP2,
		GEO_BOTTOM2,
		GEO_FRONT2,
		GEO_BACK2,

		//Skybox 3 (Midnight Silence)
		GEO_LEFT3,
		GEO_RIGHT3,
		GEO_TOP3,
		GEO_BOTTOM3,
		GEO_FRONT3,
		GEO_BACK3,

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

	SceneMain();
	~SceneMain();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

     bool shouldEnterArchery = false;      // Flag to enter the archery minigame
     bool shouldEnterBalloonPop = false;   // Flag to enter the balloon pop minigame
     bool shouldEnterHole = false;         // Flag to enter the hole minigame
     bool shouldEnterWhackAMole = false;   // Flag to enter the whack-a-mole minigame
	 bool shouldEnterBumperBalls = false;   // Flag to enter the Bumper balls minigame
     bool shouldEnterSpinningRing = false; // Flag to enter the spinning ring minigame
     bool shouldEnterFinal = false;        // Flag to enter the final scene

	 // Save the current state (to be called before Exit())
	 void SaveState();

	 // Restore the previously saved state (to be called after Init())
	 void RestoreState();

	 // Flag to check if state should be restored
	 static bool hasStateToRestore;


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

	// structure for dialogue lines
	struct DialogueLine {
		std::vector<std::string> textLines;  
		bool isMultiLine;                    
	};

	int cutsceneStage = -1;
	float tempCompensation;

	std::vector<DialogueLine> sceneMainCutsceneLines = {
		{{"Hmm... What's this?"}, false},
		{{"A carnival? In the middle of town?!"}, false},
		{{"It wasn't here before",
		  "if I recall correctly..."}, true},
		{{"I see a sign there!"}, false},
		{{"I guess it wouldn't hurt",
		  "to have a look..."}, true},
	};

	std::vector<DialogueLine> signDialogueLines;

	// dialogue displays
	int currentLineIndex = -1;
	float dialogueTimer = 0;
	bool isTyping;
	float typewriterTimer; 
	std::string currentText; 
	int currentCharIndex;

	bool isCutsceneDialogueActive;
	bool hasPlayedCutsceneDialogue;

	// sign interaction
	glm::vec3 signPosition;
	bool showSignText;
	bool readSign;

	bool isSignDialogueActive;
	bool hasReadSign;
	bool showReadSignText;
	float readSignTextTimer;

	// tent positions and tent interactions
	glm::vec3 tentPositions[6];
	bool showEnterTentText[6];
	bool tentCompleted[6];
	bool CheckAllTentsCompleted();

	glm::vec3 finalTentPosition;
	bool showEnterFinalTentText;
	bool isFinalChallengeCompleted;

	// Collisions
	struct playerBox : public GameObject {
		glm::vec3 playerDimensions{ 1.5f,7.f,1.5f };
		playerBox(int id, int type) : GameObject(id, type) {}
	};

	struct tentBoxes : public GameObject {
		glm::vec3 tentDimensions{ 10.f,15.f,10.f };
		tentBoxes(int id, int type) : GameObject(id, type) {}
	};

	struct finalTentBox : public GameObject {
		glm::vec3 tentDimensions{ 15.f,20.f,15.f };
		finalTentBox(int id, int type) : GameObject(id, type) {}
	};

	struct sideFenceBox : public GameObject {
		glm::vec3 fenceDimensions{ 2.f,20.f,20.f };
		sideFenceBox(int id, int type) : GameObject(id, type) {}
	};

	struct frontFenceBox : public GameObject {
		glm::vec3 fenceDimensions{ 20.f,20.f,2.f };
		frontFenceBox(int id, int type) : GameObject(id, type) {}
	};

	struct frontBoundaryBox : public GameObject {
		glm::vec3 boundaryDimensions{ 60.f,20.f,2.f };
		frontBoundaryBox(int id, int type) : GameObject(id, type) {}
	};

	struct sideBoundaryBox : public GameObject {
		glm::vec3 boundaryDimensions{ 2.f,20.f,10.f };
		sideBoundaryBox(int id, int type) : GameObject(id, type) {}
	};

	std::vector<playerBox> player;
	std::vector<tentBoxes> tentList;
	std::vector<finalTentBox> finalTent;
	std::vector<sideFenceBox> sideFenceList;
	std::vector<frontFenceBox> frontFenceList;
	std::vector<frontBoundaryBox> frontBoundary;
	std::vector<sideBoundaryBox> sideBoundary;

	Application app;

	float fps = 0;

	bool isSceneLoaded; 
	bool cutsceneSkipped;

	void RenderUI();
	void RenderObjectives();
	void RenderDialogue();
	void UpdateDialogue(double dt);
	void UpdateSignText();

	void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey);
	void RenderText(Mesh* mesh, std::string text, glm::vec3 color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, glm::vec3 color, float size, float x, float y);
	void Material(GEOMETRY_TYPE obj, float AmR, float AmG, float AmB, float DifA, float DifG, float DifB, float SpA, float SpG, float SpB, float Shiny);
	void RenderSkyBox();

	// Structure to hold saved state
	static struct SavedState {
		bool tentCompleted[6];
		bool hasReadSign;
		bool hasPlayedCutsceneDialogue;
		bool isFinalChallengeCompleted;
		bool isInitialized;
	} savedState;
};

#endif
