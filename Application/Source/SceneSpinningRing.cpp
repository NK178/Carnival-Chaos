#pragma once
#include "SceneSpinningRing.h"
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


SceneSpinningRing::SceneSpinningRing()
{
}

SceneSpinningRing::~SceneSpinningRing()
{
}

void SceneSpinningRing::Init()
{
	camera.enableFNAF = false;
	camera.allowMovement = true;
	camera.allowJump = true;
	camera.allowSprint = false;
	camera.allowCrouch = false;
	camera.allowProne = false;
	camera.allowLocomotiveTilt = true;
	camera.allowLocomotiveBop = false;

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
	camera.Init(glm::vec3(-30,9,-20), glm::vec3(0,0,0), glm::vec3(0,1,0));

	// Init VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = nullptr;
	}

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("Axes", 10000.f, 10000.f, 10000.f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("Sun", glm::vec3(1.f, 1.f, 1.f), 1.f, 16, 16);
	meshList[GEO_PLANE] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 2.f);
	meshList[GEO_PLANE]->textureID = LoadTGA("Images//grass.tga");
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("STAMINA_BAR", glm::vec3(1, 1, 1), 1.f);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("Cube", glm::vec3(1, 1, 1), 1.f);
	meshList[GEO_CUBE]->textureID = LoadTGA("Images//lava.tga");
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("Sphere", glm::vec3(1, 1, 1), 1.f);
	meshList[GEO_CYLINDER] = MeshBuilder::GenerateCylinder("Cylinder", glm::vec3(1, 1, 1), 16, 1.0f, 16.0f);
	meshList[GEO_CYLINDER]->textureID = LoadTGA("Images//platform.tga");

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Images//circus_skybox.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Images//circus_skybox.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Images//tenttop.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("Plane",glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Images//tenttop.tga");
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Images//circus_skybox.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Images//circus_skybox.tga");

	// 16 x 16 is the number of columns and rows for the text
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16,16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Images//calibri.tga");
	meshList[GEO_TEXT2] = MeshBuilder::GenerateText("text2", 16, 16);
	meshList[GEO_TEXT2]->textureID = LoadTGA("Images//yugothicuisemibold.tga");
	meshList[GEO_FPS] = MeshBuilder::GenerateText("fpstext", 16, 16);
	meshList[GEO_FPS]->textureID = LoadTGA("Images//bizudgothic.tga");
	meshList[GEO_UI] = MeshBuilder::GenerateQuad("UIBox", glm::vec3(0.12f, 0.12f, 0.12f), 10.f);

	meshList[GEO_KEY_E] = MeshBuilder::GenerateQuad("KeyE", glm::vec3(1.f, 1.f, 1.f), 2.f);
	meshList[GEO_KEY_E]->textureID = LoadTGA("Images//keyboard_key_e.tga");
	meshList[GEO_WARNING] = MeshBuilder::GenerateQuad("warning", glm::vec3(1.f, 1.f, 1.f), 2.f);
	meshList[GEO_WARNING]->textureID = LoadTGA("Images//warning.tga");

	meshList[GEO_SPINNER] = MeshBuilder::GenerateOBJ("Spinner", "Models//spinner.obj");
	meshList[GEO_SPINNER]->textureID = LoadTGA("Images//spinner.tga");
	meshList[GEO_SPINNER2] = MeshBuilder::GenerateOBJ("Spinner", "Models//spinner2.obj");
	meshList[GEO_SPINNER2]->textureID = LoadTGA("Images//spinner2.tga");

	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	projectionStack.LoadMatrix(projection);

	glUniform1i(m_parameters[U_NUMLIGHTS], NUM_LIGHTS);

	//light[0].position = glm::vec3(30, 30, 0);
	//light[0].color = glm::vec3(1, 1, 1);
	//light[0].type = Light::LIGHT_SPOT;
	//light[0].power = 1.f;
	//light[0].kC = 1.f;
	//light[0].kL = 0.01f;
	//light[0].kQ = 0.001f;
	//light[0].cosCutoff = 30.f;
	//light[0].cosInner = 15.f;
	//light[0].exponent = 3.f;
	//light[0].spotDirection = glm::vec3(0.f, 1.f, 0.f);

	//light[1].position = glm::vec3(0, 3, 0);
	//light[1].color = glm::vec3(1, 1, 1);
	//light[1].type = Light::LIGHT_SPOT;
	//light[1].power = 0.4f;
	//light[1].kC = 1.f;
	//light[1].kL = 0.01f;
	//light[1].kQ = 0.001f;
	//light[1].cosCutoff = 45.f;
	//light[1].cosInner = 30.f;
	//light[1].exponent = 3.f;
	//light[1].spotDirection = glm::vec3(0.f, 1.f, 0.f);

	//light[2].position = glm::vec3(-50,4,0);
	//light[2].color = glm::vec3(1, 1, 1);
	//light[2].type = Light::LIGHT_POINT;
	//light[2].power = 1.f;
	//light[2].kC = 1.f;
	//light[2].kL = 0.01f;
	//light[2].kQ = 0.001f;
	//light[2].cosCutoff = 45.f;
	//light[2].cosInner = 30.f;
	//light[2].exponent = 3.f;
	//light[2].spotDirection = glm::vec3(0.f, 1.f, 0.f);

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

	// Initialize Variables
	enableLight = true;

	wallRotation = 0.0f;
	wallSpeed = 50.0f;
	beamRotation = 0.0f;
	beamSpeed = 0.0f;
	beamWarningTimer = 3.f;
	isBeamSpawned = false;
	isShowingBeamWarning = false;

	isObjectiveRead = false;
	remainingTime = 30.0f;
	countdownTime = 4.0f;
	playerWon = false;
	playerLost = false;

	// Collisions
	player.push_back(playerBox(1, GameObject::CUBE));
	wallSideList.push_back(spinningWallSide(2, GameObject::CUBE));
	wallSideList.push_back(spinningWallSide(3, GameObject::CUBE));
	wallSideList.push_back(spinningWallSide(4, GameObject::CUBE));
	wallSideList.push_back(spinningWallSide(5, GameObject::CUBE));

	wallTopList.push_back(spinningWallTop(6, GameObject::CUBE));
	wallTopList.push_back(spinningWallTop(7, GameObject::CUBE));

	middleWall.push_back(middleWallBox(8, GameObject::CUBE));

	beamList.push_back(spinningBeam(8, GameObject::CUBE));
	beamList.push_back(spinningBeam(9, GameObject::CUBE));

	// Collision Boxes Position
	player[0].pos = camera.pos;
	wallSideList[0].pos = glm::vec3{ 0,0,8 };
	wallSideList[1].pos = glm::vec3{ 0,0,45 };
	wallSideList[2].pos = glm::vec3{ 0,0,-8 };
	wallSideList[3].pos = glm::vec3{ 0,0,-45 };
	wallTopList[0].pos = glm::vec3{ 0,0,0 };
	wallTopList[1].pos = glm::vec3{ 0,20,0 };
	middleWall[0].pos = glm::vec3{ 0,0,0 };
	beamList[0].pos = glm::vec3{ 50,0,0 };
	beamList[1].pos = glm::vec3{ -5,0,0 };

	for (int i = 0; i < wallSideList.size(); i++) {
		wallSideList[i].mass = 0.f;
	}

	for (int i = 0; i < wallTopList.size(); i++) {
		wallTopList[i].mass = 0.f;
	}

	for (int i = 0; i < beamList.size(); i++) {
		beamList[i].mass = 0.f;
	}

	middleWall[0].mass = 0.f;
}

