#pragma once
#include "SceneWhackAMole.h"
#include <iostream>
#include "GL\glew.h"

// GLM Headers
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_inverse.hpp>

//Include GLFW
#include <GLFW/glfw3.h>

#include "shader.hpp"
#include "MeshBuilder.h"
#include "KeyboardController.h"
#include "MouseController.h"
#include "LoadTGA.h"


bool SceneWhackAMole::scenecomplete = false;


SceneWhackAMole::SceneWhackAMole()
{
}

SceneWhackAMole::~SceneWhackAMole()
{
}

void SceneWhackAMole::Init()
{

	CAudioManager::GetInstance()->PlayMusic("Audio/Naktigonis - The Windsinger's Dance (Deepwoken OST) [ ezmp3.cc ].mp3");

	// Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);

	////Enable back face culling
	//glEnable(GL_CULL_FACE);

	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	// Load the shader programs
	m_programID = LoadShaders("Shader//Texture.vertexshader",
		"Shader//Text.fragmentshader");
	glUseProgram(m_programID);

	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");

	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

	m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
	m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID, "lights[2].position_cameraspace");
	m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
	m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID, "lights[2].power");
	m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
	m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
	m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");
	m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[2].spotDirection");
	m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[2].cosCutoff");
	m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID, "lights[2].cosInner");
	m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID, "lights[2].exponent");

	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");

	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	m_parameters[U_TEXT_ENABLED] =
		glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID,
		"textColor");

	// Step 4 - Add the following code
	Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT], m_parameters[U_MATERIAL_DIFFUSE],
		m_parameters[U_MATERIAL_SPECULAR],
		m_parameters[U_MATERIAL_SHININESS]);

	// Initialise camera properties
	camera.Init(glm::vec3(2, 9, 0), glm::vec3(-1, 9, 0), glm::vec3(0, 1, 0));

	// Init VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = nullptr;
	}

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("Axes", 10000.f, 10000.f, 10000.f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("Sun", glm::vec3(1.f, 1.f, 1.f), 1.f, 16, 16);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("STAMINA_BAR", glm::vec3(1, 1, 1), 1.f);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("Cube", glm::vec3(1, 1, 1), 1.f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("Sphere", glm::vec3(1, 1, 1), 1.f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("Sphere", glm::vec3(1, 1, 1), 1.f);

	//tgas
	meshList[GEO_BASE] = MeshBuilder::GenerateQuad("base", glm::vec3(1.f, 1.f, 1.f), 2.f);
	meshList[GEO_BASE]->textureID = LoadTGA("Images//whackamole_grid2.tga");
	meshList[GEO_BACKBOARD] = MeshBuilder::GenerateQuad("backboard", glm::vec3(1.f, 1.f, 1.f), 2.f);
	meshList[GEO_BACKBOARD]->textureID = LoadTGA("Images//whackamole_backboard.tga");


	meshList[GEO_HAMMER1] = MeshBuilder::GenerateOBJMTL("war hammer", "Models//lava_hammer.obj", "Models//lava_hammer.mtl");
	meshList[GEO_HAMMER1]->textureID = LoadTGA("Images//hammer.tga");
	meshList[GEO_HAMMER2] = MeshBuilder::GenerateOBJMTL("mallet", "Models//mallet.obj", "Models//mallet.mtl");
	meshList[GEO_HAMMER2]->textureID = LoadTGA("Images//Mallet_BaseColor.tga");
	meshList[GEO_HAMMER3] = MeshBuilder::GenerateOBJMTL("mallet3", "Models//mallet3.obj", "Models//mallet3.mtl");
	meshList[GEO_HAMMER3]->textureID = LoadTGA("Images//mallet_color.tga");

	//skybox
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Images//whackamole_sides.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Images//whackamole_sides.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Images//dark_tenttop.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("Plane",glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Images//nightsky_dnD.tga");
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Images//whackamole_sides.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Images//whackamole_sides.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16,16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Images//calibri.tga");
	meshList[GEO_TEXT2] = MeshBuilder::GenerateText("text2", 16, 16);
	meshList[GEO_TEXT2]->textureID = LoadTGA("Images//yugothicuisemibold.tga");
	meshList[GEO_FPS] = MeshBuilder::GenerateText("fpstext", 16, 16);
	meshList[GEO_FPS]->textureID = LoadTGA("Images//bizudgothic.tga");

	meshList[GEO_KEY_E] = MeshBuilder::GenerateQuad("KeyE", glm::vec3(1.f, 1.f, 1.f), 2.f);
	meshList[GEO_KEY_E]->textureID = LoadTGA("Images//keyboard_key_e.tga");
	meshList[GEO_KEY_R] = MeshBuilder::GenerateQuad("KeyE", glm::vec3(1.f, 1.f, 1.f), 2.f);
	meshList[GEO_KEY_R]->textureID = LoadTGA("Images//keyboard_key_r.tga");
	meshList[GEO_UI] = MeshBuilder::GenerateQuad("UIBox", glm::vec3(0.12f, 0.12f, 0.12f), 10.f);

	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	projectionStack.LoadMatrix(projection);

	glUniform1i(m_parameters[U_NUMLIGHTS], NUM_LIGHTS);

	light[0].position = glm::vec3(0, 25, 0);
	light[0].color = glm::vec3(1, 1, 1);
	light[0].type = Light::LIGHT_DIRECTIONAL;
	light[0].power = 1.f;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = 100.f;
	light[0].cosInner = 50.f;
	light[0].exponent = 3.f;
	light[0].spotDirection = glm::vec3(0.f, 1.f, 0.f);

	light[1].position = glm::vec3(0, 80, 0);
	light[1].color = glm::vec3(1, 1, 1);
	light[1].type = Light::LIGHT_SPOT;
	light[1].power = 0.4f;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = 45.f;
	light[1].cosInner = 30.f;
	light[1].exponent = 3.f;
	light[1].spotDirection = glm::vec3(0.f, 1.f, 0.f);

	light[2].position = glm::vec3(0,-20,0);
	light[2].color = glm::vec3(1, 1, 1);
	light[2].type = Light::LIGHT_POINT;
	light[2].power = 1.f;
	light[2].kC = 1.f;
	light[2].kL = 0.01f;
	light[2].kQ = 0.001f;
	light[2].cosCutoff = 45.f;
	light[2].cosInner = 30.f;
	light[2].exponent = 3.f;
	light[2].spotDirection = glm::vec3(0.f, 1.f, 0.f);

	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], cosf(glm::radians<float>(light[0].cosCutoff)));
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], cosf(glm::radians<float>(light[0].cosInner)));
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], cosf(glm::radians<float>(light[1].cosCutoff)));
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], cosf(glm::radians<float>(light[1].cosInner)));
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);

	glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, &light[2].color.r);
	glUniform1i(m_parameters[U_LIGHT2_TYPE], light[2].type);
	glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
	glUniform1f(m_parameters[U_LIGHT2_KC], light[2].kC);
	glUniform1f(m_parameters[U_LIGHT2_KL], light[2].kL);
	glUniform1f(m_parameters[U_LIGHT2_KQ], light[2].kQ);
	glUniform1f(m_parameters[U_LIGHT2_COSCUTOFF], cosf(glm::radians<float>(light[2].cosCutoff)));
	glUniform1f(m_parameters[U_LIGHT2_COSINNER], cosf(glm::radians<float>(light[2].cosInner)));
	glUniform1f(m_parameters[U_LIGHT2_EXPONENT], light[2].exponent);

	enableLight = true;

	//Hitbox for hammers
	//in order keypad numbering style
	cubelist.push_back(HammerCollide(1,GameObject::CUBE));
	cubelist.push_back(HammerCollide(2, GameObject::CUBE));
	cubelist.push_back(HammerCollide(3, GameObject::CUBE));
	cubelist.push_back(HammerCollide(4, GameObject::CUBE));
	cubelist.push_back(HammerCollide(5, GameObject::CUBE));
	cubelist.push_back(HammerCollide(6, GameObject::CUBE));
	cubelist.push_back(HammerCollide(7, GameObject::CUBE));
	cubelist.push_back(HammerCollide(8, GameObject::CUBE));
	cubelist.push_back(HammerCollide(9, GameObject::CUBE));
	cubelist[0].pos = glm::vec3{ 67,2,66 };
	cubelist[1].pos = glm::vec3{ 0,2,66 };
	cubelist[2].pos = glm::vec3{ -67,2,66 };
	cubelist[3].pos = glm::vec3{ 67,2,0 };
	cubelist[4].pos = glm::vec3{ 0,2,0 };
	cubelist[5].pos = glm::vec3{ -67,2,0 };
	cubelist[6].pos = glm::vec3{ 67,2,-66 };
	cubelist[7].pos = glm::vec3{ 0,2,-66 };
	cubelist[8].pos = glm::vec3{ -67,2,-66 };

	//hitbox for game walls
	glm::vec3 boxextent{ 100.f, 50.f, 1.f };

	walllist.push_back(Walls(101, GameObject::CUBE, glm::vec3{ 100.f, 50.f, 1.f }));
	walllist.push_back(Walls(102, GameObject::CUBE, glm::vec3{ 100.f, 50.f, 1.f }));
	walllist.push_back(Walls(103, GameObject::CUBE, glm::vec3{ 1.f, 50.f, 100.f }));
	walllist.push_back(Walls(103, GameObject::CUBE, glm::vec3{ 1.f, 50.f, 100.f }));
	walllist.push_back(Walls(104, GameObject::CUBE, glm::vec3{ 100.f, 5.f, 100.f }));
	walllist[0].pos = glm::vec3{ 0,0,-100};
	walllist[1].pos = glm::vec3{ 0,0,100 };
	walllist[2].pos = glm::vec3{ -100,0,0 };
	walllist[3].pos = glm::vec3{ 100,0,0 };
	walllist[4].pos = glm::vec3{ 0,-5,0 };

	//Player 
	player.push_back(Player(999, GameObject::CUBE));
	player[0].pos = camera.pos;
	player[0].target = camera.target;

	for (int i = 0; i < cubelist.size(); i++) {
		cubelist[i].mass = 0.f;
	}
	for (int i = 0; i < walllist.size(); i++) {
		walllist[i].mass = 0.f;
	}

	attackorder.Push(NodeHammer(1, 5, GEO_HAMMER2));
	attackorder.Push(NodeHammer(2, 4, GEO_HAMMER3));
	attackorder.Push(NodeHammer(2, 6, GEO_HAMMER1));
	attackorder.Push(NodeHammer(3, 1, GEO_HAMMER3));
	attackorder.Push(NodeHammer(3, 5, GEO_HAMMER2));
	attackorder.Push(NodeHammer(3, 9, GEO_HAMMER1));
	attackorder.Push(NodeHammer(4, 1, GEO_HAMMER3));
	attackorder.Push(NodeHammer(4, 3, GEO_HAMMER1));
	attackorder.Push(NodeHammer(4, 7, GEO_HAMMER3));
	attackorder.Push(NodeHammer(4, 9, GEO_HAMMER1));
	attackorder.Push(NodeHammer(5, 1, GEO_HAMMER3));
	attackorder.Push(NodeHammer(5, 3, GEO_HAMMER1));
	attackorder.Push(NodeHammer(5, 4, GEO_HAMMER3));
	attackorder.Push(NodeHammer(5, 8, GEO_HAMMER2));
	attackorder.Push(NodeHammer(5, 9, GEO_HAMMER1));
	attackorder.Push(NodeHammer(6, 1, GEO_HAMMER3));
	attackorder.Push(NodeHammer(6, 3, GEO_HAMMER1));
	attackorder.Push(NodeHammer(6, 5, GEO_HAMMER2));
	attackorder.Push(NodeHammer(6, 7, GEO_HAMMER3));
	attackorder.Push(NodeHammer(6, 9, GEO_HAMMER1));
	attackorder.Push(NodeHammer(7, 1, GEO_HAMMER3));
	attackorder.Push(NodeHammer(7, 4, GEO_HAMMER3));
	attackorder.Push(NodeHammer(7, 5, GEO_HAMMER2));
	attackorder.Push(NodeHammer(7, 6, GEO_HAMMER1));
	attackorder.Push(NodeHammer(7, 8, GEO_HAMMER2));
	attackorder.Push(NodeHammer(8, 2, GEO_HAMMER2));
	attackorder.Push(NodeHammer(8, 4, GEO_HAMMER3));
	attackorder.Push(NodeHammer(8, 5, GEO_HAMMER2));
	attackorder.Push(NodeHammer(8, 6, GEO_HAMMER1));
	attackorder.Push(NodeHammer(8, 8, GEO_HAMMER2));

	camera.allowJump = false;
}

