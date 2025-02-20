#pragma once
#include "SceneArchery.h"
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

SceneArchery::SceneArchery() :
	m_vertexArrayID(0),
	m_programID(0),
	meshList(),
	m_parameters(),
	camera(),
	projType(1),
	modelStack(),
	viewStack(),
	projectionStack(),
	light(),
	enableLight(false),
	app(),
	m_arrowPower(0.0f),
	m_maxArrowPower(100.0f),
	m_powerChargeRate(2.0f),
	m_isChargingShot(false),
	m_arrowsLeft(10),    // Start with 10 arrows
	m_playerScore(0)     // Start with 0 score

{
	// Additional initialization if needed
	for (int i = 0; i < NUM_GEOMETRY; ++i) {
		meshList[i] = nullptr;
	}

	// Initialize all parameters to 0
	for (int i = 0; i < U_TOTAL; ++i) {
		m_parameters[i] = 0;
	}
}

SceneArchery::~SceneArchery()
{
}

void SceneArchery::Init()
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
	meshList[GEO_PLANE]->textureID = LoadTGA("Images//floorcircus.tga");
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("STAMINA_BAR", glm::vec3(1, 1, 1), 1.f);

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Images//archery_room.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Images//archery_room.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Images//tenttop.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Images//tenttop.tga");
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Images//archery_room.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Images//archery_room.tga");

	// 16 x 16 is the number of columns and rows for the text
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Images//calibri.tga");

	meshList[GEO_TARGET] = MeshBuilder::GenerateOBJ("Target",
		"Models//10480_Archery_target_v1_max2011_iteration-2.obj");
	meshList[GEO_TARGET]->textureID = LoadTGA("Images//archery_target_texture.tga");

	meshList[GEO_ARROW] = MeshBuilder::GenerateOBJ("Arrow",
		"Models//dart.obj");
	meshList[GEO_ARROW]->textureID = LoadTGA("Images//arrow.tga");

	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateQuad("Crosshair", glm::vec3(1, 1, 1), 1.f);

	meshList[GEO_CARPET] = MeshBuilder::GenerateOBJ("Carpet",
		"Models//3DBK_100121.obj");
	meshList[GEO_CARPET]->textureID = LoadTGA("Images//3DBK_100121_rug_Diffuse.tga");

	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	projectionStack.LoadMatrix(projection);

	glUniform1i(m_parameters[U_NUMLIGHTS], NUM_LIGHTS);

	//// Spotlight for Target 1 (center target)
	//light[0].position = glm::vec3(0, 20, 10);  // Position above first target
	//light[0].color = glm::vec3(1, 1, 1);      // White light
	//light[0].type = Light::LIGHT_SPOT;
	//light[0].power = 2.0f;
	//light[0].kC = 1.f;
	//light[0].kL = 0.01f;
	//light[0].kQ = 0.001f;
	//light[0].cosCutoff = 200.f;          // Narrow beam
	//light[0].cosInner = 10.f;           // Softer inner cone
	//light[0].exponent = 3.f;
	//light[0].spotDirection = glm::vec3(0.f, -0.5f, -1.f);  // Points diagonally down in front of the target

	//// Spotlight for Target 2 (right target at 20, 0, 20)
	//light[1].position = glm::vec3(20, 20, 30);
	//light[1].color = glm::vec3(1, 1, 1);
	//light[1].type = Light::LIGHT_SPOT;
	//light[1].power = 2.0f;
	//light[1].kC = 1.f;
	//light[1].kL = 0.01f;
	//light[1].kQ = 0.001f;
	//light[1].cosCutoff = 200.f;
	//light[1].cosInner = 10.f;
	//light[1].exponent = 3.f;
	//light[1].spotDirection = glm::vec3(0.f, -0.5f, -1.f);  // Points diagonally down in front of the target

	//// Spotlight for Target 3 (left target at -20, 0, 30)
	//light[2].position = glm::vec3(-20, 20, 40);
	//light[2].color = glm::vec3(1, 1, 1);
	//light[2].type = Light::LIGHT_SPOT;
	//light[2].power = 2.0f;
	//light[2].kC = 1.f;
	//light[2].kL = 0.01f;
	//light[2].kQ = 0.001f;
	//light[2].cosCutoff = 200.f;
	//light[2].cosInner = 10.f;
	//light[2].exponent = 3.f;
	//light[2].spotDirection = glm::vec3(0.f, -0.5f, -1.f);  // Points diagonally down in front of the target


	// Spotlight for Target 1 (center target)
	light[0].position = glm::vec3(0, 20, -40);  // Position above center target
	light[0].color = glm::vec3(1, 1, 1);
	light[0].type = Light::LIGHT_SPOT;
	light[0].power = 1.0f;                     // Increased power
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = 90.f;                // Wider cutoff angle
	light[0].cosInner = 60.f;                 // Wider inner angle
	light[0].exponent = 2.f;
	light[0].spotDirection = glm::vec3(0.f, -1.f, 0.f);  // Point downward

	// Spotlight for Target 2 (right target)
	light[1].position = glm::vec3(20, 20, -30);
	light[1].color = glm::vec3(1, 1, 1);
	light[1].type = Light::LIGHT_SPOT;
	light[1].power = 1.0f;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = 90.f;
	light[1].cosInner = 60.f;
	light[1].exponent = 2.f;
	light[1].spotDirection = glm::vec3(0.f, -1.f, 0.f);

	// Spotlight for Target 3 (left target)
	light[2].position = glm::vec3(-20, 20, -20);
	light[2].color = glm::vec3(1, 1, 1);
	light[2].type = Light::LIGHT_SPOT;
	light[2].power = 1.0f;
	light[2].kC = 1.f;
	light[2].kL = 0.01f;
	light[2].kQ = 0.001f;
	light[2].cosCutoff = 90.f;
	light[2].cosInner = 60.f;
	light[2].exponent = 2.f;
	light[2].spotDirection = glm::vec3(0.f, -1.f, 0.f);


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