void SceneSpinningRing::Update(double dt)
{
	HandleKeyPress();

	// Countdown Timer
	if (isObjectiveRead) {
		if (countdownTime > 0) {
			countdownTime -= dt; // decrease countdown time
			if (countdownTime < 0) {
				countdownTime = 0; // ensure countdown does not go below 0
			}
		}
		else {
			remainingTime -= dt; // decrease game time after countdown ends
			if (remainingTime < 0) {
				remainingTime = 0; // ensure time does not go below 0
				playerWon = true; // player wins
				wallSpeed = 0.0f;
				beamSpeed = 0.0f;
			}
		}
	}

	// only update player and camera if countdown has ended
	if (countdownTime <= 0) {
		camera.Update(dt);
		player[0].pos = camera.pos;
		glm::vec3 viewDir = glm::normalize(camera.target - camera.pos);

		// slowly increase rotation speed
		wallSpeed += 1.0f * dt;
		wallRotation += wallSpeed * dt;
		if (wallSpeed >= 120.f) {
			wallSpeed = 120.f;
		}

		if (!isBeamSpawned) {
			if (!isShowingBeamWarning && remainingTime <= 19.0f) {
				isShowingBeamWarning = true;
				beamWarningTimer = 3.0f;
			}

			if (isShowingBeamWarning) {
				beamWarningTimer -= dt;
				if (beamWarningTimer <= 0.0f) {
					isShowingBeamWarning = false;
					isBeamSpawned = true;

					if (beamList.empty()) { // Ensure only one beam is added
						beamSpeed = 50.f;
						beamList.push_back(spinningBeam(beamList.size() + 1, GameObject::CUBE));
					}
				}
			}
		}

		if (isBeamSpawned) {
			beamSpeed += 3.0f * dt;
			beamRotation += beamSpeed * dt;
			if (beamSpeed >= 90.f) {
				beamSpeed = 90.f;
			}
		}

		// Collision
		CollisionData cd;
		playerNormals.clear();
		playerVertices.clear();
		spinningWallSideNormals.clear();
		spinningWallSideVertices.clear();
		spinningWallTopNormals.clear();
		spinningWallTopVertices.clear();
		middleWallNormals.clear();
		middleWallVertices.clear();
		spinningBeamNormals.clear();
		spinningBeamVertices.clear();

		player[0].UpdatePhysics(dt);

		for (int i = 0; i < wallSideList.size(); i++) {
			wallSideList[i].UpdatePhysics(dt);
		}

		for (int i = 0; i < wallTopList.size(); i++) {
			wallTopList[i].UpdatePhysics(dt);
		}

		for (int i = 0; i < beamList.size(); i++) {
			beamList[i].UpdatePhysics(dt);
		}

		middleWall[0].UpdatePhysics(dt);

		// collision normals and vertices
		{
			for (int o = 0; o < player.size(); o++) {
				std::vector<glm::vec3> tempNormals = player[o].normals;
				Updatenormals(player[o], tempNormals);
				playerNormals.push_back(tempNormals);

				std::vector<glm::vec3> tempVertices = player[o].vertices;
				Updatevertices(player[o], tempVertices);
				playerVertices.push_back(tempVertices);
			}

			for (int o = 0; o < wallSideList.size(); o++) {
				std::vector<glm::vec3> tempNormals = wallSideList[o].normals;
				Updatenormals(wallSideList[o], tempNormals);
				spinningWallSideNormals.push_back(tempNormals);

				std::vector<glm::vec3> tempVertices = wallSideList[o].vertices;
				Updatevertices(wallSideList[o], tempVertices);
				spinningWallSideVertices.push_back(tempVertices);
			}

			for (int o = 0; o < wallTopList.size(); o++) {
				std::vector<glm::vec3> tempNormals = wallTopList[o].normals;
				Updatenormals(wallTopList[o], tempNormals);
				spinningWallTopNormals.push_back(tempNormals);

				std::vector<glm::vec3> tempVertices = wallTopList[o].vertices;
				Updatevertices(wallTopList[o], tempVertices);
				spinningWallTopVertices.push_back(tempVertices);
			}

			for (int o = 0; o < middleWall.size(); o++) {
				std::vector<glm::vec3> tempNormals = middleWall[o].normals;
				Updatenormals(middleWall[o], tempNormals);
				middleWallNormals.push_back(tempNormals);

				std::vector<glm::vec3> tempVertices = middleWall[o].vertices;
				Updatevertices(middleWall[o], tempVertices);
				middleWallVertices.push_back(tempVertices);
			}

			for (int o = 0; o < beamList.size(); o++) {
				std::vector<glm::vec3> tempNormals = beamList[o].normals;
				Updatenormals(beamList[o], tempNormals);
				spinningBeamNormals.push_back(tempNormals);

				std::vector<glm::vec3> tempVertices = beamList[o].vertices;
				Updatevertices(beamList[o], tempVertices);
				spinningBeamVertices.push_back(tempVertices);
			}
		}

		// SAT collision
		{
			for (int i = 0; i < wallSideList.size(); i++) {
				if (SAT(wallSideList[i], spinningWallSideNormals[i], spinningWallSideVertices[i], player[0], playerNormals[0], playerVertices[0], cd)) {
					ResolveCollision(cd);
					camera.pos = player[0].pos;
					camera.target = camera.pos + viewDir * 1.2f;
				}
			}

			for (int i = 0; i < wallTopList.size(); i++) {
				if (SAT(wallTopList[i], spinningWallTopNormals[i], spinningWallTopVertices[i], player[0], playerNormals[0], playerVertices[0], cd)) {
					ResolveCollision(cd);
					camera.pos = player[0].pos;
					camera.target = camera.pos + viewDir * 1.2f;
				}
			}

			for (int i = 0; i < middleWall.size(); i++) {
				if (SAT(middleWall[i], middleWallNormals[i], middleWallVertices[i], player[0], playerNormals[0], playerVertices[0], cd)) {
					ResolveCollision(cd);
					camera.pos = player[0].pos;
					camera.target = camera.pos + viewDir * 1.2f;
				}
			}

			for (int i = 0; i < beamList.size(); i++) {
				if (SAT(beamList[i], spinningBeamNormals[i], spinningBeamVertices[i], player[0], playerNormals[0], playerVertices[0], cd)) {
					ResolveCollision(cd);
					camera.pos = player[0].pos;
					camera.target = camera.pos + viewDir * 1.2f;
				}
			}
		}
	}

	// FPS
	float temp = 1.f / dt;
	fps = glm::round(temp * 100.f) / 100.f;
}

