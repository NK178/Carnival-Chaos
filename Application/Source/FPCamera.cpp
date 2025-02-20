#include "FPCamera.h"
#include "KeyboardController.h"
#include "MouseController.h"
#include <iostream>
#include <glm\gtx\compatibility.hpp>

//Include GLFW
#include <GLFW/glfw3.h>
#include <iostream>

float FPCamera::currentPitch = 0.f;

//DONT INIT ANYTHING
FPCamera::FPCamera() : isDirty(false), GameObject(10,GameObject::CAMERA)
{

	//this->position = position;
	//this->position.y += 15.0f;
	//this->target = target;
	//this->forward = glm::vec3(0, 0, 0);
	//this->up = up;
	//this->isDirty = true;
	//amplitude = 0.0f;
	//frequency = 7.f;
	//time = 0.f;
	//jumpflag = false;
	//crouchflag = false;
	//crouchduration = 0.2f;
	//proneflag = false;
	//proneduration = 0.5f;
	//jumpphase1 = true;
	//crouchphase1 = true;
	//pronephase1 = true;
	//swaytimer = 0.f;
	//swayduration = 2.f;
	//swayphase1 = true;
	//sway = 0.f;
	//boptimer = 0.f;
	//bop = 0.f;
	//bopflag = true;
}

FPCamera::~FPCamera()
{
}

void FPCamera::Init(glm::vec3 position, glm::vec3 target, glm::vec3 up) //use this one
{
	pos = position;
	this->target = target;
	camheight = position.y;
	this->forward = glm::vec3(0, 0, 0);
	this->up = up;
	this->isDirty = true;
	amplitude = 0.0f;
	frequency = 7.f;
	time = 0.f;
	jumpflag = false;
	crouchflag = false;
	jumpduration = 0.08f;
	crouchduration = 0.2f;
	jumpphase1 = true;
	crouchphase1 = true;
	swaytimer = 0.f;
	swayduration = 2.f;
	swayphase1 = true;
	sway = 0.f;
	boptimer = 0.f;
	bop = 0.f;
	bopflag = true;
	sprintstamina = 100.f;
	staminaflag = true;
	runflag = false;

	Refresh();
}

void FPCamera::Reset()
{
}

