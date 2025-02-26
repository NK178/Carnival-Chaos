#pragma once
#include "SceneFinal.h"
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


SceneFinal::SceneFinal()
{
	m_battleTimer = 80.0f;        // 80 seconds total
	m_bossHealth = 100;           // 100 health
	m_battleStarted = false; // DISCLAIMER : Set this to true for now, Yong Quan youre in charge of adding the UI before the battle starts!!
	m_battleEnded = false;
	m_playerWon = false;
	m_playerLost = false;
	isObjectiveRead = false;
	countdownTime = 0.0f;

	isEnterSceneDialogueActive = true;
	hasESDialogueCompleted = false;
	
	for (int i = 0; i < MAX_BALLOONS; ++i) {
		m_balloons[i].active = true;  // Always active
		m_balloons[i].size = 1.0f;

		// Spread them out in a triangle formation
		if (i == 0) {
			m_balloons[i].offset = glm::vec3(-4.0f, 30.0f, -2.0f);  // Left
		}
		else if (i == 1) {
			m_balloons[i].offset = glm::vec3(4.0f, 30.0f, -2.0f);   // Right
		}
		else {
			m_balloons[i].offset = glm::vec3(0.0f, 40.0f, 0.0f);    // Top
		}
	}

	m_balloonSpawnTimer = 0.0f;
	m_balloonSpawnInterval = 15.0f; // Spawn a balloon every 15 seconds
}

SceneFinal::~SceneFinal()
{
}

void SceneFinal::Init()
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
	camera.Init(glm::vec3(-10, 9, -10), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0));

	// Init VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = nullptr;
	}
	// scenefinal
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("Axes", 10000.f, 10000.f, 10000.f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("Sun", glm::vec3(1.f, 1.f, 1.f), 1.f, 16, 16);
	meshList[GEO_PLANE] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 2.f);
	meshList[GEO_PLANE]->textureID = LoadTGA("Images//stadiumfloor.tga");
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("STAMINA_BAR", glm::vec3(1, 1, 1), 1.f);

	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("Sphere", glm::vec3(1, 1, 1), 1.f);


	//skybox
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Images//stadium.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Images//stadium.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Images//skystadium.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Images//stadium.tga");
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Images//stadium.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);

	meshList[GEO_BACK]->textureID = LoadTGA("Images//stadium.tga");

	//meshList[GEO_CAR] = MeshBuilder::GenerateCar("AICar", glm::vec3(1.f, 1.f, 1.f), 0);

	// 16 x 16 is the number of columns and rows for the text
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Images//calibri.tga");
	meshList[GEO_TEXT2] = MeshBuilder::GenerateText("text2", 16, 16);
	meshList[GEO_TEXT2]->textureID = LoadTGA("Images//yugothicuisemibold.tga");
	meshList[GEO_FPS] = MeshBuilder::GenerateText("fpstext", 16, 16);
	meshList[GEO_FPS]->textureID = LoadTGA("Images//bizudgothic.tga");
	meshList[GEO_UI] = MeshBuilder::GenerateQuad("UIBox", glm::vec3(0.12f, 0.12f, 0.12f), 10.f);

	meshList[GEO_KEY_E] = MeshBuilder::GenerateQuad("KeyE", glm::vec3(1.f, 1.f, 1.f), 2.f);
	meshList[GEO_KEY_E]->textureID = LoadTGA("Images//keyboard_key_e.tga");
	meshList[GEO_KEY_Q] = MeshBuilder::GenerateQuad("KeyQ", glm::vec3(1.f, 1.f, 1.f), 2.f);
	meshList[GEO_KEY_Q]->textureID = LoadTGA("Images//keyboard_key_q.tga");

	meshList[GEO_FENCE] = MeshBuilder::GenerateOBJ("Fence", "Models//wall_OBJ.obj");
	meshList[GEO_FENCE]->textureID = LoadTGA("Images//duvar.tga");

	meshList[GEO_BUMPERCAR] = MeshBuilder::GenerateOBJ("Car", "Models//ATV.obj");
	meshList[GEO_BUMPERCAR]->textureID = LoadTGA("Images//cart.tga");

	meshList[GEO_PELLETGUN] = MeshBuilder::GenerateOBJ("PelletGun", "Models//Ray_Gun.obj");
	meshList[GEO_PELLETGUN]->textureID = LoadTGA("Images//Ray_Gun_Diffuse.tga");
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateQuad("Crosshair", glm::vec3(1, 1, 1), 1.f);

	meshList[GEO_CLOWN] = MeshBuilder::GenerateOBJ("Clown", "Models//bear.obj");
	meshList[GEO_CLOWN]->textureID = LoadTGA("Images//texbear.tga");

	meshList[GEO_BALLOON] = MeshBuilder::GenerateOBJ("Balloon",
		"Models//Balloon.obj");
	meshList[GEO_BALLOON]->textureID = LoadTGA("Images//BalloonAlbedo.tga");
	meshList[GEO_HEALTHBAR] = MeshBuilder::GenerateQuad("HealthBar", glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);


	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.4f, 1000.0f);
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

	carPhysics.pos = glm::vec3(-10, 1, -10); // Starting position
	carPhysics.mass = 1.0f;
	carPhysics.bounciness = 0.3f;

	// Initialize camera to follow car
	//camera.Init(glm::vec3(-10, 9, -15), carPhysics.pos, glm::vec3(0, 1, 0));
	// Typewriting Dialogue
	isTyping = false;
	typewriterTimer = 0.0f;
	currentText = "";
	currentCharIndex = 0;
}

