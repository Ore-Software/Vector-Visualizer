#pragma once

#include "Mode.h"

#include "glad/glad.h"

#include "../external/glm/glm.hpp"
#include "../external/glm/gtc/matrix_transform.hpp"

namespace displayMode 
{
	class ModeVectorTransformation : public Mode
	{
	public:
		ModeVectorTransformation(std::shared_ptr<std::vector<VectorObject>> vectors, std::shared_ptr<std::vector<float>> vectorBuffer, std::shared_ptr<VertexArray> vectorVA, std::shared_ptr<VertexBuffer> vectorVB, const VertexBufferLayout& vertexLayout);
		~ModeVectorTransformation();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		void CalculateMatrix();
		void UpdateTransformedVector();
		void CalcParallelepipedVertices();

	private:
		glm::vec3 m_Translate;
		glm::vec3 m_RotateAxis;
		float m_RotateAngle;
		glm::vec3 m_Scale;
		glm::mat4 m_Transform;

		std::shared_ptr<std::vector<float>> m_TriangleBuffer;
		std::shared_ptr<VertexArray> m_TriangleVA;
		std::shared_ptr<VertexBuffer> m_TriangleVB;
		unsigned int m_TriangleIB;
		std::vector<unsigned int> m_Indices;
		VertexBufferLayout m_VertexLayout;

		bool m_ShowParallelepiped = false;
	};
}