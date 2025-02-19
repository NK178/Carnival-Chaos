#include "CollisionDetection.h"

#include <iostream>

void ResolveCollision(CollisionData& cd)
{
	PhysicsObject& obj1 = *cd.pObj1;
	PhysicsObject& obj2 = *cd.pObj2;

	float totalInvMass = 1.f / (obj1.mass + obj2.mass);
	float bounciness = std::min(obj1.bounciness, obj2.bounciness);
	glm::vec3 deltaVel;
	if (cd.spinvec == glm::vec3{ 0,0,0 })
		deltaVel = (glm::dot(obj1.vel, cd.normal) - glm::dot(obj2.vel, cd.normal)) * cd.normal;
	else 
		deltaVel = (glm::dot(obj1.vel, cd.normal) - glm::dot(obj2.vel + cd.spinvec, cd.normal)) * cd.normal;

	//Resolve velocity 
	if (obj1.mass > 0 && obj2.mass == 0) {
		obj1.pos += cd.pd * cd.normal;	
		obj1.vel -= (bounciness + 1) * deltaVel;
	}
	else if (obj1.mass == 0 && obj2.mass > 0) {
		obj2.pos -= cd.pd * cd.normal;	
		obj2.vel += (bounciness + 1) * deltaVel;
	}
	else {
		obj1.pos += obj2.mass * totalInvMass * cd.pd * cd.normal;
		obj2.pos -= obj1.mass * totalInvMass * cd.pd * cd.normal;
		obj1.vel -= (bounciness + 1) * obj2.mass * totalInvMass * deltaVel;
		obj2.vel += (bounciness + 1) * obj1.mass * totalInvMass * deltaVel;
	}
}

////////////////////////////////////////// BOX 2 BOX /////////////////////////////////////
bool OverlapAABB2AABB(PhysicsObject& obj1, const glm::vec3& obj1extent, PhysicsObject& obj2, const glm::vec3& obj2extent, CollisionData& cd)
{
	glm::vec3 max1 = obj1.pos + obj1extent;
	glm::vec3 min1 = obj1.pos - obj1extent;
	glm::vec3 max2 = obj2.pos + obj2extent;
	glm::vec3 min2 = obj2.pos - obj2extent;
	//the condition is checking for IF NOT COLLIDE, so as long as one fail it not collide 
	if (max1.x < min2.x || min1.x > max2.x || max1.y < min2.y || min1.y > max2.y || max1.z < min2.z || min1.z > max2.z)
		return false;
	else {
		float overlapX, overlapY, overlapZ;
		overlapY = overlapX = overlapZ = 0;
		overlapX = std::min(max1.x, max2.x) - std::max(min1.x, min2.x);
		overlapY = std::min(max1.y, max2.y) - std::max(min1.y, min2.y);
		overlapZ = std::min(max1.z, max2.z) - std::max(min1.z, min2.z);

		float minpen = std::min({ overlapX, overlapY,overlapZ });
		if (minpen == overlapX) {
			cd.pd = overlapX;
			if (max1.x < max2.x)
				cd.normal = glm::vec3{ -1,0,0 };
			else
				cd.normal = glm::vec3{ 1,0,0 };
		}
		else if (minpen == overlapY) {
			cd.pd = overlapY;
			if (max1.y < max2.y)
				cd.normal = glm::vec3{ 0,-1,0 };
			else
				cd.normal = glm::vec3{ 0,1,0 };
		}
		else {
			cd.pd = overlapZ;
			if (max1.z < max2.z)
				cd.normal = glm::vec3{ 0,0,-1 };
			else
				cd.normal = glm::vec3{ 0,0,1 };
		}
		cd.pObj1 = &obj1;
		cd.pObj2 = &obj2;
		return true;
	}
}

