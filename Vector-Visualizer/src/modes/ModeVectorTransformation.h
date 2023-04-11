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
		void CalculateMatrix();
		void UpdateTransformedVector();

	private:
		glm::vec3 m_Translate;
		glm::vec3 m_RotateAxis;
		float m_RotateAngle;
		glm::vec3 m_Scale;
		glm::mat4 m_Transform;
	};
}