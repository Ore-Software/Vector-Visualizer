#include "ModeBrownian.h"

namespace displayMode
{
    ModeBrownian::ModeBrownian(std::shared_ptr<std::vector<VectorObject>> vectors, std::shared_ptr<std::vector<float>> vectorBuffer, std::shared_ptr<VertexArray> vectorVA, std::shared_ptr<VertexBuffer> vectorVB)
        : m_Steps(1)
    {
        m_Vectors = vectors;
        m_VectorBuffer = vectorBuffer;
        m_VectorVA = vectorVA;
        m_VectorVB = vectorVB;
    }

    ModeBrownian::~ModeBrownian()
    {
    }

    void ModeBrownian::Redraw()
    {
        m_VectorVA->Bind();
        m_VectorVB->Bind();
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_VectorBuffer->size(), m_VectorBuffer->data(), GL_DYNAMIC_DRAW);
    }

    void ModeBrownian::OnUpdate(float deltaTime)
    {
    }

    void ModeBrownian::OnRender()
    {
    }

    void ModeBrownian::OnImGuiRender()
    {
        ImGui::Begin("Brownian Motion");

        // set number of steps
        ImGui::SliderInt("Number of steps", &m_Steps, 1, 100);

        // generate random vectors
        if (ImGui::Button("Begin"))
        {
            if (m_Vectors->empty())
                m_LastPos = { 0, 0, 0 };
            else
                m_LastPos = m_Vectors->back().GetEndPoint(); 

            for (int j = 0; j < m_Steps; j++)
            {
                m_RandDir = glm::ballRand(1.0f);
                VectorObject newVec(m_LastPos, m_RandDir, glm::vec4(glm::abs(glm::ballRand(1.0f)), 1.0f)); // uses random vector direction and random color
                m_Vectors->push_back(newVec);
                AddVectorBufferData(*m_VectorBuffer, newVec);
                
                m_LastPos += m_RandDir; // update lastPos for next iteration

                // redraw vectors
                Redraw();
            }
        }
        if (ImGui::Button("Clear scene"))
        {
            // clear scene
            m_Vectors->clear();
            m_VectorBuffer->clear();

            // redraw vectors
            Redraw();
        }
        ImGui::End();
    }

}