#pragma once

#include "Mode.h"

#include "glad/glad.h"

namespace displayMode 
{
	class ModeVectorMultiple : public Mode
	{
	public:
		ModeVectorMultiple();
		ModeVectorMultiple(std::vector<VectorObject>* vectors, std::vector<float>* vectorBuffer, VertexArray* vectorVA, VertexBuffer* vectorVB);
		~ModeVectorMultiple();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::vector<VectorObject>* m_Vectors;
		std::vector<float>* m_VectorBuffer;
		VertexArray* m_VectorVA;
		VertexBuffer* m_VectorVB;
	};

}