#include "ModeMenu.h"

namespace displayMode
{
	ModeMenu::ModeMenu(Mode* currentModePtr,
		std::shared_ptr<std::vector<VectorObject>> vectors,
		std::shared_ptr<std::vector<float>> vectorBuffer,
		std::shared_ptr<VertexArray> vectorVA,
		std::shared_ptr<VertexBuffer> vectorVB)
		: m_CurrentMode(currentModePtr)
	{
		m_Vectors = vectors;
		m_VectorBuffer = vectorBuffer;
		m_VectorVA = vectorVA;
		m_VectorVB = vectorVB;
	}

	void ModeMenu::OnImGuiRender()
	{
		for (auto& mode : m_Modes)
		{
			if (ImGui::Button(mode.first.c_str()))
				m_CurrentMode = mode.second;
		}

		ImGui::Begin("Guide");
		ImGui::Text("Move around:");
		ImGui::Text("Use \"W\", \"S\", \"A\", \"D\", \"Shift\", \"Space\"");
		ImGui::Spacing;
		ImGui::Text("Look around:");
		ImGui::Text("Use \"Up\", \"Down\", \"Left\", \"Right\"");
		ImGui::Text("Click and Drag with mouse");
		ImGui::End();
	}
}