void FPCamera::Update(double dt)
{

	static const float ROTATE_SPEED = 100.0f;
	static const float DEFAULT_SPEED = 30.f;
	static float ZOOM_SPEED = DEFAULT_SPEED;
	static const float DEFAULT_SWAY = 1.6f;
	static float SWAY_SPEED = DEFAULT_SWAY;
	static float SWAY_AMPLITUDE = 0.1f;
	static const float DEFAULT_BOP = 15.f;
	static float BOP_SPEED = DEFAULT_BOP;
	static float BOP_AMPLITUDE = 0.1f;
	static const float JUMP_POWER = 0.75f;

	glm::vec3 view = glm::normalize(target - pos); //calculate the new view vector
	glm::vec3 front = view;
	glm::vec3 right = glm::normalize(glm::cross(view, glm::vec3{0,1,0}));

	//prevent airplane mode 
	front.y = 0.0f;
	front = glm::normalize(front);
	forward = front;

	//glm::clamp the angle somewhere 

	bool isMoving = false;
	//Movement
	if (allowMovement)
	{
		if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_W)) {
			pos += front * ZOOM_SPEED * static_cast<float>(dt);
			target += view * ZOOM_SPEED * static_cast<float>(dt);
			if (bop != 0.f && bopflag)
				pos.y += bop;
			isDirty = isMoving = true;
		}
		if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_S)) {
			pos -= front * ZOOM_SPEED * static_cast<float>(dt);
			target -= view * ZOOM_SPEED * static_cast<float>(dt);
			if (bop != 0.f && bopflag)
				pos.y += bop;
			isDirty = isMoving = true;
		}
		if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_A)) {
			pos -= right * ZOOM_SPEED * static_cast<float>(dt);
			target -= right * ZOOM_SPEED * static_cast<float>(dt);
			isDirty = isMoving = true;
		}
		if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_D)) {
			pos += right * ZOOM_SPEED * static_cast<float>(dt);
			target += right * ZOOM_SPEED * static_cast<float>(dt);
			isDirty = isMoving = true;
		}
	}

	/////////////////////////////////////// bad implentation 
	//Jump 
	if (KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE) && allowJump) { 
		if (!crouchflag && !proneflag && !jumpflag) {
			pos.y = camheight;
			jumpflag = jumpphase1 = true;
			bopflag = false;
			time = 0.f;
		}
	}
	if (jumpphase1 && jumpflag) {
		time += dt;
		amplitude = glm::sin(time * frequency) * JUMP_POWER;
		pos += glm::vec3(0, 1, 0) * amplitude;
		if (time > 0.9f)
			jumpflag = false;
	}
	else if (!jumpflag && !crouchflag && !bopflag) {
		time += dt;
		pos.y = camheight;
		time = amplitude = 0.f;
		bopflag = true;
	}

	//Crouch
	if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_C) && allowCrouch) { 
		if (!jumpflag && !crouchflag && !proneflag) {
			pos.y = camheight;
			crouchflag = true;
			time = amplitude = 0.f;
		}
		else if (!jumpflag && !crouchflag && proneflag) {
			pos.y = camheight - 6;
			crouchflag = true;
			proneflag = false;
			time = amplitude = 0.f;
		}
		else if (crouchflag && crouchphase1) {
			time = 0.f;
			crouchphase1 = false;
		}
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_X) && allowProne) {
		if (!jumpflag && !proneflag && !crouchflag) {
			pos.y = camheight;
			proneflag = true;
			time = amplitude = 0.f;
		}
		else if (!jumpflag && !proneflag && crouchflag) {
			pos.y = camheight - 3;
			proneflag = true;
			crouchflag = false;
			time = amplitude = 0.f;
		}
		else if (proneflag && pronephase1) {
			time = 0.f;
			crouchphase1 = true;
			pronephase1 = false;
		}
	}
	if (crouchflag && crouchphase1) {
		time += dt;
		ZOOM_SPEED = 15.f;
		if (time < crouchduration)
			pos.y = glm::lerp(camheight, camheight - 3.f, time / crouchduration);
	}
	if (!crouchphase1) {
		time += dt;
		ZOOM_SPEED = 15.f;
		if (time < crouchduration)
			pos.y = glm::lerp(camheight - 3.f, camheight, time / crouchduration);
		else {
			pos.y = camheight;
			crouchphase1 = true;
			crouchflag = false;
			ZOOM_SPEED = DEFAULT_SPEED;
		}
	}	
	if (proneflag && pronephase1) {
		time += dt;
		ZOOM_SPEED = 15.f;
		if (time < proneduration)
			pos.y = glm::lerp(camheight, camheight - 6.f, time / proneduration);
	}
	if (!pronephase1) {
		time += dt;
		ZOOM_SPEED = 15.f;
		if (time < proneduration)
			pos.y = glm::lerp(camheight - 6.f, camheight, time / proneduration);
		else {
			pos.y = camheight;
			pronephase1 = true;
			proneflag = false;
			ZOOM_SPEED = DEFAULT_SPEED;
		}
	}
	if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_LEFT_SHIFT) && staminaflag && !crouchflag && allowSprint) {
		ZOOM_SPEED = 100.f;
		sprintstamina -= 2.f;
		heightresetflag = false;
		runflag = true;
		if (sprintstamina < 0) {
			sprintstamina = 0.f;
			staminaflag = false;
		}
		BOP_SPEED = 23.f;
		BOP_AMPLITUDE = 0.2f;
	}
	else {
		BOP_SPEED = DEFAULT_BOP;
		BOP_AMPLITUDE = 0.1f;
		if (sprintstamina < 100) {
			sprintstamina += 0.5f;
			if (!heightresetflag && runflag) {
				pos.y = camheight;
				heightresetflag = true;
				runflag = false;
			}
			if (sprintstamina > 40.f) 
				staminaflag = true;		
		}
		if (!crouchflag)
			ZOOM_SPEED = DEFAULT_SPEED;
	}

	//std::cout << camheight << std::endl;

	////Sway camera 
	//swaytimer += static_cast<float>(dt);
	//sway = glm::sin(swaytimer * SWAY_SPEED) * SWAY_AMPLITUDE;
	//if (sway != 0.f)
	//	up.x = sin(sway);
	////Up down bobbing 
	//boptimer += static_cast<float>(dt);
	//bop = glm::sin(boptimer * BOP_SPEED) * BOP_AMPLITUDE;

	//double deltaX = MouseController::GetInstance() -> GetMouseDeltaX();
	//float angleX = -deltaX * ROTATE_SPEED * 0.05 * static_cast<float>(dt);
	//glm::mat4 yaw = glm::rotate(
	//	glm::mat4(1.f), // matrix to modify
	//	glm::radians(angleX), // rotation angle in degree and
	//	glm::vec3(up.x, up.y, up.z)// the axis to rotate along
	//);
	//glm::vec3 yawView = yaw * glm::vec4(view, 0.f);

	//double deltaY = MouseController::GetInstance()->GetMouseDeltaY();
	//float angleY = deltaY * ROTATE_SPEED * 0.05 * static_cast<float>(dt);
	//glm::mat4 pitch = glm::rotate(
	//	glm::mat4(1.f), // matrix to modify
	//	glm::clamp(glm::radians(angleY),-1.5f,1.5f), // rotation angle in degree and
	//	glm::vec3(right.x, right.y, right.z)// the axis to rotate along
	//);
	//glm::vec3 pitchView = pitch * glm::vec4(view, 0.f);
	//target = pos + pitchView + yawView;
	//isDirty = true;
	//this->Refresh();

	/*if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_A)) {
		up.x += ((1 - up.x) * 0.05);
		up.z += ((1 - up.z) * 0.05);
	}
	if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_D)) {
		up.x -= ((up.x + 1) * 0.05);
		up.z -= ((up.z + 1) * 0.05);
	}
	if (!KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_D) && !KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_A)) {
		up.x *= 0.9;
		up.z *= 0.9;
	}*/

	if (allowLocomotiveTilt)
	{
		if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_A)) {
			sway += ((1 - sway) * 0.05);

		}
		if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_D)) {
			sway -= ((sway + 1) * 0.05);
		}
		if (!KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_D) && !KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_A)) {
			sway *= 0.9;
		}

		up.x = sin(sway) * 0.25 * multDebugX;
		up.z = sin(sway) * 0.25 * multDebugZ;
	}

	if (allowLocomotiveBop)
	{
		//Up down bobbing 
		boptimer += static_cast<float>(dt);
		bop = glm::sin(boptimer * BOP_SPEED) * BOP_AMPLITUDE;
	}

	double deltaX = MouseController::GetInstance()->GetMouseDeltaX();
	float angleX = -deltaX * ROTATE_SPEED * 0.05 * static_cast<float>(dt);
	glm::mat4 yaw = glm::rotate(
		glm::mat4(1.f), // matrix to modify
		glm::radians(angleX), // rotation angle in degree and
		glm::vec3(up.x, up.y, up.z)// the axis to rotate along
	);
	glm::vec3 yawView = yaw * glm::vec4(view, 0.f);

	double deltaY = MouseController::GetInstance()->GetMouseDeltaY();
	float angleY = deltaY * ROTATE_SPEED * 0.05 * static_cast<float>(dt);
	glm::mat4 pitch = glm::rotate(
		glm::mat4(1.f), // matrix to modify
		glm::clamp(glm::radians(angleY), -1.5f, 1.5f), // rotation angle in degree and
		glm::vec3(right.x, right.y, right.z)// the axis to rotate along
	);
	glm::vec3 pitchView = pitch * glm::vec4(view, 0.f);
	target = pos + pitchView + yawView;
	isDirty = true;
	this->Refresh();

	//std::cout << pitchView.x << std::endl;

	

	multDebugX = pitchView.z > 0 ? 1 : -1;
	multDebugZ = pitchView.x > 0 ? -1 : 1;
	UpdatePhysics(dt);
	
}

