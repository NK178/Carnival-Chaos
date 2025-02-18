#include "GameObject.h"

GameObject::GameObject(int ID)
{
	this->ID = ID;

}
GameObject::~GameObject()
{
}

int GameObject::GetID(void)
{
	return ID;
}
