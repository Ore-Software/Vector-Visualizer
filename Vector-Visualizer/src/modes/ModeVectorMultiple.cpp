#include "ModeVectorMultiple.h"

namespace displayMode 
{
    ModeVectorMultiple::ModeVectorMultiple(std::shared_ptr<std::vector<VectorObject>> vectors, std::shared_ptr<std::vector<float>> vectorBuffer, std::shared_ptr<VertexArray> vectorVA, std::shared_ptr<VertexBuffer> vectorVB)
    {
        m_Vectors = vectors;
        m_VectorBuffer = vectorBuffer;
        m_VectorVA = vectorVA;
        m_VectorVB = vectorVB;
    }

    ModeVectorMultiple::~ModeVectorMultiple()
    {
    }

    void ModeVectorMultiple::OnUpdate(float deltaTime)
    {
    }

    void ModeVectorMultiple::OnRender()
    {
    }

    void ModeVectorMultiple::OnImGuiRender()
    {
        // imgui vector controls
        ImGui::Begin("Multiple Vectors Controls");
        for (int j = 0; j < m_Vectors->size(); j++)
        {
            ImGui::PushID(j);
            ImGui::Text("Vector %d", j + 1);
            ImGui::SliderFloat3("Origin", &(*m_Vectors)[j].m_Origin.x, -10.0f, 10.0f);
            ImGui::SliderFloat3("Direction", &(*m_Vectors)[j].m_Direction.x, -10.0f, 10.0f); // since origin is (0,0,0), endpoint is direction
            ImGui::ColorEdit4("Color", &(*m_Vectors)[j].m_Color.x);
            
            EditVectorBufferLineData(*m_VectorBuffer, *m_Vectors, j);    

            if (ImGui::Button("Remove vector"))
            {
                m_Vectors->erase(m_Vectors->begin() + j);
                
                // reconstruct buffer of vector vertices
                m_VectorBuffer->clear();
                for (VectorObject vec : *m_Vectors)
                {
                    AddVectorBufferLineData(*m_VectorBuffer, vec);
                }
            }

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::PopID();
        }
        if (ImGui::Button("Add vector"))
        {
            VectorObject newVec(glm::vec3(0.0f, 0.0f, 0.0f), glm::ballRand(5.0f), glm::vec4(glm::abs(glm::ballRand(1.0f)), 1.0f)); // uses random vector direction and random color
            m_Vectors->push_back(newVec);
            AddVectorBufferLineData(*m_VectorBuffer, newVec);
        }

        ImGui::End();

        // redraw vectors
        Redraw();
    }

}