//
//void SceneArchery::HandleArrowInput() {
//	// When left mouse button is held down
//	if (MouseController::GetInstance()->IsButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
//		if (!m_isChargingShot) {
//			// Start charging
//			m_isChargingShot = true;
//			m_arrowPower = 0.0f;
//		}
//		else {
//			// Continue charging, increase power
//			m_arrowPower = std::min<float>(m_arrowPower + m_powerChargeRate, m_maxArrowPower);
//		}
//	}
//	// When left mouse button is released
//	else if (MouseController::GetInstance()->IsButtonUp(GLFW_MOUSE_BUTTON_LEFT)) {
//		if (m_isChargingShot) {
//			// Fire the arrow with current power
//			FireArrow();
//			// Reset charging state
//			m_isChargingShot = false;
//			m_arrowPower = 0.0f;
//		}
//	}
//}
//
//void SceneArchery::FireArrow() {
//	// Only fire if player has arrows left
//	if (m_arrowsLeft <= 0) {
//		return;  // Don't fire if no arrows left
//	}
//
//	// Attempt to find an inactive arrow
//	for (int i = 0; i < MAX_ARROWS; ++i) {
//		if (!arrows[i].isActive) {
//			// Decrease arrows left
//			m_arrowsLeft--;
//
//			// Calculate fire direction based on camera
//			glm::vec3 fireDirection = glm::normalize(camera.target - camera.position);
//
//			// Rest of the firing code remains the same
//			float minSpeed = 1.0f;
//			float maxSpeed = 40.0f;
//			float powerPercentage = m_arrowPower / m_maxArrowPower;
//			float speedMultiplier = powerPercentage * powerPercentage;
//			float finalSpeed = minSpeed + (maxSpeed - minSpeed) * speedMultiplier;
//
//			arrows[i].Fire(
//				camera.position,
//				fireDirection,
//				finalSpeed
//			);
//
//			arrows[i].AddForce(glm::vec3(0, 15.0f, 0));
//			break;
//		}
//	}
//}
//
//void SceneArchery::CheckArrowCollisions()
//{
//	struct TargetInfo {
//		glm::vec3 position;
//		glm::vec3 normal;
//		float radius;
//	};
//
//	TargetInfo targetPositions[3] = {
//		{glm::vec3(0, 0, -40), glm::vec3(0, 1, 0), 15.0f},
//		{glm::vec3(20, 0, -30), glm::vec3(0, 1, 0), 15.0f},
//		{glm::vec3(-20, 0, -20), glm::vec3(0, 1, 0), 15.0f}
//	};
//
//	for (int i = 0; i < MAX_ARROWS; ++i) {
//		if (arrows[i].isActive && !arrows[i].isStuck) {
//			for (int j = 0; j < 3; ++j) {
//				float distance = glm::length(arrows[i].pos - targetPositions[j].position);
//
//				if (distance < targetPositions[j].radius) {
//					// Increment score when arrow hits target
//					m_playerScore++;
//
//					arrows[i].StickToTarget(
//						targetPositions[j].position,
//						targetPositions[j].normal
//					);
//					break;
//				}
//			}
//		}
//	}
//}

