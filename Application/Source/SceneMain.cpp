#pragma once
#include "SceneMain.h"
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
#include <iostream>

SceneMain::SceneMain()
{
	for (int i = 0; i < 6; i++) {
		tentCompleted[i] = false;
	}
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
	tempCompensation = 0;
	camera.enableFNAF = true;
	camera.allowMovement = false;
	camera.allowJump = false;
	camera.allowSprint = false;
	camera.allowCrouch = false;
	camera.allowProne = false;
	camera.allowLocomotiveTilt = false;
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
	camera.Init(glm::vec3(-50, 10, -90), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	// Init VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = nullptr;
	}

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("Axes", 10000.f, 10000.f, 10000.f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("Sun", glm::vec3(1.f, 1.f, 1.f), 1.f, 16, 16);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("Cube", glm::vec3(1, 1, 1), 1.f);
	meshList[GEO_PLANE] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 2.f);
	meshList[GEO_PLANE]->textureID = LoadTGA("Images//grass.tga");
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("STAMINA_BAR", glm::vec3(1, 1, 1), 1.f);

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Images//hills_lf.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Images//hills_rt.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Images//hills_up.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Images//hills_dn.tga");
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Images//hills_bk.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Images//hills_ft.tga");

	meshList[GEO_LEFT2] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_LEFT2]->textureID = LoadTGA("Images//sunset_lf.tga");
	meshList[GEO_RIGHT2] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_RIGHT2]->textureID = LoadTGA("Images//sunset_rt.tga");
	meshList[GEO_TOP2] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_TOP2]->textureID = LoadTGA("Images//sunset_up.tga");
	meshList[GEO_BOTTOM2] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_BOTTOM2]->textureID = LoadTGA("Images//sunset_dn.tga");
	meshList[GEO_FRONT2] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_FRONT2]->textureID = LoadTGA("Images//sunset_bk.tga");
	meshList[GEO_BACK2] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_BACK2]->textureID = LoadTGA("Images//sunset_ft.tga");

	meshList[GEO_LEFT3] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_LEFT3]->textureID = LoadTGA("Images//midnight-silence_lf.tga");
	meshList[GEO_RIGHT3] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_RIGHT3]->textureID = LoadTGA("Images//midnight-silence_rt.tga");
	meshList[GEO_TOP3] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_TOP3]->textureID = LoadTGA("Images//midnight-silence_up.tga");
	meshList[GEO_BOTTOM3] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_BOTTOM3]->textureID = LoadTGA("Images//midnight-silence_dn.tga");
	meshList[GEO_FRONT3] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_FRONT3]->textureID = LoadTGA("Images//midnight-silence_bk.tga");
	meshList[GEO_BACK3] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_BACK3]->textureID = LoadTGA("Images//midnight-silence_ft.tga");

	// 16 x 16 is the number of columns and rows for the text
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Images//calibri.tga");
	meshList[GEO_FPS] = MeshBuilder::GenerateText("fpstext", 16, 16);
	meshList[GEO_FPS]->textureID = LoadTGA("Images//bizudgothic.tga");

	meshList[GEO_KEY_E] = MeshBuilder::GenerateQuad("KeyE", glm::vec3(1.f, 1.f, 1.f), 2.f);
	meshList[GEO_KEY_E]->textureID = LoadTGA("Images//keyboard_key_e.tga");
	meshList[GEO_KEY_F] = MeshBuilder::GenerateQuad("KeyF", glm::vec3(1.f, 1.f, 1.f), 2.f);
	meshList[GEO_KEY_F]->textureID = LoadTGA("Images//keyboard_key_f.tga");
	meshList[GEO_KEY_Q] = MeshBuilder::GenerateQuad("KeyQ", glm::vec3(1.f, 1.f, 1.f), 2.f);
	meshList[GEO_KEY_Q]->textureID = LoadTGA("Images//keyboard_key_q.tga");
	meshList[GEO_UI] = MeshBuilder::GenerateQuad("UIBox", glm::vec3(0.12f, 0.12f, 0.12f), 10.f);

	meshList[GEO_TENT] = MeshBuilder::GenerateOBJ("Tent", "Models//circus_tent.obj");
	meshList[GEO_TENT]->textureID = LoadTGA("Images//circus_tent.tga");
	meshList[GEO_SIGN] = MeshBuilder::GenerateOBJ("Sign", "Models//sign.obj");
	meshList[GEO_SIGN]->textureID = LoadTGA("Images//sign.tga");
	meshList[GEO_TREE] = MeshBuilder::GenerateOBJMTL("Tree", "Models//tree.obj", "Models//tree.mtl");
	meshList[GEO_TREE]->textureID = LoadTGA("Images//tree.tga");
	meshList[GEO_FENCE] = MeshBuilder::GenerateOBJMTL("Fence", "Models//fence.obj", "Models//fence.mtl");
	meshList[GEO_FENCE]->textureID = LoadTGA("Images//fence.tga");
	meshList[GEO_HOUSE] = MeshBuilder::GenerateOBJMTL("House", "Models//cottage_obj.obj", "Models//cottage_obj.mtl");
	meshList[GEO_HOUSE]->textureID = LoadTGA("Images//cottage_diffuse.tga");
	meshList[GEO_ROAD] = MeshBuilder::GenerateOBJMTL("Road", "Models//road.obj", "Models//road.mtl");
	meshList[GEO_ROAD]->textureID = LoadTGA("Images//road.tga");
	meshList[GEO_MONEYBAG] = MeshBuilder::GenerateOBJMTL("MoneyBag", "Models//moneybag.obj", "Models//moneybag.mtl");
	meshList[GEO_MONEYBAG]->textureID = LoadTGA("Images//moneybag.tga");

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
	light[1].power = 0.f;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = 45.f;
	light[1].cosInner = 30.f;
	light[1].exponent = 3.f;
	light[1].spotDirection = glm::vec3(0.f, 1.f, 0.f);

	light[2].position = glm::vec3(-50, 4, 0);
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

	// collisions
	{
		// player & tent collisions
		{
			player.push_back(playerBox(1, GameObject::CUBE));
			tentList.push_back(tentBoxes(2, GameObject::CUBE));
			tentList.push_back(tentBoxes(3, GameObject::CUBE));
			tentList.push_back(tentBoxes(4, GameObject::CUBE));
			tentList.push_back(tentBoxes(5, GameObject::CUBE));
			tentList.push_back(tentBoxes(6, GameObject::CUBE));
			tentList.push_back(tentBoxes(7, GameObject::CUBE));
			finalTent.push_back(finalTentBox(8, GameObject::CUBE));

			player[0].pos = camera.pos;
			tentList[0].pos = glm::vec3{ 30, 3, -40 };
			tentList[1].pos = glm::vec3{ 30, 3, 0 };
			tentList[2].pos = glm::vec3{ 30, 3, 40 };
			tentList[3].pos = glm::vec3{ -30, 3, -40 };
			tentList[4].pos = glm::vec3{ -30, 3, 0 };
			tentList[5].pos = glm::vec3{ -30, 3, 40 };
			finalTent[0].pos = glm::vec3{ 0.f, 0.f, 70.f };

			for (int i = 0; i < tentList.size(); i++) {
				tentList[i].mass = 0.f;
			}

			finalTent[0].mass = 0.f;
		}

		// side fence collisions
		{
			sideFenceList.push_back(sideFenceBox(9, GameObject::CUBE));
			sideFenceList.push_back(sideFenceBox(10, GameObject::CUBE));
			sideFenceList.push_back(sideFenceBox(11, GameObject::CUBE));
			sideFenceList.push_back(sideFenceBox(12, GameObject::CUBE));
			sideFenceList.push_back(sideFenceBox(13, GameObject::CUBE));
			sideFenceList.push_back(sideFenceBox(14, GameObject::CUBE));
			sideFenceList.push_back(sideFenceBox(15, GameObject::CUBE));
			sideFenceList.push_back(sideFenceBox(16, GameObject::CUBE));


			sideFenceList[0].pos = glm::vec3{ 62.f, 0.f, -60.f };
			sideFenceList[1].pos = glm::vec3{ 62.f, 0.f, -20.f };
			sideFenceList[2].pos = glm::vec3{ 62.f, 0.f, 20.f };
			sideFenceList[3].pos = glm::vec3{ 62.f, 0.f, 60.f };
			sideFenceList[4].pos = glm::vec3{ -62.f, 0.f, -60.f };
			sideFenceList[5].pos = glm::vec3{ -62.f, 0.f, -20.f };
			sideFenceList[6].pos = glm::vec3{ -62.f, 0.f, 20.f };
			sideFenceList[7].pos = glm::vec3{ -62.f, 0.f, 60.f };

			for (int i = 0; i < sideFenceList.size(); i++) {
				sideFenceList[i].mass = 0.f;
			}
		}

		// front fence collisions
		{
			frontFenceList.push_back(frontFenceBox(17, GameObject::CUBE));
			frontFenceList.push_back(frontFenceBox(18, GameObject::CUBE));
			frontFenceList.push_back(frontFenceBox(19, GameObject::CUBE));
			frontFenceList.push_back(frontFenceBox(20, GameObject::CUBE));
			frontFenceList.push_back(frontFenceBox(21, GameObject::CUBE));

			frontFenceList[0].pos = glm::vec3{ 40.f, 0.f, 80.f };
			frontFenceList[1].pos = glm::vec3{ 0.f, 0.f, 80.f };
			frontFenceList[2].pos = glm::vec3{ -40.f, 0.f, 80.f };
			frontFenceList[3].pos = glm::vec3{ 40.f, 0.f, -80.f };
			frontFenceList[4].pos = glm::vec3{ -40.f, 0.f, -80.f };

			for (int i = 0; i < frontFenceList.size(); i++) {
				frontFenceList[i].mass = 0.f;
			}
		}

		// boundary collisions
		{
			frontBoundary.push_back(frontBoundaryBox(22, GameObject::CUBE));
			sideBoundary.push_back(sideBoundaryBox(23, GameObject::CUBE));
			sideBoundary.push_back(sideBoundaryBox(24, GameObject::CUBE));

			frontBoundary[0].pos = glm::vec3{ 0.f, 0.f, -100.f };
			sideBoundary[0].pos = glm::vec3{ 62.f, 0.f, -90.f }; 
			sideBoundary[1].pos = glm::vec3{ -62.f, 0.f, -90.f };

			for (int i = 0; i < sideBoundary.size(); i++) {
				sideBoundary[i].mass = 0.f;
			}

			frontBoundary[0].mass = 0.f;
		}
	}

	// Typewriting Dialogue
	isTyping = false; 
	typewriterTimer = 0.0f; 
	currentText = "";
	currentCharIndex = 0;
	isCutsceneDialogueActive = true;
	hasPlayedCutsceneDialogue = false;

	// Tent Position (for interaction use)
	tentPositions[0] = glm::vec3(30.f, 0.f, -40.f); 
	tentPositions[1] = glm::vec3(30.f, 0.f, 0.f);
	tentPositions[2] = glm::vec3(30.f, 0.f, 40.f);
	tentPositions[3] = glm::vec3(-30.f, 0.f, -40.f);
	tentPositions[4] = glm::vec3(-30.f, 0.f, 0.f);
	tentPositions[5] = glm::vec3(-30.f, 0.f, 40.f);

	for (int i = 0; i < 6; i++)
	{
		showEnterTentText[i] = false;
		tentCompleted[i] = false;
	}

	finalTentPosition = glm::vec3(0.f, 0.f, 70.f); 
	showEnterFinalTentText = false;
	isFinalChallengeCompleted = false;

	hasReadSign = false;
	showReadSignText = false;
	readSignTextTimer = 0.0f;

	signPosition = glm::vec3(30.f, 3.f, -70.f);
	showSignText = false;
	readSign = false;
	isSignDialogueActive = false;
	currentLineIndex = -1;
	dialogueTimer = 0;

	cutsceneStage = -1;
	cutsceneSkipped = false;
}