glm::vec3 FPCamera::GetView(void)
{
	return glm::normalize(target - pos);
}

glm::vec3 FPCamera::GetFront(void)
{
	return forward;
}

float FPCamera::GetStamina(void)
{
	return sprintstamina;
}

void FPCamera::Refresh()
{
	if (!this->isDirty) return;
	glm::vec3 view = glm::normalize(target - pos);
	glm::vec3 right = glm::normalize(glm::cross(up, view));
	// Recalculate the up vector  (used for tilting head) 
	//this->up = glm::normalize(glm::cross(view, right));
	//this->up = glm::vec3{ 0,1,0 };
	this->isDirty = false;
}

void FPCamera::RotateCamera(float angle) {
	glm::vec3 view = glm::normalize(target - pos); 

	float radians = glm::radians(angle);
	glm::mat4 rotation = glm::mat4(1.0f);
	rotation = glm::rotate(glm::mat4(1.0f), radians, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 rot = rotation * glm::vec4(view, 0.f);
	target = pos + rot;
	isDirty = true;
	this->Refresh();
}

//void FPCamera::RotateAboutCamera(glm::vec3 rotpt, float angle)
//{
//	glm::vec3 view = glm::normalize(target - pos);
//
//	float radians = glm::radians(angle);
//	glm::mat4 rotation = glm::mat4(1.0f);
//	rotation = glm::rotate(glm::mat4(1.0f), radians, glm::vec3(0.0f, 1.0f, 0.0f));
//	glm::vec3 rot = rotation * glm::vec4(view, 0.f);
//	pos = target + rot;
//	isDirty = true;
//	this->Refresh();
//	//float anglerad = glm::radians(angle);
//	//pos = glm::vec3(cos(anglerad), camheight, sin(anglerad));
//
//	//glm::vec3 relativepos = pos - rotpt;
//	//glm::mat4 rotmat = glm::rotate(glm::mat4(1.0f), glm::radians(rotsp * dt), glm::vec3{0,1,0});
//	//relativepos = glm::vec3(rotmat * glm::vec4(relativepos, 1.0f));
//	//pos = relativepos + rotpt;
//}


