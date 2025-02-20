#pragma once
#include "PhysicsObject.h"
#include "CollisionDetection.h"
#include <glm\glm.hpp>


class GameObject : public PhysicsObject
{
public:
	enum COLLISIONTYPE {
		CUBE = 1,
		SPHERE = 2,
		CAMERA = 3,
	};
	GameObject(int ID, int type);
	~GameObject();
	int GetID(void);
	int GetCollideType(void);
	void SetID(int id);


private:
	//SET ID FOR ALL GOS !!!!!
	int ID;
	int collidetype;
};