void SceneWhackAMole::Update(double dt)
{

	HandleKeyPress();
	const float SPEED = 15.f;
	GameObject testobj = walllist[0];
	if (KeyboardController::GetInstance()->IsKeyDown('I'))
		testobj.pos.z -= static_cast<float>(dt) * SPEED;
	if (KeyboardController::GetInstance()->IsKeyDown('K'))
		testobj.pos.z += static_cast<float>(dt) * SPEED;
	if (KeyboardController::GetInstance()->IsKeyDown('J'))
		testobj.pos.x -= static_cast<float>(dt) * SPEED;
	if (KeyboardController::GetInstance()->IsKeyDown('L'))
		testobj.pos.x += static_cast<float>(dt) * SPEED;
	if (KeyboardController::GetInstance()->IsKeyDown('O'))
		testobj.pos.y -= static_cast<float>(dt) * SPEED;
	if (KeyboardController::GetInstance()->IsKeyDown('P'))
		testobj.pos.y += static_cast<float>(dt) * SPEED;

	if (KeyboardController::GetInstance()->IsKeyPressed('E') && !gamestart)
		isObjectiveRead = true;

	//Game start and game end logic
	if (isObjectiveRead) {
		iscameramove = true;
		if (startcountdown > 0.f)
			startcountdown -= dt;
		else if (!isplayerhit)
			gamestart = true;
	}


	CollisionData cd;
	//Game logic 
	if (gamestart) {
		if (isattack) {
			for (int j = 0; j < attackorder.size; j++) {
				if (attackorder.size != 0) {
					if (attackorder.GetCurrentPhase() == orderiter)
						inactionorder.push_back(attackorder.Pop());
				}
			}
		}
		if (isattack) {
			if (hammerrot < 110) {
				hammerrot += 60.f * dt;
			}
			else {
				attackchecktimer -= dt;
				if (attackchecktimer < 0.f)
					isattack = false;
			}
		}
		else {
			attackcooldown -= dt;
			if (attackcooldown < 0.f) {
				attackchecktimer = 0.1f;
				orderiter++;
				isattack = true;
				hammerrot = 0.f;
				attackcooldown = 3.f;
			}
		}

		//Detect hit 
		if (isattack && attackchecktimer < 0.1f) {
			for (int i = 0; i < inactionorder.size(); i++) {
				int iter = inactionorder[i].grid - 1; //to do index 0 
				if (OverlapAABB2AABB(player[0], player[0].boxextent, cubelist[iter], cubelist[iter].boxextent, cd)) {
					isplayerhit = true;
					cubelist[iter].iscollide = true;
				}
				else
					cubelist[iter].iscollide = false;
			}
		}
	}

	player[0].pos = camera.pos;
	glm::vec3 viewDir = glm::normalize(camera.target - camera.pos);
	for (int i = 0; i < walllist.size(); i++) {
		if (OverlapAABB2AABB(player[0], player[0].boxextent, walllist[i], walllist[i].boxextent, cd)) {
			ResolveCollision(cd);
			camera.pos = player[0].pos;
			camera.target = camera.pos + viewDir * 1.2f;
		}
	}
	if (isplayerhit || gamewin) {
		gamestart = false;
		iscameramove = false;
	}
	if (attackorder.size == 0 && inactionorder.empty())
		gamewin = true;
	if (KeyboardController::GetInstance()->IsKeyDown('R') && !gamestart) 
		InitGame();
	for (int i = 0; i < walllist.size(); i++) {
		walllist[i].UpdatePhysics(dt);
	}
	player[0].UpdatePhysics(dt);
	if (iscameramove)
		camera.Update(dt);
	if (gamewin)
		scenecomplete = true;

	float temp = 1.f / dt;
	fps = glm::round(temp * 100.f) / 100.f;
}