void SceneMain::Update(double dt)
{
	Application::SetPointerStatus(false);
	HandleKeyPress();

	if (KeyboardController::GetInstance()->IsKeyDown('I'))
		light[0].position.z -= static_cast<float>(dt) * 5.f;
	if (KeyboardController::GetInstance()->IsKeyDown('K'))
		light[0].position.z += static_cast<float>(dt) * 5.f;
	if (KeyboardController::GetInstance()->IsKeyDown('J'))
		light[0].position.x -= static_cast<float>(dt) * 5.f;
	if (KeyboardController::GetInstance()->IsKeyDown('L'))
		light[0].position.x += static_cast<float>(dt) * 5.f;
	if (KeyboardController::GetInstance()->IsKeyDown('O'))
		light[0].position.y -= static_cast<float>(dt) * 5.f;
	if (KeyboardController::GetInstance()->IsKeyDown('P'))
		light[0].position.y += static_cast<float>(dt) * 5.f;

	//light[0].spotDirection = -glm::normalize (camera.target - camera.pos);
	//light[0].position = camera.pos;

	switch (cutsceneStage)
	{
	case 0:
		camera.pos.x = -50;
		camera.pos.y = 10;
		camera.pos.z = -90;
		cutsceneStage = 1;
		break;
	case -1:
		camera.pos.x = -50;
		camera.pos.y = 10;
		camera.pos.z = -90;
		cutsceneStage = 0;
		break;
	case 1:
		camera.pos.x += abs(camera.pos.x + 10) / 2 * dt;
		camera.pos.z += 1 * dt;

		if (camera.pos.x > -10.005)
		{
			cutsceneStage = 2;
		}
		break;
	case 2:
		camera.pos.x += abs(72.15 + camera.pos.z) * dt;
		tempCompensation = abs(camera.pos.z + 70) / 2;
		camera.pos.z += tempCompensation * dt;

		if (camera.pos.x > -3)
		{
			cutsceneStage = 3;
		}
		break;
	case 3:
		//camera.pos.x += abs(camera.pos.x + 1) / 2 * dt;
		//camera.pos.z += tempCompensation * abs(camera.pos.x + 2.5) * dt;

		if (1)
		{
			cutsceneStage = 4;
			camera.enableFNAF = false;
			camera.allowMovement = true;
			camera.allowJump = true;
			camera.allowSprint = false;
			camera.allowCrouch = true;
			camera.allowProne = false;
			camera.allowLocomotiveTilt = true;
			camera.allowLocomotiveBop = false;
		}

		break;
	}

	// camera
	camera.Update(dt);
	player[0].pos = camera.pos;
	glm::vec3 viewDir = glm::normalize(camera.target - camera.pos);

	// Collision
	{
		CollisionData cd;
		for (int i = 0; i < tentList.size(); i++) {
			if (OverlapAABB2AABB(player[0], player[0].playerDimensions, tentList[i], tentList[i].tentDimensions, cd)) {
				ResolveCollision(cd);
				camera.pos = player[0].pos;
				camera.target = camera.pos + viewDir * 1.2f;
			}
		}

		if (CheckAllTentsCompleted()) {
			if (OverlapAABB2AABB(player[0], player[0].playerDimensions, finalTent[0], finalTent[0].tentDimensions, cd)) {
				ResolveCollision(cd);
				camera.pos = player[0].pos;
				camera.target = camera.pos + viewDir * 1.2f;
			}
		}

		for (int i = 0; i < sideFenceList.size(); i++) {
			if (OverlapAABB2AABB(player[0], player[0].playerDimensions, sideFenceList[i], sideFenceList[i].fenceDimensions, cd)) {
				ResolveCollision(cd);
				camera.pos = player[0].pos;
				camera.target = camera.pos + viewDir * 1.2f;
			}
		}

		for (int i = 0; i < frontFenceList.size(); i++) {
			if (OverlapAABB2AABB(player[0], player[0].playerDimensions, frontFenceList[i], frontFenceList[i].fenceDimensions, cd)) {
				ResolveCollision(cd);
				camera.pos = player[0].pos;
				camera.target = camera.pos + viewDir * 1.2f;
			}
		}

		for (int i = 0; i < sideBoundary.size(); i++) {
			if (OverlapAABB2AABB(player[0], player[0].playerDimensions, sideBoundary[i], sideBoundary[i].boundaryDimensions, cd)) {
				ResolveCollision(cd);
				camera.pos = player[0].pos;
				camera.target = camera.pos + viewDir * 1.2f;
			}
		}

        if (OverlapAABB2AABB(player[0], player[0].playerDimensions, frontBoundary[0], frontBoundary[0].boundaryDimensions, cd)) {
			ResolveCollision(cd);
			camera.pos = player[0].pos;
			camera.target = camera.pos + viewDir * 1.2f;
		}

		for (int i = 0; i < tentList.size(); i++) {
			tentList[i].UpdatePhysics(dt);
		}

		for (int i = 0; i < sideFenceList.size(); i++) {
			sideFenceList[i].UpdatePhysics(dt);
		}

		for (int i = 0; i < frontFenceList.size(); i++) {
			frontFenceList[i].UpdatePhysics(dt);
		}

		for (int i = 0; i < sideBoundary.size(); i++) {
			sideBoundary[i].UpdatePhysics(dt);
		}

		player[0].UpdatePhysics(dt);
		finalTent[0].UpdatePhysics(dt);
		frontBoundary[0].UpdatePhysics(dt);
	}

	// Interaction between Sign and Tents
	{
		float distance = glm::distance(camera.pos, signPosition);
		if (distance < 12.0f)
		{
			showSignText = true;
		}
		else
		{
			showSignText = false;
		}

		for (int i = 0; i < 6; i++)
		{
			float distanceToTent = glm::distance(camera.pos, tentPositions[i]);
			if (distanceToTent < 22.0f)
			{
				showEnterTentText[i] = true;
			}
			else
			{
				showEnterTentText[i] = false;
			}
		}

		bool allTentsCompleted = true;
		for (int i = 0; i < 6; ++i)
		{
			if (!tentCompleted[i])
			{
				allTentsCompleted = false;
				break;
			}
		}

		if (allTentsCompleted)
		{
			float distanceToFinalTent = glm::distance(camera.pos, finalTentPosition);
			if (distanceToFinalTent < 25.0f)
			{
				showEnterFinalTentText = true;
			}
			else
			{
				showEnterFinalTentText = false;
			}
		}

		if (showReadSignText)
		{
			readSignTextTimer += dt;
			if (readSignTextTimer >= 3.0f)
			{
				showReadSignText = false;
				readSignTextTimer = 0.0f;
			}
		}
	}

	// FPS
	float temp = 1.f / dt;
	fps = glm::round(temp * 100.f) / 100.f;

	UpdateDialogue(dt);

	// Add this inside HandleKeyPress() method
if (KeyboardController::GetInstance()->IsKeyPressed('1')) {
	// Force enter Archery scene
	shouldEnterArchery = true;
}

if (KeyboardController::GetInstance()->IsKeyPressed('2')) {
	// Force enter BalloonPop scene
	shouldEnterBalloonPop = true;
}

if (KeyboardController::GetInstance()->IsKeyPressed('3')) {
	// Force enter Hole scene
	shouldEnterHole = true;
}

if (KeyboardController::GetInstance()->IsKeyPressed('4')) {
	// Force enter WhackAMole scene
	shouldEnterWhackAMole = true;
}

if (KeyboardController::GetInstance()->IsKeyPressed('5')) {
	// Force enter Spinning Ring scene
	shouldEnterSpinningRing = true;
}

if (KeyboardController::GetInstance()->IsKeyPressed('6')) {
	// Force enter WIU Test scene
	shouldEnterWIUTest = true;
}

if (KeyboardController::GetInstance()->IsKeyPressed('7')) {
	// Force enter Final scene (after completing all games)
	for (int i = 0; i < 6; i++) {
		tentCompleted[i] = true;
	}
	shouldEnterFinal = true;
}
}

