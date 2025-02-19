#ifndef MESH_H
#define MESH_H

#include <string>
#include "Material.h"
#include <vector>
/******************************************************************************/
/*!
		Class Mesh:
\brief	To store VBO (vertex & color buffer) and IBO (index buffer)
*/
/******************************************************************************/
class Mesh
{
public:
	enum DRAW_MODE
	{
		DRAW_TRIANGLES, //default mode
		DRAW_TRIANGLE_STRIP,
		DRAW_LINES,
		DRAW_MODE_LAST,
	};
	Mesh(const std::string &meshName);
	~Mesh();
	void Render();
	void Render(unsigned offset, unsigned count);

	// Step 10 - Add the following function
	static void SetMaterialLoc(unsigned kA, unsigned
		kD, unsigned kS, unsigned nS);

	std::vector<Material> materials;
	static unsigned locationKa;
	static unsigned locationKd;
	static unsigned locationKs;
	static unsigned locationNs;

	const std::string name;
	DRAW_MODE mode;
	unsigned vertexBuffer;
	unsigned indexBuffer;
	unsigned indexSize;

	Material material;

	unsigned textureID; // Step 2 - Add the textureID variable

	// Step 11 - Add the following variables
};

#endif