// Disclaimer = SORRY FOR THE MILLION DEBUG STATEMENTS IT TOOK AWHILE - Mathea
bool SceneFinal::CheckRayBalloonCollision(glm::vec3 rayOrigin, glm::vec3 rayDirection)
{
	/*std::cout << "SHOOTING - Origin: " << rayOrigin.x << ", "
		<< rayOrigin.y << ", " << rayOrigin.z << std::endl;
	std::cout << "Direction: " << rayDirection.x << ", "
		<< rayDirection.y << ", " << rayDirection.z << std::endl;*/

	// Iterate through each balloon
	for (int i = 0; i < MAX_BALLOONS; ++i)
	{
		if (m_balloons[i].active)
		{

			glm::vec3 balloonPos = m_cpu.pos + m_balloons[i].offset;
			std::cout << "Balloon " << i << " position: " << balloonPos.x << ", "
				<< balloonPos.y << ", " << balloonPos.z << std::endl;

			// Compute vector from ray origin to balloon center
			glm::vec3 toBalloon = balloonPos - rayOrigin;

			// Project 'toBalloon' onto the ray direction
			float projection = glm::dot(toBalloon, rayDirection);
			if (projection < 0)
				continue; // Balloon is behind the ray origin

			// Find the closest point on the ray to the balloon's center
			glm::vec3 closestPoint = rayOrigin + projection * rayDirection;

			// Distance from the balloon's center to this closest point
			float distance = glm::length(balloonPos - closestPoint);

			float balloonRadius = 3.0f * m_balloons[i].size;

			// If the distance is less than the radius, the ray hits the balloon
			if (distance < balloonRadius)
			{
				std::cout << "HIT BALLOON " << i << "!" << std::endl;

				// Balloon hit! Deactivate it and deal damage.
				m_balloons[i].active = false;
				m_bossHealth -= 10; // Each balloon hit deals 10 damage

				if (m_bossHealth <= 0)
				{
					m_bossHealth = 0;
					m_battleEnded = true;
					m_playerWon = true;
					std::cout << "BOSS DEFEATED!" << std::endl;
				}

				return true; // Collision detected.
			}
		}
	}

	std::cout << "No balloon hit" << std::endl;
	return false;
}