void SceneArchery::Update(double dt)
{
	HandleKeyPress();

	//if (KeyboardController::GetInstance()->IsKeyDown('I'))
	//	light[0].position.z -= static_cast<float>(dt) * 5.f;
	//if (KeyboardController::GetInstance()->IsKeyDown('K'))
	//	light[0].position.z += static_cast<float>(dt) * 5.f;
	//if (KeyboardController::GetInstance()->IsKeyDown('J'))
	//	light[0].position.x -= static_cast<float>(dt) * 5.f;
	//if (KeyboardController::GetInstance()->IsKeyDown('L'))
	//	light[0].position.x += static_cast<float>(dt) * 5.f;
	//if (KeyboardController::GetInstance()->IsKeyDown('O'))
	//	light[0].position.y -= static_cast<float>(dt) * 5.f;
	//if (KeyboardController::GetInstance()->IsKeyDown('P'))
	//	light[0].position.y += static_cast<float>(dt) * 5.f;

	glm::vec3 targetPositions[3] = {
		glm::vec3(0, 0, -40),      // Center target
		glm::vec3(20, 0, -30),     // Right target
		glm::vec3(-20, 0, -20)     // Left target
	};

	for (int i = 0; i < NUM_LIGHTS; ++i)
	{
		// Calculate direction from light to target
		glm::vec3 direction = targetPositions[i] - light[i].position;
		light[i].spotDirection = -glm::normalize(direction);  // Note the negative sign
	}


	camera.Update(dt);


	//// Handle arrow input
	//HandleArrowInput();

	//// Update all active arrows
	//for (int i = 0; i < MAX_ARROWS; ++i) {
	//	if (arrows[i].isActive) {
	//		// Cast dt to float for physics calculations
	//		arrows[i].Update(static_cast<float>(dt));
	//	}
	//}

	//// Check for arrow collisions with targets
	//CheckArrowCollisions();

}


