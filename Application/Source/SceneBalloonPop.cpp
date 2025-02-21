#pragma once
#include "SceneBalloonPop.h"
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

SceneBalloonPop::SceneBalloonPop() :
	m_vertexArrayID(0),
	m_programID(0),
	meshList(),  // This will zero-initialize the array
	m_parameters(),  // This will zero-initialize the array
	camera(),
	projType(1),
	modelStack(),
	viewStack(),
	projectionStack(),
	light(),  // This will default construct all lights in the array
	enableLight(false),
	app()
{
	// Additional initialization if needed
	for (int i = 0; i < NUM_GEOMETRY; ++i) {
		meshList[i] = nullptr;
	}

	// Initialize all parameters to 0
	for (int i = 0; i < U_TOTAL; ++i) {
		m_parameters[i] = 0;
	}

	// Initialize game variables
	gameTimer = GAME_TIME_LIMIT;
	playerScore = 0;
	gameOver = false;
	spawnTimer = 0.0f;

	// Spawn initial balloons
	for (int i = 0; i < 5; ++i) {
		SpawnBalloon();
	}
}

SceneBalloonPop::~SceneBalloonPop()
{
}

void SceneBalloonPop::Init()
{
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
	meshList[GEO_PLANE]->textureID = LoadTGA("Images//wooden-stage-flooring-250x250.tga");
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("STAMINA_BAR", glm::vec3(1, 1, 1), 1.f);

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Images//stage.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Images//stage.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Images//stageroof.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Images//stage.tga");
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Images//stage.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Images//stage.tga");
	// sdjidskjsdjkskja
	// 16 x 16 is the number of columns and rows for the text
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Images//calibri.tga");

	meshList[GEO_CRATE] = MeshBuilder::GenerateOBJ("Crate",
		"Models//cube_low.obj");
	meshList[GEO_CRATE]->textureID = LoadTGA("Images//cube_low_None_BaseColor.tga");

	meshList[GEO_BALLOONS] = MeshBuilder::GenerateOBJ("Balloons",
		"Models//13499_Balloon_Cluster_v1_L2.obj");
	meshList[GEO_BALLOONS]->textureID = LoadTGA("Images//13499_Balloon_Cluster_diffuse_ballon_yellow.tga");

	meshList[GEO_PRESENT] = MeshBuilder::GenerateOBJ("Present",
		"Models//uploads_files_4008356_GiftBox_obj.obj");
	meshList[GEO_PRESENT]->textureID = LoadTGA("Images//present.tga");

	meshList[GEO_BALLOON] = MeshBuilder::GenerateOBJ("Balloon",
		"Models//Balloon.obj");
	meshList[GEO_BALLOON]->textureID = LoadTGA("Images//BalloonAlbedo.tga");

	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateQuad("Crosshair", glm::vec3(1, 1, 1), 1.f);

	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	projectionStack.LoadMatrix(projection);

	glUniform1i(m_parameters[U_NUMLIGHTS], NUM_LIGHTS);

	//light[0].position = glm::vec3(30, 30, 0);
	//light[0].color = glm::vec3(1, 1, 1);
	//light[0].type = Light::LIGHT_DIRECTIONAL;
	//light[0].power = 1.f;
	//light[0].kC = 1.f;
	//light[0].kL = 0.01f;
	//light[0].kQ = 0.001f;
	//light[0].cosCutoff = 30.f;
	//light[0].cosInner = 15.f;
	//light[0].exponent = 3.f;
	//light[0].spotDirection = glm::vec3(0.f, 1.f, 0.f);

	light[1].position = glm::vec3(0, 60, 0);
	light[1].color = glm::vec3(1, 1, 1);
	light[1].type = Light::LIGHT_SPOT;
	light[1].power = 16.0f;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = 110.f;
	light[1].cosInner = 30.f;
	light[1].exponent = 3.f;
	light[1].spotDirection = glm::vec3(0.f, 1.f, 0.f);

	/*light[2].position = glm::vec3(-50,4,0);
	light[2].color = glm::vec3(1, 1, 1);
	light[2].type = Light::LIGHT_POINT;
	light[2].power = 1.f;
	light[2].kC = 1.f;
	light[2].kL = 0.01f;
	light[2].kQ = 0.001f;
	light[2].cosCutoff = 45.f;
	light[2].cosInner = 30.f;
	light[2].exponent = 3.f;
	light[2].spotDirection = glm::vec3(0.f, 1.f, 0.f);*/

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


}

