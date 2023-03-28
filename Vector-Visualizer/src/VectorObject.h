#pragma once

#include "external/glm/glm.hpp"

class VectorObject
{
public:
	VectorObject();
	VectorObject(glm::vec3 origin,
		glm::vec3 direction,
		glm::vec4 color);
	~VectorObject();
public:
	glm::vec3 m_Origin;
	glm::vec3 m_Direction;
	glm::vec4 m_Color;
};