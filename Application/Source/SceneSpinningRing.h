#ifndef SCENE_SPINNING_RING_H
#define SCENE_SPINNING_RING_H

#include "Scene.h"
#include "Mesh.h"
#include "Application.h"
#include "FPCamera.h"
#include "MatrixStack.h"
#include "Light.h"
#include "CTree.h"
#include "GameObject.h"

class SceneSpinningRing : public Scene
{
public:
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		//shapes
		GEO_CUBE,
		GEO_QUAD,
		GEO_SPHERE,
		GEO_CYLINDER,
		GEO_PLANE,

		// Models
		GEO_SPINNER,
		GEO_SPINNER2,

		//Text
		GEO_TEXT,
		GEO_TEXT2,
		GEO_FPS,
		GEO_KEY_E,
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

	SceneSpinningRing();
	~SceneSpinningRing();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	bool playerWon;

private:

	struct playerBox : public GameObject {
		std::vector<glm::vec3> normals;
		std::vector<glm::vec3> vertices;

		playerBox(int id, int type) : GameObject(id, type) {
			normals.push_back(glm::vec3(0, 1, 0));
			normals.push_back(glm::vec3(0, -1, 0));
			normals.push_back(glm::vec3(1, 0, 0));
			normals.push_back(glm::vec3(-1, 0, 0));
			normals.push_back(glm::vec3(0, 0, 1));
			normals.push_back(glm::vec3(0, 0, -1));

			vertices.push_back(glm::vec3(-1.5f, 1.5f, 1.5f));    // Vertex 0
			vertices.push_back(glm::vec3(1.5f, 1.5f, 1.5f));    // Vertex 1
			vertices.push_back(glm::vec3(-1.5f, -1.5f, 1.5f));    // Vertex 2
			vertices.push_back(glm::vec3(1.5f, -1.5f, 1.5f));    // Vertex 3
			vertices.push_back(glm::vec3(-1.5f, 1.5f, -1.5f));    // Vertex 4
			vertices.push_back(glm::vec3(1.5f, 1.5f, -1.5f));    // Vertex 5
			vertices.push_back(glm::vec3(-1.5f, -1.5f, -1.5f));    // Vertex 6
			vertices.push_back(glm::vec3(1.5f, -1.5f, -1.5f));    // Vertex 7
		}
	};

	struct spinningWallSide : public GameObject {
		std::vector<glm::vec3> normals;
		std::vector<glm::vec3> vertices;

		spinningWallSide(int id, int type) : GameObject(id, type) {
			normals.push_back(glm::vec3(0, 1, 0));
			normals.push_back(glm::vec3(0, -1, 0));
			normals.push_back(glm::vec3(1, 0, 0));
			normals.push_back(glm::vec3(-1, 0, 0));
			normals.push_back(glm::vec3(0, 0, 1));
			normals.push_back(glm::vec3(0, 0, -1));

			vertices.push_back(glm::vec3(-3.f, 30.f, 3.5f));    // Vertex 0
			vertices.push_back(glm::vec3(3.f, 30.f, 3.5f));    // Vertex 1
			vertices.push_back(glm::vec3(-3.f, 1.5f, 3.5f));    // Vertex 2
			vertices.push_back(glm::vec3(3.f, 1.5f, 3.5f));    // Vertex 3
			vertices.push_back(glm::vec3(-3.f, 30.f, -3.5f));    // Vertex 4
			vertices.push_back(glm::vec3(3.f, 30.f, -3.5f));    // Vertex 5
			vertices.push_back(glm::vec3(-3.f, 1.5f, -3.5f));    // Vertex 6
			vertices.push_back(glm::vec3(3.f, 1.5f, -3.5f));    // Vertex 7
		}
	};

	std::vector<std::vector<glm::vec3>> playerNormals;
	std::vector<std::vector<glm::vec3>> playerVertices;
	std::vector<std::vector<glm::vec3>> spinningWallSideNormals;
	std::vector<std::vector<glm::vec3>> spinningWallSideVertices;

	struct spinningWallTop : public GameObject {
		glm::vec3 spinningWallTopDimensions{ 1.5f,7.f,1.5f };
		spinningWallTop(int id, int type) : GameObject(id, type) {}
	};

	struct spinningBeam: public GameObject {
		glm::vec3 spinningBeamDimensions{ 1.5f,7.f,1.5f };
		spinningBeam(int id, int type) : GameObject(id, type) {}
	};

	std::vector<playerBox> player;
	std::vector<spinningWallSide> wallSideList;
	std::vector<spinningWallTop> wallTopList;
	std::vector<spinningBeam> beamList;

	void HandleKeyPress();
	void RenderMesh(Mesh* mesh, bool enableLight);

	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	//AltAzCamera camera;
	FPCamera camera;
	int projType = 1; // fix to 0 for orthographic, 1 for projection

	bool isObjectiveRead;
	float remainingTime;
	float countdownTime;
	bool playerLost;

	MatrixStack modelStack, viewStack, projectionStack;	

	static const int NUM_LIGHTS = 3;
	Light light[NUM_LIGHTS];
	bool enableLight;

	Application app;

	float fps = 0;
	
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey);
	void RenderText(Mesh* mesh, std::string text, glm::vec3 color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, glm::vec3 color, float size, float x, float y);
	void Material(GEOMETRY_TYPE obj, float AmR, float AmG, float AmB, float DifA, float DifG, float DifB, float SpA, float SpG, float SpB, float Shiny);
	void RenderSkyBox();
};

#endif