void SceneSpinningRing::Render()
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

	// Render Lava
	modelStack.PushMatrix();
	modelStack.Translate(0.f, -120.f, 0.f);
	modelStack.Scale(500.f, 10.f, 500.f);
	meshList[GEO_CUBE]->material.kAmbient = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_CUBE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_CUBE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_CUBE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_CUBE], true);
	modelStack.PopMatrix();

	// Render Spinners
	{
		modelStack.PushMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		modelStack.Translate(0.f, 15.f, 0.f);
		modelStack.Rotate(wallRotation, 0.f, 1.f, 0.f);
		modelStack.Scale(30.f, 30.f, 50.f);
		meshList[GEO_SPINNER]->material.kAmbient = glm::vec3(0.5f, 0.5f, 0.5f);
		meshList[GEO_SPINNER]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		meshList[GEO_SPINNER]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
		meshList[GEO_SPINNER]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_SPINNER], true);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		modelStack.Translate(0.f, 5.f, 0.f);
		modelStack.Rotate(beamRotation, 0.f, 1.f, 0.f);
		modelStack.Scale(50.f, 30.f, 50.f);
		meshList[GEO_SPINNER2]->material.kAmbient = glm::vec3(0.5f, 0.5f, 0.5f);
		meshList[GEO_SPINNER2]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		meshList[GEO_SPINNER2]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
		meshList[GEO_SPINNER2]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_SPINNER2], true);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		modelStack.PopMatrix();
	}

	// Render Platform
	modelStack.PushMatrix();
	modelStack.Translate(0.f, -120.f, 0.f);
	modelStack.Scale(50.f, 15.f, 50.f);
	meshList[GEO_CYLINDER]->material.kAmbient = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_CYLINDER]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_CYLINDER]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_CYLINDER]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_CYLINDER], true);
	modelStack.PopMatrix();

	for (int j = 0; j < spinningWallSideVertices.size(); j++) {
		for (int i = 0; i < wallSideList[j].vertices.size(); i++) {
			modelStack.PushMatrix();
			modelStack.Translate(spinningWallSideVertices[j][i].x, spinningWallSideVertices[j][i].y, spinningWallSideVertices[j][i].z);
			modelStack.Scale(0.1, 0.1, 0.1);
			meshList[GEO_SPHERE]->material.kAmbient;
			meshList[GEO_SPHERE]->material.kDiffuse;
			meshList[GEO_SPHERE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
			meshList[GEO_SPHERE]->material.kShininess = 2.0f;
			RenderMesh(meshList[GEO_SPHERE], true);
			modelStack.PopMatrix();
		}
	}

	for (int j = 0; j < spinningWallTopVertices.size(); j++) {
		for (int i = 0; i < wallTopList[j].vertices.size(); i++) {
			modelStack.PushMatrix();
			modelStack.Translate(spinningWallTopVertices[j][i].x, spinningWallTopVertices[j][i].y, spinningWallTopVertices[j][i].z);
			modelStack.Scale(0.1, 0.1, 0.1);
			meshList[GEO_SPHERE]->material.kAmbient;
			meshList[GEO_SPHERE]->material.kDiffuse;
			meshList[GEO_SPHERE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
			meshList[GEO_SPHERE]->material.kShininess = 2.0f;
			RenderMesh(meshList[GEO_SPHERE], true);
			modelStack.PopMatrix();
		}
	}

	for (int j = 0; j < middleWallVertices.size(); j++) {
		for (int i = 0; i < middleWall[j].vertices.size(); i++) {
			modelStack.PushMatrix();
			modelStack.Translate(middleWallVertices[j][i].x, middleWallVertices[j][i].y, middleWallVertices[j][i].z);
			modelStack.Scale(0.1, 0.1, 0.1);
			meshList[GEO_SPHERE]->material.kAmbient;
			meshList[GEO_SPHERE]->material.kDiffuse;
			meshList[GEO_SPHERE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
			meshList[GEO_SPHERE]->material.kShininess = 2.0f;
			RenderMesh(meshList[GEO_SPHERE], true);
			modelStack.PopMatrix();
		}
	}

	for (int j = 0; j < spinningBeamVertices.size(); j++) {
		for (int i = 0; i < beamList[j].vertices.size(); i++) {
			modelStack.PushMatrix();
			modelStack.Translate(spinningBeamVertices[j][i].x, spinningBeamVertices[j][i].y, spinningBeamVertices[j][i].z);
			modelStack.Scale(0.1, 0.1, 0.1);
			meshList[GEO_SPHERE]->material.kAmbient;
			meshList[GEO_SPHERE]->material.kDiffuse;
			meshList[GEO_SPHERE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
			meshList[GEO_SPHERE]->material.kShininess = 2.0f;
			RenderMesh(meshList[GEO_SPHERE], true);
			modelStack.PopMatrix();
		}
	}

	if (!isObjectiveRead) { // Render Objective
		RenderMeshOnScreen(meshList[GEO_UI], 400, 320, 45, 30);
		RenderTextOnScreen(meshList[GEO_TEXT2], "- SPINNING RING -", glm::vec3(1, 1, 0), 25, 200, 430);
		RenderTextOnScreen(meshList[GEO_TEXT2], "- Avoid the spinning walls and", glm::vec3(1, 1, 1), 13, 195, 380);
		RenderTextOnScreen(meshList[GEO_TEXT2], "beams by jumping over them!", glm::vec3(1, 1, 1), 14, 205, 350);
		RenderTextOnScreen(meshList[GEO_TEXT2], "- Don't fall into the lava!", glm::vec3(1, 1, 1), 14, 210, 300);
		RenderTextOnScreen(meshList[GEO_TEXT2], "- Survive until the timer ends!", glm::vec3(1, 1, 1), 14, 190, 250);

		RenderMeshOnScreen(meshList[GEO_KEY_E], 310, 210, 15, 15);
		RenderTextOnScreen(meshList[GEO_TEXT2], "Continue", glm::vec3(1, 1, 1), 20, 340, 200);
	}

	if (isObjectiveRead) { // Render Countdown
		if (countdownTime > 0) {
			std::string countdownText;
			if (countdownTime > 3.0f) {
				countdownText = "3..";
			}
			else if (countdownTime > 2.0f) {
				countdownText = "2..";
			}
			else if (countdownTime > 1.0f) {
				countdownText = "1..";
			}
			else {
				countdownText = "GO!";
			}
			RenderTextOnScreen(meshList[GEO_TEXT2], countdownText, glm::vec3(1, 1, 1), 50, 350, 300);
		}
		else if (playerWon) { // Render UI if player Wins
			RenderMeshOnScreen(meshList[GEO_UI], 400, 320, 45, 25);
			RenderTextOnScreen(meshList[GEO_TEXT2], "YOU WON!", glm::vec3(0, 1, 0), 50, 220, 350);
			RenderTextOnScreen(meshList[GEO_TEXT2], "You've beaten the", glm::vec3(1, 1, 1), 20, 240, 300);
			RenderTextOnScreen(meshList[GEO_TEXT2], "Spinning Rings Game!", glm::vec3(1, 1, 1), 20, 210, 270);

			RenderMeshOnScreen(meshList[GEO_KEY_E], 250, 220, 15, 15);
			RenderTextOnScreen(meshList[GEO_TEXT2], "Back to Carnival", glm::vec3(1, 1, 1), 20, 290, 210);
		}
		else { // Render Time Left
			RenderMeshOnScreen(meshList[GEO_UI], 45, 560, 45, 3);
			std::string timeText = "Time Left: " + std::to_string(static_cast<int>(remainingTime));
			RenderTextOnScreen(meshList[GEO_TEXT2], timeText, glm::vec3(1, 1, 1), 20, 10, 550);
		}
	}

	if (isShowingBeamWarning) {
		int countdown = static_cast<int>(glm::ceil(beamWarningTimer)); // countdown warning
		RenderMeshOnScreen(meshList[GEO_UI], 100, 530, 100, 3);
		RenderMeshOnScreen(meshList[GEO_WARNING], 20, 530, 15, 15);
		RenderTextOnScreen(meshList[GEO_TEXT2],
			"Beam is spawning in " + std::to_string(countdown) + " seconds!",
			glm::vec3(1, 0, 0), 18, 45, 520);
	}

	// Render FPS
	std::string temp("FPS:" + std::to_string(fps)); 
	RenderTextOnScreen(meshList[GEO_FPS], temp.substr(0, 9), glm::vec3(0, 1, 0), 20, 620, 20);
}

void SceneSpinningRing::RenderMesh(Mesh* mesh, bool enableLight)
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

void SceneSpinningRing::Exit()
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

void SceneSpinningRing::HandleKeyPress()
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

	if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_E)) {
		if (playerWon) 
		{
			// go back to scene main
		}
		else 
		{
			isObjectiveRead = true; // set to true when the objective is read
		}
	}

}

