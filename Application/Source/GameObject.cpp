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

int GameObject::GetCollideType(void)
{
	return collidetype;
}

void GameObject::SetID(int id)
{
	ID = id;
}
