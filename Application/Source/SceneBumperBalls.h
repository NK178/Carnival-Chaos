#ifndef SCENE_BUMPER_BALLS_H
#define SCENE_BUMPER_BALLS_H

#include "Scene.h"
#include "Mesh.h"
#include "Application.h"
#include "FPCamera.h"
#include "MatrixStack.h"
#include "Light.h"
#include "CTree.h"
#include "GameObject.h"

class SceneBumperBalls : public Scene
{
public:
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		//shapes
		GEO_CUBE,
		GEO_QUAD,
		GEO_SPHERE,
		GEO_PLANE,
		GEO_CYLINDER,

		//Obj
		GEO_BEACHBALL,
		GEO_BASKETBALL,
		GEO_BARREL,

		//UI
		GEO_TEXT,
		GEO_TEXT2,
		GEO_UI,
		GEO_KEY_E,
		GEO_KEY_R,
		GEO_FPS,

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

	SceneBumperBalls();
	~SceneBumperBalls();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	bool gamewin = false;

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
	
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey);
	void RenderText(Mesh* mesh, std::string text, glm::vec3 color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, glm::vec3 color, float size, float x, float y);
	void Material(GEOMETRY_TYPE obj, float AmR, float AmG, float AmB, float DifA, float DifG, float DifB, float SpA, float SpG, float SpB, float Shiny);
	void UpdateMovement(float dt);
	void RenderSkyBox();
	void InitGame();

	struct Sphere : public GameObject {
		float radius;
		glm::vec3 target;
		char lat;
		char vert;
		float xrot;
		float zrot;
		float xrotvel;
		float zrotvel;
		Sphere(int id, float r, int type) : radius(r), GameObject(id, type) {}
	};
	struct PlayerBall : public GameObject {
		float radius;
		float xrot; 
		float zrot;
		float xrotvel;
		float zrotvel;
		PlayerBall(int id, float r, int type) : radius(r), xrot(0), zrot(0), xrotvel(0), zrotvel(0),GameObject(id, type) {}
	};
	struct Cylinder : public GameObject {
		float height;
		float radius;
		Cylinder(int id, int type, float h, float r) : radius(r), height(h), GameObject(id, type) {}
	};

	std::vector<Sphere> spherelist;
	std::vector<PlayerBall> player;
	std::vector<Cylinder> cylinderlist;
	glm::vec3 newcampos;
	bool gamestart = false; 
	bool gamelose = false;
	const float PLAYER_SPEED = 50.f;
	int ballcount = 4;
	bool isballactive[4] = { true,true,true,true };
	bool isObjectiveRead = false;
	float countdown = 4.f;
};

#endif