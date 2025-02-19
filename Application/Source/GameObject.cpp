#include "GameObject.h"

GameObject::GameObject(void)
{
	ID = 0;
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