////////////////////////////////////////// CIRCLE 2 CIRCLE /////////////////////////////////////
bool OverlapSphere2Sphere(PhysicsObject& obj1, float r1, PhysicsObject& obj2, float r2, CollisionData& cd)
{
	glm::vec3 dispvec = obj1.pos - obj2.pos;
	float length = glm::length(dispvec);
	if (length <= (r2 + r1)) {
		//calculate collision data 
		cd.pObj1 = &obj1;
		cd.pObj2 = &obj2;
		cd.pd = (r1 + r2) - length;
		cd.normal = glm::normalize(dispvec);
		return true;
	}
	return false;
}

//////////////////////////////////////////// CIRCLE 2 AABB /////////////////////////////////////
bool OverlapAABB2Sphere(PhysicsObject& circle, float radius, PhysicsObject& box, glm::vec3 boxMin, glm::vec3 boxMax, CollisionData& cd)
{
	float nearestPoint, nearestPoint2,nearestPoint3, distance;
	if (circle.pos.x > boxMax.x)
		nearestPoint = boxMax.x;
	else if (circle.pos.x < boxMin.x)
		nearestPoint = boxMin.x;
	else
		nearestPoint = circle.pos.x;

	if (circle.pos.y > boxMax.y)
		nearestPoint2 = boxMax.y;
	else if (circle.pos.y < boxMin.y)
		nearestPoint2 = boxMin.y;
	else
		nearestPoint2 = circle.pos.y;

	if (circle.pos.z > boxMax.z)
		nearestPoint3 = boxMax.z;
	else if (circle.pos.z < boxMin.z)
		nearestPoint3 = boxMin.z;
	else
		nearestPoint3 = circle.pos.z;

	glm::vec3 nearpos(nearestPoint, nearestPoint2, nearestPoint3);
	glm::vec3 dispvector = circle.pos - nearpos;
	distance = glm::length(dispvector);

	if (distance < radius) {
		cd.pObj1 = &circle;
		cd.pObj2 = &box;
		cd.pd = radius - distance;
		cd.normal = glm::normalize(dispvector);
		return true;
	}
	else
		return false;
}

