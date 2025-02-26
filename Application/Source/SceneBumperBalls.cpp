#pragma once
#include "SceneBumperBalls.h"
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


SceneBumperBalls::SceneBumperBalls()
{
}

SceneBumperBalls::~SceneBumperBalls()
{
}

void SceneBumperBalls::Init()
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
	camera.Init(glm::vec3(-16, 13, -3), glm::vec3(-10, 4, 0), glm::vec3(0, 1, 0));
	 
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
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("Sphere", glm::vec3(1, 1, 1), 1.f);
	meshList[GEO_CYLINDER] = MeshBuilder::GenerateCylinder("Cylinder", glm::vec3(1, 1, 1), 20.f,1.f,1.f);
	meshList[GEO_BEACHBALL] = MeshBuilder::GenerateOBJMTL("PlayerBall", "Models//beach-ball.obj", "Models//beach-ball.mtl");
	meshList[GEO_BEACHBALL]->textureID = LoadTGA("Images//BeachBall_albedo.tga");
	meshList[GEO_BARREL] = MeshBuilder::GenerateOBJMTL("PlayerBall", "Models//Oil_barrel.obj", "Models//Oil_barrel.mtl");
	meshList[GEO_BARREL]->textureID = LoadTGA("Images//Oil_barrel_Albedo.tga");
	meshList[GEO_BASKETBALL] = MeshBuilder::GenerateOBJMTL("PlayerBall", "Models//basketballball.obj", "Models//basketballball.mtl");
	meshList[GEO_BASKETBALL]->textureID = LoadTGA("Images//basketballball_bball_Mat_Diffuse.tga");

	//GEO_BASKETBALL
	//skybox
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Images//circus_skybox.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Images//circus_skybox.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Images//tenttop.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("Plane",glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Images//floorcircus.tga");
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Images//circus_skybox.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Images//circus_skybox.tga");

	//UI
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16,16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Images//calibri.tga");
	meshList[GEO_KEY_E] = MeshBuilder::GenerateQuad("KeyE", glm::vec3(1.f, 1.f, 1.f), 2.f);
	meshList[GEO_KEY_E]->textureID = LoadTGA("Images//keyboard_key_e.tga");
	meshList[GEO_UI] = MeshBuilder::GenerateQuad("UIBox", glm::vec3(0.12f, 0.12f, 0.12f), 10.f);


	//GEO_KEY_E

	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	projectionStack.LoadMatrix(projection);

	glUniform1i(m_parameters[U_NUMLIGHTS], NUM_LIGHTS);

	light[0].position = glm::vec3(83, 129, 5.6);
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

	light[1].position = glm::vec3(0, 45, 0);
	light[1].color = glm::vec3(0.8, 0.8, 1);
	light[1].type = Light::LIGHT_SPOT;
	light[1].power = 0.4f;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = 70.f;
	light[1].cosInner = 50.f;
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

	player.push_back(PlayerBall(100, 6.f, GameObject::SPHERE));
	spherelist.push_back(Sphere(3,6.f,GameObject::SPHERE));
	spherelist.push_back(Sphere(4,6.f,GameObject::SPHERE));
	spherelist.push_back(Sphere(5,6.f,GameObject::SPHERE));
	spherelist.push_back(Sphere(6,6.f,GameObject::SPHERE));

	player[0].pos = glm::vec3{ -16,3,-3 };
	player[0].bounciness = 0.2f;
	spherelist[0].pos = glm::vec3{ 10,3,10 };
	spherelist[1].pos = glm::vec3{ -10,3,15 };
	spherelist[2].pos = glm::vec3{ 10,0,-10};
	spherelist[3].pos = glm::vec3{ 0,3,-15 };
	for (int i = 0; i < spherelist.size(); ++i) {
		spherelist[i].bounciness = 0.2f;
	}
	cylinderlist.push_back(Cylinder(101, GameObject::CYLINDER,70.f,28.f));
	cylinderlist[0].pos = glm::vec3{0,-36,0};
	cylinderlist[0].mass = 0.f;
	cylinderlist[0].bounciness = 0.f;

	newcampos = glm::vec3{ player[0].pos.x,player[0].pos.y + 10.f,player[0].pos.z };

	camera.enableFNAF = false;
	camera.allowMovement = false; // 
	camera.allowJump = false; // 
	camera.allowSprint = false; // 
	camera.allowCrouch = false; // 
	camera.allowProne = false; // 
	camera.allowLocomotiveTilt = false;
	camera.allowLocomotiveBop = false;
}