void SceneFinal::Update(double dt) {

	HandleKeyPress();
	// Store old position for collision
	glm::vec3 oldPos = carPhysics.pos;

	// Calculate forward direction based on car's angle
	float angleRad = glm::radians(carPhysics.angleDeg);
	glm::vec3 forward(-sin(angleRad), 0, -cos(angleRad));

	// Handle input for car movement
	if (KeyboardController::GetInstance()->IsKeyDown('W')) {
		carPhysics.AddForce(forward * CAR_FORCE);
	}
	else if (KeyboardController::GetInstance()->IsKeyDown('S')) {
		carPhysics.AddForce(-forward * CAR_FORCE * 0.5f); // Reverse at half force
	}
	carPhysics.vel.y = 0;
	float plrCarTotalVel = sqrt((carPhysics.vel.x * carPhysics.vel.x) + (carPhysics.vel.z * carPhysics.vel.z));
	float cpuCarTotalVel = sqrt((m_cpu.vel.x * m_cpu.vel.x) + (m_cpu.vel.z * m_cpu.vel.z));


	// Handle turning
	if (KeyboardController::GetInstance()->IsKeyDown('A')) {
		carPhysics.angularVel = KeyboardController::GetInstance()->IsKeyDown('S') ? CAR_TURN_RATE * -plrCarTotalVel / 100 : CAR_TURN_RATE * plrCarTotalVel / 100;
	}
	else if (KeyboardController::GetInstance()->IsKeyDown('D')) {
		carPhysics.angularVel = KeyboardController::GetInstance()->IsKeyDown('S') ? -CAR_TURN_RATE * -plrCarTotalVel / 100 : -CAR_TURN_RATE * plrCarTotalVel / 100;
	}
	else {
		carPhysics.angularVel = 0;
	}


	float aiAngleRad = glm::radians(m_cpu.angleDeg);
	glm::vec3 aiForward(-sin(aiAngleRad), 0, -cos(aiAngleRad));

	glm::vec3 aiToPlayer = carPhysics.pos - m_cpu.pos;

	float cross = (aiForward.x * aiToPlayer.z) - (aiForward.z * aiToPlayer.x);
	float dot = glm::dot(aiForward, aiToPlayer);

	//std::cout << cross << std::endl;

	if (dot > 20)
	{
		AImove = 'F';
	}
	else {
		AImove = 'B';
	}

	if (AImove == 'F')
	{
		m_cpu.AddForce(aiForward * CAR_FORCE);

		if (cross > 2)
		{
			AIsteer = 'R';
		}
		else if (cross < -2)
		{
			AIsteer = 'L';
		}
		else
		{
			AIsteer = 'N';
		}
	}
	else if (AImove == 'B')
	{
		m_cpu.AddForce(-aiForward * CAR_FORCE);

		if (cross > 2)
		{
			AIsteer = 'L';
		}
		else if (cross < 2)
		{
			AIsteer = 'R';
		}
		else
		{
			AIsteer = 'N';
		}
	}
	if (AIsteer == 'L')
	{
		m_cpu.angularVel = AImove == 'B' ? CAR_TURN_RATE * -cpuCarTotalVel / 100 : CAR_TURN_RATE * cpuCarTotalVel / 100;
	}
	else if (AIsteer == 'R')
	{
		m_cpu.angularVel = AImove == 'B' ? -CAR_TURN_RATE * -cpuCarTotalVel / 100 : -CAR_TURN_RATE * cpuCarTotalVel / 100;
	}
	else
	{
		m_cpu.angularVel = 0;
	}

	float overallDist = sqrt(((carPhysics.pos.x - m_cpu.pos.x) * (carPhysics.pos.x - m_cpu.pos.x)) + ((carPhysics.pos.z - m_cpu.pos.z) * (carPhysics.pos.z - m_cpu.pos.z)));

	std::cout << overallDist << std::endl;

	if (overallDist <= 20)
	{
		glm::vec3 displacementVect = carPhysics.pos - m_cpu.pos;
		displacementVect.y = 0;
		carPhysics.AddForce(displacementVect * 100.f);
		m_cpu.AddForce(-displacementVect * 100.f);
	}

	// Apply drag force
	carPhysics.vel *= CAR_DRAG;
	m_cpu.vel *= CAR_DRAG;

	// Update physics
	carPhysics.UpdatePhysics(dt);
	m_cpu.UpdatePhysics(dt);

	// Check collisions with fences
	PhysicsObject frontFence;
	frontFence.pos = glm::vec3(0, 0, 100);
	frontFence.mass = 0.0f;
	// Match the fence scale from render: (10.0f, 3.0f, 2.0f)
	// For front/back fences, multiply x scale by 10 to match actual width
	glm::vec3 frontBackFenceExtent(100.0f, 3.0f, 2.0f);

	PhysicsObject backFence;
	backFence.pos = glm::vec3(0, 0, -100);
	backFence.mass = 0.0f;

	PhysicsObject leftFence;
	leftFence.pos = glm::vec3(-100, 0, 0);
	leftFence.mass = 0.0f;
	// For left/right fences, swap x and z due to 90-degree rotation
	glm::vec3 leftRightFenceExtent(2.0f, 3.0f, 100.0f);

	PhysicsObject rightFence;
	rightFence.pos = glm::vec3(100, 0, 0);
	rightFence.mass = 0.0f;

	// Keep car extent relatively small for better collision response
	glm::vec3 carExtent(2.0f, 2.0f, 2.0f);

	CollisionData cd;
	if (OverlapAABB2AABB(carPhysics, carExtent, frontFence, frontBackFenceExtent, cd) ||
		OverlapAABB2AABB(carPhysics, carExtent, backFence, frontBackFenceExtent, cd) ||
		OverlapAABB2AABB(carPhysics, carExtent, leftFence, leftRightFenceExtent, cd) ||
		OverlapAABB2AABB(carPhysics, carExtent, rightFence, leftRightFenceExtent, cd)) {
		ResolveCollision(cd);
	}

	if (OverlapAABB2AABB(m_cpu, carExtent, frontFence, frontBackFenceExtent, cd) ||
		OverlapAABB2AABB(m_cpu, carExtent, backFence, frontBackFenceExtent, cd) ||
		OverlapAABB2AABB(m_cpu, carExtent, leftFence, leftRightFenceExtent, cd) ||
		OverlapAABB2AABB(m_cpu, carExtent, rightFence, leftRightFenceExtent, cd)) {
		ResolveCollision(cd);
	}

	const float ARENA_LIMIT = 95.0f; // Just inside the arena walls

	// Player car boundary check
	if (carPhysics.pos.x > ARENA_LIMIT) carPhysics.pos.x = ARENA_LIMIT;
	if (carPhysics.pos.x < -ARENA_LIMIT) carPhysics.pos.x = -ARENA_LIMIT;
	if (carPhysics.pos.z > ARENA_LIMIT) carPhysics.pos.z = ARENA_LIMIT;
	if (carPhysics.pos.z < -ARENA_LIMIT) carPhysics.pos.z = -ARENA_LIMIT;

	// AI car boundary check
	if (m_cpu.pos.x > ARENA_LIMIT) m_cpu.pos.x = ARENA_LIMIT;
	if (m_cpu.pos.x < -ARENA_LIMIT) m_cpu.pos.x = -ARENA_LIMIT;
	if (m_cpu.pos.z > ARENA_LIMIT) m_cpu.pos.z = ARENA_LIMIT;
	if (m_cpu.pos.z < -ARENA_LIMIT) m_cpu.pos.z = -ARENA_LIMIT;

	// --- Battle Logic ---
   // If the battle has started and not yet ended, update the timer
	if (m_battleStarted && !m_battleEnded) {
		m_battleTimer -= dt;
		if (m_battleTimer <= 0.0f) {
			// Time's up; if the boss still has health, the player loses.
			m_battleTimer = 0.0f;
			m_battleEnded = true;
			m_playerWon = (m_bossHealth <= 0);
			// Optionally trigger a "loss" event here if m_bossHealth > 0.
		}
	}


	if (m_battleStarted && !m_battleEnded) {
		m_balloonSpawnTimer += dt;
		if (m_balloonSpawnTimer >= m_balloonSpawnInterval) {
			m_balloonSpawnTimer = 0.0f;

			// Activate all three balloons at once after each interval
			for (int i = 0; i < MAX_BALLOONS; ++i) {
				m_balloons[i].active = true;

				// Update positions based on current boss location
				m_balloons[i].pos = m_cpu.pos + m_balloons[i].offset;
			}

			std::cout << "All balloons respawned!" << std::endl;
		}
	}

	//if (KeyboardController::GetInstance()->IsKeyPressed('H')) {
	//	m_bossHealth--;
	//}

	// Update camera to match driver's perspective
	float driverHeight = 7.0f;
	camera.pos = carPhysics.pos + glm::vec3(0, driverHeight, 0);
	camera.Update(dt);

	// FPS
	float temp = 1.f / dt;
	fps = glm::round(temp * 100.f) / 100.f;

	UpdateDialogue(dt);
}

