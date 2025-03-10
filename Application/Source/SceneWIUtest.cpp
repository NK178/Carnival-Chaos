#pragma once
#include "SceneWIUtest.h"
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


SceneWIUtest::SceneWIUtest()
{
}

SceneWIUtest::~SceneWIUtest()
{
}

void SceneWIUtest::Init()
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
	camera.Init(glm::vec3(-10, 9, -10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

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
	//	meshList[GEO_CYLINDER] = MeshBuilder::GenerateCyclinder("Cylinder", glm::vec3(1, 1, 1), 20.f,1.5,5.f);
	meshList[GEO_CYLINDER] = MeshBuilder::GenerateCylinder("Cylinder", glm::vec3(1, 1, 1), 20.f,1.f,1.f);
	meshList[GEO_HAMMER1] = MeshBuilder::GenerateOBJMTL("war hammer", "Models//lava_hammer.obj", "Models//lava_hammer.mtl");
	meshList[GEO_HAMMER1]->textureID = LoadTGA("Images//hammer.tga");
	meshList[GEO_HAMMER2] = MeshBuilder::GenerateOBJMTL("mallet", "Models//mallet.obj", "Models//mallet.mtl");
	meshList[GEO_HAMMER2]->textureID = LoadTGA("Images//Mallet_BaseColor.tga");
	//meshList[GEO_HAMMER3] = MeshBuilder::GenerateOBJMTL("mallet2", "Models//mallet2.obj", "Models//mallet2.mtl");
	//meshList[GEO_HAMMER3]->textureID = LoadTGA("Images//Mallet2_BaseColor.tga");

	//skybox
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Images//nightsky_lfD.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Images//nightsky_rtD.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Images//nightsky_upD.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("Plane",glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Images//nightsky_dnD.tga");
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Images//nightsky_bkD.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Images//nightsky_ftD.tga");

	// 16 x 16 is the number of columns and rows for the text
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16,16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Images//calibri.tga");

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

	cubelist.push_back(Cube(1,GameObject::CUBE));
	cubelist.push_back(Cube(2,GameObject::CUBE));
	cubelist[0].pos = glm::vec3{ 10,0,4 };
	cubelist[1].pos = glm::vec3{10,5,0 };
	cubelist[0].mass = 0.f;

	spherelist.push_back(Sphere(3, 1.f,GameObject::SPHERE));
	spherelist.push_back(Sphere(4,1.f,GameObject::SPHERE));
	spherelist[0].pos = glm::vec3{ 0,10,0 };
	spherelist[1].pos = glm::vec3{ -15,3,0 };
	spherelist[1].mass = 0.f;

	obblistV2.push_back(OBBV2(300, GameObject::CUBE));
	obblistV2.push_back(OBBV2(301, GameObject::CUBE));
	obblistV2[0].pos = glm::vec3{ -15,3,-15 };
	obblistV2[0].angleDeg = 0;
	obblistV2[1].pos = glm::vec3{ -15,3,0 };
	obblistV2[1].angleDeg = 0;

	obblist.push_back(OBB(5, GameObject::CUBE));
	obblist.push_back(OBB(6, GameObject::CUBE));
	obblist[0].pos = glm::vec3{ -30,3,-30 };
	obblist[1].pos = glm::vec3{ -30,3,-20 };
	obblist[0].angleDeg = 0;

	cylinderlist.push_back(Cylinder(72, GameObject::CYLINDER, 5, 1.5f));
	cylinderlist[0].pos = glm::vec3{ 0,5,0 };


}

void SceneWIUtest::Update(double dt)
{

	HandleKeyPress();
	const float SPEED = 15.f;
	if (KeyboardController::GetInstance()->IsKeyDown('I'))
		spherelist[0].pos.x -= static_cast<float>(dt) * SPEED;
	if (KeyboardController::GetInstance()->IsKeyDown('K'))
		spherelist[0].pos.x += static_cast<float>(dt) * SPEED;
	if (KeyboardController::GetInstance()->IsKeyDown('J'))
		spherelist[0].pos.z -= static_cast<float>(dt) * SPEED;
	if (KeyboardController::GetInstance()->IsKeyDown('L'))
		spherelist[0].pos.z += static_cast<float>(dt) * SPEED;
	if (KeyboardController::GetInstance()->IsKeyDown('O'))
		spherelist[0].pos.y -= static_cast<float>(dt) * SPEED;
	if (KeyboardController::GetInstance()->IsKeyDown('P'))
		spherelist[0].pos.y += static_cast<float>(dt) * SPEED;
	
	//Update my stuff 
	worldnormals.clear();
	worldvertices.clear();
	//normals
	for (int k = 0; k < obblistV2.size(); k++) {
		std::vector<glm::vec3> temp = obblistV2[k].normals;
		Updatenormals(obblistV2[k], temp);
		worldnormals.push_back(temp);
	}
	//vertices 
	for (int k = 0; k < obblistV2.size(); k++) {
		std::vector<glm::vec3> temp = obblistV2[k].vertices;
		Updatevertices(obblistV2[k], temp);
		worldvertices.push_back(temp);
	}
	//////////////////////////OBB V1 Test 
	V1worldnormals.clear();
	V1worldvertices.clear();
	//normals
	for (int k = 0; k < obblist.size(); k++) {
		std::vector<glm::vec3> temp = obblist[k].normals;
		Updatenormals(obblist[k], temp);
		V1worldnormals.push_back(temp);
	}
	//vertices 
	for (int k = 0; k < obblist.size(); k++) {
		std::vector<glm::vec3> temp = obblist[k].verticebuffer;
		Updatevertices(obblist[k], temp);
		V1worldvertices.push_back(temp);
	}
	if (KeyboardController::GetInstance()->IsKeyPressed('Q')) {
		std::cout << "OBB V2 NORMALS(THE ONE WITH HARDCODED NORMALS)" << std::endl;
		for (int k = 0; k < obblistV2.size(); k++) {
			std::cout << "Obj" << k + 1 << std::endl;
			for (int i = 0; i < obblistV2[k].normals.size(); i++) {
				std::cout << "n" << i + 1 << " x: " << worldnormals[k][i].x << " y: " << worldnormals[k][i].y << " z: " << worldnormals[k][i].z << std::endl;
			}
		}
		std::cout << "OBB V1 NORMALS(THE ONE WITH DYNAMIC NORMAL CALCULATION)" << std::endl;
		for (int k = 0; k < obblist.size(); k++) {
			std::cout << "Obj" << k + 1 << std::endl;
			for (int i = 0; i < obblist[k].normals.size(); i++) {
				std::cout << "n" << i + 1 << " x: " << V1worldnormals[k][i].x << " y: " << V1worldnormals[k][i].y << " z: " << V1worldnormals[k][i].z << std::endl;
			}
		}
	}




	if (KeyboardController::GetInstance()->IsKeyPressed('T')) {
		iter += 1;
		if (iter > 7)
			iter = iter % obblist[0].verticebuffer.size();
		normaliter += 1;
		if (normaliter > 5)
			normaliter = normaliter % obblistV2[0].normals.size();
	}

	if (KeyboardController::GetInstance()->IsKeyPressed('R')) {
		obblist[0].angularVel += 50.f;
		obblistV2[0].angularVel += 30.f;
		//obblistV2[0].angularVel += 50.f;
	}
	

	CollisionData cd;
	if (OverlapAABB2AABB(cubelist[1], cubelist[1].boxextent, cubelist[0], cubelist[0].boxextent, cd)) 
		ResolveCollision(cd);

	if (OverlapSphere2Sphere(spherelist[1], spherelist[1].radius, spherelist[0], spherelist[0].radius, cd)) 
		ResolveCollision(cd);
	
	if (OverlapAABB2Sphere(spherelist[0], spherelist[0].radius, cubelist[1], cubelist[1].pos - cubelist[1].boxextent, cubelist[1].pos + cubelist[1].boxextent, cd))
		ResolveCollision(cd);

	//Cylinder 2 sphere overlap
	if (OverlapSphere2Cylinder(spherelist[0], spherelist[0].radius, cylinderlist[0], cylinderlist[0].radius, cylinderlist[0].height, cd))
		ResolveCollision(cd);


	//V1 testing
	if (SAT(obblist[0], V1worldnormals[0], V1worldvertices[0], obblist[1], V1worldnormals[1], V1worldvertices[1], cd))
		std::cout << "colliding" << std::endl;
		//ResolveCollision(cd);

	//V2 works 
	if (SAT(obblistV2[0], worldnormals[0], worldvertices[0], obblistV2[1], worldnormals[1], worldvertices[1], cd))
		ResolveCollision(cd);

	//std::cout << obblistV2[0].pos.x << " " << obblistV2[0].pos.y << " " << obblistV2[0].pos.z << std::endl;

	if (KeyboardController::GetInstance()->IsKeyPressed('G')) {
		if (!activate)
			activate = true;
		else
			activate = false;
	}
	if (activate) 
		cubelist[1].AddForce(glm::vec3{ 0,-1,0 } *10.f);


	for (int i = 0; i < cubelist.size(); i++) {
		cubelist[i].UpdatePhysics(dt);
	}
	for (int i = 0; i < spherelist.size(); i++) {
		spherelist[i].UpdatePhysics(dt);
	}
	for (int i = 0; i < obblist.size(); i++) {
		obblist[i].UpdatePhysics(dt);
	}
	for (int i = 0; i < obblistV2.size(); i++) {
		obblistV2[i].UpdatePhysics(dt);
	}
	//camera.AddForce(glm::vec3(1, 0, 0) * 50.f);

	camera.Update(dt);

}

void SceneWIUtest::Render()
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

	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	modelStack.Scale(0.1f, 0.1f, 0.1f);
	RenderMesh(meshList[GEO_SPHERE], false);
	modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Scale(100.f, 1.f, 100.f);
	//modelStack.Rotate(-90.f, 1, 0, 0);
	//meshList[GEO_PLANE]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	//meshList[GEO_PLANE]->material.kDiffuse = glm::vec3(0.5f,0.5f, 0.5f);
	//meshList[GEO_PLANE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	//meshList[GEO_PLANE]->material.kShininess = 1.0f;
	//RenderMesh(meshList[GEO_PLANE], true);
	//modelStack.PopMatrix();

	for (int i = 0; i < cubelist.size(); i++) {
		modelStack.PushMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		modelStack.Translate(cubelist[i].pos.x, cubelist[i].pos.y, cubelist[i].pos.z);
		modelStack.Rotate(cubelist[i].angleDeg, 0, 1, 0);
		modelStack.Scale(2*cubelist[i].boxextent.x, 2*cubelist[i].boxextent.y, 2*cubelist[i].boxextent.z);
		meshList[GEO_CUBE]->material.kAmbient = glm::vec3(1.f, 0.1f, 0.1f);
		meshList[GEO_CUBE]->material.kDiffuse = glm::vec3(1.f, 0.1f, 0.1f);
		meshList[GEO_CUBE]->material.kSpecular = glm::vec3(1.f, 0.2f, 0.2f);
		meshList[GEO_CUBE]->material.kShininess = 2.0f;
		RenderMesh(meshList[GEO_CUBE], true);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		modelStack.PopMatrix();
	}

	for (int i = 0; i < spherelist.size(); i++) {
		modelStack.PushMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		modelStack.Translate(spherelist[i].pos.x, spherelist[i].pos.y, spherelist[i].pos.z);
		modelStack.Scale(spherelist[i].radius, spherelist[i].radius, spherelist[i].radius);
		meshList[GEO_SPHERE]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
		meshList[GEO_SPHERE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		meshList[GEO_SPHERE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
		meshList[GEO_SPHERE]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_SPHERE], true);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		modelStack.PopMatrix();
	}

	//Cylidner 
	modelStack.PushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	modelStack.Translate(cylinderlist[0].pos.x, cylinderlist[0].pos.y, cylinderlist[0].pos.z);
	modelStack.Scale(cylinderlist[0].radius, cylinderlist[0].height, cylinderlist[0].radius);
	meshList[GEO_CYLINDER]->material.kAmbient = glm::vec3(0.5f, 0.1f, 0.1f);
	meshList[GEO_CYLINDER]->material.kDiffuse = glm::vec3(0.7f, 0.5f, 0.5f);
	meshList[GEO_CYLINDER]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
	meshList[GEO_CYLINDER]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_CYLINDER], true);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	modelStack.PopMatrix();

	////////////////////////////////////////////OBB V2 
	for (int n = 0; n < obblistV2.size(); n++) {
		modelStack.PushMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		modelStack.Translate(obblistV2[n].pos.x, obblistV2[n].pos.y, obblistV2[n].pos.z);
		modelStack.Rotate(obblistV2[n].angleDeg, 0, 1, 0);
		modelStack.Scale(2 * cubelist[0].boxextent.x, 2 * cubelist[0].boxextent.y, 2 * cubelist[0].boxextent.z);
		meshList[GEO_CUBE]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
		meshList[GEO_CUBE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		meshList[GEO_CUBE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
		meshList[GEO_CUBE]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_CUBE], true);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		modelStack.PopMatrix();
	}

	for (int j = 0; j < worldvertices.size(); j++) {
		for (int i = 0; i < obblistV2[j].vertices.size(); i++) {
			glm::vec3 diffuse, ambient;
			if (j == iter)
				diffuse = ambient = glm::vec3(1.f, 0.1f, 0.1f);
			else
				diffuse = ambient = glm::vec3(1.f, 1.f, 1.f);
			modelStack.PushMatrix();
			modelStack.Translate(worldvertices[j][i].x, worldvertices[j][i].y, worldvertices[j][i].z);
			modelStack.Scale(0.1, 0.1, 0.1);
			meshList[GEO_SPHERE]->material.kAmbient = ambient;
			meshList[GEO_SPHERE]->material.kDiffuse = diffuse;
			meshList[GEO_SPHERE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
			meshList[GEO_SPHERE]->material.kShininess = 2.0f;
			RenderMesh(meshList[GEO_SPHERE], true);
			modelStack.PopMatrix();
		}
	}

	///////////////////////////////////////////////////////OBB V1 
	for (int j = 0; j < obblist.size(); j++) {
		modelStack.PushMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		modelStack.Translate(obblist[j].pos.x, obblist[j].pos.y, obblist[j].pos.z);
		modelStack.Rotate(obblist[j].angleDeg, 0, 1, 0);
		modelStack.Scale(2 * cubelist[0].boxextent.x, 2 * cubelist[0].boxextent.y, 2 * cubelist[0].boxextent.z);
		meshList[GEO_CUBE]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
		meshList[GEO_CUBE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		meshList[GEO_CUBE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
		meshList[GEO_CUBE]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_CUBE], true);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		modelStack.PopMatrix();
		for (int i = 0; i < obblist[j].verticebuffer.size(); i++) {
			glm::vec3 diffuse, ambient;
			if (j == iter)
				diffuse = ambient = glm::vec3(1.f, 0.1f, 0.1f);
			else
				diffuse = ambient = glm::vec3(1.f, 1.f, 1.f);
			modelStack.PushMatrix();
			modelStack.Translate(V1worldvertices[j][i].x, V1worldvertices[j][i].y, V1worldvertices[j][i].z);
			modelStack.Scale(0.1, 0.1, 0.1);
			meshList[GEO_SPHERE]->material.kAmbient = ambient;
			meshList[GEO_SPHERE]->material.kDiffuse = diffuse;
			meshList[GEO_SPHERE]->material.kSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
			meshList[GEO_SPHERE]->material.kShininess = 2.0f;
			RenderMesh(meshList[GEO_SPHERE], true);
			modelStack.PopMatrix();
		}
	}	

	RenderTextOnScreen(meshList[GEO_TEXT], "Stamina", glm::vec3(0, 1, 0), 40, 0, 0);

	RenderSkyBox();
}