void SceneWhackAMole::Render()
{
	// Clear color buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Load view matrix stack and set it with camera position, target position and up direction
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.pos.x, camera.pos.y, camera.pos.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);

	// Load identity matrix into the model stack
	modelStack.LoadIdentity();

	//Light
	for (int i = 0; i < NUM_LIGHTS; i++) {
		UNIFORM_TYPE pos, spotdir;
		switch (i) {
		//index 0 be like
		case 0: pos = U_LIGHT0_POSITION; spotdir = U_LIGHT0_SPOTDIRECTION; break;
		case 1: pos = U_LIGHT1_POSITION; spotdir = U_LIGHT1_SPOTDIRECTION; break;
		case 2: pos = U_LIGHT2_POSITION; spotdir = U_LIGHT2_SPOTDIRECTION; break;
		}
		{
			glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[i].position, 1);
			glUniform3fv(m_parameters[pos], 1, glm::value_ptr(lightPosition_cameraspace));
			glm::vec3 spotDirection_cameraspace = viewStack.Top() * glm::vec4(light[i].spotDirection, 0);
			glUniform3fv(m_parameters[spotdir], 1, glm::value_ptr(spotDirection_cameraspace));
		}

		{
			glm::vec3 lightDir(light[i].position.x, light[i].position.y, light[i].position.z);
			glm::vec3 lightDirection_cameraspace = viewStack.Top() * glm::vec4(lightDir, 0);
			glUniform3fv(m_parameters[pos], 1, glm::value_ptr(lightDirection_cameraspace));
		}

		{
			glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[i].position, 1);
			glUniform3fv(m_parameters[pos], 1, glm::value_ptr(lightPosition_cameraspace));

		}
	}

	RenderSkyBox();

	//for (int i = 0; i < cubelist.size(); i++) {
	//	glm::vec3 material;
	//	if (!cubelist[i].iscollide)
	//		material = glm::vec3(0.1f, 1.f, 0.1f);
	//	else
	//		material = glm::vec3(1.f, 0.1f, 0.1f);

	//	modelStack.PushMatrix();
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//	modelStack.Translate(cubelist[i].pos.x, cubelist[i].pos.y, cubelist[i].pos.z);
	//	modelStack.Scale(2*cubelist[i].boxextent.x, 2*cubelist[i].boxextent.y, 2*cubelist[i].boxextent.z);
	//	meshList[GEO_CUBE]->material.kAmbient = material;
	//	meshList[GEO_CUBE]->material.kDiffuse = material;
	//	meshList[GEO_CUBE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	//	meshList[GEO_CUBE]->material.kShininess = 2.0f;
	//	RenderMesh(meshList[GEO_CUBE], true);
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//	modelStack.PopMatrix();
	//}


	//for (int i = 0; i < walllist.size(); i++) {
	//	modelStack.PushMatrix();
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//	modelStack.Translate(walllist[i].pos.x, walllist[i].pos.y, walllist[i].pos.z);
	//	modelStack.Scale( 2*walllist[i].boxextent.x, 2*walllist[i].boxextent.y, 2*walllist[i].boxextent.z);
	//	meshList[GEO_CUBE]->material.kAmbient = glm::vec3(1.f, 0.1f, 0.1f);
	//	meshList[GEO_CUBE]->material.kDiffuse = glm::vec3(1.f, 0.1f, 0.1f);
	//	meshList[GEO_CUBE]->material.kSpecular = glm::vec3(1.f, 0.2f, 0.2f);
	//	meshList[GEO_CUBE]->material.kShininess = 2.0f;
	//	RenderMesh(meshList[GEO_CUBE], true);
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//	modelStack.PopMatrix();
	//}

	modelStack.PushMatrix();
	modelStack.Rotate(-90.f, 1, 0, 0);
	modelStack.Scale(100.f, 100.f, 100.f);
	meshList[GEO_BASE]->material.kAmbient = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_BASE]->material.kDiffuse = glm::vec3(0.5f,0.5f, 0.5f);
	meshList[GEO_BASE]->material.kSpecular = glm::vec3(0.3f, 0.3f, 0.3f);
	meshList[GEO_BASE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_BASE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 60, -100);
	modelStack.Scale(100.f, 70.f, 1.f);
	meshList[GEO_BACKBOARD]->material.kAmbient = glm::vec3(0.9f,0.9f,0.9f);
	meshList[GEO_BACKBOARD]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_BACKBOARD]->material.kSpecular = glm::vec3(0.3f, 0.3f, 0.3f);
	meshList[GEO_BACKBOARD]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_BACKBOARD], true);
	modelStack.PopMatrix();

	//attacking anims
	if (isattack) {
		for (int j = 0; j < inactionorder.size(); j++) {
			GEOMETRY_TYPE model = inactionorder[j].type;
			glm::vec3 scale = inactionorder[j].scale, pos = inactionorder[j].position, pivot = inactionorder[j].pivot;
			float orientangle = inactionorder[j].orientateangle;

			modelStack.PushMatrix();
			modelStack.Translate(pos.x, pos.y, pos.z);
			if (model != GEO_HAMMER2)
				modelStack.Rotate(inactionorder[j].flipangle * hammerrot, 0, 0, 1.f); //animation
			else
				modelStack.Rotate(inactionorder[j].flipangle * hammerrot, 1.f, 0, 0.f); //animation

			if (model == GEO_HAMMER1)			//for hammer 1 only 
				modelStack.Rotate(90.f, 0.f, 0, 1.f); //keep upright
			modelStack.PushMatrix();
			modelStack.Translate(pivot.x, pivot.y, pivot.z);
			modelStack.Rotate(orientangle, 1.f, 0, 0);
			if (model == GEO_HAMMER2)
				modelStack.Rotate(-90.f, 0.f, 0.f, 1.f); //orientation
			modelStack.Scale(scale.x, scale.y, scale.z);
			meshList[model]->material.kAmbient = glm::vec3(0.5f, 0.5f, 0.5f);
			meshList[model]->material.kDiffuse = glm::vec3(0.8f, 0.8f, 0.8f);
			meshList[model]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
			meshList[model]->material.kShininess = 1.0f;
			RenderMesh(meshList[model], true);
			modelStack.PopMatrix();
			modelStack.PopMatrix();
		}
	}
	else
		inactionorder.clear();

	if (!gamestart && !isplayerhit) {
		if (startcountdown > 0) {
			std::string countdownText;
			if (startcountdown > 3.0f) {
				countdownText = "3..";
			}
			else if (startcountdown > 2.0f) {
				countdownText = "2..";
			}
			else if (startcountdown > 1.0f) {
				countdownText = "1..";
			}
			else {
				countdownText = "GO!";
			}
			RenderTextOnScreen(meshList[GEO_TEXT2], countdownText, glm::vec3(1, 1, 1), 50, 350, 300);
		}
	}
	else {
		RenderMeshOnScreen(meshList[GEO_UI], 45, 75, 25, 6);
		RenderMeshOnScreen(meshList[GEO_QUAD], 80, 60, 1.5*camera.GetStamina(), 15);
		RenderTextOnScreen(meshList[GEO_TEXT2], "Stamina: ", glm::vec3(1, 1, 1), 20, 10, 75);
		if (attackcooldown > 0.1f && !isattack) {
			RenderMeshOnScreen(meshList[GEO_UI], 55, 560, 55, 3);
			RenderTextOnScreen(meshList[GEO_TEXT2], "Next phase in: ", glm::vec3(1, 1, 1), 20, 10, 550);
			std::string attackcountdown;
			if (attackcooldown > 2.0f) {
				attackcountdown = "3";
			}
			else if (attackcooldown > 1.0f) {
				attackcountdown = "2";
			}
			else if (attackcooldown > 0.0f) {
				attackcountdown = "1";
			}
			RenderTextOnScreen(meshList[GEO_TEXT2], attackcountdown, glm::vec3(1, 1, 1), 20, 300, 550);
		}
	}

	if (!isObjectiveRead) { // Render Objective
		RenderMeshOnScreen(meshList[GEO_UI], 400, 320, 45, 30);
		RenderTextOnScreen(meshList[GEO_TEXT2], "- WHACK A MOLE -", glm::vec3(1, 1, 0), 25, 200, 430);
		RenderTextOnScreen(meshList[GEO_TEXT2], "- You are the mole!", glm::vec3(1, 1, 1), 13, 270, 380);
		RenderTextOnScreen(meshList[GEO_TEXT2], "- Dodge the hammers to survive!", glm::vec3(1, 1, 1), 14, 180, 350);
		RenderTextOnScreen(meshList[GEO_TEXT2], "- Each phase gets progressively", glm::vec3(1, 1, 1), 14, 180, 300);
		RenderTextOnScreen(meshList[GEO_TEXT2], " harder", glm::vec3(1, 1, 1), 14, 350, 280);
		RenderTextOnScreen(meshList[GEO_TEXT2], "- Survive all the waves to win! ", glm::vec3(1, 1, 1), 14, 180, 250);

		RenderMeshOnScreen(meshList[GEO_KEY_E], 310, 210, 15, 15);
		RenderTextOnScreen(meshList[GEO_TEXT], "Continue", glm::vec3(1, 1, 1), 20, 340, 200);
	}
	if (isplayerhit && !gamewin) {
		RenderMeshOnScreen(meshList[GEO_UI], 400, 320, 45, 20);
		RenderTextOnScreen(meshList[GEO_TEXT2], "GAME OVER!", glm::vec3(1, 0, 0), 40, 210, 350);

		RenderTextOnScreen(meshList[GEO_TEXT2], "You got hit!", glm::vec3(1, 1, 1), 20, 300, 300);

		RenderMeshOnScreen(meshList[GEO_KEY_R], 350, 250, 15, 15);
		RenderTextOnScreen(meshList[GEO_TEXT2], "Retry", glm::vec3(1, 1, 1), 20, 390, 240);
	}
	if (gamewin) {
		RenderMeshOnScreen(meshList[GEO_UI], 400, 320, 45, 25);
		RenderTextOnScreen(meshList[GEO_TEXT2], "YOU WON!", glm::vec3(0, 1, 0), 50, 220, 350);
		RenderTextOnScreen(meshList[GEO_TEXT2], "You've beaten the", glm::vec3(1, 1, 1), 20, 240, 300);
		RenderTextOnScreen(meshList[GEO_TEXT2], "Whack-A-Mole Game!", glm::vec3(1, 1, 1), 20, 230, 270);

		RenderMeshOnScreen(meshList[GEO_KEY_E], 250, 220, 15, 15);
		RenderTextOnScreen(meshList[GEO_TEXT2], "Back to Carnival", glm::vec3(1, 1, 1), 20, 290, 210);
	}

	std::string temp("FPS:" + std::to_string(fps));
	RenderTextOnScreen(meshList[GEO_FPS], temp.substr(0, 9), glm::vec3(0, 1, 0), 20, 620, 20);
}

