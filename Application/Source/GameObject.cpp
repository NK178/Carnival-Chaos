#include "GameObject.h"

GameObject::GameObject(int ID, int type)
{
	this->ID = ID;
	this->collidetype = type;
}

GameObject::~GameObject()
{
}

int GameObject::GetID(void)
{
	return ID;
}

void GameObject::SetID(int id)
{
	ID = id;
}