void SceneWIUtest::RenderMesh(Mesh* mesh, bool enableLight)
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


void SceneWIUtest::Exit()
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

void SceneWIUtest::HandleKeyPress()
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

void SceneWIUtest::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey)
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

void SceneWIUtest::RenderText(Mesh* mesh, std::string text, glm::vec3
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

void SceneWIUtest::RenderTextOnScreen(Mesh* mesh, std::string
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

void SceneWIUtest::Material(GEOMETRY_TYPE obj, float AmR, float AmG, float AmB, float DifA, float DifG, float DifB, float SpA, float SpG, float SpB, float Shiny)
{
	meshList[obj]->material.kAmbient = glm::vec3(AmR, AmG, AmB);
	meshList[obj]->material.kDiffuse = glm::vec3(DifA, DifG, DifB);
	meshList[obj]->material.kSpecular = glm::vec3(SpA, SpG, SpB);
	meshList[obj]->material.kShininess = Shiny;
}

void SceneWIUtest::RenderSkyBox() {
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

void SceneWIUtest::CubeCollisions(std::vector<int> idlist, std::vector<Cube>& Cubelist, CollisionData cd)
{
	for (int i = 0; i < idlist.size() - 1; i++) {	
		for (int j = i + 1; j < idlist.size(); j++) {
			int idA = idlist[i];
			int idB = idlist[j];
			Cube* cubeA = nullptr;
			Cube* cubeB = nullptr;

			for (int k = 0; k < Cubelist.size(); k++) {
				if (Cubelist[k].GetID() == idA) 
					cubeA = &Cubelist[k];
				if (Cubelist[k].GetID() == idB) 
					cubeB = &Cubelist[k];
			}

			if (cubeA != nullptr && cubeB != nullptr) {
				if (OverlapAABB2AABB(*cubeA, cubeA->boxextent, *cubeB, cubeB->boxextent, cd)) 
					ResolveCollision(cd);
			}

		}
	}
}

void SceneWIUtest::SortCube2Collide(std::vector<GameObject> GOlist, std::vector<Cube> maincubelist, std::vector<Cube>& newcubelist)
{
	for (int i = 0; i < maincubelist.size(); i++) {
		for (int j = 0; j < GOlist.size(); j++) {
			if (maincubelist[i].GetID() == GOlist[j].GetID()) 
				newcubelist.push_back(maincubelist[i]);
		}
	}
}

//void SceneWIUtest::CubeCollisions(std::vector<Cube>& Cubelist, CollisionData cd)
//{
//	//Filter out the correct GOs (includes filtering the correct GO types)
//	for (int i = 0; i < Cubelist.size() - 1; ++i) {
//		for (int j = i + 1; j < Cubelist.size(); ++j) {
//			if (OverlapAABB2AABB(Cubelist[i], Cubelist[i].boxextent, Cubelist[j], Cubelist[j].boxextent, cd)) {
//				ResolveCollision(cd);
//			}
//		}
//	}

//}

