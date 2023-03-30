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

namespace displayMode {

	class Mode
	{
	public:
		Mode() {}
		virtual ~Mode() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};

	class ModeMenu : public Mode
	{
	public:
		ModeMenu(Mode*& currentModePtr);

		void OnImGuiRender() override;

		template<typename T>
		void RegisterMode(const std::string& name)
		{
			std::cout << "Registering mode " << name << std::endl;

			m_Modes.push_back(std::make_pair(name, []() { return new T(); }));
		}
	private:
		Mode*& m_CurrentMode;
		std::vector<std::pair<std::string, std::function<Mode* ()>>> m_Modes;
	};
}