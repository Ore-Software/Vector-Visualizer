#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <functional>

#include "../external/imgui/imgui.h"

#include "../VectorObject.h"
#include "../BufferUtils.h"
#include "../renderer/VertexBuffer.h"
#include "../renderer/VertexArray.h"

namespace displayMode
{
	class Mode
	{
	public:
		Mode() {}
		virtual ~Mode() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() { m_VectorVA->Bind(); }
		virtual void OnImGuiRender() {}
		virtual void Redraw() {m_VectorVA->Bind();
								m_VectorVB->Bind();
								glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_VectorBuffer->size(), m_VectorBuffer->data(), GL_DYNAMIC_DRAW);}

	public:
		std::shared_ptr<std::vector<VectorObject>> m_Vectors;
		std::shared_ptr<std::vector<float>> m_VectorBuffer;
		std::shared_ptr<VertexArray> m_VectorVA;
		std::shared_ptr<VertexBuffer> m_VectorVB;
	};
}