#include "ModeVectorMultiple.h"

#include "../external/glm/glm.hpp"
#include "../external/glm/gtc/matrix_transform.hpp"

namespace displayMode 
{
    ModeVectorMultiple::ModeVectorMultiple()
    {
    }

    ModeVectorMultiple::ModeVectorMultiple(std::shared_ptr<std::vector<VectorObject>> vectors, std::shared_ptr<std::vector<float>> vectorBuffer, std::shared_ptr<VertexArray> vectorVA, std::shared_ptr<VertexBuffer> vectorVB)
    {
        m_Vectors = vectors;
        m_VectorBuffer = vectorBuffer;
        m_VectorVA = vectorVA;
        m_VectorVB = vectorVB;
    }

    ModeVectorMultiple::~ModeVectorMultiple()
    {
        // remove all but vector #1
        VectorObject first = m_Vectors->front();
        m_Vectors->clear();
        m_Vectors->push_back(first);

        // reconstruct buffer of vector vertices
        m_VectorBuffer->clear();
        for (VectorObject vec : *m_Vectors)
        {
            AddVectorBufferData(*m_VectorBuffer, vec);
        }

        // redraw vectors
        m_VectorVA->Bind();
        m_VectorVB->Bind();
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_VectorBuffer->size(), m_VectorBuffer->data(), GL_DYNAMIC_DRAW);
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
            ImGui::SliderFloat3("Direction", &(*m_Vectors)[j].m_EndPoint.x, -10.0f, 10.0f); // since origin is (0,0,0), endpoint is direction
            ImGui::ColorEdit4("Color", &(*m_Vectors)[j].m_Color.x);
            if (ImGui::Button("Apply Changes"))
            {
                EditVectorBufferData(*m_VectorBuffer, *m_Vectors, j);

                m_VectorVA->Bind();
                m_VectorVB->Bind();
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_VectorBuffer->size(), m_VectorBuffer->data(), GL_DYNAMIC_DRAW);
            };

            if (ImGui::Button("Remove vector"))
            {
                m_Vectors->erase(m_Vectors->begin() + j);
                
                // reconstruct buffer of vector vertices
                m_VectorBuffer->clear();
                for (VectorObject vec : *m_Vectors)
                {
                    AddVectorBufferData(*m_VectorBuffer, vec);
                }

                m_VectorVA->Bind();
                m_VectorVB->Bind();
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_VectorBuffer->size(), m_VectorBuffer->data(), GL_DYNAMIC_DRAW);
            }

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::PopID();
        }
        if (ImGui::Button("Add vector"))
        {
            VectorObject newVec(glm::vec3(0.0f, 0.0f, 0.0f), glm::ballRand(5.0f), glm::vec4(glm::abs(glm::ballRand(1.0f)), 1.0f)); // uses random vector direction and random color
            m_Vectors->push_back(newVec);
            AddVectorBufferData(*m_VectorBuffer, newVec);

            m_VectorVA->Bind();
            m_VectorVB->Bind();
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_VectorBuffer->size(), m_VectorBuffer->data(), GL_DYNAMIC_DRAW);
        }
        ImGui::End();
    }

}