void SceneBalloonPop::Update(double dt)
{
	HandleKeyPress();
	if (!gameOver) {
		// Update game timer
		gameTimer -= static_cast<float>(dt);
		if (gameTimer <= 0 && playerScore < WIN_SCORE) {
			gameOver = true;
			return;
		}

		// Update spawn timer
		spawnTimer -= static_cast<float>(dt);
		if (spawnTimer <= 0) {
			SpawnBalloon();
			spawnTimer = SPAWN_INTERVAL;
		}

		for (auto& balloon : balloons) {
			if (!balloon.isPopped) {
				// Basic upward force
				balloon.physics.AddForce(glm::vec3(0, BALLOON_UP_FORCE, 0));

				// Add sine wave motion to create floating effect
				float time = static_cast<float>(glfwGetTime());
				float wobbleX = BALLOON_WOBBLE_FORCE * sin(time + balloon.timeAlive);
				float wobbleZ = BALLOON_WOBBLE_FORCE * cos(time * 0.5f + balloon.timeAlive);

				// Add wobble forces
				balloon.physics.AddForce(glm::vec3(wobbleX, 0, wobbleZ));

				// Add rightward force that varies with height
				float rightForce = BALLOON_RIGHT_FORCE * (1.0f + 0.5f * sin(time * 2.0f + balloon.timeAlive));
				balloon.physics.AddForce(glm::vec3(rightForce, 0, 0));

				// Random movement to prevent synchronization
				float randX = (rand() % 200 - 100) / 100.0f; // Random value between -1 and 1
				float randZ = (rand() % 200 - 100) / 100.0f;
				balloon.physics.AddForce(glm::vec3(randX, 0, randZ));

				// Update balloon's time alive
				balloon.timeAlive += static_cast<float>(dt);

				// Wall repulsion forces 
				const float WALL_REPULSION = 500.0f;
				const float BOUNCE_FACTOR = 1.7f;

				// X-axis walls
				if (balloon.physics.pos.x < -120.0f) {
					balloon.physics.pos.x = -120.0f;
					balloon.physics.vel.x *= -BOUNCE_FACTOR;
					balloon.physics.AddForce(glm::vec3(WALL_REPULSION, 0, 0));  // Strong push right
				}
				if (balloon.physics.pos.x > 120.0f) {
					balloon.physics.pos.x = 120.0f;
					balloon.physics.vel.x *= -BOUNCE_FACTOR;
					balloon.physics.AddForce(glm::vec3(-WALL_REPULSION, 0, 0));  // Strong push left
				}

				// Z-axis walls
				if (balloon.physics.pos.z < -120.0f) {
					balloon.physics.pos.z = -120.0f;
					balloon.physics.vel.z *= -BOUNCE_FACTOR;
					balloon.physics.AddForce(glm::vec3(0, 0, WALL_REPULSION));  // Strong push forward
				}
				if (balloon.physics.pos.z > 120.0f) {
					balloon.physics.pos.z = 120.0f;
					balloon.physics.vel.z *= -BOUNCE_FACTOR;
					balloon.physics.AddForce(glm::vec3(0, 0, -WALL_REPULSION));  // Strong push back
				}

				// Ceiling behavior
				if (balloon.physics.pos.y > CEILING_HEIGHT) {
					balloon.physics.pos.y = CEILING_HEIGHT;
					balloon.physics.vel.y *= -0.5f;
					balloon.physics.AddForce(glm::vec3(0, -10.0f, 0));  // Push down from ceiling
				}

				// Update physics
				balloon.physics.UpdatePhysics(static_cast<float>(dt));

				// More gentle damping to maintain movement
				balloon.physics.vel *= 0.99f;
			}
		}

		// Mouse click handling
		if (MouseController::GetInstance()->IsButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
			glm::vec3 rayDir = glm::normalize(camera.target - camera.pos);
			for (auto& balloon : balloons) {
				if (!balloon.isPopped) {
					glm::vec3 toBalloon = balloon.physics.pos - camera.pos;
					float dotProduct = glm::dot(toBalloon, rayDir);
					glm::vec3 projection = rayDir * dotProduct;
					glm::vec3 closestPoint = camera.pos + projection;

					if (glm::length(balloon.physics.pos - closestPoint) < 2.0f) {
						balloon.isPopped = true;
						playerScore++;
						if (playerScore >= WIN_SCORE) {
							gameOver = true;
						}
					}
				}
			}
		}
	}
	else {
		if (KeyboardController::GetInstance()->IsKeyPressed('R')) {
			gameTimer = GAME_TIME_LIMIT;
			playerScore = 0;
			gameOver = false;
			balloons.clear();
			for (int i = 0; i < 5; ++i) {
				SpawnBalloon();
			}
		}
	}

	camera.Update(dt);
}