void SceneArchery::Render()
{
	// Clear color buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Load view matrix stack and set it with camera position, target position and up direction
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);

	// Load identity matrix into the model stack
	modelStack.LoadIdentity();

	//Light
	for (int i = 0; i < NUM_LIGHTS; i++) {
		UNIFORM_TYPE pos = U_LIGHT0_POSITION;
		UNIFORM_TYPE color = U_LIGHT0_COLOR;
		UNIFORM_TYPE type = U_LIGHT0_TYPE;
		UNIFORM_TYPE power = U_LIGHT0_POWER;
		UNIFORM_TYPE kC = U_LIGHT0_KC;
		UNIFORM_TYPE kL = U_LIGHT0_KL;
		UNIFORM_TYPE kQ = U_LIGHT0_KQ;
		UNIFORM_TYPE spotdir = U_LIGHT0_SPOTDIRECTION;
		UNIFORM_TYPE coscutoff = U_LIGHT0_COSCUTOFF;
		UNIFORM_TYPE cosinner = U_LIGHT0_COSINNER;
		UNIFORM_TYPE exponent = U_LIGHT0_EXPONENT;

		// Adjust uniform locations based on light index
		switch (i) {
		case 1:
			pos = U_LIGHT1_POSITION;
			color = U_LIGHT1_COLOR;
			type = U_LIGHT1_TYPE;
			power = U_LIGHT1_POWER;
			kC = U_LIGHT1_KC;
			kL = U_LIGHT1_KL;
			kQ = U_LIGHT1_KQ;
			spotdir = U_LIGHT1_SPOTDIRECTION;
			coscutoff = U_LIGHT1_COSCUTOFF;
			cosinner = U_LIGHT1_COSINNER;
			exponent = U_LIGHT1_EXPONENT;
			break;
		case 2:
			pos = U_LIGHT2_POSITION;
			color = U_LIGHT2_COLOR;
			type = U_LIGHT2_TYPE;
			power = U_LIGHT2_POWER;
			kC = U_LIGHT2_KC;
			kL = U_LIGHT2_KL;
			kQ = U_LIGHT2_KQ;
			spotdir = U_LIGHT2_SPOTDIRECTION;
			coscutoff = U_LIGHT2_COSCUTOFF;
			cosinner = U_LIGHT2_COSINNER;
			exponent = U_LIGHT2_EXPONENT;
			break;
		}

		// Calculate light position in camera space
		glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[i].position, 1);
		glUniform3fv(m_parameters[pos], 1, glm::value_ptr(lightPosition_cameraspace));

		// Calculate spotlight direction in camera space
		glm::vec3 spotDirection_cameraspace = viewStack.Top() * glm::vec4(light[i].spotDirection, 0);
		glUniform3fv(m_parameters[spotdir], 1, glm::value_ptr(spotDirection_cameraspace));

		// Set other light parameters
		glUniform3fv(m_parameters[color], 1, &light[i].color.r);
		glUniform1i(m_parameters[type], light[i].type);
		glUniform1f(m_parameters[power], light[i].power);
		glUniform1f(m_parameters[kC], light[i].kC);
		glUniform1f(m_parameters[kL], light[i].kL);
		glUniform1f(m_parameters[kQ], light[i].kQ);
		glUniform1f(m_parameters[coscutoff], cosf(glm::radians<float>(light[i].cosCutoff)));
		glUniform1f(m_parameters[cosinner], cosf(glm::radians<float>(light[i].cosInner)));
		glUniform1f(m_parameters[exponent], light[i].exponent);
	}


	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_AXES], false);
	modelStack.PopMatrix();

	// Render all three light sources
	for (int i = 0; i < NUM_LIGHTS; ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate(light[i].position.x, light[i].position.y, light[i].position.z);
		modelStack.Scale(0.1f, 0.1f, 0.1f);
		RenderMesh(meshList[GEO_SPHERE], false);
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Scale(100.f, 1.f, 100.f);
	modelStack.Rotate(-90.f, 1, 0, 0);
	meshList[GEO_PLANE]->material.kAmbient = glm::vec3(0.7f, 0.7f, 0.7f);
	meshList[GEO_PLANE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_PLANE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_PLANE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_PLANE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -40);
	modelStack.Rotate(45.f, 1, 0, 0);
	modelStack.Rotate(23.f, 0, 1, 0);
	modelStack.Rotate(-15.f, 1, 0, 0);
	modelStack.Scale(0.19f, 0.17f, 0.19f);
	meshList[GEO_TARGET]->material.kAmbient = glm::vec3(0.7f, 0.7f, 0.7f);
	meshList[GEO_TARGET]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_TARGET]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_TARGET]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_TARGET], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(20, 0, -30);
	modelStack.Rotate(45.f, 1, 0, 0);
	modelStack.Rotate(23.f, 0, 1, 0);
	modelStack.Rotate(-15.f, 1, 0, 0);
	modelStack.Scale(0.19f, 0.17f, 0.19f);
	meshList[GEO_TARGET]->material.kAmbient = glm::vec3(0.7f, 0.7f, 0.7f);
	meshList[GEO_TARGET]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_TARGET]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_TARGET]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_TARGET], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-20, 0, -20);
	modelStack.Rotate(45.f, 1, 0, 0);
	modelStack.Rotate(23.f, 0, 1, 0);
	modelStack.Rotate(-15.f, 1, 0, 0);
	modelStack.Scale(0.19f, 0.17f, 0.19f);
	meshList[GEO_TARGET]->material.kAmbient = glm::vec3(0.7f, 0.7f, 0.7f);
	meshList[GEO_TARGET]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_TARGET]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_TARGET]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_TARGET], true);
	modelStack.PopMatrix();


	// Render all active arrows
	for (int i = 0; i < MAX_ARROWS; ++i) {
		if (arrows[i].isActive) {
			modelStack.PushMatrix();

			// Translate to arrow's current position
			if (arrows[i].isStuck) {
				// If stuck, use the stuck position
				modelStack.Translate(
					arrows[i].stuckPosition.x,
					arrows[i].stuckPosition.y,
					arrows[i].stuckPosition.z
				);
			}
			else {
				// If flying, use current physics position
				modelStack.Translate(
					arrows[i].pos.x,
					arrows[i].pos.y,
					arrows[i].pos.z
				);
			}

			// Scale and render arrow mesh
			modelStack.Scale(0.1f, 0.1f, 0.1f);
			meshList[GEO_ARROW]->material.kAmbient = glm::vec3(0.5f, 0.5f, 0.5f);
			meshList[GEO_ARROW]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
			meshList[GEO_ARROW]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
			meshList[GEO_ARROW]->material.kShininess = 1.0f;
			RenderMesh(meshList[GEO_ARROW], true);
			modelStack.PopMatrix();
		}
	}


	// CARPET IS PLAYER'S SPAWN POINT!
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 50);
	modelStack.Scale(0.25f, 0.17f, 0.25f);
	meshList[GEO_CARPET]->material.kAmbient = glm::vec3(0.7f, 0.7f, 0.7f);
	meshList[GEO_CARPET]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_CARPET]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_CARPET]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_CARPET], true);
	modelStack.PopMatrix();

	
	RenderSkyBox();

	// Render vertical line of crosshair
	RenderMeshOnScreen(meshList[GEO_CROSSHAIR], 400, 300, 2, 20);  // Thin vertical line
	// Render horizontal line of crosshair
	RenderMeshOnScreen(meshList[GEO_CROSSHAIR], 400, 300, 20, 2);  // Thin horizontal line



	// Render "Power:" text
	RenderTextOnScreen(meshList[GEO_TEXT],
		"Power:",
		glm::vec3(1, 1, 1),  // White color
		25,                   // Size
		560, 60);            // Position bottom right

	// Display numerical power value
	std::string powerText = std::to_string(static_cast<int>(m_arrowPower));
	RenderTextOnScreen(meshList[GEO_TEXT],
		powerText,
		glm::vec3(1, 1, 1),  // White color
		25,                   // Size
		710, 60);            // Position (right of the power bar)


	// Calculate power percentage (0.0 to 1.0)
	float powerPercentage = m_arrowPower / m_maxArrowPower;

	// Calculate scaled width
	float maxWidth = 150.0f;
	float scaledWidth = maxWidth * powerPercentage;

	// Render the power bar with the scaled width
	RenderMeshOnScreen(meshList[GEO_QUAD], 570 + scaledWidth / 2, 30, scaledWidth, 20);



	// Render objective text at top left
	RenderTextOnScreen(meshList[GEO_TEXT],
		"Objective: Hit the targets with ",
		glm::vec3(1, 1, 1),  // White color
		25,                   // Size
		10, 550);            // Position (x,y)

	// Render objective text at top left
	RenderTextOnScreen(meshList[GEO_TEXT],
		"your arrows!",
		glm::vec3(1, 1, 1),  // White color
		25,                   // Size
		10, 510);            // Position (x,y)

	// Render score counter
	std::string scoreText = "Score: " + std::to_string(m_playerScore);
	RenderTextOnScreen(meshList[GEO_TEXT],
		scoreText,
		glm::vec3(1, 1, 0),  // Yellow color
		25,                   // Size
		10, 470);            // Position (x,y)

	// Render arrows left counter at bottom left
	std::string arrowsText = "Arrows Left: " + std::to_string(m_arrowsLeft);
	RenderTextOnScreen(meshList[GEO_TEXT],
		arrowsText,
		glm::vec3(1, 0, 0),  // Red color
		25,                   // Size
		10, 30);             // Position (x,y)
}