void SceneMain::Render()
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

	modelStack.PushMatrix();
	modelStack.Scale(150.f, 1.f, 200.f);
	modelStack.Rotate(-90.f, 1, 0, 0);
	meshList[GEO_PLANE]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_PLANE]->material.kDiffuse = glm::vec3(0.5f,0.5f, 0.5f);
	meshList[GEO_PLANE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_PLANE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_PLANE], true);
	modelStack.PopMatrix();

	// Render Tents
	{
		meshList[GEO_TENT]->material.kAmbient = glm::vec3(0.5f, 0.5f, 0.5f);
		meshList[GEO_TENT]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		meshList[GEO_TENT]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
		meshList[GEO_TENT]->material.kShininess = 1.0f;
		{
			modelStack.PushMatrix();
			modelStack.Translate(30.f, 0.f, -40.f);
			modelStack.Rotate(180, 0, 1, 0);
			modelStack.Scale(0.03f, 0.03f, 0.03f);
			RenderMesh(meshList[GEO_TENT], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(30.f, 0.f, 0.f);
			modelStack.Rotate(180, 0, 1, 0);
			modelStack.Scale(0.03f, 0.03f, 0.03f);
			RenderMesh(meshList[GEO_TENT], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(30.f, 0.f, 40.f);
			modelStack.Rotate(180, 0, 1, 0);
			modelStack.Scale(0.03f, 0.03f, 0.03f);
			RenderMesh(meshList[GEO_TENT], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-30.f, 0.f, -40.f);
			modelStack.Scale(0.03f, 0.03f, 0.03f);
			RenderMesh(meshList[GEO_TENT], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-30.f, 0.f, 0.f);
			modelStack.Scale(0.03f, 0.03f, 0.03f);
			RenderMesh(meshList[GEO_TENT], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-30.f, 0.f, 40.f);
			modelStack.Scale(0.03f, 0.03f, 0.03f);
			RenderMesh(meshList[GEO_TENT], true);
			modelStack.PopMatrix();

			bool allTentsCompleted = true;
			for (int i = 0; i < 6; i++)
			{
				if (!tentCompleted[i])
				{
					allTentsCompleted = false;
					break;
				}
			}

			if (CheckAllTentsCompleted())
			{
				modelStack.PushMatrix();
				modelStack.Translate(0.f, 0.f, 70.f);
				modelStack.Rotate(90, 0, 1, 0);
				modelStack.Scale(0.05f, 0.05f, 0.05f);
				RenderMesh(meshList[GEO_TENT], true);
				modelStack.PopMatrix();

				//modelStack.PushMatrix();
				//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				//modelStack.Translate(finalTent[0].pos.x, finalTent[0].pos.y, finalTent[0].pos.z);
				//modelStack.Rotate(finalTent[0].angleDeg, 0, 1, 0);
				//modelStack.Scale(2 * finalTent[0].tentDimensions.x, 2 * finalTent[0].tentDimensions.y, 2 * finalTent[0].tentDimensions.z);
				//meshList[GEO_CUBE]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
				//meshList[GEO_CUBE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
				//meshList[GEO_CUBE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
				//meshList[GEO_CUBE]->material.kShininess = 1.0f;
				//RenderMesh(meshList[GEO_CUBE], true);
				//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				//modelStack.PopMatrix();
			}
		}
	}

	// Render Sign
	modelStack.PushMatrix();
	modelStack.Translate(30.f, 3.f, -70.f);
	modelStack.Rotate(225.f, 0, 1, 0);
	modelStack.Scale(3.f, 3.f, 3.f);
	meshList[GEO_SIGN]->material.kAmbient = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_SIGN]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_SIGN]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_SIGN]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_SIGN], true);
	modelStack.PopMatrix();

	// Render Fences
	{
		meshList[GEO_FENCE]->material.kAmbient = glm::vec3(0.5f, 0.5f, 0.5f);
		meshList[GEO_FENCE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		meshList[GEO_FENCE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
		meshList[GEO_FENCE]->material.kShininess = 1.0f;

		// Left side fences (4 segments)
		{
			modelStack.PushMatrix();
			modelStack.Translate(62.f, 0.f, -80.f);
			modelStack.Rotate(-11, 0, 1, 0);
			modelStack.Scale(5.f, 5.f, 10.f);
			RenderMesh(meshList[GEO_FENCE], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(62.f, 0.f, -40.f);
			modelStack.Rotate(-11, 0, 1, 0);
			modelStack.Scale(5.f, 5.f, 10.f);
			RenderMesh(meshList[GEO_FENCE], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(62.f, 0.f, 0.f);
			modelStack.Rotate(-11, 0, 1, 0);
			modelStack.Scale(5.f, 5.f, 10.f);
			RenderMesh(meshList[GEO_FENCE], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(62.f, 0.f, 40.f);
			modelStack.Rotate(-11, 0, 1, 0);
			modelStack.Scale(5.f, 5.f, 10.f);
			RenderMesh(meshList[GEO_FENCE], true);
			modelStack.PopMatrix();
		}

		// Right side fences (4 segments)
		{
			modelStack.PushMatrix();
			modelStack.Translate(-62.f, 0.f, -80.f);
			modelStack.Rotate(-11, 0, 1, 0);
			modelStack.Scale(5.f, 5.f, 10.f);
			RenderMesh(meshList[GEO_FENCE], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-62.f, 0.f, -40.f);
			modelStack.Rotate(-11, 0, 1, 0);
			modelStack.Scale(5.f, 5.f, 10.f);
			RenderMesh(meshList[GEO_FENCE], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-62.f, 0.f, 0.f);
			modelStack.Rotate(-11, 0, 1, 0);
			modelStack.Scale(5.f, 5.f, 10.f);
			RenderMesh(meshList[GEO_FENCE], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-62.f, 0.f, 40.f);
			modelStack.Rotate(-11, 0, 1, 0);
			modelStack.Scale(5.f, 5.f, 10.f);
			RenderMesh(meshList[GEO_FENCE], true);
			modelStack.PopMatrix();
		}

		// Back Fence
		{
			modelStack.PushMatrix();
			modelStack.Translate(62.f, 0.f, 80.f);
			modelStack.Rotate(-101, 0, 1, 0);
			modelStack.Scale(5.f, 5.f, 10.f);
			RenderMesh(meshList[GEO_FENCE], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(20.f, 0.f, 80.f);
			modelStack.Rotate(-101, 0, 1, 0);
			modelStack.Scale(5.f, 5.f, 10.f);
			RenderMesh(meshList[GEO_FENCE], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-22.f, 0.f, 80.f);
			modelStack.Rotate(-101, 0, 1, 0);
			modelStack.Scale(5.f, 5.f, 10.f);
			RenderMesh(meshList[GEO_FENCE], true);
			modelStack.PopMatrix();
		}

		// Entrance left piece
		modelStack.PushMatrix();
		modelStack.Translate(-20.5f, 0.f, -80.f);
		modelStack.Rotate(-101, 0, 1, 0);
		modelStack.Scale(5.f, 5.f, 10.f);          
		RenderMesh(meshList[GEO_FENCE], true);
		modelStack.PopMatrix();

		// Entrance right piece
		modelStack.PushMatrix();
		modelStack.Translate(60.5f, 0.f, -80.f);
		modelStack.Rotate(-101, 0, 1, 0);
		modelStack.Scale(5.f, 5.f, 10.f);          
		RenderMesh(meshList[GEO_FENCE], true);
		modelStack.PopMatrix();
	}

	// Render Houses
	{
		meshList[GEO_HOUSE]->material.kAmbient = glm::vec3(0.5f, 0.5f, 0.5f);
		meshList[GEO_HOUSE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		meshList[GEO_HOUSE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
		meshList[GEO_HOUSE]->material.kShininess = 1.0f;
		{
			modelStack.PushMatrix();
			modelStack.Translate(30.f, 0.f, -165.f);
			modelStack.Rotate(-5.f, 0, 1, 0);
			modelStack.Scale(2.f, 2.f, 2.f);
			RenderMesh(meshList[GEO_HOUSE], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(100.f, 0.f, -165.f);
			modelStack.Rotate(-5.f, 0, 1, 0);
			modelStack.Scale(2.f, 2.f, 2.f);
			RenderMesh(meshList[GEO_HOUSE], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-40.f, 0.f, -165.f);
			modelStack.Rotate(-5.f, 0, 1, 0);
			modelStack.Scale(2.f, 2.f, 2.f);
			RenderMesh(meshList[GEO_HOUSE], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-110.f, 0.f, -165.f);
			modelStack.Rotate(-5.f, 0, 1, 0);
			modelStack.Scale(2.f, 2.f, 2.f);
			RenderMesh(meshList[GEO_HOUSE], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(100.f, 0.f, -65.f);
			modelStack.Rotate(-185.f, 0, 1, 0);
			modelStack.Scale(2.f, 2.f, 2.f);
			RenderMesh(meshList[GEO_HOUSE], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-110.f, 0.f, -65.f);
			modelStack.Rotate(-185.f, 0, 1, 0);
			modelStack.Scale(2.f, 2.f, 2.f);
			RenderMesh(meshList[GEO_HOUSE], true);
			modelStack.PopMatrix();
		}
	}

	// Render Road
	{
		meshList[GEO_ROAD]->material.kAmbient = glm::vec3(0.5f, 0.5f, 0.5f);
		meshList[GEO_ROAD]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		meshList[GEO_ROAD]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
		meshList[GEO_ROAD]->material.kShininess = 1.0f;

		modelStack.PushMatrix();
		modelStack.Translate(140.f, 1.f, -100.f);
		modelStack.Rotate(41.5f, 0, 1, 0);
		modelStack.Scale(4.f, 3.f, 4.f);
		RenderMesh(meshList[GEO_ROAD], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(50.f, 1.f, -100.f);
		modelStack.Rotate(41.5f, 0, 1, 0);
		modelStack.Scale(4.f, 3.f, 4.f);
		RenderMesh(meshList[GEO_ROAD], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-40.f, 1.f, -100.f);
		modelStack.Rotate(41.5f, 0, 1, 0);
		modelStack.Scale(4.f, 3.f, 4.f);
		RenderMesh(meshList[GEO_ROAD], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-130.f, 1.f, -100.f);
		modelStack.Rotate(41.5f, 0, 1, 0);
		modelStack.Scale(4.f, 3.f, 4.f);
		RenderMesh(meshList[GEO_ROAD], true);
		modelStack.PopMatrix();
	}

	// Render Money Bag (if player completes final challenge)
	if (isFinalChallengeCompleted) {
		modelStack.PushMatrix();
		modelStack.Translate(0.f, 0.f, -60.f);
		modelStack.Scale(30.f, 50.f, 30.f);
		meshList[GEO_MONEYBAG]->material.kAmbient = glm::vec3(0.5f, 0.5f, 0.5f);
		meshList[GEO_MONEYBAG]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		meshList[GEO_MONEYBAG]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
		meshList[GEO_MONEYBAG]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_MONEYBAG], true);
		modelStack.PopMatrix();
	}

	RenderUI();
	RenderObjectives();
	RenderDialogue();

	std::string temp("FPS:" + std::to_string(fps));
	RenderTextOnScreen(meshList[GEO_FPS], temp.substr(0, 9), glm::vec3(0, 1, 0), 20, 620, 20);

	//for (int n = 0; n < tentList.size(); n++) {
	//	modelStack.PushMatrix();
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//	modelStack.Translate(tentList[n].pos.x, tentList[n].pos.y, tentList[n].pos.z);
	//	modelStack.Rotate(tentList[n].angleDeg, 0, 1, 0);
	//  modelStack.Scale(2 * tentList[n].tentDimensions.x, 2 * tentList[n].tentDimensions.y, 2 * tentList[n].tentDimensions.z);
	//	meshList[GEO_CUBE]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	//	meshList[GEO_CUBE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	//	meshList[GEO_CUBE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	//	meshList[GEO_CUBE]->material.kShininess = 1.0f;
	//	RenderMesh(meshList[GEO_CUBE], true);
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//	modelStack.PopMatrix();
	//}

	//for (int n = 0; n < sideFenceList.size(); n++) {
	//	modelStack.PushMatrix();
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//	modelStack.Translate(sideFenceList[n].pos.x, sideFenceList[n].pos.y, sideFenceList[n].pos.z);
	//	modelStack.Rotate(sideFenceList[n].angleDeg, 0, 1, 0);
	//	modelStack.Scale(2 * sideFenceList[n].fenceDimensions.x, 2 * sideFenceList[n].fenceDimensions.y, 2 * sideFenceList[n].fenceDimensions.z);
	//	meshList[GEO_CUBE]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	//	meshList[GEO_CUBE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	//	meshList[GEO_CUBE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	//	meshList[GEO_CUBE]->material.kShininess = 1.0f;
	//	RenderMesh(meshList[GEO_CUBE], true);
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//	modelStack.PopMatrix();
	//}
	//for (int n = 0; n < frontFenceList.size(); n++) {
	//	modelStack.PushMatrix();
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//	modelStack.Translate(frontFenceList[n].pos.x, frontFenceList[n].pos.y, frontFenceList[n].pos.z);
	//	modelStack.Rotate(frontFenceList[n].angleDeg, 0, 1, 0);
	//	modelStack.Scale(2 * frontFenceList[n].fenceDimensions.x, 2 * frontFenceList[n].fenceDimensions.y, 2 * frontFenceList[n].fenceDimensions.z);
	//	meshList[GEO_CUBE]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	//	meshList[GEO_CUBE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	//	meshList[GEO_CUBE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	//	meshList[GEO_CUBE]->material.kShininess = 1.0f;
	//	RenderMesh(meshList[GEO_CUBE], true);
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//	modelStack.PopMatrix();
	//}
	//for (int n = 0; n < sideBoundary.size(); n++) {
	//	modelStack.PushMatrix();
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//	modelStack.Translate(sideBoundary[n].pos.x, sideBoundary[n].pos.y, sideBoundary[n].pos.z);
	//	modelStack.Rotate(sideBoundary[n].angleDeg, 0, 1, 0);
	//	modelStack.Scale(2 * sideBoundary[n].boundaryDimensions.x, 2 * sideBoundary[n].boundaryDimensions.y, 2 * sideBoundary[n].boundaryDimensions.z);
	//	meshList[GEO_CUBE]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	//	meshList[GEO_CUBE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	//	meshList[GEO_CUBE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	//	meshList[GEO_CUBE]->material.kShininess = 1.0f;
	//	RenderMesh(meshList[GEO_CUBE], true);
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//	modelStack.PopMatrix();
	//}

	//modelStack.PushMatrix();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//modelStack.Translate(frontBoundary[0].pos.x, frontBoundary[0].pos.y, frontBoundary[0].pos.z);
	//modelStack.Rotate(frontBoundary[0].angleDeg, 0, 1, 0);
	//modelStack.Scale(2 * frontBoundary[0].boundaryDimensions.x, 2 * frontBoundary[0].boundaryDimensions.y, 2 * frontBoundary[0].boundaryDimensions.z);
	//meshList[GEO_CUBE]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	//meshList[GEO_CUBE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	//meshList[GEO_CUBE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	//meshList[GEO_CUBE]->material.kShininess = 1.0f;
	//RenderMesh(meshList[GEO_CUBE], true);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//modelStack.PopMatrix();
}

// boolean to check if player complete all 6 minigames
bool SceneMain::CheckAllTentsCompleted()
{
	for (int i = 0; i < 6; i++) {
		if (!tentCompleted[i]) return false;
	}
	return true;
}

void SceneMain::RenderUI()
{
	if (showSignText)
	{
		RenderMeshOnScreen(meshList[GEO_KEY_F], 40, 120, 20, 20);
		RenderTextOnScreen(meshList[GEO_TEXT], "Interact", glm::vec3(1, 1, 1), 30, 80, 105);
	}

	for (int i = 0; i < 6; i++)
	{
		if (showEnterTentText[i])
		{
			RenderMeshOnScreen(meshList[GEO_KEY_E], 40, 120, 20, 20);
			RenderTextOnScreen(meshList[GEO_TEXT], "Enter", glm::vec3(1, 1, 1), 30, 80, 105);
		}
	}

	if (showEnterFinalTentText)
	{
		RenderMeshOnScreen(meshList[GEO_KEY_E], 40, 120, 20, 20);
		RenderTextOnScreen(meshList[GEO_TEXT], "Enter Final Tent", glm::vec3(1, 1, 1), 30, 80, 105);
	}

	if (showReadSignText)
	{
		RenderMeshOnScreen(meshList[GEO_UI], 150, 550, 150, 6);
		RenderTextOnScreen(meshList[GEO_TEXT], "How about we read the sign first?", glm::vec3(1, 1, 1), 20, 10, 550);
	}
}

void SceneMain::RenderDialogue() {
	if (isCutsceneDialogueActive) {
		RenderMeshOnScreen(meshList[GEO_UI], 150, 535, 150, 9);
		RenderMeshOnScreen(meshList[GEO_KEY_Q], 20, 510, 10, 10);
		RenderTextOnScreen(meshList[GEO_TEXT], "[ SKIP CUTSCENE ]", glm::vec3(1, 1, 1), 15, 40, 505);
	}
	else if (isSignDialogueActive) {
		RenderMeshOnScreen(meshList[GEO_UI], 150, 535, 150, 9);
		RenderMeshOnScreen(meshList[GEO_KEY_Q], 20, 510, 10, 10);
		RenderTextOnScreen(meshList[GEO_TEXT], "[ SKIP ]", glm::vec3(1, 1, 1), 15, 40, 505);
		RenderMeshOnScreen(meshList[GEO_KEY_E], 170, 510, 10, 10);
		RenderTextOnScreen(meshList[GEO_TEXT], "[ NEXT DIALOGUE ]", glm::vec3(1, 1, 1), 15, 200, 505);
	}
	else {
		return; // exit function early if no dialogue is active
	}

	// rendering of dialogue when player read the sign
	RenderMeshOnScreen(meshList[GEO_UI], 150, 550, 150, 6);
	if (isSignDialogueActive && currentLineIndex < signDialogueLines.size()) {
		const DialogueLine& currentDialogue = signDialogueLines[currentLineIndex];

		if (currentDialogue.isMultiLine) {
			std::string textToRender = currentText.substr(0, currentCharIndex);
			size_t newlinePos = textToRender.find('\n');
			if (newlinePos != std::string::npos) {
				std::string firstLine = textToRender.substr(0, newlinePos);
				std::string secondLine = textToRender.substr(newlinePos + 1);
				RenderTextOnScreen(meshList[GEO_TEXT], firstLine, glm::vec3(1, 1, 1), 20, 10, 550);
				RenderTextOnScreen(meshList[GEO_TEXT], secondLine, glm::vec3(1, 1, 1), 20, 10, 530);
			}
			else {
				RenderTextOnScreen(meshList[GEO_TEXT], textToRender, glm::vec3(1, 1, 1), 20, 10, 550);
			}
		}
		else {
			std::string textToRender = currentText.substr(0, currentCharIndex);
			RenderTextOnScreen(meshList[GEO_TEXT], textToRender, glm::vec3(1, 1, 1), 20, 10, 550);
		}
	}

	// rendering of dialogue when player first enter the game
	if (isCutsceneDialogueActive && currentLineIndex < sceneMainCutsceneLines.size()) {
		const DialogueLine& currentDialogue = sceneMainCutsceneLines[currentLineIndex];

		if (currentDialogue.isMultiLine) {
			std::string textToRender = currentText.substr(0, currentCharIndex);
			size_t newlinePos = textToRender.find('\n');
			if (newlinePos != std::string::npos) {
				std::string firstLine = textToRender.substr(0, newlinePos);
				std::string secondLine = textToRender.substr(newlinePos + 1);
				RenderTextOnScreen(meshList[GEO_TEXT], firstLine, glm::vec3(1, 1, 1), 20, 10, 550);
				RenderTextOnScreen(meshList[GEO_TEXT], secondLine, glm::vec3(1, 1, 1), 20, 10, 530);
			}
			else {
				RenderTextOnScreen(meshList[GEO_TEXT], textToRender, glm::vec3(1, 1, 1), 20, 10, 550);
			}
		}
		else {
			std::string textToRender = currentText.substr(0, currentCharIndex);
			RenderTextOnScreen(meshList[GEO_TEXT], textToRender, glm::vec3(1, 1, 1), 20, 10, 550);
		}
	}
}

// typing speed for the dialogues
void SceneMain::UpdateDialogue(double dt) {
	if (readSign && !isSignDialogueActive) {
		UpdateSignText();

		isSignDialogueActive = true;
		currentLineIndex = 0;
		dialogueTimer = 0;
		isTyping = true; // Start typing the first line
		typewriterTimer = 0.0f;
		const DialogueLine& currentDialogue = signDialogueLines[currentLineIndex];
		if (currentDialogue.isMultiLine) {
			// Store both lines
			currentText = currentDialogue.textLines[0] + "\n" + currentDialogue.textLines[1];
		}
		else {
			currentText = currentDialogue.textLines[0];
		}
		currentCharIndex = 0;
	}

	if (isSignDialogueActive) {
		if (KeyboardController::GetInstance()->IsKeyPressed('E')) {
			if (isTyping) {
				// Skip rendering the current text
				currentCharIndex = currentText.length();
				isTyping = false;
			}
			else {
				// Skip to the next line
				dialogueTimer = 4.0f;
			}
		}

		if (isTyping) {
			typewriterTimer += dt;
			if (typewriterTimer >= 0.05f) { // Adjust the typing speed here
				typewriterTimer = 0.0f;
				currentCharIndex++;
				if (currentCharIndex >= currentText.length()) {
					isTyping = false;
				}
			}
		}
		else {
			dialogueTimer += dt;
			if (dialogueTimer >= 4.0f) {
				dialogueTimer = 0;
				currentLineIndex++;
				if (currentLineIndex >= signDialogueLines.size()) {
					isSignDialogueActive = false;
					readSign = false;
					camera.enableFNAF = false;
					camera.allowMovement = true;
					camera.allowJump = true;
					camera.allowLocomotiveTilt = true;
				}
				else {
					isTyping = true;
					typewriterTimer = 0.0f;
					const DialogueLine& currentDialogue = signDialogueLines[currentLineIndex];
					if (currentDialogue.isMultiLine) {
						currentText = currentDialogue.textLines[0] + "\n" + currentDialogue.textLines[1];
					}
					else {
						currentText = currentDialogue.textLines[0];
					}
					currentCharIndex = 0;
				}
			}
		}
	}

	if (isCutsceneDialogueActive && !hasPlayedCutsceneDialogue) {
		if (isTyping) {
			typewriterTimer += dt;
			if (typewriterTimer >= 0.05f) { // Adjust the typing speed here
				typewriterTimer = 0.0f;
				currentCharIndex++;
				if (currentCharIndex >= currentText.length()) {
					isTyping = false;
				}
			}
		}
		else {
			dialogueTimer += dt;
			if (dialogueTimer >= 4.0f) {
				dialogueTimer = 0;
				currentLineIndex++;
				if (currentLineIndex >= sceneMainCutsceneLines.size()) {
					isCutsceneDialogueActive = false;
					hasPlayedCutsceneDialogue = true;
				}
				else {
					isTyping = true;
					typewriterTimer = 0.0f;
					const DialogueLine& currentDialogue = sceneMainCutsceneLines[currentLineIndex];
					if (currentDialogue.isMultiLine) {
						currentText = currentDialogue.textLines[0] + "\n" + currentDialogue.textLines[1];
					}
					else {
						currentText = currentDialogue.textLines[0];
					}
					currentCharIndex = 0;
				}
			}
		}
	}
}

void SceneMain::UpdateSignText() {
	// clear previous dialogue
	signDialogueLines.clear();

	int completedTents = 0;
	for (int i = 0; i < 6; ++i) {
		if (tentCompleted[i]) {
			completedTents++;
		}
	}

	if (completedTents == 0) {
		// if no minigames are completed
		signDialogueLines = {
			{{"Let me see..."}, false},
			{{"If you complete all six games..."}, false},
			{{"And the final challenge..."}, false},
			{{"You get to bring home", "a grand prize of..."}, true},
			{{"A million dollars?!"}, false},
			{{"Sounds too good to be true..."}, false},
			{{"Whatever, since I'm already here,", "why not do it anyway?"}, true},
			{{"Looks like I have to complete all six", "games in their respective tents first."}, true}
		};
	}
	else if (completedTents < 6) {
		// dialogue changes based on how many minigames player completed
		signDialogueLines = {
			{{"I've completed " + std::to_string(completedTents) + " out of 6 games."}, false},
			{{"Still a long way to go."}, false},
			{{"Better keep going."}, false}
		};
	}
	else if (completedTents == 6 && !isFinalChallengeCompleted) {
		// all minigames complete and final challenge not completed
		signDialogueLines = {
			{{"Alright! I finished all six games!"}, false},
			{{"Now it's time for the final challenge..."}, false},
			{{"I wonder what kind of challenge awaits."}, false}
		};
	}
	else if (completedTents == 6 && isFinalChallengeCompleted) {
		// player finished all the game
		signDialogueLines = {
			{{"I can't believe it..."}, false},
			{{"I've actually done it."}, false},
			{{"All six games, and the final challenge."}, false},
			{{"What a journey!"}, false}
		};
	}
}

// render objectives
void SceneMain::RenderObjectives() {
	modelStack.PushMatrix();
	modelStack.Translate(30.f, 8.f, -75.f);
	modelStack.Rotate(270.f, 0, 1, 0);
	modelStack.Scale(0.5f, 0.5f, 0.5f);
	
	int completedGames = 0;
	for (int i = 0; i < 6; i++) {
		if (tentCompleted[i]) {
			completedGames++;
		}
	}

	int visibleObjectives = 1; 
	if (hasReadSign) {
		visibleObjectives++;
		if (completedGames == 6) {
			visibleObjectives++;
		}
	}

	constexpr float LINE_SPACING = 1.5f;
	float textOffset = (visibleObjectives - 1) * LINE_SPACING;

	modelStack.PushMatrix();
	modelStack.Translate(0, textOffset, 0);
	RenderText(meshList[GEO_TEXT], "OBJECTIVES :", glm::vec3(1, 1, 0));
	modelStack.PopMatrix();

	textOffset -= LINE_SPACING;
	modelStack.PushMatrix();
	modelStack.Translate(0, textOffset, 0);
	std::string signText;
	glm::vec3 signColor;
	if (hasReadSign) {
		signText = "Read the Sign [COMPLETED]";
		signColor = glm::vec3(0, 1, 0);
	}
	else {
		signText = "Read the Sign [INCOMPLETE]";
		signColor = glm::vec3(1, 1, 1);
	}
	RenderText(meshList[GEO_TEXT], signText, signColor);
	modelStack.PopMatrix();

	if (hasReadSign) {
		textOffset -= 1.5f;

		modelStack.PushMatrix();
		modelStack.Translate(0, textOffset, 0);
		std::string progressText = "Games Complete: " + std::to_string(completedGames) + "/6";
		glm::vec3 progressColour;
		if (completedGames == 6) {
			progressColour = glm::vec3(0, 1, 0);
		}
		else {
			progressColour = glm::vec3(1, 1, 1);
		}
		RenderText(meshList[GEO_TEXT], progressText, progressColour);
		modelStack.PopMatrix();

		if (completedGames == 6) {
			textOffset -= 1.5f;
			modelStack.PushMatrix();
			modelStack.Translate(0, textOffset, 0);
			std::string finalText;
			glm::vec3 color;
			if (isFinalChallengeCompleted) {
				finalText = "Complete Final Challenge [COMPLETED]";
				color = glm::vec3(0, 1, 0);
			}
			else {
				finalText = "Complete Final Challenge [INCOMPLETE]";
				color = glm::vec3(1, 1, 1);
			}
			RenderText(meshList[GEO_TEXT], finalText, color);
			modelStack.PopMatrix();
		}
	}

	modelStack.PopMatrix();
}

void SceneMain::RenderMesh(Mesh* mesh, bool enableLight)
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


void SceneMain::Exit()
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

void SceneMain::HandleKeyPress()
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
	if (KeyboardController::GetInstance()->IsKeyPressed('Q') && !cutsceneSkipped) {
		// Skip cutscene

		if (cutsceneStage != 4)
		{
			camera.pos = glm::vec3(-3, 10, -70);
			camera.target = glm::vec3(0, 10, -70);
		}

		cutsceneStage = 4;
		camera.enableFNAF = false;
		camera.allowMovement = true;
		camera.allowJump = true;
		camera.allowSprint = false;
		camera.allowCrouch = true;
		camera.allowProne = false;
		camera.allowLocomotiveTilt = true;
		camera.allowLocomotiveBop = false;

		isCutsceneDialogueActive = false;
		hasPlayedCutsceneDialogue = true;

		cutsceneSkipped = true; // Set the flag to indicate the cutscene has been skipped
	}
	if (KeyboardController::GetInstance()->IsKeyPressed('Q') && isSignDialogueActive) {
		// Skip the entire sign dialogue
		isSignDialogueActive = false;
		readSign = false;
		camera.enableFNAF = false;
		camera.allowMovement = true;
		camera.allowJump = true;
		camera.allowSprint = false;
		camera.allowCrouch = true;
		camera.allowProne = false;
		camera.allowLocomotiveTilt = true;
		camera.allowLocomotiveBop = false;
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

	if (KeyboardController::GetInstance()->IsKeyPressed('F') && showSignText)
	{
		camera.allowMovement = false;
		camera.enableFNAF = true;
		camera.allowJump = false;
		camera.allowLocomotiveTilt = false;
		readSign = true;
		hasReadSign = true;
	}

	for (int i = 0; i < 6; i++)
	{
		if (KeyboardController::GetInstance()->IsKeyPressed('E') && showEnterTentText[i])
		{
			if (hasReadSign)
			{
				tentCompleted[i] = true; // placeholder
				// enter tent scene here
			}
			else
			{
				showReadSignText = true;
				readSignTextTimer = 0.0f;
			}
		}
	}
	if (KeyboardController::GetInstance()->IsKeyPressed('E') && showEnterFinalTentText)
	{
		// logic to complete the final challenge
		isFinalChallengeCompleted = true;
	}
}

void SceneMain::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey)
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

void SceneMain::RenderText(Mesh* mesh, std::string text, glm::vec3 color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	// Save current OpenGL states
	GLboolean blendEnabled;
	GLboolean cullFaceEnabled;
	GLint blendSrcAlpha, blendDstAlpha;
	glGetBooleanv(GL_BLEND, &blendEnabled);
	glGetBooleanv(GL_CULL_FACE, &cullFaceEnabled);
	glGetIntegerv(GL_BLEND_SRC_ALPHA, &blendSrcAlpha);
	glGetIntegerv(GL_BLEND_DST_ALPHA, &blendDstAlpha);

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Disable back face culling
	glDisable(GL_CULL_FACE);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);

	// Save current active texture
	GLint previousTexture;
	glGetIntegerv(GL_ACTIVE_TEXTURE, &previousTexture);

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

	// Restore texture state
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(previousTexture);

	// Restore shader states
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);

	// Restore OpenGL states to their original values
	if (!blendEnabled)
		glDisable(GL_BLEND);
	else
		glBlendFunc(blendSrcAlpha, blendDstAlpha);

	if (cullFaceEnabled)
		glEnable(GL_CULL_FACE);

	// Restore lighting state if needed
	glUniform1i(m_parameters[U_LIGHTENABLED], 1);
}

void SceneMain::RenderTextOnScreen(Mesh* mesh, std::string
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

void SceneMain::Material(GEOMETRY_TYPE obj, float AmR, float AmG, float AmB, float DifA, float DifG, float DifB, float SpA, float SpG, float SpB, float Shiny)
{
	meshList[obj]->material.kAmbient = glm::vec3(AmR, AmG, AmB);
	meshList[obj]->material.kDiffuse = glm::vec3(DifA, DifG, DifB);
	meshList[obj]->material.kSpecular = glm::vec3(SpA, SpG, SpB);
	meshList[obj]->material.kShininess = Shiny;
}

void SceneMain::RenderSkyBox() {
	modelStack.PushMatrix();

	int completedGames = 0;
	for (int i = 0; i < 6; i++) {
		if (tentCompleted[i]) {
			completedGames++;
		}
	}

	bool useFinalSkybox = isFinalChallengeCompleted;

	if (useFinalSkybox) {
		modelStack.PushMatrix();
		modelStack.Translate(0.f, 0.f, 250.f);
		modelStack.Rotate(180, 0, 1, 0);
		modelStack.Scale(5.f, 5.f, 5.f);
		RenderMesh(meshList[GEO_FRONT3], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0.f, 0.f, -250.f);
		modelStack.Scale(5.f, 5.f, 5.f);
		RenderMesh(meshList[GEO_BACK3], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(250.f, 0.f, 0.f);
		modelStack.Rotate(270, 0, 1, 0);
		modelStack.Scale(5.f, 5.f, 5.f);
		RenderMesh(meshList[GEO_LEFT3], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-250.f, 0.f, 0.f);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(5.f, 5.f, 5.f);
		RenderMesh(meshList[GEO_RIGHT3], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Translate(0.f, 250.f, 0.f);
		modelStack.Rotate(90, 1, 0, 0);
		modelStack.Scale(5.f, 5.f, 5.f);
		RenderMesh(meshList[GEO_TOP3], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0.f, -250.f, 0.f);
		modelStack.Rotate(270, 1, 0, 0);
		modelStack.Scale(5.f, 5.f, 5.f);
		RenderMesh(meshList[GEO_BOTTOM3], false);
		modelStack.PopMatrix();
	}
	else if (completedGames >= 6) {
		modelStack.PushMatrix();
		modelStack.Translate(0.f, 0.f, 250.f);
		modelStack.Rotate(180, 0, 1, 0);
		modelStack.Scale(5.f, 5.f, 5.f);
		RenderMesh(meshList[GEO_FRONT2], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0.f, 0.f, -250.f);
		modelStack.Scale(5.f, 5.f, 5.f);
		RenderMesh(meshList[GEO_BACK2], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(250.f, 0.f, 0.f);
		modelStack.Rotate(270, 0, 1, 0);
		modelStack.Scale(5.f, 5.f, 5.f);
		RenderMesh(meshList[GEO_LEFT2], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-250.f, 0.f, 0.f);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(5.f, 5.f, 5.f);
		RenderMesh(meshList[GEO_RIGHT2], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Translate(0.f, 250.f, 0.f);
		modelStack.Rotate(90, 1, 0, 0);
		modelStack.Scale(5.f, 5.f, 5.f);
		RenderMesh(meshList[GEO_TOP2], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0.f, -250.f, 0.f);
		modelStack.Rotate(270, 1, 0, 0);
		modelStack.Scale(5.f, 5.f, 5.f);
		RenderMesh(meshList[GEO_BOTTOM2], false);
		modelStack.PopMatrix();
	}
	else {
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

	modelStack.PopMatrix();
}
