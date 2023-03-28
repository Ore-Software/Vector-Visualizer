#pragma once

enum class MODE {
	STATIC, DYNAMIC
};

class VertexBuffer
{
private:
	unsigned int m_ID;

public:
	VertexBuffer(const void *data, unsigned int size, MODE mode);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};