void SceneBumperBalls::Update(double dt)
{

	HandleKeyPress();
	const float SPEED = 30.f;
	if (KeyboardController::GetInstance()->IsKeyDown('I'))
		light[1].position.z -= static_cast<float>(dt) * SPEED;
	if (KeyboardController::GetInstance()->IsKeyDown('K'))
		light[1].position.z += static_cast<float>(dt) * SPEED;
	if (KeyboardController::GetInstance()->IsKeyDown('J'))
		light[1].position.x -= static_cast<float>(dt) * SPEED;
	if (KeyboardController::GetInstance()->IsKeyDown('L'))
		light[1].position.x += static_cast<float>(dt) * SPEED;
	if (KeyboardController::GetInstance()->IsKeyDown('O'))
		light[1].position.y -= static_cast<float>(dt) * SPEED;
	if (KeyboardController::GetInstance()->IsKeyDown('P'))
		light[1].position.y += static_cast<float>(dt) * SPEED;

	//Instructions 
	if (KeyboardController::GetInstance()->IsKeyPressed('E') && !gamestart)
		isObjectiveRead = true;

	if (isObjectiveRead && !gamestart && !gamelose && !gamewin ) {
		countdown -= dt;
		if (countdown < 0.f)
			gamestart = true;
	}

	glm::vec3 viewDir = glm::normalize(camera.target - camera.pos);
	//Game 
	if (gamestart) {

		UpdateMovement(dt);

		//ball rotation
		player[0].xrot += player[0].xrotvel * dt;
		player[0].zrot += player[0].zrotvel * dt;

		for (int i = 0; i < spherelist.size(); ++i)
		{
			spherelist[i].xrot += spherelist[i].xrotvel * dt;
			spherelist[i].zrot += spherelist[i].zrotvel * dt;
		}

		CollisionData cd;
		newcampos = glm::vec3{ player[0].pos.x,player[0].pos.y + 10.f,player[0].pos.z };
		camera.pos = newcampos;
		camera.target = camera.pos + viewDir * 1.2f;

		//my friend here justin he's already taken and he's cracked at bumper balls my guy

		//spherelist[0].target = glm::vec3(player[0].pos.x, 0, player[0].pos.z);
		//spherelist[1].target = spherelist[0].pos + ((player[0].pos - spherelist[0].pos) / 2.f);

		float dispDist = (((player[0].pos.x - spherelist[0].pos.x) * (player[0].pos.x - spherelist[0].pos.x)) + ((player[0].pos.z - spherelist[0].pos.z) * (player[0].pos.z - spherelist[0].pos.z)));

		//spherelist[0].target = (player[0].pos * 0.5f) + ((player[0].vel * 0.1f) * (abs(dispDist * 0.3f) / (50.f - spherelist[0].vel)));
		spherelist[0].target = (player[0].pos * 0.5f) + ((player[0].vel * 0.25f) * (abs(dispDist * 0.1f) / (60.f - spherelist[0].vel)));

		if (isballactive[1] || isballactive[3])
		{
			spherelist[0].target = -spherelist[0].vel * 5.f;
		}
		
		if (abs(spherelist[0].target.x) > 25)
		{
			spherelist[0].target.x *= 0.5f;
			if (abs(spherelist[0].target.x) > 25)
			{
				spherelist[0].target = glm::normalize(spherelist[0].target) * 10.f;
			}
		}
		if (abs(spherelist[0].target.z) > 25)
		{
			spherelist[0].target.z *= 0.5f;
			if (abs(spherelist[0].target.z) > 25)
			{
				spherelist[0].target = glm::normalize(spherelist[0].target) * 10.f;
			}
		}

		spherelist[1].target = -player[0].pos * 0.5f;

		if (isballactive[0] || isballactive[3])
		{
			spherelist[1].target = (player[0].pos * 0.5f) + ((player[0].vel * 0.1f) * (abs(dispDist * 0.3f) / (50.f - spherelist[1].vel)));
		}

		spherelist[2].target = spherelist[0].pos * 0.9f;
		spherelist[3].target = spherelist[2].pos * 1.f;

		float enemyNerf = 1.5f;
		const float BALL_ROT = 150.f;

		//all AI enemy movement systems
		for (int i = 0; i < spherelist.size(); ++i)
		{
			spherelist[i].lat  = spherelist[i].pos.x < spherelist[i].target.x ? 'R' : 'L';
			spherelist[i].vert = spherelist[i].pos.z < spherelist[i].target.z ? 'U' : 'D';
			
			if (spherelist[i].lat == 'R')
			{
				spherelist[i].AddForce(glm::vec3(PLAYER_SPEED / enemyNerf, 0, 0));
				spherelist[i].zrotvel += BALL_ROT * dt;

				/*if (spherelist[i].vel.x < 0)
				{
					spherelist[i].AddForce(glm::vec3(PLAYER_SPEED / enemyNerf, 0, 0));
					spherelist[i].zrotvel += BALL_ROT * dt;
				}*/
			}
			else
			{
				spherelist[i].AddForce(glm::vec3(-PLAYER_SPEED / enemyNerf, 0, 0));
				spherelist[i].zrotvel -= BALL_ROT * dt;

				/*if (spherelist[i].vel.x > 0)
				{
					spherelist[i].AddForce(glm::vec3(-PLAYER_SPEED / enemyNerf, 0, 0));
					spherelist[i].zrotvel -= BALL_ROT * dt;
				}*/
			}
			if (spherelist[i].vert == 'U')
			{
				spherelist[i].AddForce(glm::vec3(0, 0, PLAYER_SPEED / enemyNerf));
				spherelist[i].xrotvel += BALL_ROT * dt;

				/*if (spherelist[i].vel.z < 0)
				{
					spherelist[i].AddForce(glm::vec3(0, 0, PLAYER_SPEED / enemyNerf));
					spherelist[i].xrotvel += BALL_ROT * dt;
				}*/
			}
			else
			{
				spherelist[i].AddForce(glm::vec3(0, 0, -PLAYER_SPEED / enemyNerf));
				spherelist[i].xrotvel -= BALL_ROT * dt;

				if (spherelist[i].vel.z > 0)
				{
					spherelist[i].AddForce(glm::vec3(0, 0, -PLAYER_SPEED / enemyNerf));
					spherelist[i].xrotvel -= BALL_ROT * dt;
				}
			}
			//spherelist[i].lat  == 'R' ? spherelist[i].AddForce(glm::vec3(PLAYER_SPEED, 0, 0)) : spherelist[i].AddForce(glm::vec3(-PLAYER_SPEED, 0, 0));
			//spherelist[i].vert == 'U' ? spherelist[i].AddForce(glm::vec3(0, 0, PLAYER_SPEED)) : spherelist[i].AddForce(glm::vec3(0, 0, -PLAYER_SPEED));
		}

		//Ball to ball
		for (int i = 0; i < spherelist.size() - 1; ++i) {
			for (int j = i + 1; j < spherelist.size(); ++j) {
				if (OverlapSphere2Sphere(spherelist[i], spherelist[i].radius, spherelist[j], spherelist[j].radius, cd))
					ResolveCollision(cd);
			}
		}

		//Playerball to ball //do the player thingy 
		for (int i = 0; i < spherelist.size(); i++) {
			if (OverlapSphere2Sphere(player[0], player[0].radius, spherelist[i], spherelist[i].radius, cd))
				ResolveCollision(cd);
		}

		//Cylinder to ball 
		for (int i = 0; i < spherelist.size(); ++i) {
			if (OverlapSphere2Cylinder(spherelist[i], spherelist[i].radius, cylinderlist[0], cylinderlist[0].radius, cylinderlist[0].height, cd))
				ResolveCollision(cd);
		}

		//Cylinder to playerBall
		if (OverlapSphere2Cylinder(player[0], player[0].radius, cylinderlist[0], cylinderlist[0].radius, cylinderlist[0].height, cd))
			ResolveCollision(cd);

		//Check if ball still in game 
		for (int i = 0; i < spherelist.size(); i++) {
			if (spherelist[i].pos.y < -50.f)
				isballactive[i] = false;
				//ballcount--;
				//spherelist.erase(spherelist.begin() + i);
		}

		player[0].AddForce(100.f * glm::vec3{ 0,-1,0 });
		player[0].UpdatePhysics(dt);
		for (int i = 0; i < spherelist.size(); i++) {
			//gravity 
			spherelist[i].AddForce(100.f * glm::vec3{ 0,-1,0 });
			spherelist[i].UpdatePhysics(dt);
		}
		for (int i = 0; i < cylinderlist.size(); i++) {
			cylinderlist[i].UpdatePhysics(dt);
		}
		camera.Update(dt);
	}

	//Check if player lose 
	if (player[0].pos.y < -50.f)
		gamelose = true;

	//check if player win 
	if (!isballactive[0]&& !isballactive[1] && !isballactive[2] && !isballactive[3])
		gamewin = true;

	if (gamelose || gamewin) {
		gamestart = false;
	}

	if (KeyboardController::GetInstance()->IsKeyPressed('R') && !gamestart)
		InitGame();


}

