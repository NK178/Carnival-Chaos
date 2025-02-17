#include "CollisionDetection.h"

//bool OverlapCircle2Circle(const Vector3& pos1, float r1, const Vector3& pos2, float r2)
//{
//	//TODO: do it yourself
//	//return (pos2 - pos1).Length() <= r1 + r2;
//	//comparision using squared distance
//	return (pos2 - pos1).LengthSquared() <= (r1 + r2) * (r1 + r2);
//	//float spDist = (pos2 - pos1).LengthSquared();
//	//return false;
//}

//bool OverlapAABB2AABB(const Vector3& min1, const Vector3& max1,
//					  const Vector3& min2, const Vector3& max2)
//{
//	return !(max1.x < min2.x || min1.x > max2.x || max1.y < min2.y || min1.y > max2.y);
//}



//bool OverlapCircle2Circle(PhysicsObject& obj1, float r1, PhysicsObject& obj2, float r2, CollisionData& cd)
//{
//	Vector3 dispVec = obj1.pos - obj2.pos;
//	if (dispVec.LengthSquared() <= (r1 + r2) * (r1 + r2))
//	{
//		// Calculate collision data
//		cd.pObj1 = &obj1;
//		cd.pObj2 = &obj2;
//
//		float len = dispVec.Length();
//		cd.pd = (r1 + r2) - len;
//		cd.normal = dispVec * (1.f / len); // calculate collision normal
//
//
//		return true;
//	}
//
//	return false;
//}
//
////bool OverlapCircle2Line(PhysicsObject& ball, float radius, const Vector3& lineStart, const Vector3& lineEnd, CollisionData& cd)
////{
////	//Vector3 lineTangent = lineEnd - lineStart;
////	//if (lineTangent.IsZero()) return false;  //this is not a line, but a point. reject collision test
////
////	//float lineLength = lineTangent.Length();
////	//lineTangent.Normalize();
////
////
////
////	////TODO:
////	////1) Calc the normal to the line (our normal is a CCW 90-deg rotation of the tangent)
////	//Vector3 normal{ -lineTangent.y, lineTangent.x };
////	////2) Find shortest distance to infinite line (projected distance along the normal)
////	//Vector3 dispVec = ball.pos - lineStart;
////	//float shortestDist = dispVec.Dot(normal);
////	//if (shortestDist < 0)
////	//	shortestDist = -shortestDist;
////	////FOR TESTING ONLY
////
////	////3) No collision if distance is big enough
////	////4) Calc projected distance along the line tangent
////	//float ProjectedDist = dispVec.Dot(lineTangent);
////	////5) check if projected distance lies within the line segment
////	////    a) if projectedDist > lineLength, shortest dist from ball to line is (ballpos - lineEnd).Length()
////	//if (ProjectedDist > lineLength)
////	//{
////	//	shortestDist = (ball.pos - lineEnd).Length();
////	//}
////	////	  b) if projectedDist < 0, shortest dist from ball to line is (ballpos - lineStart).Length()
////	//if (ProjectedDist < 0)
////	//{
////	//	shortestDist = (ball.pos - lineStart).Length();
////	//}
////	////6) Use calculated shortest dist to determine whether a collision has taken place
////	//if (shortestDist <= radius)
////	//{
////	//	cd.pObj1 = &ball;
////	//	//PhysicsObject line;
////	//	//cd.pObj2 = &line;
////	//	cd.normal = normal;
////	//	cd.pd = shortestDist;
////	//	return true;
////	//}
////	//else
////	//	return false;
////}
//
//bool OverlapCircle2Line(PhysicsObject& obj1, float radius, const Vector3& lineA, const Vector3& lineB, CollisionData& cd)
//{
//	// Get line segment vectors
//	Vector3 lineDir = lineB - lineA;
//	Vector3 toCircle = obj1.pos - lineA;
//
//	// Project circle's position onto the line segment
//	float t = toCircle.Dot(lineDir) / lineDir.LengthSquared();
//	t = Math::Clamp(t, 0.0f, 1.0f);
//	Vector3 closestPoint = lineA + lineDir * t;
//
//	// Calculate distance from circle to closest point
//	Vector3 diff = obj1.pos - closestPoint;
//	float distanceSq = diff.LengthSquared();
//
//	if (distanceSq > radius * radius) return false;
//
//	// Calculate collision data
//	cd.pObj1 = &obj1;
//	cd.pObj2 = nullptr;
//
//	float distance = std::sqrt(distanceSq);
//	cd.pd = radius - distance;
//	cd.normal = diff * (1.0f / distance);
//
//	return true;
//}
//
//bool ResolveCircle2Line(PhysicsObject& ball, float radius, const Vector3& lineStart, const Vector3& lineEnd, CollisionData& cd)
//{
//	Vector3 lineTangent = lineEnd - lineStart;
//	float lineLength = lineTangent.Length();
//	if (lineLength == 0.0f) return false; //(single point) no need to resolve
//	lineTangent.Normalize();
//
//	float penetrationDist{};
//	Vector3 normal{};
//	Vector3 lineVec = ball.pos - lineStart; // displacement vector from start to ball
//	float projectedDist = lineVec.Dot(lineTangent); // essentially the x-axis distance of the ball from the start
//	Vector3 Collision;
//	float dist;
//
//	PhysicsObject line;
//	line.pos = (lineEnd - lineStart) * 0.5f;
//
//	cd.pObj1 = &ball;
//
//	//ball is to the "right" of lineEnd
//	if (projectedDist > lineLength)
//	{
//		//TODO:
//		//1) calculate the collision normal
//		Collision = ball.pos - lineEnd;
//		dist = Collision.Length();
//		normal = Collision * (1 / dist);
//		//2) calculate the penetration distance
//		penetrationDist = radius - dist;
//
//		//normal = ???
//		//penetrationDist = ???
//
//		cd.normal = normal;
//		cd.pd = penetrationDist;
//
//		return true;
//	}
//	//ball is to the "left" of lineStart
//	else if (projectedDist < 0)
//	{
//		//TODO:
//		//1) calculate the collision normal
//		dist = lineVec.Length();
//		normal = lineVec * (1 / dist);
//
//		//2) calculate the penetration distance
//		penetrationDist = radius - dist;
//
//		//normal = ???
//		//penetrationDist = ???
//		cd.normal = normal;
//		cd.pd = penetrationDist;
//
//		return true;
//
//	}
//	//ball is within line segment
//	else
//	{
//		//TODO:
//		//1) calculate the collision normal
//		Collision = ball.pos - (lineStart + projectedDist * lineTangent);
//		dist = Collision.Length();
//		normal = Collision * (1 / dist);
//
//		//2) calculate the penetration distance
//		penetrationDist = radius - dist;
//
//		cd.normal = normal;
//		cd.pd = penetrationDist;
//
//		return true;
//
//	}
//	//resolve penetration and velocity
//	//ball.pos += normal * penetrationDist;
//	//ball.vel -= ball.vel.Dot(normal) * normal;
//	return false;
//}
//
//bool OverlapCircle2AABB(Vector3 circlePos, float radius, Vector3 boxMin, Vector3 boxMax)
//{
//	Vector3 NearestPos;
//	Vector3 Dist;
//	//TODO:: implement this
//	//1) FIND NEAREST POINT ON AABB TO CIRCLE
//	if (circlePos.x > boxMax.x)
//	{
//		NearestPos.x = boxMax.x;
//	}
//	else if (circlePos.x < boxMin.x)
//	{
//		NearestPos.x = boxMin.x;
//	}
//	else
//	{
//		NearestPos.x = circlePos.x;
//	}
//	if (circlePos.y > boxMax.y)
//	{
//		NearestPos.y = boxMax.y;
//	}
//	else if (circlePos.y < boxMin.y)
//	{
//		NearestPos.y = boxMin.y;
//	}
//	else
//	{
//		NearestPos.y = circlePos.y;
//	}
//	//2) CALCULATE DIST FROM NEAREST PT TO CIRCLE
//	if (NearestPos.x < circlePos.x)
//	{
//		Dist.x = circlePos.x - NearestPos.x;
//	}
//	else if (NearestPos.x > circlePos.x)
//	{
//		Dist.x = NearestPos.x - circlePos.x;
//	}
//	if (NearestPos.y < circlePos.y)
//	{
//		Dist.y = circlePos.y - NearestPos.y;
//	}
//	if (NearestPos.y > circlePos.y)
//	{
//		Dist.y = NearestPos.y - circlePos.y;
//	}
//	//abs(Dist.x);
//	//abs(Dist.y);
//	//3) CHECK IF DIST IS SMALL ENOUGH FOR AN OVERLAP
//	//checking for diagonal
//	//TDist.x = (boxMax.x - boxMin.x)/2;
//	//TDist.y = (boxMax.y - boxMin.y)/2;
//	//abs(TDist.x);
//	//abs(TDist.y);
//	//if (TDist.x <= radius && TDist.y <= radius)
//
//	if (Dist.x <= radius && Dist.y <= radius && (Dist.x * Dist.x) + (Dist.y * Dist.y) <= radius * radius)
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}
//
//bool SAT2Circle(PhysicsObject& circle, const float& circleRadius, PhysicsObject& SAT, const std::vector<Vector3>& polA, Vector3& contactpt, const Vector3& offset, CollisionData& cd)
//{
//	size_t sizeA = polA.size();
//	Vector3 circlePos = circle.pos;
//
//	float overlap = std::numeric_limits<float>::infinity();
//	Vector3 minNormal = Vector3(0, 0, 0);
//
//	//PolyA
//	for (size_t i = 0; i < sizeA; ++i)
//	{
//		//pt1 and pt2 together forms an edge of the polygon
//		const Vector3& pt1 = polA[i];
//		const Vector3& pt2 = polA[(i + 1) % sizeA];
//
//		//TODO:
//		//1) compute the normal to the edge(aka the seperation Axis)
//		Vector3 edge = pt2 - pt1;
//		Vector3 normal{ -edge.y, edge.x };
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
//		Vector3 dispVec = polA[i] - circlePos;
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
//	if (offset != Vector3{ 0,0,0 }) {
//		Vector3 pt = circlePos + (minNormal * (circleRadius - overlap));
//		Vector3 direvec = pt - (SAT.pos + offset);
//		Vector3 e = Vector3{ -direvec.y, direvec.x };
//		cd.spinvec = Math::DegreeToRadian(SAT.angularVel) * e;
//	}
//	cd.pObj1 = &circle;
//	cd.pObj2 = &SAT;
//	cd.normal = -minNormal;
//	cd.pd = overlap;
//	return true;
//}
//
//bool OverlapCircle2OBB(PhysicsObject& circle, float radius, PhysicsObject& Box, const float Width, const float Height, CollisionData& cd)
//{
//	// Resolving collision with AABB and Circle.
//	// TODO:
//	// Set the normal for the 4 sides
//
//	cd.pd = FLT_MAX;
//
//	//Vector3 pt1 = Box.pos - Vector3(Width, -Height); // top left
//	//Vector3 pt2 = Box.pos + Vector3(Width, Height); // top right
//	//Vector3 pt3 = Box.pos - Vector3(Width, Height); // bottom left
//	//Vector3 pt4 = Box.pos + Vector3(Width, -Height); // bottom right
//
//	float BoxAngleRadT = Math::DegreeToRadian(Box.angleDeg + 90);
//	float BoxAngleRadR = Math::DegreeToRadian(Box.angleDeg);
//
//	Vector3 RightNormal{ cos(BoxAngleRadR), sin(BoxAngleRadR) };
//	Vector3 TopNormal{ cos(BoxAngleRadT), sin(BoxAngleRadT) };
//	Vector3 LeftNormal = -RightNormal;
//	Vector3 BottomNormal = -TopNormal;
//
//	//CHECK TOP AXIS
//	// Project the min and max of the circle on the normal axis
//	float CTopAxisMin = (circle.pos).Dot(TopNormal) - radius;
//	float CTopAxisMax = (circle.pos).Dot(TopNormal) + radius;
//	// Project the min and max of the AABB on the normal axis
//	float BTopAxisWMin = Box.pos.Dot(TopNormal) - Height * 0.5f;
//	float BTopAxisWMax = Box.pos.Dot(TopNormal) + Height * 0.5f;
//
//
//	if (CTopAxisMax < BTopAxisWMin || CTopAxisMin > BTopAxisWMax)
//	{
//		return false;
//	}
//	else
//	{
//		cd.pd = BTopAxisWMax - CTopAxisMin;
//		cd.normal = TopNormal;
//
//	}
//
//	//RIGHT AXIS
//	float CRightAxisMin = (circle.pos).Dot(RightNormal) - radius;
//	float CRightAxisMax = (circle.pos).Dot(RightNormal) + radius;
//	float BRightAxisHMin = Box.pos.Dot(RightNormal) - Width * 0.5f;
//	float BRightAxisHMax = Box.pos.Dot(RightNormal) + Width * 0.5f;
//
//	if (CRightAxisMax < BRightAxisHMin || CRightAxisMin > BRightAxisHMax)
//	{
//		return false;
//	}
//	else
//	{
//		if (cd.pd > BRightAxisHMax - CRightAxisMin)
//		{
//			cd.pd = BRightAxisHMax - CRightAxisMin;
//			cd.normal = RightNormal;
//		}
//	}
//
//	float CBottomAxisMin = (circle.pos).Dot(BottomNormal) - radius;
//	float CBottomAxisMax = (circle.pos).Dot(BottomNormal) + radius;
//	float BBottomAxisWMin = Box.pos.Dot(BottomNormal) - Height * 0.5f;
//	float BBottomAxisWMax = Box.pos.Dot(BottomNormal) + Height * 0.5f;
//
//	if (CBottomAxisMax < BBottomAxisWMin || CBottomAxisMin > BBottomAxisWMax)
//	{
//		return false;
//	}
//	else
//	{
//		if (cd.pd > BBottomAxisWMax - CBottomAxisMin)
//		{
//			cd.pd = BBottomAxisWMax - CBottomAxisMin;
//			cd.normal = BottomNormal;
//		}
//	}
//
//	float CLeftAxisMin = (circle.pos).Dot(LeftNormal) - radius;
//	float CLeftAxisMax = (circle.pos).Dot(LeftNormal) + radius;
//	float BLeftAxisHMin = Box.pos.Dot(LeftNormal) - Width * 0.5f;
//	float BLeftAxisHMax = Box.pos.Dot(LeftNormal) + Width * 0.5f;
//
//	if (CLeftAxisMax < BLeftAxisHMin || CLeftAxisMin > BLeftAxisHMax)
//	{
//		return false;
//	}
//	else
//	{
//		if (cd.pd > BLeftAxisHMax - CLeftAxisMin)
//		{
//			cd.pd = BLeftAxisHMax - CLeftAxisMin;
//			cd.normal = LeftNormal;
//		}
//	}
//
//	cd.pObj1 = &circle;
//	cd.pObj2 = &Box;
//	return true;
//	// repeat these steps 4 times to check for 4 sides
//	// find the minimum penetration
//	// use cd to key in the collisionData of normal and pd alongside the objects
//}
//
//
//bool ScenePolygonPt::OverlapPt2Polygon(const Vector3& pt, const std::vector<Vector3>& polygon) const
//{
//	size_t size = polygon.size();
//	for (size_t i = 0; i < size; ++i)
//	{
//		//pt1 and pt2 together forms an edge of the polygon
//		const Vector3& pt1 = polygon[i];
//		const Vector3& pt2 = polygon[(i + 1) % size];
//
//		//TODO:
//		//1) compute the normal to the edge
//		Vector3 SideTangent = pt2 - pt1;
//		float SideLine = SideTangent.Length();
//		// position of mouse to line start
//		Vector3 DispVec = pt - pt1;
//		SideTangent.Normalize();
//		Vector3 normal{ -SideTangent.y,SideTangent.x };
//		float ShortestDist = DispVec.Dot(normal);
//		//2) if pt is "outside" of the line, there is no collision with this polygon. just return false.
//		if (ShortestDist > 0)
//			return false;
//		//3) if you can do this question, you will be able to understand the Separating Axis Theorem for collision test betw any 2 convex polygons
//
//	}
//	return true;
//}
//
//bool SeparatingAxisTheorem(PhysicsObject& Poly1, const std::vector<Vector3>& polygon1, PhysicsObject& Poly2, const std::vector<Vector3>& polygon2, CollisionData& cd) // this for Polygon against Polygon (NEED TO MAKE A NEW ONE FOR AABB TO CIRCLE)
//{
//	size_t sizeA = polygon1.size();
//	size_t sizeB = polygon2.size();
//	float overlap = std::numeric_limits<float>::infinity();
//	Vector3 minNormal = Vector3(0, 0, 0);
//
//	//PolyA
//	for (size_t i = 0; i < sizeA; ++i)
//	{
//		//pt1 and pt2 together forms an edge of the polygon
//		const Vector3& pt1 = polygon1[i];
//		const Vector3& pt2 = polygon1[(i + 1) % sizeA];
//
//		//TODO:
//		//1) compute the normal to the edge(aka the seperation Axis)
//		Vector3 edge = pt2 - pt1;
//		Vector3 normal{ -edge.y, edge.x };
//		normal.Normalize();
//
//		std::vector<float> MinMaxA;
//		std::vector<float> MinMaxB;
//
//		//2) Map the point to the Seperating Axis) 
//		for (int i = 0; i < sizeA; i++) {
//			MinMaxA.push_back(polygon1[i].Dot(normal));
//		}
//		for (int i = 0; i < sizeB; i++) {
//			MinMaxB.push_back(polygon2[i].Dot(normal));
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
//			return 0.0f;
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
//		const Vector3& pt1 = polygon2[i];
//		const Vector3& pt2 = polygon2[(i + 1) % sizeB];
//
//		//TODO:
//		//1) compute the normal to the edge(aka the seperation Axis)
//		Vector3 edge = pt2 - pt1;
//		Vector3 normal{ -edge.y, edge.x };
//		normal.Normalize();
//
//		std::vector<float> MinMaxA;
//		std::vector<float> MinMaxB;
//
//		//2) Map the point to the Seperating Axis) 
//		for (int i = 0; i < sizeA; i++) {
//			MinMaxA.push_back(polygon1[i].Dot(normal));
//		}
//		for (int i = 0; i < sizeB; i++) {
//			MinMaxB.push_back(polygon2[i].Dot(normal));
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
//			return 0.0f;
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
//	cd.pObj1 = &Poly1;
//	cd.pObj2 = &Poly2;
//	cd.normal = minNormal;
//	cd.normal.Normalize();
//	cd.pd = overlap;
//	return true;
//}
//
//void ResolveCircle2StaticCircle(PhysicsObject& ball1, float radius1, PhysicsObject& ball2, float radius2)
//{
//	float pnt;
//	Vector3 normal;
//	//1) resolve penetration
//	pnt = (radius1 + radius2) - (ball1.pos - ball2.pos).Length();
//	//Resolve Collision Normal
//	normal = (ball1.pos - ball2.pos).Normalized();
//	ball1.pos = ball1.pos + normal * pnt;
//	//2) resolve velocity
//	Vector3 Wheee;
//	Wheee = ((ball1.vel).Dot(normal)) * normal;
//	ball1.vel = ball1.vel - ((1 + ball1.bounciness) * Wheee);
//	//3) implement bounciness
//}
//
//void ResolveCircle2Circle(PhysicsObject& Ball1, float r1, PhysicsObject& Ball2, float r2)
//{
//	if (Ball1.mass <= 0)
//	{
//
//	}
//	float PntDist, W;
//	Vector3 normal , DispVec, UVel1, UVel2;
//	DispVec = Ball1.pos - Ball2.pos;
//	PntDist = (r1 + r2) - DispVec.Length();
//	normal = DispVec.Normalized();
//	Ball1.pos = Ball1.pos + normal * PntDist * (Ball2.mass / (Ball1.mass + Ball2.mass));
//	Ball2.pos = Ball2.pos - normal * PntDist * (Ball1.mass / (Ball1.mass + Ball2.mass));
//	UVel1 = Ball1.vel;
//	UVel2 = Ball2.vel;
//	W = (Ball1.vel).Dot(normal) - (Ball2.vel).Dot(normal);
//	// Now to include for the mass
//	Ball1.vel = UVel1 - 2 * (Ball2.mass / (Ball1.mass + Ball2.mass)) * (W) * normal;
//	Ball2.vel = UVel2 - 2 * (Ball1.mass / (Ball1.mass + Ball2.mass)) * (-W) * normal;
//}
//
//void ResolveCollision(CollisionData& Collide) // TO::DO !
//{
//
//	if (Collide.pObj2 == nullptr)
//	{
//		PhysicsObject& obj = *Collide.pObj1;
//
//		// Resolve penetration
//		obj.pos += Collide.normal * Collide.pd;
//
//		// Resolve velocity (reflect over normal)
//		float velocityDot = obj.vel.Dot(Collide.normal);
//		obj.vel -= 2.0f * velocityDot * Collide.normal;
//	}
//	else
//	{
//		PhysicsObject& obj1 = *Collide.pObj1;
//		PhysicsObject& obj2 = *Collide.pObj2;
//
//
//
//		float totalInvMass;
//		float bounciness;
//
//		if (Collide.pObj2 == NULL)
//		{
//			obj2.bounciness = 0;
//			obj2.mass = 0;
//		}
//		totalInvMass = 1.f / (obj1.mass + obj2.mass);
//		bounciness = std::min(obj1.bounciness, obj2.bounciness);
//
//		Vector3 W;
//
//		if (obj1.mass > 0 && obj2.mass == 0)
//		{
//			// resolve collision differently
//			W = (obj1.vel.Dot(Collide.normal) - obj2.vel.Dot(Collide.normal)) * Collide.normal;
//
//			obj1.pos += Collide.pd * Collide.normal;
//			obj1.vel -= (1 + bounciness) * W;
//		}
//		else
//		{
//			// this is under the assumption that mass != 0 (Well in theory, a massless obj will go at the speed of light)
//			   // resolve penetration
//			obj1.pos += (obj2.mass * totalInvMass) * Collide.pd * Collide.normal;
//			obj2.pos -= (obj1.mass * totalInvMass) * Collide.pd * Collide.normal;
//
//			W = (obj1.vel.Dot(Collide.normal) - obj2.vel.Dot(Collide.normal)) * Collide.normal;
//			// Now to include for the mass
//			obj1.vel -= (1 + bounciness) * obj2.mass * totalInvMass * W;
//			obj2.vel += (1 + bounciness) * obj1.mass * totalInvMass * W;
//		}
//	}
//}