void SceneArchery::RenderMesh(Mesh* mesh, bool enableLight)
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


void SceneArchery::Exit()
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

void SceneArchery::HandleKeyPress()
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

	//if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_TAB))
	//{
	//	if (light[0].type == Light::LIGHT_POINT) {
	//		light[0].type = Light::LIGHT_DIRECTIONAL;
	//		std::cout << "DIRECTIONAL" << std::endl;
	//	}
	//	else if (light[0].type == Light::LIGHT_DIRECTIONAL) {
	//		light[0].type = Light::LIGHT_SPOT;
	//		std::cout << "LIGHT SPOT" << std::endl;
	//	}
	//	else {
	//		light[0].type = Light::LIGHT_POINT;
	//		std::cout << "LIGHT POINT" << std::endl;
	//	}

	//	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	//}

}


bool SceneArchery::OverlapAABB2AABB(glm::vec3 Obj1, const int Width1, const int Height1,
	glm::vec3 Obj2, const int Width2, const int Height2)
{

	float MinX1, MaxX1, MinY1, MaxY1, MinX2, MaxX2, MinY2, MaxY2;
	MinX1 = Obj1.x - Width1 / 2; MaxX1 = Obj1.x + Width1 / 2;
	MinY1 = Obj1.z - Height1 / 2; MaxY1 = Obj1.z + Height1 / 2;

	MinX2 = Obj2.x - Width2 / 2; MaxX2 = Obj2.x + Width2 / 2;
	MinY2 = Obj2.z - Height2 / 2; MaxY2 = Obj2.z + Height2 / 2;

	return false;
}

void SceneArchery::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey)
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

void SceneArchery::RenderText(Mesh* mesh, std::string text, glm::vec3
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

void SceneArchery::RenderTextOnScreen(Mesh* mesh, std::string
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

void SceneArchery::Material(GEOMETRY_TYPE obj, float AmR, float AmG, float AmB, float DifA, float DifG, float DifB, float SpA, float SpG, float SpB, float Shiny)
{
	meshList[obj]->material.kAmbient = glm::vec3(AmR, AmG, AmB);
	meshList[obj]->material.kDiffuse = glm::vec3(DifA, DifG, DifB);
	meshList[obj]->material.kSpecular = glm::vec3(SpA, SpG, SpB);
	meshList[obj]->material.kShininess = Shiny;
}

void SceneArchery::RenderSkyBox() {
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