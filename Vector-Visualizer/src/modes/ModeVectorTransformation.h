#pragma once

#include "Mode.h"

#include "glad/glad.h"

namespace displayMode 
{
	class ModeVectorTransformation : public Mode
	{
	public:
		ModeVectorTransformation();
		ModeVectorTransformation(std::shared_ptr<std::vector<VectorObject>> vectors, std::shared_ptr<std::vector<float>> vectorBuffer, std::shared_ptr<VertexArray> vectorVA, std::shared_ptr<VertexBuffer> vectorVB);
		~ModeVectorTransformation();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::shared_ptr<std::vector<VectorObject>> m_Vectors;
		std::shared_ptr<std::vector<float>> m_VectorBuffer;
		std::shared_ptr<VertexArray> m_VectorVA;
		std::shared_ptr<VertexBuffer> m_VectorVB;
	};
}