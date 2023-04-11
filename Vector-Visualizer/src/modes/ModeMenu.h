#pragma once

#include "Mode.h"

namespace displayMode
{
	class ModeMenu : public Mode
	{
	public:
		ModeMenu(Mode* currentModePtr, std::shared_ptr<std::vector<VectorObject>> vectors, std::shared_ptr<std::vector<float>> vectorBuffer, std::shared_ptr<VertexArray> vectorVA, std::shared_ptr<VertexBuffer> vectorVB);

		void OnImGuiRender();

		template<typename T>
		void RegisterMode(std::shared_ptr<std::vector<VectorObject>> vectors, std::shared_ptr<std::vector<float>> vectorBuffer, std::shared_ptr<VertexArray> vectorVA, std::shared_ptr<VertexBuffer> vectorVB, const std::string& name)
		{
			std::cout << "Registering mode " << name << std::endl;

			m_Modes.push_back(std::make_pair(name, [&]() { return new T(vectors, vectorBuffer, vectorVA, vectorVB); }));
		}

	public:
		Mode* m_CurrentMode;
	
	private:
		std::vector<std::pair<std::string, std::function<Mode* ()>>> m_Modes;
	};
}