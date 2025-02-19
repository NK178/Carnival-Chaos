#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H

#include "Scene.h"
#include "Mesh.h"
#include "Application.h"
#include "FPCamera.h"
#include "MatrixStack.h"
#include "Light.h"
#include "CollisionDetection.h"

class SceneMain : public Scene
{
public:
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		//shapes
		GEO_SPHERE,
		GEO_QUAD,
		GEO_PLANE,

		//Text
		GEO_TEXT,

		//Models
		GEO_TENT,
		GEO_TREE,
		GEO_FENCE,
		GEO_SIGN,

		//GUI
		GEO_KEY_E,
		GEO_KEY_F,

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

	SceneMain();
	~SceneMain();

	virtual void Init();
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

	glm::vec3 signPosition;
	bool showSignText;
	bool readSign;

	struct DialogueLine {
		std::vector<std::string> textLines;  
		bool isMultiLine;                    
	};

	std::vector<DialogueLine> enterMainSceneLines = {
		{{"Hmm... What's this?"}, false},
		{{"A carnival? In the middle of the town?!"}, false},
		{{"It wasn't here before",
		  "if I recall before..."}, true},
		{{"I see a sign there!"}, false},
		{{"I guess it wouldn't hurt",
		  "to take a look..."}, true},
	};

	std::vector<DialogueLine> signDialogueLines = {
		{{"Let me see..."}, false},
		{{"If you complete all 6 games..."}, false},
		{{"Along with the final challenge..."}, false},
		{{"You get to bring home",
		  "a grand prize of..."}, true},
		{{"A MILLION DOLLARS?!"}, false},
		{{"Sounds too good to be true..."}, false},
		{{"Whatever, since I am already here,",
		  "why not do it anyways?"}, true},
		{{"Looks like I have to complete all 6",
		  "games in their respective tents first."}, true},
	};

	int currentLineIndex = 0;
	float dialogueTimer = 0;
	const float TEXT_DISPLAY_TIME = 4.0f;

	bool isEnterMainSceneDialogueActive;
	bool hasPlayedEnterMainSceneDialogue;

	bool isSignDialogueActive;
	bool hasReadSign;
	bool showReadSignText;
	float readSignTextTimer;
	const float READ_SIGN_TEXT_DISPLAY_TIME = 3.0f;

	glm::vec3 tentPositions[6];
	bool showEnterTentText[6];
	bool tentCompleted[6];
	glm::vec3 finalTentPosition;
	bool showEnterFinalTentText;
	bool isFinalChallengeCompleted;

	Application app;

	bool OverlapAABB2AABB(glm::vec3 Obj1, const int Width1, const int Height1, glm::vec3 Obj2, const int Width2, const int Height2);
	void RenderUI();
	void RenderObjectives();
	void UpdateDialogue(double dt);
	void RenderDialogue();

	void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey);
	void RenderText(Mesh* mesh, std::string text, glm::vec3 color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, glm::vec3 color, float size, float x, float y);
	void Material(GEOMETRY_TYPE obj, float AmR, float AmG, float AmB, float DifA, float DifG, float DifB, float SpA, float SpG, float SpB, float Shiny);
	void RenderSkyBox();
};

#endif