#pragma once
#include "MainMenu.h"
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

MainMenu::MainMenu()
{
	shouldStartGame = false;
}

MainMenu::~MainMenu()
{
}

void MainMenu::Init()
{


	CAudioManager::GetInstance()->PlayMusic("Audio/Naktigonis - A Memory Of Tides (Band Version) (Deepwoken OST) [ ezmp3.cc ].mp3");

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
	camera.Init(glm::vec3(-10, 3, -10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

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
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Images//nightsky_lfD.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Images//nightsky_rtD.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Images//nightsky_upD.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Images//nightsky_dnD.tga");
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Images//nightsky_bkD.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Images//nightsky_ftD.tga");

	// 16 x 16 is the number of columns and rows for the text
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Images//calibri.tga");
	meshList[GEO_TITLE] = MeshBuilder::GenerateText("text2", 16, 16);
	meshList[GEO_TITLE]->textureID = LoadTGA("Images//lucidasansregular.tga");
	meshList[GEO_UI] = MeshBuilder::GenerateQuad("UIBox", glm::vec3(0.12f, 0.12f, 0.12f), 10.f);

	meshList[GEO_IMAGE] = MeshBuilder::GenerateQuad("Image", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_IMAGE]->textureID = LoadTGA("Images//carnival_chaos.tga");

	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	projectionStack.LoadMatrix(projection);

	glUniform1i(m_parameters[U_NUMLIGHTS], NUM_LIGHTS);

	light[0].position = glm::vec3(30, 30, 0);
	light[0].color = glm::vec3(1, 1, 1);
	light[0].type = Light::LIGHT_DIRECTIONAL;
	light[0].power = 1.f;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = 30.f;
	light[0].cosInner = 15.f;
	light[0].exponent = 3.f;
	light[0].spotDirection = glm::vec3(0.f, 1.f, 0.f);

	light[1].position = glm::vec3(0, 3, 0);
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

	light[2].position = glm::vec3(-50,4,0);
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
	isCreditsEntered = false;
	isControlsEntered = false;
	isScrollUp = true;
	isScrollDown = false;
}

void MainMenu::Update(double dt)
{
	Application::SetPointerStatus(true);
	double x = MouseController::GetInstance()->GetMousePositionX();
	double y = 1080 - MouseController::GetInstance()->GetMousePositionY();

	selectedOption = -1;

	if ((!isCreditsEntered && !isControlsEntered) && (x >= 1248 && x <= 1725))
	{
		if (y >= 659 && y <= 801) {
			selectedOption = 0;
		}
		else if (y >= 482 && y <= 625)
		{
			selectedOption = 1;  
		}
		else if (y >= 304 && y <= 453) 
		{
			selectedOption = 2;
		}
		else if (y >= 130 && y <= 275)
		{
			selectedOption = 3;
		}
	}

	HandleMouseClick();
	HandleKeyPress();
}

void MainMenu::Render()
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

	RenderMeshOnScreen(meshList[GEO_IMAGE], 400, 350, 10, 10); // adjust if neccessary

	if (!isCreditsEntered && !isControlsEntered) {
		RenderTextOnScreen(meshList[GEO_TITLE], "CARNIVAL CHAOS", glm::vec3(1, 0, 0), 30, 20, 82);
		RenderTextOnScreen(meshList[GEO_TITLE], "presented by: x86 Assembly", glm::vec3(1, 1, 1), 15, 20, 60);

		RenderMeshOnScreen(meshList[GEO_UI], 620, 400, 20, 8);
		RenderTextOnScreen(meshList[GEO_TITLE], "PLAY", glm::vec3(1, 1, 1), 40, 550, 380);
		RenderMeshOnScreen(meshList[GEO_UI], 620, 300, 20, 8);
		RenderTextOnScreen(meshList[GEO_TITLE], "CREDITS", glm::vec3(1, 1, 1), 28, 530, 280);
		RenderMeshOnScreen(meshList[GEO_UI], 620, 200, 20, 8);
		RenderTextOnScreen(meshList[GEO_TITLE], "CONTROLS", glm::vec3(1, 1, 1), 25, 530, 185);
		RenderMeshOnScreen(meshList[GEO_UI], 620, 100, 20, 8);
		RenderTextOnScreen(meshList[GEO_TITLE], "EXIT", glm::vec3(1, 1, 1), 40, 550, 80);
	}

	if (isCreditsEntered) {
		RenderMeshOnScreen(meshList[GEO_UI], 120, 100, 20, 8);
		RenderTextOnScreen(meshList[GEO_TITLE], "BACK", glm::vec3(1, 1, 1), 40, 50, 80);
		RenderMeshOnScreen(meshList[GEO_UI], 120, 520, 20, 8);
		RenderTextOnScreen(meshList[GEO_TITLE], "CREDITS", glm::vec3(1, 1, 0), 28, 30, 505);
		RenderMeshOnScreen(meshList[GEO_UI], 520, 310, 50, 50);

		if (isScrollUp) {
			// Ainsley
			RenderTextOnScreen(meshList[GEO_TITLE], "Ainsley", glm::vec3(1, 0, 0), 15, 280, 530);
			RenderTextOnScreen(meshList[GEO_TITLE], "- Collision Detection ", glm::vec3(1, 1, 1), 15, 280, 500);
			RenderTextOnScreen(meshList[GEO_TITLE], "- Binary Tree ", glm::vec3(1, 1, 1), 15, 280, 470);
			RenderTextOnScreen(meshList[GEO_TITLE], "- Whack-A-Mole Minigame ", glm::vec3(1, 1, 1), 15, 280, 440);
			RenderTextOnScreen(meshList[GEO_TITLE], "- Bumper Balls Minigame ", glm::vec3(1, 1, 1), 15, 280, 410);

			// Mathea
			RenderTextOnScreen(meshList[GEO_TITLE], "Mathea", glm::vec3(1, 0, 0), 15, 280, 360);
			RenderTextOnScreen(meshList[GEO_TITLE], "- Archery Minigame ", glm::vec3(1, 1, 1), 15, 280, 330);
			RenderTextOnScreen(meshList[GEO_TITLE], "- Balloon Pop Minigame ", glm::vec3(1, 1, 1), 15, 280, 300);
			RenderTextOnScreen(meshList[GEO_TITLE], "- Final Minigame ", glm::vec3(1, 1, 1), 15, 280, 270);
			RenderTextOnScreen(meshList[GEO_TITLE], "- Scene Manager &", glm::vec3(1, 1, 1), 15, 280, 240);
			RenderTextOnScreen(meshList[GEO_TITLE], "  Linking Scenes Together ", glm::vec3(1, 1, 1), 15, 280, 210);
			RenderTextOnScreen(meshList[GEO_TITLE], "- Game Audio", glm::vec3(1, 1, 1), 15, 280, 180);
			RenderTextOnScreen(meshList[GEO_TITLE], "  (source: Deepwoken OST)", glm::vec3(1, 1, 1), 15, 280, 150);

			// Scroll Down Text
			RenderTextOnScreen(meshList[GEO_TITLE], "Press 'S' to scroll down", glm::vec3(1, 1, 0), 15, 400, 70); 
		}

		if (isScrollDown) {
			// Yong Quan (cont.)
			RenderTextOnScreen(meshList[GEO_TITLE], "Yong Quan ", glm::vec3(1, 0, 0), 15, 280, 530);
			RenderTextOnScreen(meshList[GEO_TITLE], "- Main Menu", glm::vec3(1, 1, 1), 15, 280, 500);
			RenderTextOnScreen(meshList[GEO_TITLE], "- Carnival Scene", glm::vec3(1, 1, 1), 15, 280, 470);
			RenderTextOnScreen(meshList[GEO_TITLE], "- Game UI & Story", glm::vec3(1, 1, 1), 15, 280, 440);
			RenderTextOnScreen(meshList[GEO_TITLE], "- Spinning Ring Minigame ", glm::vec3(1, 1, 1), 15, 280, 410);

			// Wilson
			RenderTextOnScreen(meshList[GEO_TITLE], "Wilson ", glm::vec3(1, 0, 0), 15, 280, 360);
			RenderTextOnScreen(meshList[GEO_TITLE], "- Cutscene & Camera ", glm::vec3(1, 1, 1), 15, 280, 330);
			RenderTextOnScreen(meshList[GEO_TITLE], "- Hole in the Wall Minigame ", glm::vec3(1, 1, 1), 15, 280, 300);
			RenderTextOnScreen(meshList[GEO_TITLE], "- Scene Bumper Balls (Boss AI) ", glm::vec3(1, 1, 1), 15, 280, 270);
			RenderTextOnScreen(meshList[GEO_TITLE], "- Scene Final", glm::vec3(1, 1, 1), 15, 280, 240);
			RenderTextOnScreen(meshList[GEO_TITLE], "  (Player Controls and Boss AI) ", glm::vec3(1, 1, 1), 15, 280, 210);

			RenderTextOnScreen(meshList[GEO_TITLE], "And You!", glm::vec3(1, 1, 0), 20, 450, 140);

			RenderTextOnScreen(meshList[GEO_TITLE], "Press 'W' to scroll up", glm::vec3(1, 1, 0), 15, 430, 70);
		}
	}

	if (isControlsEntered) {
		RenderMeshOnScreen(meshList[GEO_UI], 120, 100, 20, 8);
		RenderTextOnScreen(meshList[GEO_TITLE], "BACK", glm::vec3(1, 1, 1), 40, 50, 80);
		RenderMeshOnScreen(meshList[GEO_UI], 120, 520, 20, 8);
		RenderTextOnScreen(meshList[GEO_TITLE], "CONTROLS", glm::vec3(1, 1, 0), 25, 25, 505);
		RenderMeshOnScreen(meshList[GEO_UI], 520, 310, 50, 50);

		RenderTextOnScreen(meshList[GEO_TITLE], "W,A,S,D - Walk", glm::vec3(1, 1, 1), 15, 280, 530);
		RenderTextOnScreen(meshList[GEO_TITLE], "(A,D - Tilts Camera Left/Right)", glm::vec3(1, 0, 0), 15, 300, 500);
		RenderTextOnScreen(meshList[GEO_TITLE], "(W,S - Backwards/Forward", glm::vec3(1, 0, 0), 15, 300, 470);
		RenderTextOnScreen(meshList[GEO_TITLE], "([in Scene Bumper Balls])", glm::vec3(1, 1, 0), 15, 320, 440);
		RenderTextOnScreen(meshList[GEO_TITLE], "SPACE - Jump", glm::vec3(1, 1, 1), 15, 280, 410);
		RenderTextOnScreen(meshList[GEO_TITLE], "SHIFT - Sprint", glm::vec3(1, 1, 1), 15, 280, 380);
		RenderTextOnScreen(meshList[GEO_TITLE], "[in Whack-A-Mole Minigame]", glm::vec3(1, 1, 0), 15, 300, 350);
		RenderTextOnScreen(meshList[GEO_TITLE], "E - Enter Tent/Skip Dialogue", glm::vec3(1, 1, 1), 15, 280, 320);
		RenderTextOnScreen(meshList[GEO_TITLE], "F - Interact", glm::vec3(1, 1, 1), 15, 280, 290);
		RenderTextOnScreen(meshList[GEO_TITLE], "Q - Skip Cutscene/Exit Minigame", glm::vec3(1, 1, 1), 15, 280, 260);
		RenderTextOnScreen(meshList[GEO_TITLE], "R - Restart Minigame", glm::vec3(1, 1, 1), 15, 280, 230);
		RenderTextOnScreen(meshList[GEO_TITLE], "C - Crouch", glm::vec3(1, 1, 1), 15, 280, 200); 
		RenderTextOnScreen(meshList[GEO_TITLE], "[in Hole in Wall Minigame]", glm::vec3(1, 1, 0), 15, 300, 170);
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void MainMenu::RenderMesh(Mesh* mesh, bool enableLight)
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

void MainMenu::Exit()
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

void MainMenu::HandleKeyPress()
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

	if (isCreditsEntered) {
		if (KeyboardController::GetInstance()->IsKeyPressed('S')) {
			isScrollUp = false;
			isScrollDown = true;
		}
		else if (KeyboardController::GetInstance()->IsKeyPressed('W')) {
			isScrollUp = true;
			isScrollDown = false;
		}
	}
}

void MainMenu::HandleMouseClick()
{
	double x = MouseController::GetInstance()->GetMousePositionX();
	double y = 1080 - MouseController::GetInstance()->GetMousePositionY();

	if (MouseController::GetInstance()->IsButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		if (isCreditsEntered || isControlsEntered) // BACK
		{
			if (x >= 41 && x <= 533 && y >= 129 && y <= 273)
			{
				isCreditsEntered = false;
				isControlsEntered = false;
			}
		}
		else
		{
			if (selectedOption == 0) // PLAY
			{
				shouldStartGame = true;
			}
			else if (selectedOption == 1) // CREDITS
			{
				isCreditsEntered = true;
			}
			else if (selectedOption == 2) // CONTROLS
			{
				isControlsEntered = true;
			}
			else if (selectedOption == 3) // EXIT
			{
				glfwSetWindowShouldClose(glfwGetCurrentContext(), GL_TRUE);
			}
		}
	}
}

void MainMenu::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey)
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

void MainMenu::RenderText(Mesh* mesh, std::string text, glm::vec3
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

void MainMenu::RenderTextOnScreen(Mesh* mesh, std::string
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

void MainMenu::Material(GEOMETRY_TYPE obj, float AmR, float AmG, float AmB, float DifA, float DifG, float DifB, float SpA, float SpG, float SpB, float Shiny)
{
	meshList[obj]->material.kAmbient = glm::vec3(AmR, AmG, AmB);
	meshList[obj]->material.kDiffuse = glm::vec3(DifA, DifG, DifB);
	meshList[obj]->material.kSpecular = glm::vec3(SpA, SpG, SpB);
	meshList[obj]->material.kShininess = Shiny;
}