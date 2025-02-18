#pragma once
#include "PhysicsObject.h"
#include <glm\glm.hpp>


class GameObject : public PhysicsObject
{
public:
	GameObject(int ID);
	~GameObject();
	int GetID(void);
private:
	//SET ID FOR ALL GOS !!!!!
	int ID;
};
