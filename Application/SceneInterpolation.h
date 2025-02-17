#pragma once
#include <glm\gtx\compatibility.hpp>

class SceneInterpolation
{
public:

private:
	float time; // keeps track of the time for the interpolation to run
	float duration; // length of the interpolation time
	glm::vec3 currentPos; // keeps track of current position
};