void SceneWhackAMole::RenderMesh(Mesh* mesh, bool enableLight)
{
	glm::mat4 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, glm::value_ptr(MVP));
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, glm::value_ptr(modelView));

	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = glm::inverseTranspose(modelView);
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, glm::value_ptr(modelView_inverse_transpose));

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	// Step 9 - Modify the code to include the logic below
	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}	
	mesh->Render();

	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}


void SceneWhackAMole::Exit()
{
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
		{
			delete meshList[i];
		}
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

void SceneWhackAMole::HandleKeyPress()
{
	if (KeyboardController::GetInstance()->IsKeyPressed(0x31))
	{
		// Key press to enable culling
		glEnable(GL_CULL_FACE);
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(0x32))
	{
		// Key press to disable culling
		glDisable(GL_CULL_FACE);
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(0x33))
	{
		// Key press to enable fill mode for the polygon
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(0x34))
	{
		// Key press to enable wireframe mode for the polygon
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode
	}
	//if (KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
	//{
	//	// Change to black background
	//	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//}

	static bool isRightUp = false;
	if (!isRightUp && MouseController::GetInstance()->IsButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
	{

	}
	else if (isRightUp && MouseController::GetInstance()->IsButtonUp(GLFW_MOUSE_BUTTON_RIGHT))
	{
		isRightUp = false;
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_TAB))
	{
		if (light[0].type == Light::LIGHT_POINT) {
			light[0].type = Light::LIGHT_DIRECTIONAL;
			std::cout << "DIRECTIONAL" << std::endl;
		}
		else if (light[0].type == Light::LIGHT_DIRECTIONAL) {
			light[0].type = Light::LIGHT_SPOT;
			std::cout << "LIGHT SPOT" << std::endl;
		}
		else {
			light[0].type = Light::LIGHT_POINT;
			std::cout << "LIGHT POINT" << std::endl;
		}

		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	}

}

void SceneWhackAMole::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey)
{
	glDisable(GL_DEPTH_TEST);
	glm::mat4 ortho = glm::ortho(0.f, 800.f, 0.f, 600.f, -
		1000.f, 1000.f); // dimension of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	// To do: Use modelStack to position GUI on screen
	modelStack.Translate(x, y, 0);
	// To do: Use modelStack to scale the GUI
	modelStack.Scale(sizex, sizey, 1);
	RenderMesh(mesh, false); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneWhackAMole::RenderText(Mesh* mesh, std::string text, glm::vec3
	color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Disable back face culling
	glDisable(GL_CULL_FACE);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		glm::mat4 characterSpacing = glm::translate(
			glm::mat4(1.f),
			glm::vec3(i * 1.0f, 0, 0));
		glm::mat4 MVP = projectionStack.Top() * viewStack.Top() *
			modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
			glm::value_ptr(MVP));
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

void SceneWhackAMole::RenderTextOnScreen(Mesh* mesh, std::string
	text, glm::vec3 color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glm::mat4 ortho = glm::ortho(0.f, 800.f, 0.f, 600.f, -
		100.f, 100.f); // dimension of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		glm::mat4 characterSpacing = glm::translate(
			glm::mat4(1.f),
			glm::vec3(0.5f + i * 1.0f, 0.5f, 0)
		);
		glm::mat4 MVP = projectionStack.Top() *
			viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
			glm::value_ptr(MVP));
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void SceneWhackAMole::Material(GEOMETRY_TYPE obj, float AmR, float AmG, float AmB, float DifA, float DifG, float DifB, float SpA, float SpG, float SpB, float Shiny)
{
	meshList[obj]->material.kAmbient = glm::vec3(AmR, AmG, AmB);
	meshList[obj]->material.kDiffuse = glm::vec3(DifA, DifG, DifB);
	meshList[obj]->material.kSpecular = glm::vec3(SpA, SpG, SpB);
	meshList[obj]->material.kShininess = Shiny;
}

