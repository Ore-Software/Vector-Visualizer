#pragma once

#include "external/glm/glm.hpp"

class VectorObject
{
public:
	VectorObject();
	VectorObject(glm::vec3 origin,
		glm::vec3 endPoint,
		glm::vec4 color);
	~VectorObject();
public:
	glm::vec3 m_Origin;
	glm::vec3 m_EndPoint;
	glm::vec4 m_Color;
};