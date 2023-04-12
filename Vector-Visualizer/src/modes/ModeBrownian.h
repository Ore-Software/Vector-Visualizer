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

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		int m_Steps;
		int m_CurrentStep;
		bool m_Drawing;
		glm::vec3 m_LastPos;
		glm::vec3 m_RandDir;
		double m_LastTime;
		float m_Delay;
	};

}