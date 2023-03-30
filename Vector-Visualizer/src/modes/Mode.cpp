#include "Mode.h"

namespace displayMode
{
	ModeMenu::ModeMenu(Mode*& currentModePtr)
		: m_CurrentMode(currentModePtr)
	{
	}

	void ModeMenu::OnImGuiRender()
	{
		for (auto& mode : m_Modes)
		{
			if (ImGui::Button(mode.first.c_str()))
				m_CurrentMode = mode.second();
		}
	}
}