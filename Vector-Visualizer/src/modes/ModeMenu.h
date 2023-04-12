#pragma once

#include "Mode.h"

namespace displayMode
{
	class ModeMenu : public Mode
	{
	public:
		ModeMenu(Mode* currentModePtr, std::shared_ptr<std::vector<VectorObject>> vectors, std::shared_ptr<std::vector<float>> vectorBuffer, std::shared_ptr<VertexArray> vectorVA, std::shared_ptr<VertexBuffer> vectorVB);

		void OnImGuiRender();

		void RegisterMode(Mode* mode, const std::string& name)
		{
			std::cout << "Registering mode " << name << std::endl;

			m_Modes.push_back(std::make_pair(name, mode));
		}

	public:
		Mode* m_CurrentMode;
	
	private:
		std::vector<std::pair<std::string, Mode*>> m_Modes;
	};
}