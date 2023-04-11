#include "VectorObject.h"

VectorObject::VectorObject()
{
	m_Origin = glm::vec3(0.0f, 0.0f, 0.0f);
	m_Direction = glm::vec3(1.0f, 1.0f, 1.0f);
	m_Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

VectorObject::VectorObject(glm::vec3 origin, glm::vec3 direction, glm::vec4 color)
	:m_Origin(origin), m_Direction(direction), m_Color(color)
{
}

VectorObject::~VectorObject()
{
}

glm::vec3 VectorObject::GetEndPoint()
{
	return m_Origin + m_Direction;
}