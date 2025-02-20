#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "Mesh.h"
#include "Vertex.h"

/******************************************************************************/
/*!
		Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{
public:
	static Mesh* GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateQuad(const std::string& meshName, glm::vec3 color, float length = 1.f);
	static Mesh* GenerateSphere(const std::string& meshName,
		glm::vec3 color,
		float radius = 1.f,
		int numSlice = 360,
		int numStack = 360);
	static Mesh* GenerateCyclinder(const std::string& meshName, glm::vec3 color, unsigned numSlice, float radius, float height);
	static Mesh* GenerateHemisphere(const std::string& meshName, glm::vec3 color, int numStack, int numSlice, float radius);
	static Mesh* GenerateCube(const std::string& meshName, glm::vec3 color, float length = 1.f);

	static Mesh* GenerateHWall1(const std::string& meshName, glm::vec3 color);
	static Mesh* GenerateHWall2(const std::string& meshName, glm::vec3 color);
	static Mesh* GenerateHWall3(const std::string& meshName, glm::vec3 color);
	static Mesh* GenerateHWall4(const std::string& meshName, glm::vec3 color);
	static Mesh* GenerateHWall5(const std::string& meshName, glm::vec3 color);

	// Step 7
	static Mesh* GenerateOBJ(const std::string& meshName, const
		std::string& file_path);

	// Step 8
	static Mesh* GenerateOBJMTL(const std::string& meshName, const
		std::string& file_path, const std::string& mtl_path);

	static Mesh* GenerateText(const std::string& meshName,
		unsigned numRow, unsigned numCol);
};

#endif