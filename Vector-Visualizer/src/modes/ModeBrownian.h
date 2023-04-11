#pragma once

#include "Mode.h"

#include "glad/glad.h"

#include "../external/glm/glm.hpp"
#include "../external/glm/gtc/matrix_transform.hpp"
#include "../external/glm/gtc/random.hpp"

namespace displayMode
{
	class ModeBrownian : public Mode
	{
	public:
		ModeBrownian(std::shared_ptr<std::vector<VectorObject>> vectors, std::shared_ptr<std::vector<float>> vectorBuffer, std::shared_ptr<VertexArray> vectorVA, std::shared_ptr<VertexBuffer> vectorVB);
		~ModeBrownian();

		void Redraw();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		int m_Steps;
		glm::vec3 m_LastPos;
		glm::vec3 m_RandDir;

		VectorObject m_OriginalVec;
		std::shared_ptr<std::vector<VectorObject>> m_Vectors;
		std::shared_ptr<std::vector<float>> m_VectorBuffer;
		std::shared_ptr<VertexArray> m_VectorVA;
		std::shared_ptr<VertexBuffer> m_VectorVB;
	};

}