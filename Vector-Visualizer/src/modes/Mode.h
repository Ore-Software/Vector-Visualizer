#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <functional>

#include "../external/imgui/imgui.h"

#include "../VectorObject.h"
#include "../VectorObjectUtils.h"
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

	public:
		std::shared_ptr<std::vector<VectorObject>> m_Vectors;
		std::shared_ptr<std::vector<float>> m_VectorBuffer;
		std::shared_ptr<VertexArray> m_VectorVA;
		std::shared_ptr<VertexBuffer> m_VectorVB;
	};
}