//////////////////////////////////////////// CIRCLE 2 AABB /////////////////////////////////////
//bool OverlapCircle2AABB(PhysicsObject& circle, float radius, PhysicsObject& box, glm::vec3 boxMin, glm::vec3 boxMax, CollisionData& cd)
//{
//	float nearestPoint, nearestPoint2, distancesquared;
//	if (circle.pos.x > boxMax.x)
//		nearestPoint = boxMax.x;
//	else if (circle.pos.x < boxMin.x)
//		nearestPoint = boxMin.x;
//	else
//		nearestPoint = circle.pos.x;
//
//	if (circle.pos.y > boxMax.y)
//		nearestPoint2 = boxMax.y;
//	else if (circle.pos.y < boxMin.y)
//		nearestPoint2 = boxMin.y;
//	else
//		nearestPoint2 = circle.pos.y;
//
//	glm::vec3 nearpos(nearestPoint, nearestPoint2, 0.f);
//	glm::vec3 dispvector = circle.pos - nearpos;
//	distancesquared = dispvector.LengthSquared();
//
//	if (distancesquared < radius * radius) {
//		cd.pObj1 = &circle;
//		cd.pObj2 = &box;
//		cd.pd = radius - dispvector.Length();
//		cd.normal = dispvector.Normalized(); 
//		return true;
//	}
//	else
//		return false;
//}
//
//////////////////////////////////////////// SAT /////////////////////////////////////
//bool SAT(PhysicsObject& obj1, const std::vector<glm::vec3>& polA, PhysicsObject& obj2, const std::vector<glm::vec3>& polB, CollisionData& cd)
//{
//	size_t sizeA = polA.size();
//	size_t sizeB = polB.size();
//	float overlap = std::numeric_limits<float>::infinity();
//	glm::vec3 minNormal = glm::vec3(0, 0, 0);
//
//	//PolyA
//	for (size_t i = 0; i < sizeA; ++i)
//	{
//		const glm::vec3& pt1 = polA[i];
//		const glm::vec3& pt2 = polA[(i + 1) % sizeA];
//		glm::vec3 edge = pt2 - pt1;
//		glm::vec3 normal{ -edge.y, edge.x };
//		normal.Normalize();
//		std::vector<float> MinMaxA;
//		std::vector<float> MinMaxB;
//
//		//2) Map the point to the Seperating Axis) 
//		for (int i = 0; i < sizeA; i++) {
//			MinMaxA.push_back(polA[i].Dot(normal));
//		}
//		for (int i = 0; i < sizeB; i++) {
//			MinMaxB.push_back(polB[i].Dot(normal));
//		}
//
//		//3) find the min max value of both 
//		float MaxA = MinMaxA[0];
//		float MinA = MinMaxA[0];
//		for (int i = 1; i < MinMaxA.size(); i++) {
//			if (MinMaxA[i] > MaxA) 
//				MaxA = MinMaxA[i];
//			else if (MinMaxA[i] < MinA) 
//				MinA = MinMaxA[i];
//		}
//
//		float MaxB = MinMaxB[0];
//		float MinB = MinMaxB[0];
//		for (int i = 1; i < MinMaxB.size(); i++) {
//			if (MinMaxB[i] > MaxB) 
//				MaxB = MinMaxB[i];
//			else if (MinMaxB[i] < MinB) 
//				MinB = MinMaxB[i];
//		}
//
//		//4) with MaxA, MinA, MaxB, MinB, check collison 
//		if (MinB > MaxA || MinA > MaxB)
//			return false;
//		else {
//			float ActualMax, ActualMin;
//			if (MaxA > MaxB)
//				ActualMax = MaxB;
//			else
//				ActualMax = MaxA;
//			if (MinA > MinB)
//				ActualMin = MinA;
//			else
//				ActualMin = MinB;
//			float currentoverlap = ActualMax - ActualMin;
//
//			//clamp penetration if too large 
//			if (currentoverlap > 1.0f)
//				currentoverlap = 1.0f;
//			if (overlap > currentoverlap) {  //only updates when a min penetration distance is found 
//				overlap = currentoverlap;
//				if (MinA < MinB)
//					minNormal = normal * -1.f;
//				else
//					minNormal = normal;
//			}
//		}
//
//	}
//
//	//PolyB
//	for (size_t i = 0; i < sizeB; ++i)
//	{
//		//pt1 and pt2 together forms an edge of the polygon
//		const glm::vec3& pt1 = polB[i];
//		const glm::vec3& pt2 = polB[(i + 1) % sizeB];
//
//		//TODO:
//		//1) compute the normal to the edge(aka the seperation Axis)
//		glm::vec3 edge = pt2 - pt1;
//		glm::vec3 normal{ -edge.y, edge.x };
//		normal.Normalize();
//
//		std::vector<float> MinMaxA;
//		std::vector<float> MinMaxB;
//
//		//2) Map the point to the Seperating Axis) 
//		for (int i = 0; i < sizeA; i++) {
//			MinMaxA.push_back(polA[i].Dot(normal));
//		}
//		for (int i = 0; i < sizeB; i++) {
//			MinMaxB.push_back(polB[i].Dot(normal));
//		}
//
//		//3) find the min max value of both 
//		float MaxA = MinMaxA[0];
//		float MinA = MinMaxA[0];
//		for (int i = 1; i < MinMaxA.size(); i++) {
//			if (MinMaxA[i] > MaxA) {
//				MaxA = MinMaxA[i];
//			}
//			else if (MinMaxA[i] < MinA) {
//				MinA = MinMaxA[i];
//			}
//		}
//
//		float MaxB = MinMaxB[0];
//		float MinB = MinMaxB[0];
//		for (int i = 1; i < MinMaxB.size(); i++) {
//			if (MinMaxB[i] > MaxB) {
//				MaxB = MinMaxB[i];
//			}
//			else if (MinMaxB[i] < MinB) {
//				MinB = MinMaxB[i];
//			}
//		}
//
//		//4) with MaxA, MinA, MaxB, MinB, check collison 
//		if (MinB > MaxA || MinA > MaxB)
//			return false;
//		else {
//			float ActualMax, ActualMin;
//			if (MaxA > MaxB)
//				ActualMax = MaxB;
//			else
//				ActualMax = MaxA;
//			if (MinA > MinB)
//				ActualMin = MinA;
//			else
//				ActualMin = MinB;
//			float currentoverlap = ActualMax - ActualMin;
//			//clamp penetration if too large 
//			if (currentoverlap > 1.0f)
//				currentoverlap = 1.0f;
//			if (overlap > currentoverlap) {  //only updates when a min penetration distance is found 
//				overlap = currentoverlap;
//				if (MinA < MinB)
//					minNormal = normal * -1.f;
//				else
//					minNormal = normal;
//			}
//		}
//
//	}
//	cd.pObj1 = &obj1;
//	cd.pObj2 = &obj2;
//	cd.normal = minNormal;
//	cd.pd = overlap;
//	return true;
//}
//
////////////////////////////////////////////CIRCLE 2 SAT /////////////////////////////////////
//bool SAT2Circle(PhysicsObject& circle, const float& circleRadius, PhysicsObject& SAT, const std::vector<glm::vec3>& polA, glm::vec3& contactpt, const glm::vec3& offset,CollisionData& cd)
//{
//	size_t sizeA = polA.size();
//	glm::vec3 circlePos = circle.pos;
//
//	float overlap = std::numeric_limits<float>::infinity();
//	glm::vec3 minNormal = glm::vec3(0, 0, 0);
//
//	//PolyA
//	for (size_t i = 0; i < sizeA; ++i)
//	{
//		//pt1 and pt2 together forms an edge of the polygon
//		const glm::vec3& pt1 = polA[i];
//		const glm::vec3& pt2 = polA[(i + 1) % sizeA];
//
//		//TODO:
//		//1) compute the normal to the edge(aka the seperation Axis)
//		glm::vec3 edge = pt2 - pt1;
//		glm::vec3 normal{ -edge.y, edge.x };
//		normal.Normalize();
//
//		std::vector<float> MinMaxA;
//
//		//2) Map the point to the Seperating Axis) 
//		for (int i = 0; i < sizeA; i++) {
//			MinMaxA.push_back(polA[i].Dot(normal));
//		}
//
//		//3) find the min max value of both 
//		float MaxA = MinMaxA[0];
//		float MinA = MinMaxA[0];
//		for (int i = 1; i < MinMaxA.size(); i++) {
//			if (MinMaxA[i] > MaxA) {
//				MaxA = MinMaxA[i];
//			}
//			else if (MinMaxA[i] < MinA) {
//				MinA = MinMaxA[i];
//			}
//		}
//
//		float pos2normal = circlePos.Dot(normal);
//		float MaxB = pos2normal + circleRadius;
//		float MinB = pos2normal - circleRadius;
//
//		//4) with MaxA, MinA, MaxB, MinB, check collison 
//		if (MinB > MaxA || MinA > MaxB)
//			return false;
//		else {
//			float ActualMax, ActualMin;
//			if (MaxA > MaxB)
//				ActualMax = MaxB;
//			else
//				ActualMax = MaxA;
//			if (MinA > MinB)
//				ActualMin = MinA;
//			else
//				ActualMin = MinB;
//			float currentoverlap = ActualMax - ActualMin;
//
//			//clamp penetration if too large 
//			if (currentoverlap > 1.0f)
//				currentoverlap = 1.0f;
//			if (overlap > currentoverlap) {  //only updates when a min penetration distance is found 
//				overlap = currentoverlap;
//				if (MinA < MinB)
//					minNormal = normal * -1.f;
//				else
//					minNormal = normal;
//			}
//		}
//
//	}
//
//	//Cirlce to corner 
//	for (size_t i = 0; i < sizeA; ++i)
//	{
//
//		//dispVec from circlepos to corner 
//		glm::vec3 dispVec = polA[i] - circlePos;
//		dispVec.Normalize();
//
//		//2) Map the point to the Seperating Axis) 
//		float pos2normal = circlePos.Dot(dispVec);
//		float MaxB = pos2normal + circleRadius;
//		float MinB = pos2normal - circleRadius;
//
//		std::vector<float> MinMaxA;
//
//		//2) Map the point to the Seperating Axis) 
//		for (int i = 0; i < sizeA; i++) {
//			MinMaxA.push_back(polA[i].Dot(dispVec));
//		}
//
//		//3) find the min max value of both 
//		float MaxA = MinMaxA[0];
//		float MinA = MinMaxA[0];
//		for (int i = 1; i < MinMaxA.size(); i++) {
//			if (MinMaxA[i] > MaxA) {
//				MaxA = MinMaxA[i];
//			}
//			else if (MinMaxA[i] < MinA) {
//				MinA = MinMaxA[i];
//			}
//		}
//
//		//3) with MaxA, MinA, MaxB, MinB, check collison 
//		if (MinB > MaxA || MinA > MaxB)
//			return false;
//		else {
//			float ActualMax, ActualMin;
//			if (MaxA > MaxB)
//				ActualMax = MaxB;
//			else
//				ActualMax = MaxA;
//			if (MinA > MinB)
//				ActualMin = MinA;
//			else
//				ActualMin = MinB;
//			float currentoverlap = ActualMax - ActualMin;
//
//			//clamp penetration if too large 
//			if (currentoverlap > 1.0f)
//				currentoverlap = 1.0f;
//			if (overlap > currentoverlap) {  //only updates when a min penetration distance is found 
//				overlap = currentoverlap;
//				if (MinA < MinB)
//					minNormal = dispVec * -1.f;
//				else
//					minNormal = dispVec;
//			}
//		}
//
//	}
//
//	if (offset != glm::vec3{ 0,0,0 }) {
//		glm::vec3 pt = circlePos + (minNormal * (circleRadius - overlap));
//		glm::vec3 direvec = pt - (SAT.pos + offset);
//		glm::vec3 e = glm::vec3{ -direvec.y, direvec.x };
//		cd.spinvec = Math::DegreeToRadian(SAT.angularVel) * e;
//	}
//	cd.pObj1 = &circle;
//	cd.pObj2 = &SAT;
//	cd.normal = -minNormal;
//	cd.pd = overlap;
//	return true;
//}
//
////////////////////////////////////////////LINE TO CIRCLE NO RESOLUTION /////////////////////////////////////
//bool OverlapCircle2Line(PhysicsObject& circle, float radius, std::vector<glm::vec3> line)
//{
//	glm::vec3 lineTangent = line[1] - line[0];
//	float lineLength = lineTangent.Length();
//	lineTangent.Normalize();
//
//	//1) Calc the normal to the line (our normal is a CCW 90-deg rotation of the tangent)
//	glm::vec3 normal{ -lineTangent.y, lineTangent.x };
//	//2) Find shortest distance to infinite line (projected distance along the normal)
//	glm::vec3 dispVec = circle.pos - line[0];
//
//	//3) No collision if distance is big enough
//	float shortestDist;
//
//	//4) Calc projected distance along the line tangent
//	float ProjtangentDist = (dispVec.Dot(lineTangent));
//
//	//5) check if projected distance lies within the line segment
//	//    a) if projectedDist < 0, shortest dist from ball to line is(ballpos - lineStart).Length()
//	if (ProjtangentDist < 0)
//		shortestDist = dispVec.Length();
//	else if (ProjtangentDist > lineLength) {
//		shortestDist = (circle.pos - line[1]).Length();
//	}
//	else {
//		shortestDist = fabs(dispVec.Dot(normal));
//	}
//	if (shortestDist >= radius)
//		return false;
//	else
//		return true;
//}