void SceneBumperBalls::Render()
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
	RenderMesh(meshList[GEO_AXES], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(light[1].position.x, light[1].position.y, light[1].position.z);
	modelStack.Scale(1.f, 1.f,1.f);
	RenderMesh(meshList[GEO_SPHERE], false);
	modelStack.PopMatrix();

	//Player 
	modelStack.PushMatrix();
	modelStack.Translate(player[0].pos.x, player[0].pos.y, player[0].pos.z);
	modelStack.Rotate(player[0].xrot, 1, 0, 0);
	modelStack.Rotate(player[0].zrot,0,0,1);
	modelStack.Scale(0.05*player[0].radius, 0.05* player[0].radius, 0.05*player[0].radius);
	meshList[GEO_BEACHBALL]->material.kAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
	meshList[GEO_BEACHBALL]->material.kDiffuse = glm::vec3(0.6f, 0.6f, 0.6f);
	meshList[GEO_BEACHBALL]->material.kSpecular = glm::vec3(0.3f, 0.3f, 0.3f);
	meshList[GEO_BEACHBALL]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_BEACHBALL], true);
	modelStack.PopMatrix();


	for (int i = 0; i < spherelist.size(); i++) {
		modelStack.PushMatrix();
		modelStack.Translate(spherelist[i].pos.x, spherelist[i].pos.y, spherelist[i].pos.z);
		modelStack.Rotate(spherelist[i].xrot, 1, 0, 0);
		modelStack.Rotate(spherelist[i].zrot, 0, 0, 1);
		modelStack.Scale(0.05*spherelist[i].radius, 0.05*spherelist[i].radius, 0.05*spherelist[i].radius);
		meshList[GEO_BEACHBALL]->material.kAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
		meshList[GEO_BEACHBALL]->material.kDiffuse = glm::vec3(0.6f, 0.6f, 0.6f);
		meshList[GEO_BEACHBALL]->material.kSpecular = glm::vec3(i, 0.3f, 0.3f);
		meshList[GEO_BEACHBALL]->material.kShininess = 1.f;
		RenderMesh(meshList[GEO_BEACHBALL], true);
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(0,-100,0);
	modelStack.Scale(0.8, 1.f, 0.8f);
	meshList[GEO_BARREL]->material.kAmbient = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_BARREL]->material.kDiffuse = glm::vec3(0.7f, 0.7f, 0.7f);
	meshList[GEO_BARREL]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_BARREL]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_BARREL], true);
	modelStack.PopMatrix();


	if (!isObjectiveRead) { // Render Objective
		RenderMeshOnScreen(meshList[GEO_UI], 400, 320, 45, 30);
		RenderTextOnScreen(meshList[GEO_TEXT], "- BUMPER BALLS -", glm::vec3(1, 1, 0), 25, 200, 430);
		RenderTextOnScreen(meshList[GEO_TEXT], "- Use WASD to control the ball", glm::vec3(1, 1, 1), 13, 195, 380);
		RenderTextOnScreen(meshList[GEO_TEXT], "- The controls are inversed!", glm::vec3(1, 1, 1), 14, 205, 350);
		RenderTextOnScreen(meshList[GEO_TEXT], "- Pressing S moves you fowards", glm::vec3(1, 1, 1), 14, 205, 320);
		RenderTextOnScreen(meshList[GEO_TEXT], "- Don't fall off the edge!", glm::vec3(1, 1, 1), 14, 210, 290);
		RenderTextOnScreen(meshList[GEO_TEXT], "- Knock all your opponents off ", glm::vec3(1, 1, 1), 14, 190, 260);
		RenderTextOnScreen(meshList[GEO_TEXT], "  to win!", glm::vec3(1, 1, 1), 14, 250, 240);

		RenderMeshOnScreen(meshList[GEO_KEY_E], 310, 210, 15, 15);
		RenderTextOnScreen(meshList[GEO_TEXT], "Continue", glm::vec3(1, 1, 1), 20, 340, 200);
	}
	else if (countdown > 0.f && isObjectiveRead){
		if (countdown < 1.f)
			RenderTextOnScreen(meshList[GEO_TEXT], "Start!", glm::vec3(0, 1, 0), 40, 350, 400);
		else
			RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(static_cast<int>(countdown)), glm::vec3(0, 1, 0), 40, 400, 400);
	}

	if (gamelose) {
		RenderMeshOnScreen(meshList[GEO_UI], 400, 320, 45, 30);
		RenderTextOnScreen(meshList[GEO_TEXT], "Game Over!", glm::vec3(1, 0, 0), 25, 220, 340);
		RenderTextOnScreen(meshList[GEO_TEXT], "Press R to restart", glm::vec3(1, 0, 0), 25, 190, 300);
	}
	if (gamewin) {
		RenderMeshOnScreen(meshList[GEO_UI], 400, 320, 45, 30);
		RenderTextOnScreen(meshList[GEO_TEXT], "Game Win!", glm::vec3(0, 1, 0), 25, 240, 340);
		RenderTextOnScreen(meshList[GEO_TEXT], "Returning ", glm::vec3(0, 1, 0), 25, 210, 300);
		RenderTextOnScreen(meshList[GEO_TEXT], "to carnival...", glm::vec3(0, 1, 0), 25, 210, 270);
	}
}