void SceneBalloonPop::Render()
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


	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_AXES], false);
	modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Translate(light[1].position.x, light[1].position.y, light[1].position.z);
	//modelStack.Scale(0.1f, 0.1f, 0.1f);
	//RenderMesh(meshList[GEO_SPHERE], false);
	//modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(150.f, 1.f, 150.f);
	modelStack.Rotate(-90.f, 1, 0, 0);
	meshList[GEO_PLANE]->material.kAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_PLANE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_PLANE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_PLANE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_PLANE], true);
	modelStack.PopMatrix();

	// Corner crates - Front left
	modelStack.PushMatrix();
	modelStack.Translate(-120, 7, 120);  // Moved inward from -140, 7, 140
	modelStack.Rotate(-30.f, 0, 1, 0);
	modelStack.Scale(10.0f, 10.0f, 10.0f);
	meshList[GEO_CRATE]->material.kAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
	meshList[GEO_CRATE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_CRATE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_CRATE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_CRATE], true);
	modelStack.PopMatrix();

	// Front right corner stack
	modelStack.PushMatrix();
	modelStack.Translate(120, 7, 120);  // Moved inward from 140, 7, 140
	modelStack.Rotate(45.f, 0, 1, 0);
	modelStack.Scale(10.0f, 10.0f, 10.0f);
	meshList[GEO_CRATE]->material.kAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
	meshList[GEO_CRATE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_CRATE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_CRATE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_CRATE], true);
	modelStack.PopMatrix();

	// Stack a second crate on top
	modelStack.PushMatrix();
	modelStack.Translate(120, 27, 120);  // Moved inward from 140, 27, 140
	modelStack.Rotate(15.f, 0, 1, 0);
	modelStack.Scale(10.0f, 10.0f, 10.0f);
	meshList[GEO_CRATE]->material.kAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
	meshList[GEO_CRATE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_CRATE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_CRATE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_CRATE], true);
	modelStack.PopMatrix();

	// Back left corner
	modelStack.PushMatrix();
	modelStack.Translate(-120, 7, -120);  // Moved inward from -140, 7, -140
	modelStack.Rotate(-15.f, 0, 1, 0);
	modelStack.Scale(10.0f, 10.0f, 10.0f);
	meshList[GEO_CRATE]->material.kAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
	meshList[GEO_CRATE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_CRATE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_CRATE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_CRATE], true);
	modelStack.PopMatrix();

	// Back right corner stack
	modelStack.PushMatrix();
	modelStack.Translate(120, 7, -120);  // Moved inward from 140, 7, -140
	modelStack.Rotate(60.f, 0, 1, 0);
	modelStack.Scale(10.0f, 10.0f, 10.0f);
	meshList[GEO_CRATE]->material.kAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
	meshList[GEO_CRATE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_CRATE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_CRATE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_CRATE], true);
	modelStack.PopMatrix();

	// Add another crate on top
	modelStack.PushMatrix();
	modelStack.Translate(120, 27, -120);  // Moved inward from 140, 27, -140
	modelStack.Rotate(30.f, 0, 1, 0);
	modelStack.Scale(10.0f, 10.0f, 10.0f);
	meshList[GEO_CRATE]->material.kAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
	meshList[GEO_CRATE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_CRATE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_CRATE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_CRATE], true);
	modelStack.PopMatrix();

	// Add crates along the left wall
	modelStack.PushMatrix();
	modelStack.Translate(-100, 7, 40);  // Against left wall
	modelStack.Rotate(25.f, 0, 1, 0);   // Slight rotation for variety
	modelStack.Scale(8.0f, 8.0f, 8.0f); // Slightly smaller scale
	meshList[GEO_CRATE]->material.kAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
	meshList[GEO_CRATE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_CRATE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_CRATE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_CRATE], true);
	modelStack.PopMatrix();

	// Add crates along the right wall
	modelStack.PushMatrix();
	modelStack.Translate(100, 7, -40);
	modelStack.Rotate(-15.f, 0, 1, 0);
	modelStack.Scale(8.0f, 8.0f, 8.0f);
	meshList[GEO_CRATE]->material.kAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
	meshList[GEO_CRATE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_CRATE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_CRATE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_CRATE], true);
	modelStack.PopMatrix();

	// Small cluster of crates
	modelStack.PushMatrix();
	modelStack.Translate(-80, 7, -60);
	modelStack.Rotate(45.f, 0, 1, 0);
	modelStack.Scale(7.0f, 7.0f, 7.0f);
	meshList[GEO_CRATE]->material.kAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
	meshList[GEO_CRATE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_CRATE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_CRATE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_CRATE], true);
	modelStack.PopMatrix();

	// Stack on top of previous crate
	modelStack.PushMatrix();
	modelStack.Translate(-80, 21, -60);
	modelStack.Rotate(30.f, 0, 1, 0);
	modelStack.Scale(7.0f, 7.0f, 7.0f);
	meshList[GEO_CRATE]->material.kAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
	meshList[GEO_CRATE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_CRATE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_CRATE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_CRATE], true);
	modelStack.PopMatrix();

	// Add a crate near the back wall
	modelStack.PushMatrix();
	modelStack.Translate(60, 7, -90);
	modelStack.Rotate(-35.f, 0, 1, 0);
	modelStack.Scale(9.0f, 9.0f, 9.0f);
	meshList[GEO_CRATE]->material.kAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
	meshList[GEO_CRATE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_CRATE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_CRATE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_CRATE], true);
	modelStack.PopMatrix();

	// Small group near front
	modelStack.PushMatrix();
	modelStack.Translate(-40, 7, 80);
	modelStack.Rotate(10.f, 0, 1, 0);
	modelStack.Scale(6.0f, 6.0f, 6.0f);
	meshList[GEO_CRATE]->material.kAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
	meshList[GEO_CRATE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_CRATE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_CRATE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_CRATE], true);
	modelStack.PopMatrix();

	// Stacked crates
	modelStack.PushMatrix();
	modelStack.Translate(80, 7, 60);
	modelStack.Rotate(-20.f, 0, 1, 0);
	modelStack.Scale(8.0f, 8.0f, 8.0f);
	meshList[GEO_CRATE]->material.kAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
	meshList[GEO_CRATE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_CRATE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_CRATE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_CRATE], true);
	modelStack.PopMatrix();

	// Top crate of stack
	modelStack.PushMatrix();
	modelStack.Translate(80, 23, 60);
	modelStack.Rotate(-50.f, 0, 1, 0);
	modelStack.Scale(8.0f, 8.0f, 8.0f);
	meshList[GEO_CRATE]->material.kAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
	meshList[GEO_CRATE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_CRATE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_CRATE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_CRATE], true);
	modelStack.PopMatrix();


	// Balloon on front left corner crate
	modelStack.PushMatrix();
	modelStack.Translate(-120, 17, 120);  // Positioned on top of crate
	modelStack.Rotate(270.f, 1, 0, 0);    // Base rotation for balloon
	modelStack.Rotate(45.f, 0, 0, 1);     // Additional rotation for variety
	modelStack.Scale(1.2f, 1.2f, 1.2f);
	meshList[GEO_BALLOONS]->material.kAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
	meshList[GEO_BALLOONS]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_BALLOONS]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_BALLOONS]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_BALLOONS], true);
	modelStack.PopMatrix();

	// Balloon on stacked crate (front right)
	modelStack.PushMatrix();
	modelStack.Translate(120, 37, 120);  // Positioned on top of stacked crate
	modelStack.Rotate(270.f, 1, 0, 0);
	modelStack.Rotate(-30.f, 0, 0, 1);
	modelStack.Scale(1.4f, 1.4f, 1.4f);
	meshList[GEO_BALLOONS]->material.kAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
	meshList[GEO_BALLOONS]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_BALLOONS]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_BALLOONS]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_BALLOONS], true);
	modelStack.PopMatrix();

	// Balloon on small cluster crate
	modelStack.PushMatrix();
	modelStack.Translate(-80, 31, -60);  // On top of stacked small cluster
	modelStack.Rotate(270.f, 1, 0, 0);
	modelStack.Rotate(15.f, 0, 0, 1);
	modelStack.Scale(1.0f, 1.0f, 1.0f);
	meshList[GEO_BALLOONS]->material.kAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
	meshList[GEO_BALLOONS]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_BALLOONS]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_BALLOONS]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_BALLOONS], true);
	modelStack.PopMatrix();

	// Balloon on back wall crate
	modelStack.PushMatrix();
	modelStack.Translate(60, 17, -90);
	modelStack.Rotate(270.f, 1, 0, 0);
	modelStack.Rotate(-20.f, 0, 0, 1);
	modelStack.Scale(1.3f, 1.3f, 1.3f);
	meshList[GEO_BALLOONS]->material.kAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
	meshList[GEO_BALLOONS]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_BALLOONS]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_BALLOONS]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_BALLOONS], true);
	modelStack.PopMatrix();

	// Balloon on stacked crates near front
	modelStack.PushMatrix();
	modelStack.Translate(80, 33, 60);    // On top of highest crate
	modelStack.Rotate(270.f, 1, 0, 0);
	modelStack.Rotate(60.f, 0, 0, 1);
	modelStack.Scale(1.5f, 1.5f, 1.5f);
	meshList[GEO_BALLOONS]->material.kAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
	meshList[GEO_BALLOONS]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_BALLOONS]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_BALLOONS]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_BALLOONS], true);
	modelStack.PopMatrix();


	// Present on front left crate
	modelStack.PushMatrix();
	modelStack.Translate(-120, 17, 120);
	modelStack.Rotate(30.f, 0, 1, 0);
	modelStack.Scale(3.0f, 3.0f, 3.0f);
	meshList[GEO_PRESENT]->material.kAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
	meshList[GEO_PRESENT]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_PRESENT]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_PRESENT]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_PRESENT], true);
	modelStack.PopMatrix();

	// Small present cluster near corner
	modelStack.PushMatrix();
	modelStack.Translate(-100, 1, -100);  // Lowered from y=3 to y=1
	modelStack.Rotate(-15.f, 0, 1, 0);
	modelStack.Scale(3.5f, 3.5f, 3.5f);
	meshList[GEO_PRESENT]->material.kAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
	meshList[GEO_PRESENT]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_PRESENT]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_PRESENT]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_PRESENT], true);
	modelStack.PopMatrix();

	// Present stacked on another
	modelStack.PushMatrix();
	modelStack.Translate(-100, 8, -100);  // This one stays at y=5 since it's stacked
	modelStack.Rotate(60.f, 0, 1, 0);
	modelStack.Scale(2.5f, 2.5f, 2.5f);
	meshList[GEO_PRESENT]->material.kAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
	meshList[GEO_PRESENT]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_PRESENT]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_PRESENT]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_PRESENT], true);
	modelStack.PopMatrix();

	// Present on right side
	modelStack.PushMatrix();
	modelStack.Translate(90, 0, 40);  // Lowered from y=5 to y=1
	modelStack.Rotate(-25.f, 0, 1, 0);
	modelStack.Scale(3.8f, 3.8f, 3.8f);
	meshList[GEO_PRESENT]->material.kAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
	meshList[GEO_PRESENT]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_PRESENT]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_PRESENT]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_PRESENT], true);
	modelStack.PopMatrix();

	// Present near back wall
	modelStack.PushMatrix();
	modelStack.Translate(30, 0, -80);  // Lowered from y=5 to y=1
	modelStack.Rotate(10.f, 0, 1, 0);
	modelStack.Scale(4.2f, 4.2f, 4.2f);
	meshList[GEO_PRESENT]->material.kAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
	meshList[GEO_PRESENT]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_PRESENT]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_PRESENT]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_PRESENT], true);
	modelStack.PopMatrix();

	// Present by left wall
	modelStack.PushMatrix();
	modelStack.Translate(-90, 0, 20);  // Lowered from y=5 to y=1
	modelStack.Rotate(35.f, 0, 1, 0);
	modelStack.Scale(3.2f, 3.2f, 3.2f);
	meshList[GEO_PRESENT]->material.kAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
	meshList[GEO_PRESENT]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_PRESENT]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_PRESENT]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_PRESENT], true);
	modelStack.PopMatrix();





	// Render all balloons
	for (const auto& balloon : balloons) {
		if (!balloon.isPopped) {
			modelStack.PushMatrix();
			modelStack.Translate(
				balloon.physics.pos.x,
				balloon.physics.pos.y,
				balloon.physics.pos.z
			);
			modelStack.Scale(1.5f, 1.5f, 1.5f);
			meshList[GEO_BALLOON]->material.kAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
			meshList[GEO_BALLOON]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
			meshList[GEO_BALLOON]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
			meshList[GEO_BALLOON]->material.kShininess = 1.0f;
			RenderMesh(meshList[GEO_BALLOON], true);
			modelStack.PopMatrix();
		}
	}

	
	/*RenderTextOnScreen(meshList[GEO_TEXT], "Stamina", glm::vec3(0, 1, 0), 40, 0, 0);*/

	RenderSkyBox();



	// Render UI
	std::string scoreText = "Score: " + std::to_string(playerScore) + "/" + std::to_string(WIN_SCORE);
	RenderTextOnScreen(meshList[GEO_TEXT], scoreText, glm::vec3(1, 1, 0), 25, 10, 550);

	std::string timeText = "Time: " + std::to_string(static_cast<int>(gameTimer));
	RenderTextOnScreen(meshList[GEO_TEXT], timeText, glm::vec3(1, 1, 1), 25, 10, 520);

	if (gameOver) {
		std::string gameOverText = playerScore >= WIN_SCORE ? "You Win!" : "Game Over!";
		RenderTextOnScreen(meshList[GEO_TEXT], gameOverText, glm::vec3(1, 0, 0), 40, 300, 300);
		RenderTextOnScreen(meshList[GEO_TEXT], "Press R to Restart", glm::vec3(1, 1, 1), 25, 300, 250);
	}

	// Render vertical line of crosshair
	RenderMeshOnScreen(meshList[GEO_CROSSHAIR], 400, 300, 2, 20);  // Thin vertical line
	// Render horizontal line of crosshair
	RenderMeshOnScreen(meshList[GEO_CROSSHAIR], 400, 300, 20, 2);  // Thin horizontal line


}