/////////// temp circle 2 sat
//bool SAT2Circle(PhysicsObject& circle, const float& circleRadius, PhysicsObject& SAT, const std::vector<glm::vec3>& polA, CollisionData& cd)
//{
//	size_t sizeA = polA.size();
//	glm::vec3 circlePos = circle.pos;
//
//	float overlap = std::numeric_limits<float>::infinity();
//	glm::vec3 minNormal = glm::vec3(0, 0, 0);
//
//	//PolyA
//	for (size_t i = 0; i < sizeA; ++i)
//	{
//		//pt1 and pt2 together forms an edge of the polygon
//		const glm::vec3& pt1 = polA[i];
//		const glm::vec3& pt2 = polA[(i + 1) % sizeA];
//
//		//TODO:
//		//1) compute the normal to the edge(aka the seperation Axis)
//		glm::vec3 edge = pt2 - pt1;
//		glm::vec3 normal{ -edge.y, edge.x };
//		normal.Normalize();
//
//		std::vector<float> MinMaxA;
//
//		//2) Map the point to the Seperating Axis) 
//		for (int i = 0; i < sizeA; i++) {
//			MinMaxA.push_back(polA[i].Dot(normal));
//		}
//
//		//3) find the min max value of both 
//		float MaxA = MinMaxA[0];
//		float MinA = MinMaxA[0];
//		for (int i = 1; i < MinMaxA.size(); i++) {
//			if (MinMaxA[i] > MaxA) {
//				MaxA = MinMaxA[i];
//			}
//			else if (MinMaxA[i] < MinA) {
//				MinA = MinMaxA[i];
//			}
//		}
//
//		float pos2normal = circlePos.Dot(normal);
//		float MaxB = pos2normal + circleRadius;
//		float MinB = pos2normal - circleRadius;
//
//		//4) with MaxA, MinA, MaxB, MinB, check collison 
//		if (MinB > MaxA || MinA > MaxB)
//			return false;
//		else {
//			float ActualMax, ActualMin;
//			if (MaxA > MaxB)
//				ActualMax = MaxB;
//			else
//				ActualMax = MaxA;
//			if (MinA > MinB)
//				ActualMin = MinA;
//			else
//				ActualMin = MinB;
//			float currentoverlap = ActualMax - ActualMin;
//
//			//clamp penetration if too large 
//			if (currentoverlap > 1.0f)
//				currentoverlap = 1.0f;
//			if (overlap > currentoverlap) {  //only updates when a min penetration distance is found 
//				overlap = currentoverlap;
//				if (MinA < MinB)
//					minNormal = normal * -1.f;
//				else
//					minNormal = normal;
//			}
//		}
//
//	}
//
//	//Cirlce to corner 
//	for (size_t i = 0; i < sizeA; ++i)
//	{
//
//		//dispVec from circlepos to corner 
//		glm::vec3 dispVec = polA[i] - circlePos;
//		dispVec.Normalize();
//
//		//2) Map the point to the Seperating Axis) 
//		float pos2normal = circlePos.Dot(dispVec);
//		float MaxB = pos2normal + circleRadius;
//		float MinB = pos2normal - circleRadius;
//
//		std::vector<float> MinMaxA;
//
//		//2) Map the point to the Seperating Axis) 
//		for (int i = 0; i < sizeA; i++) {
//			MinMaxA.push_back(polA[i].Dot(dispVec));
//		}
//
//		//3) find the min max value of both 
//		float MaxA = MinMaxA[0];
//		float MinA = MinMaxA[0];
//		for (int i = 1; i < MinMaxA.size(); i++) {
//			if (MinMaxA[i] > MaxA) {
//				MaxA = MinMaxA[i];
//			}
//			else if (MinMaxA[i] < MinA) {
//				MinA = MinMaxA[i];
//			}
//		}
//
//		//3) with MaxA, MinA, MaxB, MinB, check collison 
//		if (MinB > MaxA || MinA > MaxB)
//			return false;
//		else {
//			float ActualMax, ActualMin;
//			if (MaxA > MaxB)
//				ActualMax = MaxB;
//			else
//				ActualMax = MaxA;
//			if (MinA > MinB)
//				ActualMin = MinA;
//			else
//				ActualMin = MinB;
//			float currentoverlap = ActualMax - ActualMin;
//
//			//clamp penetration if too large 
//			if (currentoverlap > 1.0f)
//				currentoverlap = 1.0f;
//			if (overlap > currentoverlap) {  //only updates when a min penetration distance is found 
//				overlap = currentoverlap;
//				if (MinA < MinB)
//					minNormal = dispVec * -1.f;
//				else
//					minNormal = dispVec;
//			}
//		}
//
//	}
//	cd.pObj1 = &circle;
//	cd.pObj2 = &SAT;
//	cd.normal = -minNormal;
//	cd.pd = overlap;
//	return true;
//}



