#pragma once
#include "PhysicsObject.h"
#include "CollisionDetection.h"
#include <glm\glm.hpp>


class GameObject : public PhysicsObject
{
public:
	GameObject(void);
	~GameObject();
	int GetID(void);
	void SetID(int id);
private:
	//SET ID FOR ALL GOS !!!!!
	int ID;
};