void SceneBumperBalls::InitGame() {
	//spherelist.clear(); //remove all the stuff
	//spherelist.push_back(Sphere(3, 6.f, GameObject::SPHERE));
	//spherelist.push_back(Sphere(4, 6.f, GameObject::SPHERE));
	//spherelist.push_back(Sphere(5, 6.f, GameObject::SPHERE));
	//spherelist.push_back(Sphere(6, 6.f, GameObject::SPHERE));
	//ballcount = 4;

	for (int i = 0; i < 4; i++) {
		isballactive[i] = true;
	}
	player[0].pos = glm::vec3{ -16,3,-3 };
	player[0].bounciness = 0.2f;
	player[0].vel = glm::vec3{ 0,0,0 };
	player[0].xrot = 0;
	player[0].zrot = 0;
	player[0].xrotvel = 0;
	player[0].zrotvel = 0;

	for (int i = 0; i < spherelist.size(); ++i)
	{
		spherelist[i].xrot = 0;
		spherelist[i].zrot = 0;
		spherelist[i].xrotvel = 0;
		spherelist[i].zrotvel = 0;
	}

	newcampos = glm::vec3{ player[0].pos.x,player[0].pos.y + 10.f,player[0].pos.z };
	camera.pos = newcampos;
	camera.target = glm::vec3(-10, 4, 0);

	spherelist[0].pos = glm::vec3{ 10,3,10 };
	spherelist[1].pos = glm::vec3{ -10,3,15 };
	spherelist[2].pos = glm::vec3{ 10,0,-10 };
	spherelist[3].pos = glm::vec3{ 0,3,-15 };

	for (int i = 0; i < spherelist.size(); i++) {
		spherelist[i].vel = glm::vec3{ 0,0,0 };
		spherelist[i].bounciness = 0.2f;
	}
	cylinderlist.push_back(Cylinder(101, GameObject::CYLINDER, 70.f, 28.f));
	cylinderlist[0].pos = glm::vec3{ 0,-36,0 };
	cylinderlist[0].mass = 0.f;
	cylinderlist[0].bounciness = 0.f;

	newcampos = glm::vec3{ player[0].pos.x,player[0].pos.y + 10.f,player[0].pos.z };

	camera.enableFNAF = false;
	camera.allowMovement = false; // 
	camera.allowJump = false; // 
	camera.allowSprint = false; // 
	camera.allowCrouch = false; // 
	camera.allowProne = false; // 
	camera.allowLocomotiveTilt = false;
	camera.allowLocomotiveBop = false;	
	gamewin = false;
	gamestart = false; 
	gamelose = false;
	countdown = 4.f;

}