//void ResolveCircle2StaticLine(PhysicsObject& ball, float radius, const glm::vec3& lineStart, const glm::vec3& lineEnd)
//{
//	glm::vec3 lineTangent = lineEnd - lineStart;
//	float lineLength = lineTangent.Length();
//	lineTangent.Normalize();
//
//	float penetrationDist{};
//	glm::vec3 normal{};
//	glm::vec3 lineVec = ball.pos - lineStart;
//	float projectedDist = lineVec.Dot(lineTangent);
//
//	//ball is to the "right" of lineEnd
//	if (projectedDist > lineLength)
//	{
//		//TODO:
//		//1) calculate the collision normal
//		//2) calculate the penetration distance
//
//		//normal = ???
//		//penetrationDist = ???
//	}
//	//ball is to the "left" of lineStart
//	else if (projectedDist < 0)
//	{
//		//TODO:
//		//1) calculate the collision normal
//		//2) calculate the penetration distance
//
//		//normal = ???
//		//penetrationDist = ???
//	}
//	//ball is within line segment
//	else
//	{
//		//TODO:
//		//1) calculate the collision normal
//		//2) calculate the penetration distance
//
//		//normal = ???
//		//penetrationDist = ???
//	}
//
//	//resolve penetration and velocity
//	ball.pos += normal * penetrationDist;
//	ball.vel -= ball.vel.Dot(normal) * normal;
//}
//
//bool OverlapPt2Polygon(const glm::vec3& pt, const std::vector<glm::vec3>& polygon)
//{
//	size_t size = polygon.size();
//	for (size_t i = 0; i < size; ++i)
//	{
//		//pt1 and pt2 together forms an edge of the polygon
//		const glm::vec3& pt1 = polygon[i];
//		const glm::vec3& pt2 = polygon[(i + 1) % size];
//
//		//TODO:
//		//1) compute the normal to the edge
//		glm::vec3 edge = pt2 - pt1;
//		glm::vec3 normal{ -edge.y, edge.x };
//		glm::vec3 dispVec = pt - pt1;
//
//		float Dist = dispVec.Dot(normal);
//
//		//2) if pt is "outside" of the line, there is no collision with this polygon. just return false.
//		if (Dist > 0) {
//			return false;
//		}
//
//
//		//3) if you can do this question, you will be able to understand the Separating Axis Theorem for collision test betw any 2 convex polygons
//	}
//
//	return true;
//}
//
//
//
////glm::vec3 SAT2Line(const std::vector<glm::vec3>& polA, const std::vector<glm::vec3>& line, const glm::vec3& polA_pos)
//{
//	bool temp = false;
//	glm::vec3 DispVec{ 0,0 };
//	SAT(polA, line, DispVec, temp);
//
//	if (DispVec != glm::vec3{ 0,0 }) {
//		glm::vec3 DispVec = polA_pos - line[0]; //from line start 
//		DispVec.Normalize();
//		return DispVec;
//	}
//	else
//		return glm::vec3{ 0,0 };
//}

