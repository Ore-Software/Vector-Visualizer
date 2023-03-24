#pragma once

#include <string>

#include "glad/glad.h"
#include "../external/glm/glm.hpp"

class Shader
{
public:
	Shader(const std::string &vertexFilepath, const std::string &fragmentFilepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	GLuint GetID() const;

	//Set uniforms
	
	void SetUniformMat4f(const std::string &name, const glm::mat4 &matrix) const;
private:

	GLint GetUniformLocation(const std::string &name) const;

private:
	GLuint m_ID;
};