void SceneFinal::Render()
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

	//modelStack.PushMatrix();
	//RenderMesh(meshList[GEO_AXES], false);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	//modelStack.Scale(0.4f, 0.4f, 0.4f);
	//RenderMesh(meshList[GEO_SPHERE], false);
	//modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(100.f, 1.f, 100.f);
	modelStack.Rotate(-90.f, 1, 0, 0);
	meshList[GEO_PLANE]->material.kAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
	meshList[GEO_PLANE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_PLANE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_PLANE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_PLANE], true);
	modelStack.PopMatrix();

	/*modelStack.PushMatrix();
	modelStack.Translate(0.f, 0.f, 0.f);
	modelStack.Scale(2.f, 2.f, 2.f);
	modelStack.Rotate(0.f, 0, 1, 0);
	meshList[GEO_CAR]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_CAR]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_CAR]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_CAR]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_CAR], false);
	modelStack.PopMatrix();*/

	// Front wall
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 100);  // Position at front edge
	modelStack.Scale(10.0f, 3.0f, 2.0f);  // Scale for full width
	meshList[GEO_FENCE]->material.kAmbient = glm::vec3(0.7f, 0.7f, 0.7f);
	meshList[GEO_FENCE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_FENCE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_FENCE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_FENCE], true);
	modelStack.PopMatrix();

	// Back wall
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -100);  // Position at back edge
	modelStack.Scale(10.0f, 3.0f, 2.0f);  // Scale for full width
	meshList[GEO_FENCE]->material.kAmbient = glm::vec3(0.7f, 0.7f, 0.7f);
	meshList[GEO_FENCE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_FENCE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_FENCE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_FENCE], true);
	modelStack.PopMatrix();

	// Left wall
	modelStack.PushMatrix();
	modelStack.Translate(-100, 0, 0);  // Position at left edge
	modelStack.Rotate(90, 0, 1, 0);    // Rotate to face inward
	modelStack.Scale(10.0f, 3.0f, 2.0f);  // Scale for full width
	meshList[GEO_FENCE]->material.kAmbient = glm::vec3(0.7f, 0.7f, 0.7f);
	meshList[GEO_FENCE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_FENCE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_FENCE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_FENCE], true);
	modelStack.PopMatrix();

	// Right wall
	modelStack.PushMatrix();
	modelStack.Translate(100, 0, 0);   // Position at right edge
	modelStack.Rotate(90, 0, 1, 0);    // Rotate to face inward
	modelStack.Scale(10.0f, 3.0f, 2.0f);  // Scale for full width
	meshList[GEO_FENCE]->material.kAmbient = glm::vec3(0.7f, 0.7f, 0.7f);
	meshList[GEO_FENCE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_FENCE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_FENCE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_FENCE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(carPhysics.pos.x, carPhysics.pos.y, carPhysics.pos.z);
	modelStack.Rotate(carPhysics.angleDeg, 0, 1, 0);
	modelStack.Scale(0.2f, 0.25f, 0.2f);
	meshList[GEO_BUMPERCAR]->material.kAmbient = glm::vec3(0.7f, 0.7f, 0.7f);
	meshList[GEO_BUMPERCAR]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_BUMPERCAR]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_BUMPERCAR]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_BUMPERCAR], true);
	modelStack.PopMatrix();

	// AI car
	modelStack.PushMatrix();
	modelStack.Translate(m_cpu.pos.x, 7, m_cpu.pos.z);
	modelStack.Rotate(m_cpu.angleDeg, 0, 1, 0);
	modelStack.Scale(0.2f, 0.25f, 0.2f);
	meshList[GEO_BUMPERCAR]->material.kAmbient = glm::vec3(0.7f, 0.7f, 0.7f);
	meshList[GEO_BUMPERCAR]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_BUMPERCAR]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_BUMPERCAR]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_BUMPERCAR], true);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	// Calculate position based on camera position and view direction
	glm::vec3 viewDirection = glm::normalize(camera.target - camera.pos);


	// Position gun slightly in front and to the right of the camera
	float gunDistance = 0.75f; // How far in front of the camera
	float gunRightOffset = 0.2f; // How far to the right
	float gunDownOffset = 0.3f; // How far down from eye level

	// Calculate right vector by cross product of view direction and up vector
	glm::vec3 rightVector = glm::normalize(glm::cross(viewDirection, camera.up));
	// Calculate gun position
	glm::vec3 gunPosition = camera.pos +
		(viewDirection * gunDistance) +
		(rightVector * gunRightOffset) -
		(camera.up * gunDownOffset);

	modelStack.Translate(gunPosition.x, gunPosition.y, gunPosition.z);

	// Orient the gun to face where the camera is looking
	// Calculate rotation angles based on view direction
	float yaw = atan2(viewDirection.x, viewDirection.z);
	float pitch = -asin(viewDirection.y); // Negative because we want to invert the pitch

	// Apply rotations
	modelStack.Rotate(glm::degrees(yaw), 0, 1, 0); // Rotate around Y axis (yaw)
	modelStack.Rotate(glm::degrees(pitch), 1, 0, 0); // Rotate around X axis (pitch)
	// Additional rotation to orient the gun model correctly (adjust as needed based on your model)
	modelStack.Rotate(180.0f, 0, 1, 0); // This may need to be adjusted depending on how the gun model is oriented

	// Scale the gun to an appropriate size
	modelStack.Scale(0.02f, 0.02f, 0.02f); // Adjust these values to make the gun the right size

	// Set materials and render
	meshList[GEO_PELLETGUN]->material.kAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
	meshList[GEO_PELLETGUN]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_PELLETGUN]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_PELLETGUN]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_PELLETGUN], true);
	modelStack.PopMatrix();


	// Render the AI driver's model (GEO_CLOWN) attached to the AI car (m_cpu)
	modelStack.PushMatrix();
	modelStack.Translate(m_cpu.pos.x, m_cpu.pos.y + 6.0f, m_cpu.pos.z - 2);

	modelStack.Rotate(m_cpu.angleDeg + 180.0f, 0, 1, 0);

	modelStack.Translate(0.5f, 0.0f, 0.0f);
	modelStack.Scale(15.0f, 15.0f, 15.0f); // Adjust scale as needed

	// Set materials for the driver (if needed)
	meshList[GEO_CLOWN]->material.kAmbient = glm::vec3(0.7f, 0.7f, 0.7f);
	meshList[GEO_CLOWN]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_CLOWN]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_CLOWN]->material.kShininess = 1.0f;

	RenderMesh(meshList[GEO_CLOWN], true);
	modelStack.PopMatrix();

	// Render boss balloons
	for (int i = 0; i < MAX_BALLOONS; ++i) {
		if (m_balloons[i].active) {
			modelStack.PushMatrix();
			// Position the balloon relative to the boss car
			// m_cpu.pos is assumed to be the boss car's position
			modelStack.Translate(m_cpu.pos.x + m_balloons[i].offset.x,
				m_cpu.pos.y + m_balloons[i].offset.y,
				m_cpu.pos.z + m_balloons[i].offset.z);
			// Scale the balloon according to its size
			modelStack.Scale(m_balloons[i].size, m_balloons[i].size, m_balloons[i].size);
			meshList[GEO_BALLOON]->material.kAmbient = glm::vec3(0.7f, 0.7f, 0.7f);
			meshList[GEO_BALLOON]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
			meshList[GEO_BALLOON]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
			meshList[GEO_BALLOON]->material.kShininess = 1.0f;
			// Render the balloon mesh (make sure GEO_BALLOON is defined and loaded)
			RenderMesh(meshList[GEO_BALLOON], true);
			modelStack.PopMatrix();
		}
	}

	RenderSkyBox();
	RenderDialogue();

	if (!isEnterSceneDialogueActive) {
		isObjectiveRead = false;
	}
	if (!isObjectiveRead) {
		// render objective
	}

	//if (isObjectiveRead) {
	//	if (countdownTime > 0) {
	//		std::string countdownText;
	//		if (countdownTime > 3.0f) {
	//			countdownText = "3..";
	//		}
	//		else if (countdownTime > 2.0f) {
	//			countdownText = "2..";
	//		}
	//		else if (countdownTime > 1.0f) {
	//			countdownText = "1..";
	//		}
	//		else {
	//			countdownText = "GO!";
	//			m_battleStarted = true;
	//		}
	//		RenderTextOnScreen(meshList[GEO_TEXT2], countdownText, glm::vec3(1, 1, 1), 50, 350, 300);
	//	}
	//}

	if (m_battleStarted) {
		// Render health label and boss health bar next to each other
		RenderTextOnScreen(meshList[GEO_TEXT], "Health:", glm::vec3(1, 1, 1), 20, 20, 550);
		float healthPercent = (float)m_bossHealth / 100.0f;
		
		RenderMeshOnScreen(meshList[GEO_HEALTHBAR], 260, 560, 200 * healthPercent, 20);

		RenderTextOnScreen(meshList[GEO_TEXT], "Time: " + std::to_string((int)m_battleTimer), glm::vec3(1, 1, 1), 20, 20, 520);
	}

	// Render vertical line of crosshair
	RenderMeshOnScreen(meshList[GEO_CROSSHAIR], 400, 300, 2, 20);  // Thin vertical line
	// Render horizontal line of crosshair
	RenderMeshOnScreen(meshList[GEO_CROSSHAIR], 400, 300, 20, 2);  // Thin horizontal line

	// Render FPS
	std::string temp("FPS:" + std::to_string(fps));
	RenderTextOnScreen(meshList[GEO_FPS], temp.substr(0, 9), glm::vec3(0, 1, 0), 20, 620, 20);
}