//bool OverlapCircle2Circle(const glm::vec3& pos1, float r1, const glm::vec3& pos2, float r2, glm::vec3& vector)
//{
//	//TODO: do it yourself
//	vector = pos1 - pos2;
//	//to avoid square root usage, make then both squared instead 
//	return (pos2 - pos1).LengthSquared() <= (r2 + r1) * (r2 + r1);
//}
//
//
//
//void ResolveCircle2Circle(PhysicsObject& ball1, float radius1, PhysicsObject& ball2, float radius2, glm::vec3& vector)
//{
//	//1) resolve penetration
//	float pendist = (radius1 + radius2) - (ball1.pos - ball2.pos).Length();
//	glm::vec3 normal = vector.Normalized();
//	ball1.pos += pendist * normal;
//
//
//	//2) resolve velocity (cant do 2 moving objects yet 1/12/25)
//	glm::vec3 opp = ball1.vel.Dot(normal) * normal; //vector triangle
//	ball1.vel -= (ball1.bounciness + 1) * opp;	//2) implement bounciness
//
//}
//
//
//// cant deal with non moving velocity 1/8/2025 
//void ResolveCircle2AABB(PhysicsObject& box, PhysicsObject& circle, float radius, glm::vec3 &vector)
//{
//	// from Box's perspective 
//	//1) resolve penetration
//	float pendist = radius - vector.Length();
//	glm::vec3 normal = vector.Normalized();
//	box.pos += pendist * -normal;
//	circle.pos += pendist * normal;
//
//	////2) resolve velocity 
//	//glm::vec3 circleopp = circle.vel.Dot(normal) * normal; 
//	//glm::vec3 boxopp = box.vel.Dot(normal) * -normal; //vector triangle
//
//	////3) implement bounciness
//	//float minbounce = 0.f;
//	//if (box.bounciness > circle.bounciness)
//	//	minbounce = circle.bounciness;
//	//else
//	//	minbounce = box.bounciness;
//
//	//box.vel -= (minbounce + 1) * boxopp;
//	//circle.vel -= (minbounce + 1) * circleopp;
//}