void SceneWhackAMole::InitGame()
{
	camera.pos = glm::vec3(0, 9, 0);
	camera.target = glm::vec3(-1, 9, 0);
	camera.up = glm::vec3(0, 1, 0);

	orderiter = 1;
	startcountdown = 4.f;
	attackcooldown = 3.f;
	attackchecktimer = 0.1f;
	hammerrot = 0.f;
	isattack = true;
	gamestart = false;
	isplayerhit = false;
	iscameramove = true;
	gamewin = false;


	//refresh
	attackorder.Clear(); 
	inactionorder.clear();
	attackorder.Push(NodeHammer(1, 5, GEO_HAMMER2));
	attackorder.Push(NodeHammer(2, 4, GEO_HAMMER3));
	attackorder.Push(NodeHammer(2, 6, GEO_HAMMER1));
	attackorder.Push(NodeHammer(3, 1, GEO_HAMMER3));
	attackorder.Push(NodeHammer(3, 5, GEO_HAMMER2));
	attackorder.Push(NodeHammer(3, 9, GEO_HAMMER1));
	attackorder.Push(NodeHammer(4, 1, GEO_HAMMER3));
	attackorder.Push(NodeHammer(4, 3, GEO_HAMMER1));
	attackorder.Push(NodeHammer(4, 7, GEO_HAMMER3));
	attackorder.Push(NodeHammer(4, 9, GEO_HAMMER1));
	attackorder.Push(NodeHammer(5, 1, GEO_HAMMER3));
	attackorder.Push(NodeHammer(5, 3, GEO_HAMMER1));
	attackorder.Push(NodeHammer(5, 4, GEO_HAMMER3));
	attackorder.Push(NodeHammer(5, 8, GEO_HAMMER2));
	attackorder.Push(NodeHammer(5, 9, GEO_HAMMER1));
	attackorder.Push(NodeHammer(6, 1, GEO_HAMMER3));
	attackorder.Push(NodeHammer(6, 3, GEO_HAMMER1));
	attackorder.Push(NodeHammer(6, 5, GEO_HAMMER2));
	attackorder.Push(NodeHammer(6, 7, GEO_HAMMER3));
	attackorder.Push(NodeHammer(6, 9, GEO_HAMMER1));
	attackorder.Push(NodeHammer(7, 1, GEO_HAMMER3));
	attackorder.Push(NodeHammer(7, 4, GEO_HAMMER3));
	attackorder.Push(NodeHammer(7, 5, GEO_HAMMER2));
	attackorder.Push(NodeHammer(7, 6, GEO_HAMMER1));
	attackorder.Push(NodeHammer(7, 8, GEO_HAMMER2));
	attackorder.Push(NodeHammer(8, 2, GEO_HAMMER2));
	attackorder.Push(NodeHammer(8, 4, GEO_HAMMER3));
	attackorder.Push(NodeHammer(8, 5, GEO_HAMMER2));
	attackorder.Push(NodeHammer(8, 6, GEO_HAMMER1));
	attackorder.Push(NodeHammer(8, 8, GEO_HAMMER2));



}

void SceneWhackAMole::RenderSkyBox() {
	modelStack.PushMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.f, 0.f, 300.f);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(6.f, 6.f, 6.f);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.f, 0.f, -300.f);
	modelStack.Scale(6.f, 6.f, 6.f);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(300.f, 0.f, 0.f);
	modelStack.Rotate(270, 0, 1, 0);
	modelStack.Scale(6.f, 6.f, 6.f);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-300.f, 0.f, 0.f);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(6.f, 6.f, 6.f);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Translate(0.f, 300.f, 0.f);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(6.f, 6.f, 6.f);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.f, -300.f, 0.f);
	modelStack.Rotate(270, 1, 0, 0);
	modelStack.Scale(6.f, 6.f, 6.f);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
}
