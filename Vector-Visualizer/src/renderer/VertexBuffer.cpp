#include "VertexBuffer.h"

#include <iostream>
#include "glad/glad.h"

VertexBuffer::VertexBuffer(const void *data, unsigned int size, MODE mode)
{
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	if (mode == MODE::STATIC) {
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}
	else if (mode == MODE::DYNAMIC) {
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
	}
	else {
		glDeleteBuffers(1, &m_ID);
		std::cout << "Buffer mode error." << std::endl;
	}
	

	int bufferSize;
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
	if (size != bufferSize)
	{
		glDeleteBuffers(1, &m_ID);
		std::cout << "Buffer error." << std::endl;
	}
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_ID);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}