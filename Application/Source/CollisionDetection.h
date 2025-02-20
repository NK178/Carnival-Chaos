#pragma once

#include <glm\glm.hpp>
#include <vector>
#include "PhysicsObject.h"

struct CollisionData {
	float pd{};
	glm::vec3 normal; //relative to object 1 
	PhysicsObject* pObj1{};
	PhysicsObject* pObj2{};
	glm::vec3 spinvec{ 0,0,0 };
};

//Updates ur vertices LIVE
void Updatevertices(PhysicsObject& obj, std::vector<glm::vec3>& vertices);

//Resolve 2 moving objects 
void ResolveCollision(CollisionData& cd);

bool OverlapAABB2AABB(PhysicsObject& obj1, const glm::vec3& obj1extent, PhysicsObject& obj2, const glm::vec3& obj2extent, CollisionData& cd);

bool OverlapSphere2Sphere(PhysicsObject& obj1, float r1, PhysicsObject& obj2, float r2, CollisionData& cd);

bool OverlapAABB2Sphere(PhysicsObject& circle, float radius, PhysicsObject& box, glm::vec3 boxMin, glm::vec3 boxMax, CollisionData& cd);

bool SAT(PhysicsObject& obj1, const std::vector<glm::vec3>& polA, PhysicsObject& obj2, const std::vector<glm::vec3>& polB, CollisionData& cd);


//
//bool SAT2Circle(PhysicsObject& circle, const float& circleRadius, PhysicsObject& SAT, const std::vector<glm::vec3>& polA, glm::vec3& contactpt, const glm::vec3& offset, CollisionData& cd);
//
//bool OverlapCircle2Line(PhysicsObject& circle, float radius, std::vector<glm::vec3> line);
//
//
//
//
////collision resolution function
//void ResolveCircle2StaticLine(PhysicsObject& ball, float radius, const glm::vec3& lineStart, const glm::vec3& lineEnd);
//
//bool OverlapPt2Polygon(const glm::vec3& pt, const std::vector<glm::vec3>& polygon);
//


//Vector3 SAT2Line(const std::vector<Vector3>& polA, const std::vector<Vector3>& line, const Vector3& polA_pos);

////Circle2Circle 
//bool OverlapCircle2Circle(const Vector3& pos1, float r1, const Vector3& pos2, float r2, Vector3& vector);
//void ResolveCircle2Circle(PhysicsObject& ball1, float radius1, PhysicsObject& ball2, float radius2, Vector3& vector);
//Circle2AABB
//void ResolveCircle2AABB(PhysicsObject& box, PhysicsObject& circle, float radius,Vector3 & vector);
//global collision detection functions
//float OverlapAABB2AABB(const Vector3& min1, const Vector3& max1,
//	const Vector3& min2, const Vector3& max2);
//bool SAT(const std::vector<Vector3>& polA, const std::vector<Vector3>& polB, Vector3& vector, float& pen);
//void ResolveSAT(PhysicsObject& SAT1, PhysicsObject& SAT2, Vector3& vector, float pen);
