#include "PhysicsObject.h"

PhysicsObject::PhysicsObject()
	: pos{}, vel{}, accel{}, mass(1.f), m_totalForces{},
	angularVel{}, angleDeg{}, bounciness{ 1.f }
{
}

void PhysicsObject::AddForce(const glm::vec3& force)
{
	m_totalForces += force;
}

void PhysicsObject::AddImpulse(const glm::vec3& impulse)
{
	vel += impulse * (1.f/mass);
}

void PhysicsObject::UpdatePhysics(float dt)
{

	glm::vec3 finalAccel{ accel };
	//if mass is 0, treat object as static/immovable
	if (mass > 0) {
		//F = MA
		float invMass = 1.f / mass;
		finalAccel = accel + m_totalForces * invMass;
	}

	//euler integration
	vel += finalAccel * dt;
	pos += vel * dt;

	//clear forces
	m_totalForces = glm::vec3(0, 0, 0);

	//do angular movement
	angleDeg += angularVel * dt;
}
