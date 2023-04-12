#include "ModeBrownian.h"
#include "glfw/glfw3.h"

namespace displayMode
{
    ModeBrownian::ModeBrownian(std::shared_ptr<std::vector<VectorObject>> vectors, std::shared_ptr<std::vector<float>> vectorBuffer, std::shared_ptr<VertexArray> vectorVA, std::shared_ptr<VertexBuffer> vectorVB)
        : m_Steps(1), m_CurrentStep(1), m_Drawing(false), m_LastTime(0), m_Delay(0.1f)
    {
        m_Vectors = vectors;
        m_VectorBuffer = vectorBuffer;
        m_VectorVA = vectorVA;
        m_VectorVB = vectorVB;
    }

    ModeBrownian::~ModeBrownian()
    {
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

        // set number of steps and delay
        ImGui::SliderInt("Number of steps", &m_Steps, 1, 100);
        ImGui::SliderFloat("Delay (ms)", &m_Delay, 0.0f, 0.5f);

        // generate random vectors
        if (ImGui::Button("Begin"))
        {
            m_Drawing = true;
            m_LastTime = 0;
            m_CurrentStep = 1;
            if (m_Vectors->empty())
                m_LastPos = { 0, 0, 0 };
            else
                m_LastPos = m_Vectors->back().GetEndPoint(); 
        }
        if (ImGui::Button("Clear scene"))
        {
            // clear scene
            m_Vectors->clear();
            m_VectorBuffer->clear();
        }

        if ((m_Drawing == true) && (m_CurrentStep <= m_Steps) && (glfwGetTime() - m_LastTime > m_Delay))
        {
            m_RandDir = glm::ballRand(1.0f);
            VectorObject newVec(m_LastPos, m_RandDir, glm::vec4(glm::abs(glm::ballRand(1.0f)), 1.0f)); // uses random vector direction and random color
            m_Vectors->push_back(newVec);
            AddVectorBufferData(*m_VectorBuffer, newVec);

            m_LastPos += m_RandDir; // update lastPos for next iteration

            m_LastTime = glfwGetTime();
            m_CurrentStep++;
        }
        else if (m_CurrentStep > m_Steps)
        {
            m_Drawing = false;
            m_CurrentStep = 1;
            m_LastTime = 0;
        }

        ImGui::End();

        // redraw vectors
        Redraw();
    }

}