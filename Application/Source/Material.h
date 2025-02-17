#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

struct Material
{
	glm::vec3 kAmbient;
	glm::vec3 kDiffuse;
	glm::vec3 kSpecular;
	float kShininess;

	unsigned size;

	Material() :
		kAmbient(0.0f, 0.0f, 0.0f),
		kDiffuse(0.0f, 0.0f, 0.0f),
		kSpecular(0.0f, 0.0f, 0.0f),
		kShininess(1.f),
		size(0)
	{
	
	}
	// Step 6 - Add the operator= function
	Material& operator=(const Material& rhs)
	{
		kAmbient = rhs.kAmbient;
		kDiffuse = rhs.kDiffuse;
		kSpecular = rhs.kSpecular;
		kShininess = rhs.kShininess;
		size = rhs.size;
		return *this;
	}
};


#endif