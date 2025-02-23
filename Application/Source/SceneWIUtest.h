#ifndef SCENE_WIU_TEST_H
#define SCENE_WIU_TEST_H

#include "Scene.h"
#include "Mesh.h"
#include "Application.h"
#include "FPCamera.h"
#include "MatrixStack.h"
#include "Light.h"
#include "CTree.h"
#include "GameObject.h"

class SceneWIUtest : public Scene
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
		GEO_HAMMER1,
		GEO_HAMMER2,
		GEO_HAMMER3,

		//Text
		GEO_TEXT,

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

	SceneWIUtest();
	~SceneWIUtest();

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

	Application app;
	
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey);
	void RenderText(Mesh* mesh, std::string text, glm::vec3 color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, glm::vec3 color, float size, float x, float y);
	void Material(GEOMETRY_TYPE obj, float AmR, float AmG, float AmB, float DifA, float DifG, float DifB, float SpA, float SpG, float SpB, float Shiny);
	void RenderSkyBox();
	static glm::vec3 Calculatenormal(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2) {
		glm::vec3 edge1 = v1 - v0;
		glm::vec3 edge2 = v2 - v0;
		return glm::normalize(glm::cross(edge1, edge2)); 
	}

	struct Cube : public GameObject {
		glm::vec3 boxextent{ 1.5f,1.5f,1.5f };
		Cube(int id, int type) : GameObject(id, type) {}
	};
	struct Sphere : public GameObject {
		float radius;
		Sphere(int id, int r, int type) : radius(r), GameObject(id, type) {}
	};
	struct Cylinder : public GameObject {
		float height;
		float radius;
		Cylinder(int id, int type, int h, int r) : radius(r), height(h), GameObject(id, type) {}
	};

	struct OBB : public GameObject {
		std::vector<glm::vec3> verticebuffer; //eg. 8
		std::vector<glm::vec3> indexbuffer;
		std::vector<glm::vec3> normals;
		OBB(int id, int type) : GameObject(id, type) {
			verticebuffer.push_back(glm::vec3(-1.5f, 1.5f, 1.5f));   
			verticebuffer.push_back(glm::vec3(1.5f, 1.5f, 1.5f));    
			verticebuffer.push_back(glm::vec3(-1.5f, -1.5f, 1.5f));  
			verticebuffer.push_back(glm::vec3(1.5f, -1.5f, 1.5f));   
			verticebuffer.push_back(glm::vec3(-1.5f, 1.5f, -1.5f));  
			verticebuffer.push_back(glm::vec3(1.5f, 1.5f, -1.5f));   
			verticebuffer.push_back(glm::vec3(-1.5f, -1.5f, -1.5f)); 
			verticebuffer.push_back(glm::vec3(1.5f, -1.5f, -1.5f));  

			indexbuffer.push_back(glm::vec3{ 0,1,2 });
			indexbuffer.push_back(glm::vec3{ 4,5,6 });
			indexbuffer.push_back(glm::vec3{ 0,4,3 });
			indexbuffer.push_back(glm::vec3{ 1,5,2 });
			indexbuffer.push_back(glm::vec3{ 0,1,4 });
			indexbuffer.push_back(glm::vec3{ 2,3,6 });				

			for (int i = 0; i < indexbuffer.size(); i++) {
				int v0index = indexbuffer[i][0];
				int v1index = indexbuffer[i][1];
				int v2index = indexbuffer[i][2];

				glm::vec3 v0 = verticebuffer[v0index];
				glm::vec3 v1 = verticebuffer[v1index];
				glm::vec3 v2 = verticebuffer[v2index];
					
				glm::vec3 normal = Calculatenormal(v0, v1, v2);
				normals.push_back(normal);
			}
		}
	};

	//try again
	struct OBBV2 : public GameObject { 
		std::vector<glm::vec3> normals;
		std::vector<glm::vec3> vertices;

		OBBV2(int id, int type) : GameObject(id, type) {
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
	std::vector<std::vector<glm::vec3>> worldnormals;
	std::vector<std::vector<glm::vec3>> worldvertices;
	std::vector<std::vector<glm::vec3>> V1worldnormals;
	std::vector<std::vector<glm::vec3>> V1worldvertices;
	bool activate = false;
	std::vector<Cube> cubelist;
	std::vector<Sphere> spherelist;	
	std::vector<Cylinder> cylinderlist;	
	std::vector<OBB> obblist;
	std::vector<OBBV2> obblistV2;

	void SortCube2Collide(std::vector<GameObject> GOlist, std::vector<Cube> maincubelist, std::vector<Cube>& newcubelist);
	//void CubeCollisions(std::vector<Cube>& Cubelist, CollisionData cd);
	void CubeCollisions(std::vector<int> idlist, std::vector<Cube>& Cubelist, CollisionData cd);

	int iter = 0;
	int normaliter = 0;

};

#endif