void SceneFinal::RenderDialogue() {
	if (isEnterSceneDialogueActive) {
		RenderMeshOnScreen(meshList[GEO_UI], 150, 535, 150, 9);
		RenderMeshOnScreen(meshList[GEO_KEY_Q], 20, 510, 10, 10);
		RenderTextOnScreen(meshList[GEO_TEXT], "[ SKIP ]", glm::vec3(1, 1, 1), 15, 40, 505);
		RenderMeshOnScreen(meshList[GEO_KEY_E], 170, 510, 10, 10);
		RenderTextOnScreen(meshList[GEO_TEXT], "[ NEXT DIALOGUE ]", glm::vec3(1, 1, 1), 15, 200, 505);
	}
	else {
		return; // exit function early if no dialogue is active
	}

	// Rendering dialogue for when the player reads the sign
	RenderMeshOnScreen(meshList[GEO_UI], 150, 550, 150, 6);
	if (isEnterSceneDialogueActive && currentLineIndex < enterSceneDialogueLines.size()) {
		const DialogueLine& currentDialogue = enterSceneDialogueLines[currentLineIndex];

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

void SceneFinal::UpdateDialogue(double dt) {
	// Start the dialogue when entering the scene
	if (!isEnterSceneDialogueActive && !hasESDialogueCompleted) {
		isEnterSceneDialogueActive = true;
		currentLineIndex = 0;
		dialogueTimer = 0;
		isTyping = true; // Start typing the first line
		typewriterTimer = 0.0f;

		const DialogueLine& currentDialogue = enterSceneDialogueLines[currentLineIndex];
		if (currentDialogue.isMultiLine) {
			// Store both lines for multi-line dialogue
			currentText = currentDialogue.textLines[0] + "\n" + currentDialogue.textLines[1];
		}
		else {
			currentText = currentDialogue.textLines[0];
		}
		currentCharIndex = 0;
	}

	if (isEnterSceneDialogueActive) {
		if (KeyboardController::GetInstance()->IsKeyPressed('E')) {
			if (isTyping) {
				// Skip rendering the current text if the player presses E
				currentCharIndex = currentText.length();
				isTyping = false;
			}
			else {
				// Skip to the next line if the player presses E after finishing typing
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
				if (currentLineIndex >= enterSceneDialogueLines.size()) {
					// Dialogue ends here
					isEnterSceneDialogueActive = false;
					hasESDialogueCompleted = true;
					camera.enableFNAF = false;
					camera.allowMovement = true;
					camera.allowJump = true;
					camera.allowLocomotiveTilt = true;
				}
				else {
					// Continue to the next line
					isTyping = true;
					typewriterTimer = 0.0f;
					const DialogueLine& currentDialogue = enterSceneDialogueLines[currentLineIndex];
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

void SceneFinal::RenderMesh(Mesh* mesh, bool enableLight)
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


void SceneFinal::Exit()
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

void SceneFinal::HandleKeyPress()
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

	/*if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_TAB))
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
	}*/


	if (MouseController::GetInstance()->IsButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
		std::cout << "LEFT MOUSE BUTTON PRESSED" << std::endl;

		// Create a ray from the camera's position in the direction the camera is facing
		glm::vec3 rayOrigin = camera.pos;
		glm::vec3 rayDirection = glm::normalize(camera.target - camera.pos);

		// Check for collision with balloons
		bool hitBalloon = CheckRayBalloonCollision(rayOrigin, rayDirection);
	}

	if (KeyboardController::GetInstance()->IsKeyPressed('Q') && isEnterSceneDialogueActive) {
		// Skip the entire sign dialogue
		isEnterSceneDialogueActive = false;
		hasESDialogueCompleted = true;
		camera.enableFNAF = false;
		camera.allowMovement = true;
		camera.allowJump = true;
		camera.allowSprint = false;
		camera.allowCrouch = true;
		camera.allowProne = false;
		camera.allowLocomotiveTilt = true;
		camera.allowLocomotiveBop = false;
	}
}

void SceneFinal::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey)
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

void SceneFinal::RenderText(Mesh* mesh, std::string text, glm::vec3
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

void SceneFinal::RenderTextOnScreen(Mesh* mesh, std::string
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

void SceneFinal::Material(GEOMETRY_TYPE obj, float AmR, float AmG, float AmB, float DifA, float DifG, float DifB, float SpA, float SpG, float SpB, float Shiny)
{
	meshList[obj]->material.kAmbient = glm::vec3(AmR, AmG, AmB);
	meshList[obj]->material.kDiffuse = glm::vec3(DifA, DifG, DifB);
	meshList[obj]->material.kSpecular = glm::vec3(SpA, SpG, SpB);
	meshList[obj]->material.kShininess = Shiny;
}

void SceneFinal::RenderSkyBox() {
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