void SceneBumperBalls::RenderMesh(Mesh* mesh, bool enableLight)
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


void SceneBumperBalls::Exit()
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

void SceneBumperBalls::HandleKeyPress()
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

void SceneBumperBalls::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey)
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

void SceneBumperBalls::RenderText(Mesh* mesh, std::string text, glm::vec3
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

void SceneBumperBalls::RenderTextOnScreen(Mesh* mesh, std::string
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

void SceneBumperBalls::Material(GEOMETRY_TYPE obj, float AmR, float AmG, float AmB, float DifA, float DifG, float DifB, float SpA, float SpG, float SpB, float Shiny)
{
	meshList[obj]->material.kAmbient = glm::vec3(AmR, AmG, AmB);
	meshList[obj]->material.kDiffuse = glm::vec3(DifA, DifG, DifB);
	meshList[obj]->material.kSpecular = glm::vec3(SpA, SpG, SpB);
	meshList[obj]->material.kShininess = Shiny;
}

void SceneBumperBalls::UpdateMovement(float dt)
{
	const float BALL_ROT = 150.f;
	glm::vec3 view = glm::normalize(camera.target - camera.pos); 
	glm::vec3 right = glm::normalize(glm::cross(view, glm::vec3{ 0,1,0 }));
	//INVERSED CONTROLS
	if (KeyboardController::GetInstance()->IsKeyDown('W')) {
		player[0].AddForce(-view *PLAYER_SPEED);
		player[0].zrotvel += BALL_ROT* dt;
	}
	if (KeyboardController::GetInstance()->IsKeyDown('S')) {
		player[0].AddForce(view * PLAYER_SPEED);
		player[0].zrotvel -= BALL_ROT * dt;
	}
	if (KeyboardController::GetInstance()->IsKeyDown('A')) {
		player[0].AddForce(right *PLAYER_SPEED);
		player[0].xrotvel += BALL_ROT * dt;
	}
	if (KeyboardController::GetInstance()->IsKeyDown('D')) {
		player[0].AddForce(-right *PLAYER_SPEED);
		player[0].xrotvel -= BALL_ROT * dt;
	}
}

void SceneBumperBalls::RenderSkyBox() {
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
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(5.f, 5.f, 5.f);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.f, -250.f, 0.f);
	modelStack.Rotate(-90.f, 0, 1, 0);
	modelStack.Rotate(-90.f, 1, 0, 0);
	modelStack.Scale(5.f, 5.f, 5.f);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
}

