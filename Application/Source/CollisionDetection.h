#pragma once
#include "GL\glew.h"

// GLM Headers
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_inverse.hpp>

//Include GLFW
#include <GLFW/glfw3.h>
#include <iostream>


//global collision detection functions
//bool OverlapCircle2Circle(const Vector3& pos1, float r1, const Vector3& pos2, float r2);

//bool OverlapAABB2AABB(const Vector3& min1, const Vector3& max1, const Vector3& min2, const Vector3& max2);


//bool OverlapCircle2Line(const Vector3& circlePos, float radius,
//						const Vector3& lineStart, 
//						const Vector3& lineEnd);
//
//bool OverlapCircle2Line(PhysicsObject& ball, float radius, const Vector3& lineStart, const Vector3& lineEnd, CollisionData& cd);

//collision resolution function
//void ResolveCircle2StaticLine(PhysicsObject& ball, float radius, const Vector3& lineStart, const Vector3& lineEnd);
//
//bool ResolveCircle2Line(PhysicsObject& ball, float radius, const Vector3& lineStart, const Vector3& lineEnd, CollisionData& cd);
//
//bool OverlapCircle2AABB(Vector3 circlePos, float radius, Vector3 boxMin, Vector3 boxMax);
//
//bool OverlapCircle2OBB(PhysicsObject& circle, float radius, PhysicsObject& Box, const float Width, const float Height, CollisionData& cd);
//
//void ResolveCircle2StaticCircle(PhysicsObject& ball1, float radius1, PhysicsObject& ball2, float radius2);
//
//bool OverlapCircle2Circle(PhysicsObject& obj1, float r1, PhysicsObject& obj2, float r2, CollisionData& cd);
//
//bool SeparatingAxisTheorem(PhysicsObject& Poly1,const std::vector<Vector3>& polygon1,PhysicsObject& Poly2, const std::vector<Vector3>& polygon2, CollisionData& cd);
//
//void ResolveCollision(CollisionData& Collide);
//
//bool OverlapCircle2Line(PhysicsObject& circle, float radius,
//	const Vector3& lineStart,
//	const Vector3& lineEnd, CollisionData& cd);
//
//bool OverlapCircle2Line(PhysicsObject& obj1, float radius, const Vector3& lineA, const Vector3& lineB, CollisionData& cd);