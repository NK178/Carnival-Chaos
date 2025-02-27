#ifndef SCENE_WHACK_A_MOLE_H
#define SCENE_WHACK_A_MOLE_H

#include "Scene.h"
#include "Mesh.h"
#include "Application.h"
#include "FPCamera.h"
#include "MatrixStack.h"
#include "Light.h"
#include "CTree.h"
#include "GameObject.h"

class SceneWhackAMole : public Scene
{
public:
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		//shapes
		GEO_CUBE,
		GEO_QUAD,
		GEO_SPHERE,

		//TGAs
		GEO_BASE,
		GEO_BACKBOARD,

		//OBJS
		GEO_HAMMER1,
		GEO_HAMMER2,
		GEO_HAMMER3,

		//Text
		GEO_TEXT,
		GEO_TEXT2,
		GEO_UI,
		GEO_KEY_E,
		GEO_KEY_R,

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

	SceneWhackAMole();
	~SceneWhackAMole();

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
	void InitGame();
	void RenderSkyBox();

	//Hammer 1 
	// + //modelStack.Rotate(90.f, 0.f, 0, 1.f); //keep upright
	//modelStack.Translate(23, 0, 0); //pivot
	//modelStack.Rotate(90.f, 1.f, 0, 0.f); //orientation
	//modelStack.Scale(0.5f, 0.5f, 0.5f); //scale
	//Hammer 2
	//modelStack.Translate(0, 8, 0); //pivot		
	//modelStack.Rotate(-90.f, 1.f, 0, 0.f); //orientation
	//modelStack.Scale(0.6f, 0.6f, 0.6f); //scale
	//Hammer 3 
	//modelStack.Translate(0, 0, 0); //pivot
	//modelStack.Rotate(0.f, 0, 0, 1.f); //orientation
	//modelStack.Scale(250.f, 250.f, 250.f); //scale


	//Queue 
	struct NodeHammer {  
		NodeHammer* next;
		int phase, grid;
		glm::vec3 position;
		glm::vec3 pivot;
		glm::vec3 scale;
		float orientateangle;
		int flipangle;
		GEOMETRY_TYPE type;

		NodeHammer() { next = nullptr; phase = 0, position = glm::vec3(0, 0, 0);  };
		NodeHammer(int phase, int grid, GEOMETRY_TYPE type){ 
			next = nullptr;
			this->phase = phase;
			switch (grid) {
			case 1: this->grid = grid; position = glm::vec3(130, 25, 66); break;
			case 2: this->grid = grid; position = glm::vec3(0, 25, 110); break;
			case 3: this->grid = grid; position = glm::vec3(-100, 25, 58); break;
			case 4: this->grid = grid; position = glm::vec3(130, 25, 0); break;
			case 5: this->grid = grid; position = glm::vec3(0, 25, 44); break;
			case 6: this->grid = grid; position = glm::vec3(-100, 25, -8); break;
			case 7: this->grid = grid; position = glm::vec3(130, 25, -66); break;
			case 8: this->grid = grid; position = glm::vec3(0, 25, -22); break;
			case 9: this->grid = grid; position = glm::vec3(-100, 25, -74); break;
			default: this->grid = 0; position = glm::vec3(0, 0, 0); break;
			}

			switch (type) {
			case GEO_HAMMER1: flipangle = -1; this->type = type; orientateangle = 90.f; scale = glm::vec3{ 0.5f,0.5f,0.5f }; pivot = glm::vec3{ 23,0,0 };  break;
			case GEO_HAMMER2: flipangle = -1;  this->type = type; orientateangle = -90.f; scale = glm::vec3{ 0.7f,0.7f,0.7f }; pivot = glm::vec3{ 0,8,0 }; break;
			case GEO_HAMMER3: flipangle = 1;  this->type = type; orientateangle = 0.f; scale = glm::vec3{ 250.f,250.f,250.f }; pivot = glm::vec3{ 0,0,0 }; break;
			default: orientateangle = 0.f; break;
			}
		}
	};
	struct Queue {
		int size;
		NodeHammer* head, * current, * tail;
		Queue() {
			head = current = tail = nullptr;
			size = 0;
		}
		~Queue() {
			if (head != nullptr) {
				while (head != nullptr) {
					current = head;
					head = head->next;
					delete current;
				}
			}
		}
		int GetCurrentPhase(void) {
			if (head != nullptr)
				return head->phase;
			else
				return 0;
		}
		void Push(NodeHammer input) {
			NodeHammer* newnode = new NodeHammer(input.phase, input.grid,input.type);

			if (head == nullptr) 
				head = tail = current = newnode;
			else {
				current = tail;
				tail = newnode;
				current->next = tail;
				tail->next = nullptr;
			}
			size++;
		}		
		NodeHammer Pop(void){
			NodeHammer returning;
			if (head == current) {
				returning = *head;
				delete head;
				head = tail = current = nullptr;
			}
			else if (head != nullptr) {
				returning = *head;
				current = head;
				head = head->next;
				delete current;
			}
			else
				std::cout << "Queue Empty " << std::endl;
			size--;
			return returning;
		}		
		void Print(void)
		{
			if (head != nullptr) {
				current = head;
				while (current != nullptr) {
					std::cout << current->phase << std::endl;
					current = current->next;
				}
			}
			std::cout << std::endl;
		}
		void Clear(void) {
			if (head != nullptr) {
				size = 0;
				current = head;
				while (current != nullptr) {
					head = current->next;
					delete current;
					current = head;
				}
			}
		}
		
	};
	struct HammerCollide : public GameObject {
		bool iscollide = false;
		glm::vec3 boxextent{ 33.f, 3.5f, 33.f };
		HammerCollide(int id, int type) : GameObject(id, type) {}
	};
	struct Walls : public GameObject {
		glm::vec3 boxextent;
		Walls(int id, int type, glm::vec3 extent) : GameObject(id, type) {
			boxextent = extent;
		}
	};
	struct Player : public GameObject {
		glm::vec3 boxextent{2.f,10.f,2.f};
		glm::vec3 target;
		Player(int id, int type) : GameObject(id,type) {}
	};



	bool activate = false;
	std::vector<HammerCollide> cubelist;
	std::vector<Walls> walllist;
	std::vector<Player> player;
	std::vector<glm::vec3> hammerspawnpts;
	std::vector<NodeHammer> inactionorder;
	Queue attackorder;
	
	int orderiter = 1; 
	float startcountdown = 4.f;
	float attackcooldown = 3.f;
	float attackchecktimer = 0.1f;
	float hammerrot = 0.f;
	bool isattack = true;
	bool gamestart = false; 
	bool isplayerhit = false;
	bool iscameramove = false;


	//Testing 
	bool shockwave = false;
	bool gravity = false;
	bool isObjectiveRead = false;



};

#endif