void SceneBalloonPop::RenderMesh(Mesh* mesh, bool enableLight)
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


void SceneBalloonPop::Exit()
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

void SceneBalloonPop::HandleKeyPress()
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


bool SceneBalloonPop::OverlapAABB2AABB(glm::vec3 Obj1, const int Width1, const int Height1,
	glm::vec3 Obj2, const int Width2, const int Height2)
{

	float MinX1, MaxX1, MinY1, MaxY1, MinX2, MaxX2, MinY2, MaxY2;
	MinX1 = Obj1.x - Width1 / 2; MaxX1 = Obj1.x + Width1 / 2;
	MinY1 = Obj1.z - Height1 / 2; MaxY1 = Obj1.z + Height1 / 2;

	MinX2 = Obj2.x - Width2 / 2; MaxX2 = Obj2.x + Width2 / 2;
	MinY2 = Obj2.z - Height2 / 2; MaxY2 = Obj2.z + Height2 / 2;

	return false;
}

void SceneBalloonPop::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey)
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

void SceneBalloonPop::RenderText(Mesh* mesh, std::string text, glm::vec3
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

void SceneBalloonPop::RenderTextOnScreen(Mesh* mesh, std::string
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

void SceneBalloonPop::Material(GEOMETRY_TYPE obj, float AmR, float AmG, float AmB, float DifA, float DifG, float DifB, float SpA, float SpG, float SpB, float Shiny)
{
	meshList[obj]->material.kAmbient = glm::vec3(AmR, AmG, AmB);
	meshList[obj]->material.kDiffuse = glm::vec3(DifA, DifG, DifB);
	meshList[obj]->material.kSpecular = glm::vec3(SpA, SpG, SpB);
	meshList[obj]->material.kShininess = Shiny;
}

void SceneBalloonPop::RenderSkyBox() {
	modelStack.PushMatrix();
	modelStack.Translate(0, 100, 0);
	{

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
}