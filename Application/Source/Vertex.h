#ifndef VERTEX_H
#define VERTEX_H

//struct Position
//{
//	float x, y, z;
//	Position(float x = 0, float y = 0, float z = 0) { Set(x, y, z); }
//	void Set(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
//};
//struct Color
//{
//	float r, g, b;
//	Color(float r = 1, float g = 1, float b = 1) { Set(r, g, b); }
//	void Set(float r, float g, float b) { this->r = r; this->g = g; this->b = b; }
//};

// GLM Headers
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

struct Vertex
{
	//Position pos;
	//Color color;
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec3 normal;
	glm::vec2 texCoord; // Step 1 - Add Texture Coordinate
};

#endif