//
//void ResolveSAT(PhysicsObject& SAT1, PhysicsObject& SAT2, glm::vec3& vector, float pen)
//{
//	//1) resolve penetration
//	glm::vec3 normal = vector.Normalized();
//	SAT1.pos += pen * normal;
//	SAT2.pos += pen * -normal;
//
//}
//
//

//NOTE: consider shifting this to CollisionDetection.cpp so you can reuse the function
//float OverlapCircle2AABB(glm::vec3 circlePos, float radius, glm::vec3& resolvenormal, glm::vec3 boxMin, glm::vec3 boxMax)
//{
//	float nearestPoint, nearestPoint2, distancesquared;
//	//TODO:: implement this
//	if (circlePos.x > boxMax.x) {
//		nearestPoint = boxMax.x;
//	}
//	else if (circlePos.x < boxMin.x) {
//		nearestPoint = boxMin.x;
//	}
//	else
//		nearestPoint = circlePos.x;
//
//	if (circlePos.y > boxMax.y) {
//		nearestPoint2 = boxMax.y;
//	}
//	else if (circlePos.y < boxMin.y) {
//		nearestPoint2 = boxMin.y;
//	}
//	else
//		nearestPoint2 = circlePos.y;
//
//	glm::vec3 nearpos(nearestPoint, nearestPoint2, 0.f);
//	glm::vec3 dispVec = circlePos - nearpos;
//
//	//normal checking 
//	if (nearestPoint == circlePos.x || nearestPoint2 == circlePos.y) {
//		resolvenormal = glm::vec3{ -nearpos.y , nearpos.x };
//		resolvenormal.Normalize();
//	}
//	else {
//		resolvenormal = dispVec.Normalized();
//	}
//
//	distancesquared = dispVec.LengthSquared();
//	if (distancesquared <= radius * radius) {
//		float penetration = radius - sqrt(distancesquared);
//		if (penetration > 1.0f)
//			penetration = 1.0f;
//		return penetration;
//	}
//	else
//		return 0.f;
//}