void SceneSpinningRing::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey)
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

void SceneSpinningRing::RenderText(Mesh* mesh, std::string text, glm::vec3
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

void SceneSpinningRing::RenderTextOnScreen(Mesh* mesh, std::string
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

void SceneSpinningRing::Material(GEOMETRY_TYPE obj, float AmR, float AmG, float AmB, float DifA, float DifG, float DifB, float SpA, float SpG, float SpB, float Shiny)
{
	meshList[obj]->material.kAmbient = glm::vec3(AmR, AmG, AmB);
	meshList[obj]->material.kDiffuse = glm::vec3(DifA, DifG, DifB);
	meshList[obj]->material.kSpecular = glm::vec3(SpA, SpG, SpB);
	meshList[obj]->material.kShininess = Shiny;
}

void SceneSpinningRing::RenderSkyBox() {
	modelStack.PushMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.f, 0.f, 250.f);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(5.f, 5.f, 5.f);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.f, 0.f, -250.f);
	modelStack.Scale(5.f, 5.f, 5.f);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(250.f, 0.f, 0.f);
	modelStack.Rotate(270, 0, 1, 0);
	modelStack.Scale(5.f, 5.f, 5.f);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-250.f, 0.f, 0.f);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(5.f, 5.f, 5.f);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Translate(0.f, 250.f, 0.f);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(5.f, 5.f, 5.f);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.f, -250.f, 0.f);
	modelStack.Rotate(270, 1, 0, 0);
	modelStack.Scale(5.f, 5.f, 5.f);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();
}